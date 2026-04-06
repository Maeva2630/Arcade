/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <dlfcn.h>

namespace arcade {

static const char* LIB_DIR = "./lib";

using GlobalInputHandler = std::function<void(Key)>;
using StatesHandler = std::function<void(GameState)>;

Core::Core(std::string libPath)
{
    bool found = false;

    loadLibs(LIB_DIR);

    if (_displayLibs.empty())
        throw CoreException("No display librairies found.");

    if (_gameLibs.empty())
        throw CoreException("No game librairies found.");

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
    _selectedGameIdx = _currentGameIdx;
    _selectedDisplayIdx = _currentDisplayIdx;
    
    switchDisplay(_currentDisplayIdx);
    switchGame(_currentGameIdx);
    
    _previousState = GameState::MENU;

    _running = true;
    _inCoreMenu = true;
}

Core::~Core()
{
    if (_display) {
        _display->stop();
    }
}

void Core::run()
{
    mainLoop();
}

void Core::mainLoop()
{
    static const std::map<GameState, StatesHandler> states = {
        {GameState::MENU,       [this](GameState) { renderMenu(); }},
        {GameState::RUNNING,    [this](GameState) { renderGame(); }},
        {GameState::PAUSED,     [this](GameState) { renderGame(); }},
        {GameState::GAME_OVER,  [this](GameState) { renderGame(); }},
        {GameState::WON,        [this](GameState) { renderGame(); }},
    };

    while (_running && _display->isOpen()) {
        Key key = _display->getKey();
        if (key != Key::UNKNOWN)
            handleGlobalInput(key);

        if (_inCoreMenu) {
            renderMenu();
        } else {
            GameState currentState = _game->getState();

            if (currentState != _previousState) {
                if (currentState == GameState::GAME_OVER || currentState == GameState::WON)
                    saveScore(_playerName, _game->getScore());
                if (currentState == GameState::PAUSED)
                    _game->pause();
                _previousState = currentState;
            }

            const auto it = states.find(currentState);
            if (it != states.end())
                it->second(currentState);
        }

        _display->display();
    }
}

void Core::mainLoop()
{
    static const std::map<GameState, StatesHandler> states = {
        {GameState::MENU,       [this](GameState) { renderMenu(); }},
        {GameState::RUNNING,    [this](GameState) { renderGame(); }},
        {GameState::PAUSED,     [this](GameState) {
            if (_game)
                _game->pause();
            renderGame();
        }},
        {GameState::GAME_OVER,  [this](GameState) { saveScore(_playerName, _game->getScore()); renderGame(); }},
        {GameState::WON,        [this](GameState) { saveScore(_playerName, _game->getScore()); renderGame(); }},
    };

    while (_running && _display->isOpen()) {
        Key key = _display->getKey();
        if (key != Key::UNKNOWN) {
            handleGlobalInput(key);
        }

        if (_inCoreMenu) {
            renderMenu();
        } else {
            GameState currentState = _game->getState();
            auto it = states.find(currentState);
            if (it != states.end()) {
                it->second(currentState);
            }
        }
        
        _display->display();
    }
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, GlobalInputHandler> inputs = {
        {Key::NEXT_GRAPHICS, [this](Key) {
            _selectedDisplayIdx = (_selectedDisplayIdx + 1) % _displayLibs.size();
        }},
        {Key::PREV_GRAPHICS, [this](Key) {
            _selectedDisplayIdx = (_selectedDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size();
        }},
        {Key::NEXT_GAME, [this](Key) {
            _selectedGameIdx = (_selectedGameIdx + 1) % _gameLibs.size();
        }},
        {Key::PREV_GAME, [this](Key) {
            _selectedGameIdx = (_selectedGameIdx + _gameLibs.size() - 1) % _gameLibs.size();
        }},
        {Key::ENTER, [this](Key) {
            if (!_inCoreMenu)
                return;
            if (_selectedGameIdx != _currentGameIdx)
                switchGame(_selectedGameIdx);
            if (_selectedDisplayIdx != _currentDisplayIdx)
                switchDisplay(_selectedDisplayIdx);
            _inCoreMenu = false;
        }},
        {Key::RESTART, [this](Key) {
            if (_game) _game->restart();
        }},
        {Key::MENU, [this](Key) {
            if (_game) _game->pause();
        }},
        {Key::EXIT, [this](Key) {
            if (_inCoreMenu) _running = false;
        }},
    };

    const auto it = inputs.find(key);
    if (it != inputs.end())
        it->second(key);
    else if (_game)
        _game->handleInput(key);
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
    auto newDisplay = std::unique_ptr<IDisplayModule>(newLoader->getInstance("entryPoint"));
    
    _displayLoader = std::move(newLoader);
    _display = std::move(newDisplay);

    _display->init();
    if (_game)
        _display->setRenderContext(_game->getRenderContext());

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
    auto newGame = std::unique_ptr<IGameModule>(newLoader->getInstance("entryPoint"));

    _gameLoader = std::move(newLoader);
    _game = std::move(newGame);

    _game->init();
    if (_display)
        _display->setRenderContext(_game->getRenderContext());

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

static std::string libName(const std::string& path)
{
    std::size_t slash = path.rfind('/');
    std::string file;

    if (slash == std::string::npos) {
        file = path;
    } else {
        file = path.substr(slash + 1);
    }

    if (file.substr(0, 7) == "arcade_")
        file = file.substr(7);

    if (file.find(".") != std::string::npos)
        file = file.substr(0, file.find("."));

    return file;
}

void Core::renderMenu()
{
    _display->clear();
    _display->drawText(Text("=== ARCADE ===", 0, 0, Colors::YELLOW, 12));

    _display->drawText(Text("Game  [F3/F4]:", 0, 2, Colors::CYAN, 10));
    for (std::size_t i = 0; i < _gameLibs.size(); i++) {
        Color c = (i == _selectedGameIdx) ? Colors::GREEN : Colors::WHITE;
        std::string marker = (i == _selectedGameIdx) ? "> " : "  ";
        _display->drawText(Text(marker + libName(_gameLibs[i]), 2, 3 + i, c, 10));
    }

    _display->drawText(Text("Graphics [F1/F2]:", 0, 4 + _gameLibs.size(), Colors::CYAN, 10));
    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        bool isSelected = (i == _selectedDisplayIdx);
        bool isActive = (i == _currentDisplayIdx);

        Color c = isSelected ? Colors::GREEN : Colors::WHITE;

        std::string marker = isSelected ? "> " : "  ";
        std::string active = isActive ? " [RUNNING]" : "";

        _display->drawText(Text(
            marker + libName(_displayLibs[i]) + active,
            2,
            5 + _gameLibs.size() + i,
            c,
            10
        ));
    }

    _display->drawText(
        Text("Entree: lancer | Echap: quitter", 0, 7 + _gameLibs.size() + _displayLibs.size(), Colors::WHITE, 9));
}

void Core::renderGame()
{
    _game->update(_display->getDeltaTime());

    _display->clear();

    for (const auto& entity : _game->getEntities())
        _display->drawEntity(entity);
        
    for (const auto& text : _game->getUIText())
        _display->drawText(text);
}

}
