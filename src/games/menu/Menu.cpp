/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Menu.cpp
*/

#include "menu/Menu.hpp"

extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::MenuModule();
}

namespace arcade {

MenuModule::MenuModule()
{
    _currentState = GameState::MENU;
    _launchWithoutName = false;
}

void MenuModule::init()
{
    _currentState = GameState::MENU;
    _launchWithoutName = false;

    loadScores();
    refreshUI();
}

void MenuModule::stop()
{
    saveScore();
    _uiText.clear();
}

const std::string& MenuModule::getName() const
{
    return _name;
}

RenderContext MenuModule::getRenderContext() const
{
    return RenderContext{80, 60, CoordinateSpace::GRID};
}

void MenuModule::refreshUI()
{
    _uiText.clear();

    std::size_t x = 1;
    std::size_t y = SPACING / 2;

    _uiText.emplace_back("=== ARCADE ===", x, y, Colors::YELLOW, FONTSIZE * 2);

    renderPlayerName(x, y);
    renderHighScores(x, y);
    renderGamesLib(x, y);
    renderDisplaysLib(x, y);

    y += SPACING;
    _uiText.emplace_back("ENTER to start", x, y, Colors::GREEN, FONTSIZE - 5);
}

void MenuModule::renderPlayerName(std::size_t& x, std::size_t& y)
{
    y += SPACING + SPACING / 2;
    _uiText.emplace_back("Player:", x, y, Colors::CYAN, FONTSIZE);

    y += SPACING;
    _uiText.emplace_back(
        _playerName.empty() ? "Enter your name..." : _playerName,
        x + INDENTATION,
        y,
        _launchWithoutName ? Colors::RED : Colors::WHITE,
        FONTSIZE - 15
    );
}

void MenuModule::renderGamesLib(std::size_t& x, std::size_t& y)
{
    y += SPACING;
    _uiText.emplace_back("Games (F3/F4):", x, y, Colors::MAGENTA, FONTSIZE - 5);

    for (size_t i = 0; i < _games.size(); i++) {
        std::string prefix = (i == _selectedGame) ? "> " : "  ";
        _uiText.emplace_back(prefix + _games[i], x + 2, y += SPACING, Colors::WHITE, FONTSIZE - 10);
    }
}

void MenuModule::renderDisplaysLib(std::size_t& x, std::size_t& y)
{
    y += SPACING;
    _uiText.emplace_back("Graphics (F1/F2):", x, y, Colors::MAGENTA, FONTSIZE - 5);

    for (size_t i = 0; i < _displays.size(); i++) {
        std::string prefix = (i == _selectedDisplay) ? "> " : "  ";
        _uiText.emplace_back(prefix + _displays[i], x + 2, y += SPACING, Colors::WHITE, FONTSIZE - 10);
    }
}

}
