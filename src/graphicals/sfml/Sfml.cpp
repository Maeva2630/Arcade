/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sfml.cpp
*/

#include "graphicals/Sfml.hpp"

extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::Sfml();
}

namespace arcade
{

Sfml::Sfml() : _window(nullptr)
{
}

Sfml::~Sfml()
{
    if (_window)
        stop();
}

void Sfml::init()
{
    _window->create(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "SFML Window");

     _deltaTime = 0.0f;
    _isOpen = true;
}

}
