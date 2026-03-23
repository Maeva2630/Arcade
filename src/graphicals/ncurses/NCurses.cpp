/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NCurses.cpp
*/

#include "graphicals/NCurses.hpp"
#include <iostream>

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
}

MouseEvent NCurses::getMouseEvent()
{
    std::cout << "NCurses getMouseEvent\n";
}

void NCurses::setTitle(std::string& title)
{
    std::cout << "NCurses setTitle: " << title << "\n";
}

void NCurses::setWindowSize(int w, int h)
{
    std::cout << "NCurses setWindowSize: width=" << w << ", height=" << h << "\n";
    // resizeterm(w, h);
}

void NCurses::setFullscreeen()
{
    std::cout << "NCurses setFullscreen\n";
    // int x = 0; // rename variables into width and height
    // int y = 0;

    // getmaxyx(_window, y, x);
    // wresize(_window, y, x);
}

Key NCurses::keyToEnum(int ch)
{
    std::cout << "NCurses keyToEnum\n";
}

int NCurses::colorPair(Color c)
{
    std::cout << "NCurses colorPair\n";
}

}
