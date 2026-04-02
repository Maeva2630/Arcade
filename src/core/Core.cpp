/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <functional>

namespace arcade {

static const char* LIB_DIR = "./lib";

using GlobalInputHandler = std::function<void(Key)>;
using StatesHandler = std::function<void(GameState)>;

Core::Core(std::string libPath)
{
    bool found = false;

    loadLibs(LIB_DIR);

    if (_displayLibs.empty()) {
        throw CoreException("No display librairies found.");
    }

    if (_gameLibs.empty()) {
        throw CoreException("No game librairies found.");
    }

    std::cout << _displayLibs.size() << " display libraries found.\n";

    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        std::cout << "display library: " << _displayLibs[i] << std::endl;
        if (_displayLibs[i] == libPath) {
            _currentDisplayIdx = i;
            found = true;
            break;
        }
    }

    if (!found)
        throw CoreException(libPath + " is not a graphics library.");

    _currentGameIdx = 0;

    switchDisplay(_currentDisplayIdx);
    switchGame(_currentGameIdx);

    _running = true;
}

void Core::run()
{
    mainLoop();
}

void Core::mainLoop()
{
    static const std::map<GameState, StatesHandler> states = {
        {GameState::MENU, [this](GameState) { renderMenu(); }},
        {GameState::RUNNING, [this](GameState) { renderGame(); }},
        {GameState::PAUSED, [this](GameState) { std::cout << "Game paused." << std::endl; }},
        {GameState::GAME_OVER, [this](GameState) { std::cout << "Rendering game over screen..." << std::endl; }},
        {GameState::WON, [this](GameState) { std::cout << "Rendering victory screen..." << std::endl; }},
        {GameState::PAUSED,    [this](GameState) { _game->pause(); renderGame(); }},
        {GameState::GAME_OVER, [this](GameState) { saveScore(_playerName, _game->getScore()); std::cout << "Game over." << std::endl; }},
        {GameState::WON,       [this](GameState) { saveScore(_playerName, _game->getScore()); std::cout << "You won!" << std::endl; }},
    };

    while (_running && _display->isOpen()) {
        for (Key key = _display->getKey(); key != Key::UNKNOWN; key = _display->getKey()) {
            handleGlobalInput(key);
        }

        auto it = states.find(_game->getState());
        if (it != states.end()) {
            it->second(_game->getState());
        }
        
        _display->display();
    }
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, GlobalInputHandler> inputs = {
        {Key::NEXT_GRAPHICS, [this](Key) { switchDisplay((_currentDisplayIdx + 1) % _displayLibs.size()); }},
        {Key::PREV_GRAPHICS, [this](Key) { switchDisplay((_currentDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size()); }},
        {Key::NEXT_GAME, [this](Key) { switchGame((_currentGameIdx + 1) % _gameLibs.size()); }},
        {Key::PREV_GAME, [this](Key) { switchGame((_currentGameIdx + _gameLibs.size() - 1) % _gameLibs.size()); }},
        {Key::RESTART, [this](Key) { _game->restart(); }},
        {Key::MENU,    [this](Key) { _game->pause(); }},
        {Key::EXIT, [this](Key) { _running = false; }}
    };

    auto it = inputs.find(key);
    if (it != inputs.end()) {
        it->second(key);
    } else if (it == inputs.end() && _game) {
        _game.get()->handleInput(key);
    }
}

void Core::switchDisplay(std::size_t idx)
{
    if (idx >= _displayLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_display) {
        _display->stop();
        _display.reset();
        _displayLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IDisplayModule>>(_displayLibs[idx]);
    auto newDisplay = std::unique_ptr<IDisplayModule>(newLoader.get()->getInstance("entryPoint"));
    
    _displayLoader = std::move(newLoader);
    _display = std::move(newDisplay);

    _display.get()->init();
    if (_game)
        _display.get()->setRenderContext(_game->getRenderContext());

    _currentDisplayIdx = idx;
}

void Core::switchGame(std::size_t idx)
{
    if (idx >= _gameLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_game) {
        _game->stop();
        _game.reset();
        _gameLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IGameModule>>(_gameLibs[idx]);
    auto newGame = std::unique_ptr<IGameModule>(newLoader.get()->getInstance("entryPoint"));

    _gameLoader = std::move(newLoader);
    _game = std::move(newGame);

    _game.get()->init();
    if (_display)
        _display.get()->setRenderContext(_game->getRenderContext());

    _currentGameIdx = idx;
}

void Core::loadLibs(std::string dir)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string lib_path = entry.path();

        try {
            DLLoader<IModule> loader(lib_path);
            std::unique_ptr<IModule> module(loader.getInstance("entryPoint"));

            switch (module->getType()) {
                case ModuleType::DISPLAY:
                std::cout << "Found display library: " << module->getName() << std::endl;
                    _displayLibs.push_back(lib_path);
                    break;
                case ModuleType::GAME:
                    _gameLibs.push_back(lib_path);
                    break;
                default:
                    throw CoreException(lib_path + " invalid module type.");
            }

        } catch (const std::exception& e) {
            std::cerr << "Skipping " << lib_path << ": " << e.what() << std::endl;
        }
    }
}

void Core::saveScore(std::string name, int score)
{
    _scores[name] = score;
}

void Core::renderMenu()
{
    _display.get()->clear();
    _display.get()->drawText(Text("Select a game:", 2, 2, Colors::WHITE));

    for (std::size_t i = 0; i < _gameLibs.size(); i++)
        _display.get()->drawText(Text(_gameLibs[i], 2, 4 + i));

    std::cout << "Rendering menu..." << std::endl;
}

void Core::renderGame()
{
    _game.get()->update(_display.get()->getDeltaTime());

    _display.get()->clear();

    for (const auto& entity : _game->getEntities())
        _display->drawEntity(entity);
        
    for (const auto& text : _game->getUIText())
        _display->drawText(text);

    std::cout << "Rendering game..." << std::endl;
}

}
