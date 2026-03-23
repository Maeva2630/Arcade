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

NCurses::NCurses()
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
    // _window = initscr();
}

void NCurses::stop()
{
    std::cout << "NCurses stop\n";
    // endwin();
    // free(_window);
}

void NCurses::clear()
{
    std::cout << "NCurses clear\n";
    // refresh();
}

void NCurses::display()
{
    std::cout << "NCurses display\n";
}

// void NCurses::drawEntity(Entity& e);
// void NCurses::drawText(Text& t);
// void NCurses::drawRectangle(int r, int c);

Key NCurses::getKey()
{
    // auto ch = getch();
    std::cout << "NCurses getKey\n";
    return Key::UNKNOWN;
}

MouseEvent NCurses::getMouseEvent()
{
    std::cout << "NCurses getMouseEvent\n";
    return MouseEvent();
}

void NCurses::setTitle(const std::string& title)
{
    std::cout << "NCurses setTitle: " << title << "\n";
}

void NCurses::setWindowSize(int w, int h)
{
    std::cout << "NCurses setWindowSize: width=" << w << ", height=" << h << "\n";
    // resizeterm(w, h);
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

Key NCurses::keyToEnum(int ch)
{
    std::cout << "NCurses keyToEnum\n";
    return Key::UNKNOWN;
}

int NCurses::colorPair(Color c)
{
    std::cout << "NCurses colorPair\n";
    return 0;
}

void NCurses::drawEntity(const Entity& e)
{
    std::cout << "NCurses drawEntity\n";
}

void NCurses::drawText(const Text& t)
{
    std::cout << "NCurses drawText\n";
}

void NCurses::drawRectangle(const Rect& rect, const Color& color)
{
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
