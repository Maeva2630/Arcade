/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** AGameModule.cpp
*/

#include "games/AGameModule.hpp"

namespace arcade
{

using GlobalInputHandler = std::function<void(Key)>;

void AGameModule::handleGameInput(Key key)
{
    static const std::map<Key, GlobalInputHandler> inputs = {
        {Key::P,       [this](Key) { if (_currentState == GameState::RUNNING) pause();
            else if (_currentState == GameState::PAUSED) resume(); }},
        {Key::R,       [this](Key) { restart(); }},
        {Key::RESTART, [this](Key) { restart(); }},
    };

    auto it = inputs.find(key);
    if (it != inputs.end()) {
        it->second(key);
    }
}

}
