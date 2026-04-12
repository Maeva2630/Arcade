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

Sfml::Sfml() : _window(nullptr), _font(nullptr)
{
}

Sfml::~Sfml()
{
    if (_window || _font || !_fontMap.empty() || !_textureMap.empty())
        stop();
}

void Sfml::init()
{
    _window = new sf::RenderWindow(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "SFML Window");
    if (!_window || !_window->isOpen()) {
        delete _window;
        _window = nullptr;
        throw ModuleException("Sfml window creation has failed");
    }

    _window->setKeyRepeatEnabled(false);

    _font = new sf::Font();
    if (!_font || !_font->loadFromFile("assets/font.ttf")) {
        delete _font;
        _font = nullptr;
        delete _window;
        _window = nullptr;
        throw ModuleException("Failed to load default font");
    }

    _keyMap = initKeyFromEnum();
     _deltaTime = 0.0f;
    _isOpen = true;
}

std::map<int, Key> Sfml::initKeyFromEnum()
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
    if (_window) {
        _window->close();
        delete _window;
        _window = nullptr;
    }

    for (auto &entry : _fontMap) {
        if (entry.second)
            delete entry.second;
    }
    _fontMap.clear();

    for (auto &entry : _textureMap) {
        if (entry.second)
            delete entry.second;
    }
    _textureMap.clear();

    if (_font) {
        delete _font;
        _font = nullptr;
    }
    _spriteMap.clear();
    _isOpen = false;
}

void Sfml::setTitle(const std::string& title)
{
    if (!_window)
        throw ModuleException("Window is not initialized");
    _window->setTitle(title);
}

void Sfml::setFullscreen(bool fullscreen)
{
    if (fullscreen == _fullscreen)
        return;
    _fullscreen = fullscreen;

    if (!_window)
        throw ModuleException("Can not change window to full screen mode");
    if (fullscreen) {
        _window->create(sf::VideoMode::getDesktopMode(), "SFML Window", sf::Style::Fullscreen);
    } else {
        _winSize = Position(static_cast<float>(WIN_SIZE_X), static_cast<float>(WIN_SIZE_Y));
        _window->create(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "SFML Window", sf::Style::Default);
    }
    _window->setKeyRepeatEnabled(false);
    sf::Vector2u newSize = _window->getSize();
    _winSize = Position(static_cast<float>(newSize.x), static_cast<float>(newSize.y));
}

void Sfml::setWindowSize(int w, int h)
{
    if (_window) {
        _winSize = {static_cast<float>(w), static_cast<float>(h)};
        _window->create(sf::VideoMode(w, h), "SFML Window");
        if (!_window)
            throw ModuleException("Window creation has failed");
    }
    else
        throw ModuleException("Can not resize window");
}

void Sfml::setRenderContext(const RenderContext &context)
{
    _renderContext = context;
}

const std::string& Sfml::getName() const
{
    static std::string name = "SFML";
    return name;
}

Key Sfml::getKey()
{
    _window->pollEvent(_event);

    if (_event.type != sf::Event::KeyPressed)
        return Key::UNKNOWN;

    auto it = _keyMap.find(_event.key.code);
    return (it != _keyMap.end()) ? it->second : Key::UNKNOWN;
}

MouseEvent Sfml::getMouseEvent()
{
    _window->pollEvent(_event);
    if (_event.type != sf::Event::MouseButtonPressed)
        return MouseEvent();

    Key button = Key::UNKNOWN;
    if (_event.mouseButton.button == sf::Mouse::Button::Left)
        button = Key::MOUSE_LEFT;
    else if (_event.mouseButton.button == sf::Mouse::Button::Middle)
        button = Key::MOUSE_MIDDLE;
    else if (_event.mouseButton.button == sf::Mouse::Button::Right)
        button = Key::MOUSE_RIGHT;

    float gridX = _event.mouseButton.x;
    float gridY = _event.mouseButton.y;

    if (_renderContext.space == CoordinateSpace::GRID) {
        float cellW = static_cast<float>(_winSize.x)  / static_cast<float>(_renderContext.width);
        float cellH = static_cast<float>(_winSize.y) / static_cast<float>(_renderContext.height);

        gridX = static_cast<float>(_event.mouseButton.x) / cellW;
        gridY = static_cast<float>(_event.mouseButton.y) / cellH;
    }

    return MouseEvent(
        Position(static_cast<float>(gridX), static_cast<float>(gridY)),
        button
    );
}

sf::Font *Sfml::getFontForText(const Text &t)
{
    if (t.fontPath.empty() && t.fontSize <= 0)
        return _font;

    const std::string fontPath = t.fontPath.empty() ? "assets/font.ttf" : t.fontPath;
    const int fontSize = (t.fontSize > 0) ? t.fontSize : 36;
    const std::pair<std::string, int> key = {fontPath, fontSize};

    auto it = _fontMap.find(key);
    if (it != _fontMap.end())
        return it->second;

    sf::Font *loadedFont = new sf::Font();
    if (!loadedFont || !loadedFont->loadFromFile(fontPath)) {
        delete loadedFont;
        return _font;
    }

    _fontMap[key] = loadedFont;
    return loadedFont;
}

