/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sdl2.cpp
*/

#include "graphicals/Sdl2.hpp"

namespace arcade
{

Sdl2::Sdl2() : _window(nullptr), _renderer(nullptr), _font(nullptr)
{
}

void Sdl2::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        throw ModuleException("Sdl init has failed");
    }
    _window = SDL_CreateWindow("SDL Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_SIZE_X, WIN_SIZE_Y,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!_window)
        throw ModuleException("SDL window creation failed");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
    if (!_renderer)
        throw ModuleException("SDL renderer creation failed");
    
    if (TTF_Init() == -1) {
        throw ModuleException("SDL_ttf init failed");
    }

    _font = TTF_OpenFont("assets/font.ttf", 12);
    if (!_font) {
        throw ModuleException("Failed to load default font");
    }

    _keyMap = initKeyFromEnum();
}

std::map<int, Key> Sdl2::initKeyFromEnum()
{
    std::map<int, Key> newMap = {
        {SDLK_UP, Key::UP},
        {SDLK_DOWN, Key::DOWN},
        {SDLK_LEFT, Key::LEFT},
        {SDLK_RIGHT, Key::RIGHT},
        {SDLK_ESCAPE, Key::ESCAPE},
        {SDLK_SPACE, Key::SPACE},
        {'\n', Key::ENTER},
        {SDLK_BACKSPACE, Key::BACKSPACE},

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

        {SDLK_F1, Key::PREV_GRAPHICS},
        {SDLK_F2, Key::NEXT_GRAPHICS},
        {SDLK_F3, Key::PREV_GAME},
        {SDLK_F4, Key::NEXT_GAME},
        {SDLK_F5, Key::RESTART},
        {SDLK_F6, Key::MENU},
        {SDLK_F12, Key::EXIT}
    };

    return newMap;
}

SDL_Color Sdl2::colorConvert(Color color)
{
    SDL_Color newColor;

    newColor.a = color.a;
    newColor.r = color.r;
    newColor.g = color.g;
    newColor.b = color.b;

    return newColor;
}

TTF_Font *Sdl2::getFontForText(const Text &t)
{
    if (t.fontPath.empty() && t.fontSize <= 0)
        return _font;

    const std::string fontPath = t.fontPath.empty() ? "assets/font.ttf" : t.fontPath;
    const int fontSize = (t.fontSize > 0) ? t.fontSize : 12;
    const std::pair<std::string, int> key = {fontPath, fontSize};

    auto it = _fontMap.find(key);
    if (it != _fontMap.end())
        return it->second;

    TTF_Font *loadedFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!loadedFont)
        return _font;

    _fontMap[key] = loadedFont;
    return loadedFont;
}

void Sdl2::stop()
{
    for (auto &entry : _fontMap) {
        if (entry.second)
            TTF_CloseFont(entry.second);
    }
    _fontMap.clear();

    if (_font)
        TTF_CloseFont(_font);
    TTF_Quit();

    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Sdl2::clear()
{
    SDL_RenderClear(_renderer);
}

void Sdl2::display()
{
    SDL_RenderPresent(_renderer);
}

void Sdl2::drawRectangle(const Rect &rect, const Color &color)
{
    Position rectPos = convertGridPosition(Position(rect.x, rect.y), _winSize.y, _winSize.x);
    Position rectSize = convertGridSize(Position(rect.width, rect.height), _winSize.y, _winSize.x);

    SDL_Rect sdl_rect = {rect.x, rect.y, rect.width, rect.height};
    
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(_renderer, &sdl_rect); 
}

void Sdl2::setRenderContext(const RenderContext &context)
{
    _renderContext = context;
}

void Sdl2::setWindowSize(int w, int h)
{
    if (!_window)
        throw ModuleException("Can not resize window");

    SDL_SetWindowSize(_window, w, h);
    _winSize = Position(static_cast<float>(w), static_cast<float>(h));
}

void Sdl2::drawText(const Text& t)
{

    Position textPos = convertGridPosition(t.pos, static_cast<size_t>(_winSize.y), static_cast<size_t>(_winSize.x));
    SDL_Color color = colorConvert(t.color);
    TTF_Font *currentFont = getFontForText(t);
    if (!currentFont)
        return;

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(currentFont, t.content.c_str(), color);
    if (!textSurface)
        return;

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture)
        return;

    int textWidth = 0;
    int textHeight = 0;
    if (SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight) != 0) {
        SDL_DestroyTexture(textTexture);
        return;
    }

    SDL_Rect dst = { static_cast<int>(textPos.x), static_cast<int>(textPos.y), textWidth, textHeight };

    SDL_RenderCopy(_renderer, textTexture, nullptr, &dst);
    SDL_DestroyTexture(textTexture);
}

Key Sdl2::getKey()
{
    if (SDL_PollEvent(&_event) <= 0 || _event.type != SDL_KEYDOWN || _event.key.repeat != 0)
        return Key::UNKNOWN;

    auto it = _keyMap.find(_event.key.keysym.sym);
    return (it != _keyMap.end()) ? it->second : Key::UNKNOWN;
}

MouseEvent Sdl2::getMouseEvent()
{
    if (SDL_PollEvent(&_event) <= 0 || _event.type != SDL_MOUSEBUTTONDOWN)
        return MouseEvent();

    Key button = Key::UNKNOWN;
    if (_event.button.button == SDL_BUTTON_LEFT)
        button = Key::MOUSE_LEFT;
    else if (_event.button.button == SDL_BUTTON_MIDDLE)
        button = Key::MOUSE_MIDDLE;
    else if (_event.button.button == SDL_BUTTON_RIGHT)
        button = Key::MOUSE_RIGHT;

    return MouseEvent(
        Position(static_cast<float>(_event.button.x), static_cast<float>(_event.button.y)),
        button
    );
}

void Sdl2::setTitle(const std::string& title)
{
    if (!_window)
        throw ModuleException("Window is not initialized");
    SDL_SetWindowTitle(_window, title.c_str());
}



}
