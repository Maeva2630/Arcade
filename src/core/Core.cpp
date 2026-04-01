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

static const char* LIB_DIR = "lib";

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

    for (std::size_t i = 0; i < _displayLibs.size(); ++i) {
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

void Core::initAll()
{
    _display->init();
    _game->init();
    _display->setRenderContext(_game->getRenderContext());
}

void Core::stopAll()
{
    //peut etre mettre un try catch ici 
    _display->stop();
    _game->stop();
}

void Core::run()
{
    initAll();
    mainLoop();
    stopAll();
}

void Core::mainLoop()
{
    static const std::map<GameState, StatesHandler> states = {
        {GameState::MENU, [this](GameState) { renderMenu(); }},
        {GameState::RUNNING, [this](GameState) { renderGame(); }},
        {GameState::PAUSED, [this](GameState) { std::cout << "Game paused." << std::endl; }},
        {GameState::GAME_OVER, [this](GameState) { std::cout << "Rendering game over screen..." << std::endl; }},
        {GameState::WON, [this](GameState) { std::cout << "Rendering victory screen..." << std::endl; }}
    };

    while (_running && _display->isOpen()) {
        handleGlobalInput(_display->getKey());

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
        {Key::EXIT, [this](Key) { _running = false; }}
    };

    auto it = inputs.find(key);
    if (it != inputs.end()) {
        it->second(key);
    }
}

void Core::switchDisplay(std::size_t idx)
{
    if (idx >= _displayLibs.size()) {
        throw CoreException("Index out of range.");
    }

    auto newLoader = std::make_unique<DLLoader<IDisplayModule>>(_displayLibs[idx]);
    auto newDisplay = std::unique_ptr<IDisplayModule>(newLoader.get()->getInstance("entryPoint"));
    
    _displayLoader = std::move(newLoader);
    _display = std::move(newDisplay);

    _currentDisplayIdx = idx;
}

void Core::switchGame(std::size_t idx)
{
    if (idx >= _gameLibs.size()) {
        throw CoreException("Index out of range.");
    }

    auto newLoader = std::make_unique<DLLoader<IGameModule>>(_gameLibs[idx]);
    auto newGame = std::unique_ptr<IGameModule>(newLoader.get()->getInstance("entryPoint"));

    _gameLoader = std::move(newLoader);
    _game = std::move(newGame);

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
    std::cout << "Rendering menu..." << std::endl;
}

void Core::renderGame()
{
    _display->clear();

    _game->handleInput(_display->getKey());

    _game->update(_display->getDeltaTime());

    for (Entity entity : _game->getEntities()) {
        _display->drawEntity(entity);
    }

    for (Text text : _game->getUIText()) {
        _display->drawText(text);
    }

    _display->display();
}

}
