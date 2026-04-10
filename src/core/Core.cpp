/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <iostream>
#include <fstream>

namespace arcade {

static const char* LIB_DIR = "lib";

Core::Core(std::string libPath)
{
    bool found = false;

    loadLibs(LIB_DIR);

    if (_displayLibs.empty())
        throw CoreException("No display libraries found.");

    if (!_menuLoader || !_menu)
        throw CoreException("No menu library found.");

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

    _running = true;
    _inCoreMenu = true;

    _currentGameIdx = 0;
    _selectedGameIdx = _currentGameIdx;
    _selectedDisplayIdx = _currentDisplayIdx;
    
    switchDisplay(_currentDisplayIdx);
    switchGame(_currentGameIdx);

    _previousState = GameState::MENU;
}

void Core::initAll()
{
    _display->init();
    _game->init();
    _menu->init();
    _display->setRenderContext(_menu->getRenderContext());
    _display->setTitle(_menu->getName());
}

void Core::stopAll()
{
    _display->stop();
    _game->stop();
    _menu->stop();
}

void Core::run()
{
    initAll();
    mainLoop();
    stopAll();
}

void Core::mainLoop()
{
    while (_running && _display->isOpen()) {
        Key key = _display->getKey();
        if (key != Key::UNKNOWN)
            handleGlobalInput(key);

        _display->clear();

        IGameModule* current = _inCoreMenu ? _menu.get() : _game.get();
        if (current) {
            current->update(_display->getDeltaTime());

            for (const auto& entity : current->getEntities())
                _display->drawEntity(entity);

            for (const auto& text : current->getUIText())
                _display->drawText(text);
        }

        if (_previousState != GameState::MENU && current->getState() != _previousState)
            saveScore();
        _previousState = current->getState();

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

void Core::saveScore()
{
    if (_playerName.empty() || _game->getScore() <= 0)
        return;

    std::ofstream file(SAVEPATH);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << SAVEPATH << " for saving scores." << std::endl;
        return;
    }

    file << libName(_gameLibs[_currentGameIdx]) << SEPARATOR;
    file << _playerName << SEPARATOR;
    file << _game->getScore() << std::endl;

    file.close();
}

}
