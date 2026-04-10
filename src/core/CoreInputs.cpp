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
        return;
    }

    if (_playerName.size() > maxNameLength)
        return;


    auto it = nameInputs.find(key);
    if (it != nameInputs.end()) {
        _playerName.push_back(it->second);
    }
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
        {Key::MENU,          [this] { saveScore(); _game->pause(); _inCoreMenu = true;
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
        handleNameInput(key);
        _menu->handleInput(key);
    } else if (_game) {
        _game->handleInput(key);
    }
}

}
