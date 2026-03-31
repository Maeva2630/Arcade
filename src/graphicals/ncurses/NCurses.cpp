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

NCurses::NCurses() : _window(nullptr), _nbPairs(0), _lastTime(std::chrono::steady_clock::now())
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
    _colorMap = initColorMap();
    _isOpen = true;
    noecho();
}

void NCurses::stop()
{
    std::cout << "NCurses stop\n";
    if (_window) {
        endwin();
        _window = nullptr;
        _isOpen = false;
    } else
        throw ModuleException("The window can not be closed");
}

void NCurses::clear()
{
    std::cout << "NCurses clear\n";
    if (_window)
        ::clear();
    else
        throw ModuleException("The window can not be cleared");
}

void NCurses::display()
{
    std::cout << "NCurses display\n";
    if (_window) {
        updateDeltaTime();
        refresh();
    } else
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
        resizeterm(h, w);
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
    size_t colorCode = checkColorPairs(e.backgroundColor, e.color);
    std::cout << "NCurses drawEntity\n";

    Position entityPos = convertGridPosition(e.pos, LINES, COLS);
    Position entitySize = convertGridSize(e.size, LINES, COLS);

    const int startX = convertToCell(entityPos.x);
    const int startY = convertToCell(entityPos.y);
    const int width = (entitySize.x <= 0.0f) ? 1 : convertToCell(entitySize.x);
    const int height = (entitySize.y <= 0.0f) ? 1 : convertToCell(entitySize.y);

    attron(COLOR_PAIR(colorCode));
    for (int y = startY; y < startY + height; y++) {
        for (int x = startX; x < startX + width; x++) {
            mvaddch(y, x, e.character);
        }
    }
    attroff(COLOR_PAIR(colorCode));
}

void NCurses::drawText(const Text& t)
{
    size_t colorCode = checkColorPairs(t.color);
    std::cout << "NCurses drawText\n";

    Position textPos = convertGridPosition(t.pos, LINES, COLS);

    attron(COLOR_PAIR(colorCode));
    mvprintw(convertToCell(textPos.y), convertToCell(textPos.x), "%s", t.content.c_str());
    attroff(COLOR_PAIR(colorCode));
}

void NCurses::drawRectangle(const Rect& rect, const Color& color)
{
    size_t colorCode = checkColorPairs(color);
    std::cout << "NCurses drawRectangle\n";

    Position rectPos = convertGridPosition(Position(rect.x, rect.y), LINES, COLS);
    Position rectSize = convertGridSize(Position(rect.width, rect.height), LINES, COLS);

    const int startX = convertToCell(rectPos.x);
    const int startY = convertToCell(rectPos.y);
    const int width = convertToCell(rectSize.x);
    const int height = convertToCell(rectSize.y);

    attron(COLOR_PAIR(colorCode));
    for (int y = startY; y <= startY + height; y++) {
        for (int x = startX; x <= startX + width; x++) {
            mvaddch(y, x, '*');
        }
    }
    attroff(COLOR_PAIR(colorCode));
}

const std::string& NCurses::getName() const
{
    static std::string name = "nCurses";
    return name;
}

bool NCurses::isOpen() const
{
    std::cout << "NCurses isOpen\n";
    return _isOpen;
}

float NCurses::getDeltaTime() const
{
    std::cout << "NCurses getDeltaTime\n";
    return _deltaTime;
}

float NCurses::updateDeltaTime()
{
    auto current = std::chrono::steady_clock::now();
    _deltaTime = std::chrono::duration<float>(current - _lastTime).count();
    _lastTime = current;
    return _deltaTime;
}

int NCurses::getWidth() const
{
    std::cout << "NCurses getWidth\n";
    
    return COLS;
}

int NCurses::getHeight() const
{
    std::cout << "NCurses getHeight\n";
    
    return LINES;
}

void NCurses::setRenderContext(const RenderContext& context)
{
    std::cout << "NCurses setRenderContext\n";
    _renderContext = context;
}

//COLOR
uint32_t NCurses::colorToKey(const Color &color)
{
    return (static_cast<uint32_t>(color.r) << 24)
        | (static_cast<uint32_t>(color.g) << 16)
        | (static_cast<uint32_t>(color.b) << 8)
        | static_cast<uint32_t>(color.a);
}

uint64_t NCurses::colorPairToKey(const Color &foreground, const Color &background)
{
    return (static_cast<uint64_t>(colorToKey(foreground)) << 32)
        | static_cast<uint64_t>(colorToKey(background));
}

size_t NCurses::checkColorPairs(Color background, Color foreground)
{
    const uint64_t pairKey = colorPairToKey(foreground, background);
    auto check = _colorPairs.find(pairKey);
    const auto foregroundKey = colorToKey(foreground);
    const auto backgroundKey = colorToKey(background);

    auto fgIterator = _colorMap.find(foregroundKey);
    auto bgIterator = _colorMap.find(backgroundKey);

    const size_t fgCode = (fgIterator != _colorMap.end()) ? fgIterator->second : COLOR_WHITE;
    const size_t bgCode = (bgIterator != _colorMap.end()) ? bgIterator->second : COLOR_BLACK;

    if (check != _colorPairs.end()) {
        return check->second;
    } else {
        _nbPairs++;
        _colorPairs[pairKey] = _nbPairs;
        init_pair(_nbPairs, fgCode, bgCode);
    }
    return _nbPairs;
}

std::unordered_map<uint32_t, size_t> NCurses::initColorMap()
{
    std::unordered_map<uint32_t, size_t> newMap = {
        {colorToKey(Colors::BLACK), COLOR_BLACK},
        {colorToKey(Colors::BLUE), COLOR_BLUE},
        {colorToKey(Colors::CYAN), COLOR_CYAN},
        {colorToKey(Colors::GREEN), COLOR_GREEN},
        {colorToKey(Colors::MAGENTA), COLOR_MAGENTA},
        {colorToKey(Colors::RED), COLOR_RED},
        {colorToKey(Colors::WHITE), COLOR_WHITE},
        {colorToKey(Colors::YELLOW), COLOR_YELLOW},
    };

    return newMap;
}

}