sf::Color Sfml::colorConvert(Color color)
{
    sf::Color newColor;

    newColor.a = color.a;
    newColor.r = color.r;
    newColor.g = color.g;
    newColor.b = color.b;

    return newColor;
}

sf::Texture *Sfml::getTexture(const std::string &texturePath)
{
    auto cached = _textureMap.find(texturePath);
    if (cached != _textureMap.end())
        return cached->second;

    sf::Texture *texture = new sf::Texture();
    if (!texture || !texture->loadFromFile(texturePath)) {
        delete texture;
        return nullptr;
    }

    texture->setRepeated(true);
    texture->setSmooth(true);
    _textureMap[texturePath] = texture;
    return texture;
}

Sfml::SpriteDefinition Sfml::getSprite(const Entity &e)
{
    auto sprite = _spriteMap.find(e.spritePath);
    if (sprite != _spriteMap.end())
        return sprite->second;

    SpriteDefinition nwSprite = SpriteDefinition(e.spritePath);
    _spriteMap[e.spritePath] = nwSprite;
    return nwSprite;
}

void Sfml::drawText(const Text& t)
{
    Position textPos = convertGridPosition(t.pos, static_cast<size_t>(_winSize.y), static_cast<size_t>(_winSize.x));
    sf::Color color = colorConvert(t.color);
    sf::Font *currentFont = getFontForText(t);
    if (!currentFont)
        return;

    sf::Text text;
    text.setFont(*currentFont);
    text.setString(t.content.c_str());
    const int fontSize = (t.fontPath.empty() && t.fontSize <= 0) ? 12 : ((t.fontSize > 0)
    ? t.fontSize : 36);
    text.setCharacterSize(fontSize);

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left, bounds.top);
    text.setPosition(sf::Vector2f(textPos.x, textPos.y));

    text.setFillColor(color);
    _window->draw(text);
}

void Sfml::drawRectangle(const Rect &rect, const Color &color)
{
    Position rectPos = convertGridPosition(Position(rect.x, rect.y), _winSize.y, _winSize.x);
    Position rectSize = convertGridSize(Position(rect.width, rect.height), _winSize.y, _winSize.x);
    sf::Color sfmlColor = colorConvert(color);

    sf::RectangleShape nwRect(sf::Vector2f(rectPos.x, rectPos.y));
    nwRect.setSize(sf::Vector2f(rectSize.x, rectSize.y));

    nwRect.setPosition(sf::Vector2f(rectPos.x, rectPos.y));
    nwRect.setFillColor(sfmlColor);
    _window->draw(nwRect);
}

void Sfml::drawEntity(const Entity& e)
{
    const Position entityPos = convertGridPosition(e.pos, _winSize.y, _winSize.x);
    const Position entitySize = convertGridSize(e.size, _winSize.y, _winSize.x);
    const Position dstSize = {
        (entitySize.x <= 0.0f) ? 1.0f : entitySize.x,
        (entitySize.y <= 0.0f) ? 1.0f : entitySize.y
    };

    if (e.spritePath.empty()) {
        Rect entityRect(static_cast<int>(e.pos.x), static_cast<int>(e.pos.y),
                        static_cast<int>(e.size.x), static_cast<int>(e.size.y));
        drawRectangle(entityRect, e.color);
        return;
    }
    SpriteDefinition spriteDef = getSprite(e);
    sf::Texture *texture = getTexture(spriteDef.texturePath);
    if (!texture) {
        Rect entityRect(static_cast<int>(e.pos.x), static_cast<int>(e.pos.y),
                        static_cast<int>(e.size.x), static_cast<int>(e.size.y));
        drawRectangle(entityRect, e.color);
        return;
    }

    sf::Sprite sprite(*texture);
    Position src = {
        static_cast<float>(texture->getSize().x),
        static_cast<float>(texture->getSize().y)
    };

    if (spriteDef.useSrcRect) {
        sprite.setTextureRect(spriteDef.srcRect);
        src.x = static_cast<float>(spriteDef.srcRect.width);
        src.y = static_cast<float>(spriteDef.srcRect.height);
    }
    sprite.setPosition(entityPos.x, entityPos.y);

    if (dstSize.x > 0.0f && dstSize.y > 0.0f && src.x > 0.0f && src.y > 0.0f)
        sprite.setScale(dstSize.x / src.x, dstSize.y / src.y);
    _window->draw(sprite);
}

void Sfml::updateDeltaTime()
{
    sf::Time time = _clock.restart();

    _deltaTime = time.asSeconds();
}

void Sfml::clear()
{
    _window->clear();
}

void Sfml::display()
{
    updateDeltaTime();
    _window->display();
}

bool Sfml::isOpen() const
{
    return _isOpen;
}

float Sfml::getDeltaTime() const
{
    return _deltaTime;
}

int Sfml::getWidth() const
{
    return convertToCell(_winSize.x);
}

int Sfml::getHeight() const
{
    return convertToCell(_winSize.y);
}

}
