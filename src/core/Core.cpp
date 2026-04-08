/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <functional>
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
    
    loadScores();
    
    _previousState = GameState::MENU;

    _running = true;
    _inCoreMenu = true;
    _launchWithoutName = false;
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
    saveScore();
    _display->stop();
    _game->stop();
}

void Core::run()
{
    initAll();
    mainLoop();
    stopAll();
}

void Core::handleNameInput(Key key)
{
    static const std::size_t maxNameLength = 20;
    static const std::map<Key, char> nameInputs = {
        {Key::NUM_0, '0'}, {Key::NUM_1, '1'}, {Key::NUM_2, '2'},
        {Key::NUM_3, '3'}, {Key::NUM_4, '4'}, {Key::NUM_5, '5'},
        {Key::NUM_6, '6'}, {Key::NUM_7, '7'}, {Key::NUM_8, '8'},
        {Key::NUM_9, '9'},
        {Key::A, 'a'}, {Key::B, 'b'}, {Key::C, 'c'}, {Key::D, 'd'},
        {Key::E, 'e'}, {Key::F, 'f'}, {Key::G, 'g'}, {Key::H, 'h'},
        {Key::I, 'i'}, {Key::J, 'j'}, {Key::K, 'k'}, {Key::L, 'l'},
        {Key::M, 'm'}, {Key::N, 'n'}, {Key::O, 'o'}, {Key::P, 'p'},
        {Key::Q, 'q'}, {Key::R, 'r'}, {Key::S, 's'}, {Key::T, 't'},
        {Key::U, 'u'}, {Key::V, 'v'}, {Key::W, 'w'}, {Key::X, 'x'},
        {Key::Y, 'y'}, {Key::Z, 'z'},
    };

    if (key == Key::BACKSPACE && !_playerName.empty()) {
        _playerName.pop_back();
    } else {
        auto it = nameInputs.find(key);
        if (it != nameInputs.end() && _playerName.size() < maxNameLength) {
            _playerName += it->second;
        }
    }
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, std::function<void()>> coreInputs = {
        {Key::NEXT_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + 1) % _displayLibs.size(); switchDisplay(_selectedDisplayIdx); }},
        {Key::PREV_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size(); switchDisplay(_selectedDisplayIdx); }},
        {Key::NEXT_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + 1) % _gameLibs.size(); switchGame(_selectedGameIdx); }},
        {Key::PREV_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + _gameLibs.size() - 1) % _gameLibs.size(); switchGame(_selectedGameIdx); }},
        {Key::MENU,          [this] { _game->pause(); _inCoreMenu = true; }},
        {Key::ENTER,         [this] { applyMenuSelection(); }},
        {Key::EXIT,          [this] { _running = false; }},
        {Key::ESCAPE,        [this] { _running = false; }},
    };
 
    const auto it = coreInputs.find(key);
    if (it != coreInputs.end()) {
        it->second();
        return;
    }
 
    if (_inCoreMenu) {
        handleNameInput(key);
    } else if (_game) {
        _game->handleInput(key);
    }
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
                    saveScore();
                _previousState = currentState;
            }

            const auto it = renderHandlers.find(currentState);
            if (it != renderHandlers.end())
                it->second();
        }

        updateScore(_game->getScore());
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

}
