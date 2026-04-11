/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** CoreInputs.cpp
*/

#include "core/Core.hpp"
#include <functional>

namespace arcade {

void Core::applyMenuSelection()
{
    if (_menu->getState() != GameState::RUNNING)
        return;
    
    _inCoreMenu = false;
    
    switchGame(_selectedGameIdx);
    switchDisplay(_selectedDisplayIdx);
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, std::function<void()>> coreInputs = {
        {Key::NEXT_GRAPHICS, [this] { 
            _selectedDisplayIdx = (_selectedDisplayIdx + 1) % _displayLibs.size();    
            switchDisplay(_selectedDisplayIdx); }},
        {Key::PREV_GRAPHICS, [this] {
            _selectedDisplayIdx = (_selectedDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size();
            switchDisplay(_selectedDisplayIdx); }},
        {Key::NEXT_GAME,     [this] {
            _selectedGameIdx = (_selectedGameIdx + 1) % _gameLibs.size();
            if (!_inCoreMenu) switchGame(_selectedGameIdx); }},
        {Key::PREV_GAME,     [this] {
            _selectedGameIdx = (_selectedGameIdx + _gameLibs.size() - 1) % _gameLibs.size();
            if (!_inCoreMenu) switchGame(_selectedGameIdx); }},
        {Key::MENU,          [this] { _game->pause(); _inCoreMenu = true;
            _display->setRenderContext(_menu->getRenderContext());
            _display->setTitle(_menu->getName());
        }},
        {Key::ENTER,         [this] { _menu->handleInput(Key::ENTER); applyMenuSelection(); }},
        {Key::EXIT,          [this] { _running = false; }},
        {Key::ESCAPE,        [this] { _running = false; }},
    };
 
    const auto it = coreInputs.find(key);
    if (it != coreInputs.end()) {
        it->second();
        if (_inCoreMenu)
            _menu->handleInput(key);
        return;
    }
 
    if (_inCoreMenu) {
        _menu->handleInput(key);
    } else if (_game) {
        _game->handleInput(key);
    }
}

}
