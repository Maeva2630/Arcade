/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** CoreMenu.cpp
*/

#include "core/Core.hpp"

namespace arcade {

void Core::renderPlayerName()
{
    _menuY += SPACING;
    _display->drawText(Text("Player Name: (authorized characters: a-z, 0-9)", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    if (_playerName.empty())
        _display->drawText(Text("Enter your name...", _menuX + 2, _menuY += SPACING, (_launchWithoutName) ? Colors::RED : Colors::WHITE, FONTSIZE));
    else
        _display->drawText(Text(_playerName, _menuX + 2, _menuY += SPACING, Colors::WHITE, FONTSIZE));

}

void Core::applyMenuSelection()
{
    if (!_inCoreMenu)
        return;
    if (_playerName.empty()) {
        _launchWithoutName = true;
        return;
    }
    if (_game->getState() == GameState::PAUSED)
        _game->resume();
    _inCoreMenu = false;
    _launchWithoutName = false;
}

void Core::renderMenu()
{
    _menuX = 0;
    _menuY = 0;

    _display->clear();
    _display->drawText(Text("=== ARCADE ===", _menuX, _menuY, Colors::YELLOW, FONTSIZE + 10));

    renderPlayerName();
    renderHighScores();
    renderSelection();
    
    // Helper
    _display->drawText(
        Text("Enter: launch | Escape: exit", _menuX, _menuY += SPACING, Colors::WHITE, FONTSIZE - 8));
}

}
