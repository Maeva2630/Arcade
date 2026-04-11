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
    if (!_window)
        throw ModuleException("Sfml window creation has failed");

     _deltaTime = 0.0f;
    _isOpen = true;
}

std::map<int, Key> initKeyFromEnum()
{
    std::map<int, Key> newMap = {

        {sf::Keyboard::Up, Key::UP},
        {sf::Keyboard::Down, Key::DOWN},
        {sf::Keyboard::Left, Key::LEFT},
        {sf::Keyboard::Right, Key::RIGHT},
        {sf::Keyboard::Escape, Key::ESCAPE},
        {sf::Keyboard::Space, Key::SPACE},
        {sf::Keyboard::Enter, Key::ENTER},
        {sf::Keyboard::BackSpace, Key::BACKSPACE},

        {sf::Keyboard::Num0, Key::NUM_0}, {sf::Keyboard::Num1, Key::NUM_1}, {sf::Keyboard::Num2, Key::NUM_2},
        {sf::Keyboard::Num3, Key::NUM_3}, {sf::Keyboard::Num4, Key::NUM_4}, {sf::Keyboard::Num5, Key::NUM_5},
        {sf::Keyboard::Num6, Key::NUM_6}, {sf::Keyboard::Num7, Key::NUM_7}, {sf::Keyboard::Num8, Key::NUM_8},
        {sf::Keyboard::Num9, Key::NUM_9},
        {sf::Keyboard::A, Key::A}, {sf::Keyboard::B, Key::B},
        {sf::Keyboard::C, Key::C}, {sf::Keyboard::D, Key::D},
        {sf::Keyboard::E, Key::E}, {sf::Keyboard::F, Key::F},
        {sf::Keyboard::G, Key::G}, {sf::Keyboard::H, Key::H},
        {sf::Keyboard::I, Key::I}, {sf::Keyboard::J, Key::J},
        {sf::Keyboard::K, Key::K}, {sf::Keyboard::L, Key::L},
        {sf::Keyboard::M, Key::M}, {sf::Keyboard::N, Key::N},
        {sf::Keyboard::O, Key::O}, {sf::Keyboard::P, Key::P},
        {sf::Keyboard::Q, Key::Q}, {sf::Keyboard::R, Key::R},
        {sf::Keyboard::S, Key::S}, {sf::Keyboard::T, Key::T},
        {sf::Keyboard::U, Key::U}, {sf::Keyboard::V, Key::V},
        {sf::Keyboard::W, Key::W}, {sf::Keyboard::X, Key::X},
        {sf::Keyboard::Y, Key::Y}, {sf::Keyboard::Z, Key::Z},

        {sf::Keyboard::F1, Key::PREV_GRAPHICS},
        {sf::Keyboard::F2, Key::NEXT_GRAPHICS},
        {sf::Keyboard::F3, Key::PREV_GAME},
        {sf::Keyboard::F4, Key::NEXT_GAME},
        {sf::Keyboard::F6, Key::MENU},
        {sf::Keyboard::F12, Key::EXIT},
    };

    return newMap;
}

void Sfml::stop()
{
    if (_window)
        _window->close();
    
}

void Sfml::setTitle(const std::string& title)
{
    if (!_window)
        throw ModuleException("Window is not initialized");
    _window->setTitle(title);
}

void Sfml::setFullscreen(bool fullscreen)
{
    if (!_window)
        throw ModuleException("Can not change window to full screen mode");
    _window->create(sf::VideoMode::getDesktopMode(), "SFML Window", sf::Style::Fullscreen);
    sf::Vector2u newSize = _window->getSize();
    _winSize = Position(static_cast<float>(newSize.x), static_cast<float>(newSize.y));
}

}
