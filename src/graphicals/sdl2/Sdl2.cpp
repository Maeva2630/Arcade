/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sdl2.cpp
*/

#include "graphicals/Sdl2.hpp"

extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::Sdl2();
}

namespace arcade
{

Sdl2::Sdl2() : _window(nullptr), _renderer(nullptr), _font(nullptr), _lastTicks(0)
{
}

Sdl2::~Sdl2()
{
    if (_window || _renderer || _font)
        stop();
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
    _lastTicks = SDL_GetTicks64();
    _deltaTime = 0.0f;
    _isOpen = true;
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
    const int fontSize = (t.fontSize > 0) ? t.fontSize : 36;
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

SDL_Texture *Sdl2::getTexture(const std::string &texturePath)
{
    auto cached = _textureMap.find(texturePath);
    if (cached != _textureMap.end())
        return cached->second;

    SDL_Surface *surface = SDL_LoadBMP(texturePath.c_str());
    if (!surface)
        return nullptr;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
        return nullptr;

    _textureMap[texturePath] = texture;
    return texture;
}

Sdl2::SpriteDefinition Sdl2::getSprite(const Entity &e) const
{
    auto sprite = _spriteMap.find(e.spritePath);
    if (sprite != _spriteMap.end())
        return sprite->second;

    return SpriteDefinition(e.spritePath);
}

void Sdl2::stop()
{
    if (!_window && !_renderer && !_font && _fontMap.empty() && _textureMap.empty())
        return;

    for (auto &entry : _fontMap) {
        if (entry.second)
            TTF_CloseFont(entry.second);
    }
    _fontMap.clear();

    for (auto &entry : _textureMap) {
        if (entry.second)
            SDL_DestroyTexture(entry.second);
    }
    _textureMap.clear();
    _spriteMap.clear();

    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
    TTF_Quit();

    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
    _isOpen = false;
}

void Sdl2::clear()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Sdl2::updateDeltaTime()
{
    const Uint64 now = SDL_GetTicks64();

    if (_lastTicks == 0)
        _deltaTime = 0.0f;
    else
        _deltaTime = static_cast<float>(now - _lastTicks) / 1000.0f;
    _lastTicks = now;
}

void Sdl2::display()
{
    updateDeltaTime();
    SDL_RenderPresent(_renderer);
}

void Sdl2::drawRectangle(const Rect &rect, const Color &color)
{
    Position rectPos = convertGridPosition(Position(rect.x, rect.y), _winSize.y, _winSize.x);
    Position rectSize = convertGridSize(Position(rect.width, rect.height), _winSize.y, _winSize.x);

    SDL_Rect sdl_rect = {
        convertToCell(rectPos.x),
        convertToCell(rectPos.y),
        convertToCell(rectSize.x),
        convertToCell(rectSize.y)
    };
    
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(_renderer, &sdl_rect); 
}

void Sdl2::setRenderContext(const RenderContext &context)
{
    _renderContext = context;
}

void Sdl2::setWindowSize(int w, int h)
{
    if (_window) {
        _winSize = {static_cast<float>(w), static_cast<float>(h)};
        SDL_SetWindowSize(_window, w, h);
    }
    else
        throw ModuleException("Can not resize window");
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

void Sdl2::setFullscreen(bool fullscreen)
{
    _fullscreen = fullscreen;

    if (_window) {
        SDL_SetWindowFullscreen(_window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        int newSizeX, newSizeY = 0;
        SDL_GetWindowSize(_window, &newSizeX, &newSizeY); 
        _winSize = Position(static_cast<float>(newSizeX), static_cast<float>(newSizeY));
    } else
        throw ModuleException("Can not change window to full screen mode");
}

void Sdl2::drawEntity(const Entity& e)
{
    Position entityPos = convertGridPosition(e.pos, _winSize.y, _winSize.x);
    Position entitySize = convertGridSize(e.size, _winSize.y, _winSize.x);

    const int startX = convertToCell(entityPos.x);
    const int startY = convertToCell(entityPos.y);
    const int width = (entitySize.x <= 0.0f) ? 1 : convertToCell(entitySize.x);
    const int height = (entitySize.y <= 0.0f) ? 1 : convertToCell(entitySize.y);

    if (e.spritePath.empty()) {
        Rect entityRect = {e.pos.x, e.pos.y, e.size.x, e.size.y};
        drawRectangle(entityRect, e.color);
        return;
    }

    SpriteDefinition sprite = getSprite(e);
    SDL_Texture *texture = getTexture(sprite.texturePath);
    if (!texture) {
        Rect entityRect = {e.pos.x, e.pos.y, e.size.x, e.size.y};
        drawRectangle(entityRect, e.color);
        return;
    }

    SDL_Rect dstRect = {startX, startY, width, height};

    if (sprite.useSrcRect) {
        SDL_Rect srcRect = {
            sprite.srcRect.x,
            sprite.srcRect.y,
            sprite.srcRect.width,
            sprite.srcRect.height
        };
        SDL_RenderCopy(_renderer, texture, &srcRect, &dstRect);
    } else {
        SDL_RenderCopy(_renderer, texture, nullptr, &dstRect);
    }
}

const std::string& Sdl2::getName() const
{
    static std::string name = "SDL2";
    return name;
}

bool Sdl2::isOpen() const
{
    return _isOpen;
}

float Sdl2::getDeltaTime() const
{
    return _deltaTime;
}

int Sdl2::getWidth() const
{
    return convertToCell(_winSize.x);
}

int Sdl2::getHeight() const
{
    return convertToCell(_winSize.y);
}

}
