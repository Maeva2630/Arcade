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

    _menu->setAvailableDisplays(formatLib(_displayLibs));
    _menu->setAvailableGames(formatLib(_gameLibs));

    _running = true;
    _inCoreMenu = true;

    _currentGameIdx = 0;
    _selectedGameIdx = _currentGameIdx;
    _selectedDisplayIdx = _currentDisplayIdx;
    
    switchDisplay(_currentDisplayIdx);
    switchGame(_currentGameIdx);
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
        if (key != Key::UNKNOWN) {
            _menu->addScore(libName(_gameLibs[_currentGameIdx]), _game->getScore());
            handleGlobalInput(key);
        }

        _display->clear();

        if (_inCoreMenu) {
            for (const auto& text : _menu->getUIText())
                _display->drawText(text);
        } else {
            _game->update(_display->getDeltaTime());

            for (const auto& entity : _game->getEntities())
                _display->drawEntity(entity);

            for (const auto& text : _game->getUIText())
                _display->drawText(text);
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

}
