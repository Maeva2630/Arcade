/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** MenuInputs.cpp
*/

#include "games/menu/Menu.hpp"

namespace arcade {

void MenuModule::handleInput(Key key)
{
    handleNameInput(key);

    if (_games.empty() || _displays.empty()) {
        if (key == Key::ENTER) {
            if (!_playerName.empty())
                _currentState = GameState::RUNNING;
            else
                _launchWithoutName = true;
        }
        return;
    }

    std::map<Key, std::function<void()>> actions = {
        {Key::PREV_GAME, [this] {
            _selectedGame = (_selectedGame + _games.size() - 1) % _games.size();
        }},
        {Key::NEXT_GAME, [this] {
            _selectedGame = (_selectedGame + 1) % _games.size();
        }},
        {Key::PREV_GRAPHICS, [this] {
            _selectedDisplay = (_selectedDisplay + _displays.size() - 1) % _displays.size();
        }},
        {Key::NEXT_GRAPHICS, [this] {
            _selectedDisplay = (_selectedDisplay + 1) % _displays.size();
        }},
        {Key::ENTER, [this] {
            if (!_playerName.empty())
                _currentState = GameState::RUNNING;
            else
                _launchWithoutName = true;
        }}
    };

    auto it = actions.find(key);
    if (it != actions.end())
        it->second();
}

void MenuModule::handleNameInput(Key key)
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
        _launchWithoutName = false;
        _playerName.push_back(it->second);
    }
}

}
