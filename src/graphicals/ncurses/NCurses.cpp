/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NCurses.cpp
*/

#include "graphicals/NCurses.hpp"
#include <iostream>

extern "C" arcade::IModule *entryPoint(void)
{
    std::cout << "NCurses entryPoint\n";
    return new arcade::NCurses();
}

namespace arcade {

NCurses::NCurses() : _window(nullptr)
{
    std::cout << "NCurses constructor\n";
}

NCurses::~NCurses()
{
    std::cout << "NCurses destructor\n";
}

void NCurses::init()
{
    std::cout << "NCurses init\n";
    _window = initscr();
    if (!_window)
        throw ModuleException("NCurses window init failed");
    _keyMap =  initKeyFromEnum();
    noecho();
}

void NCurses::stop()
{
    std::cout << "NCurses stop\n";
    if (_window) {
        endwin();
        free(_window);
        _window = nullptr;
    } else
        throw ModuleException("The window can not be closed");
}

void NCurses::clear()
{
    std::cout << "NCurses clear\n";
    if (_window)
        clear();
    else
        throw ModuleException("The window can not be cleared");
}

void NCurses::display()
{
    std::cout << "NCurses display\n";
    if (_window)
        refresh();
    else
        throw ModuleException("The window can not be refreshed");
}

Key NCurses::getKey()
{
    std::cout << "NCurses getKey\n";
    int ch = getch();
    auto key = this->_keyMap.find(ch);

    return (key != this->_keyMap.end()) ? key->second : Key::UNKNOWN;
}

std::map<int, Key> NCurses::initKeyFromEnum()
{
    std::map<int, Key> newMap = {
        {KEY_UP, Key::UP},
        {KEY_DOWN, Key::DOWN},
        {KEY_LEFT, Key::LEFT},
        {KEY_RIGHT, Key::RIGHT},
        {27, Key::ESCAPE},
        {' ', Key::SPACE},
        {'\n', Key::ENTER},
        {KEY_BACKSPACE, Key::BACKSPACE},

        {'0', Key::NUM_0}, {'1', Key::NUM_1}, {'2', Key::NUM_2},
        {'3', Key::NUM_3}, {'4', Key::NUM_4}, {'5', Key::NUM_5},
        {'6', Key::NUM_6}, {'7', Key::NUM_7}, {'8', Key::NUM_8},
        {'9', Key::NUM_9},
        {'a', Key::A}, {'b', Key::B}, {'c', Key::C}, {'d', Key::D},
        {'e', Key::E}, {'f', Key::F}, {'g', Key::G}, {'h', Key::H},
        {'i', Key::I}, {'j', Key::J}, {'k', Key::K}, {'l', Key::L},
        {'m', Key::M}, {'n', Key::N}, {'o', Key::O}, {'p', Key::P},
        {'q', Key::Q}, {'r', Key::R}, {'s', Key::S}, {'t', Key::T},
        {'u', Key::U}, {'v', Key::V}, {'w', Key::W}, {'x', Key::X},
        {'y', Key::Y}, {'z', Key::Z},

        {KEY_F(1), Key::PREV_GRAPHICS},
        {KEY_F(2), Key::NEXT_GRAPHICS},
        {KEY_F(3), Key::PREV_GAME},
        {KEY_F(4), Key::NEXT_GAME},
        {KEY_F(5), Key::RESTART},
        {KEY_F(6), Key::MENU},
        {KEY_F(12), Key::EXIT}
    };

    return newMap;
}

MouseEvent NCurses::getMouseEvent()
{
    int ch = getch();
    if (ch != KEY_MOUSE) {
        return MouseEvent();
    }

    MEVENT event;
    if (getmouse(&event) != OK) {
        return MouseEvent();
    }

    Key button = Key::UNKNOWN;
    if (event.bstate & BUTTON1_PRESSED)
        button = Key::MOUSE_LEFT;
    else if (event.bstate & BUTTON2_PRESSED)
        button = Key::MOUSE_MIDDLE;
    else if (event.bstate & BUTTON3_PRESSED)
        button = Key::MOUSE_RIGHT;
    return MouseEvent(Position(static_cast<float>(event.x), static_cast<float>(event.y)), button);
}

void NCurses::setTitle(const std::string& title)
{
    std::cout << "NCurses setTitle: " << title << "\n";
}

void NCurses::setWindowSize(int w, int h)
{
    std::cout << "NCurses setWindowSize: width=" << w << ", height=" << h << "\n";
    if (_window)
        resizeterm(w, h);
    else
        throw ModuleException("Can not resize window");
}

void NCurses::setFullscreen(bool fullscreen)
{
    std::cout << "NCurses setFullscreen\n";
    _fullscreen = fullscreen;

    // int x = 0; // rename variables into width and height
    // int y = 0;

    // getmaxyx(_window, y, x);
    // wresize(_window, y, x);
}

void NCurses::drawEntity(const Entity& e)
{
    std::cout << "NCurses drawEntity\n";
    mvprintw(e.pos.x, e.pos.y, "%c", e.character);
}

void NCurses::drawText(const Text& t)
{
    std::cout << "NCurses drawText\n";
    mvprintw(t.pos.x, t.pos.y, "%s", t.content);
}

void NCurses::drawRectangle(const Rect& rect, const Color& color)
{
    init_pair()
    for (size_t y = 0; )
    std::cout << "NCurses drawRectangle\n";
}

const std::string& NCurses::getName() const
{
    static std::string name = "nCurses";
    return name;
}

bool NCurses::isOpen() const
{
    std::cout << "NCurses isOpen\n";
    return true;
}

float NCurses::getDeltaTime() const
{
    std::cout << "NCurses getDeltaTime\n";
    return 0.0f;
}

int NCurses::getWidth() const
{
    std::cout << "NCurses getWidth\n";
    return 0;
}

int NCurses::getHeight() const
{
    std::cout << "NCurses getHeight\n";
    return 0;
}

void NCurses::setRenderContext(const RenderContext& context)
{
    std::cout << "NCurses setRenderContext\n";
}

}
