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

static const char* LIB_DIR = "lib";

Core::Core(std::string libPath)
{
    bool found = false;

    loadLibs(LIB_DIR);

    if (_displayLibs.empty())
        throw CoreException("No display librairies found.");

    if (_gameLibs.empty())
        throw CoreException("No game librairies found.");

    if (libPath.substr(0, 2) == "./")
        libPath = libPath.substr(2);

    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
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

void Core::initAll()
{
    _display->init();
    _game->init();
    _display->setRenderContext(_game->getRenderContext());
    _display->setTitle(_game->getName());
}

void Core::stopAll()
{
    _display->stop();
    _game->stop();
}

void Core::run()
{
    initAll();
    mainLoop();
    stopAll();
}

void Core::applyMenuSelection()
{
    if (_selectedGameIdx != _currentGameIdx)
        switchGame(_selectedGameIdx);
    if (_selectedDisplayIdx != _currentDisplayIdx)
        switchDisplay(_selectedDisplayIdx);
    if (_game->getState() == GameState::PAUSED)
        _game->resume();
    _inCoreMenu = false;
}

void Core::applyGameState(GameState target)
{
    static const std::map<GameState, std::function<void()>> stateActions = {
        {GameState::RUNNING,  [this] { _game->resume(); }},
        {GameState::PAUSED,   [this] { _game->pause(); }},
        {GameState::MENU,     [this] { _game->pause(); _inCoreMenu = true; }},
    };
 
    const auto it = stateActions.find(target);
    if (it != stateActions.end())
        it->second();
}

void Core::handleGameInput(Key key)
{
    GameState pauseTarget = GameState::RUNNING;
    if (_game->getState() == GameState::RUNNING)
        pauseTarget = GameState::PAUSED;

    const std::map<Key, GameState> keyToState = {
        {Key::P, pauseTarget},
        {Key::M, GameState::MENU},
    };
 
    const auto it = keyToState.find(key);
    if (it != keyToState.end())
        applyGameState(it->second);
    else
        _game->handleInput(key);
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, std::function<void()>> coreInputs = {
        {Key::NEXT_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + 1) % _displayLibs.size(); }},
        {Key::PREV_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size(); }},
        {Key::NEXT_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + 1) % _gameLibs.size(); }},
        {Key::PREV_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + _gameLibs.size() - 1) % _gameLibs.size(); }},
        {Key::RESTART,       [this] { _game->restart(); }},
        {Key::ENTER,         [this] { applyMenuSelection(); }},
        {Key::EXIT,          [this] { _running = false; }},
    };
 
    if (_inCoreMenu) {
        const auto it = coreInputs.find(key);
        if (it != coreInputs.end())
            it->second();
        return;
    }
 
    handleGameInput(key);
}

void Core::mainLoop()
{
    static const std::map<GameState, std::function<void()>> renderHandlers = {
        {GameState::MENU,       [this]() { renderMenu(); }},
        {GameState::RUNNING,    [this]() { renderGame(); }},
        {GameState::PAUSED,     [this]() { renderGame(); }},
        {GameState::GAME_OVER,  [this]() { renderGame(); }},
        {GameState::WON,        [this]() { renderGame(); }},
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
                _previousState = currentState;
            }

            const auto it = renderHandlers.find(currentState);
            if (it != renderHandlers.end())
                it->second();
        }

        _display->display();
    }
}

void Core::renderGame()
{
    _display->clear();
    _game->update(_display->getDeltaTime());

    for (Entity entity : _game->getEntities())
        _display->drawEntity(entity);

    for (Text text : _game->getUIText())
        _display->drawText(text);
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
    static const std::size_t fontSize = 48;
    static const std::size_t spacing = 4;
    std::size_t x = 0;
    std::size_t y = 0;
    
    _display->clear();
    _display->drawText(Text("=== ARCADE ===", x, y, Colors::YELLOW, fontSize + 10));

    _display->drawText(Text("Game  [F3/F4]:", x, y += spacing, Colors::CYAN, fontSize + 2));
    for (std::size_t i = 0; i < _gameLibs.size(); i++) {
        Color c = (i == _selectedGameIdx) ? Colors::GREEN : Colors::WHITE;
        std::string marker = (i == _selectedGameIdx) ? "> " : "  ";
        _display->drawText(Text(marker + libName(_gameLibs[i]), x + 2, y += spacing / 2, c, fontSize));
    }

    _display->drawText(Text("Graphics [F1/F2]:", x, y += spacing, Colors::CYAN, fontSize + 2));
    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        bool isSelected = (i == _selectedDisplayIdx);
        bool isActive = (i == _currentDisplayIdx);

        Color c = isSelected ? Colors::GREEN : Colors::WHITE;

        std::string marker = isSelected ? "> " : "  ";
        std::string active = isActive ? " [RUNNING]" : "";

        _display->drawText(Text(
            marker + libName(_displayLibs[i]) + active,
            2,
            y += spacing / 2,
            c,
            fontSize
        ));
    }

    _display->drawText(
        Text("Enter: launch | Escape: exit", x, y += spacing, Colors::WHITE, fontSize - 8));
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

}
