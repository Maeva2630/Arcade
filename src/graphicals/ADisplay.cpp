/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** ADisplay.cpp
*/
#include "graphicals/ADisplayModule.hpp"

namespace arcade
{

int ADisplayModule::convertToCell(float value)
{
    return static_cast<int>(value);
}

Position ADisplayModule::convertGridPosition(Position gridPos, size_t screenHeight, size_t screenWidth)
{
    if (_renderContext.space != CoordinateSpace::GRID) {
        return gridPos;
    }
    if (_renderContext.width <= 0 || _renderContext.height <= 0) {
        return gridPos;
    }

    const Position screen = { static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
    const Position world = { static_cast<float>(_renderContext.width), static_cast<float>(_renderContext.height) };
    const Position scalePos = { screen.x / world.x,  screen.y / world.y  };
    const float scale = (scalePos.x < scalePos.y) ? scalePos.x : scalePos.y;
    const Position scaledWorld = { world.x * scale, world.y * scale };
    const Position offset = { (screen.x - scaledWorld.x) / 2.0f, (screen.y - scaledWorld.y) / 2.0f };

    return Position{offset.x + (gridPos.x * scale), offset.y + (gridPos.y * scale)};
}

Position ADisplayModule::convertGridSize(Position gridSize, size_t screenHeight, size_t screenWidth)
{
    if (_renderContext.space != CoordinateSpace::GRID) {
        return gridSize;
    }
    if (_renderContext.width <= 0 || _renderContext.height <= 0) {
        return gridSize;
    }

    const Position screen = { static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
    const Position world = { static_cast<float>(_renderContext.width), static_cast<float>(_renderContext.height) };
    const Position scalePos = { screen.x / world.x, screen.y / world.y };

    const float scaleX = scalePos.x;
    const float scaleY = scalePos.y;
    const float scale = (scaleX < scaleY) ? scaleX : scaleY;

    return Position{gridSize.x * scale, gridSize.y * scale};
}

}
