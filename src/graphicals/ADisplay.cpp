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

    const float screenW = static_cast<float>(screenWidth);
    const float screenH = static_cast<float>(screenHeight);
    const float worldW = static_cast<float>(_renderContext.width);
    const float worldH = static_cast<float>(_renderContext.height);

    const float scaleX = screenW / worldW;
    const float scaleY = screenH / worldH;
    const float scale = (scaleX < scaleY) ? scaleX : scaleY;

    const float scaledWorldW = worldW * scale;
    const float scaledWorldH = worldH * scale;
    const float offsetX = (screenW - scaledWorldW) / 2.0f;
    const float offsetY = (screenH - scaledWorldH) / 2.0f;

    return Position{offsetX + (gridPos.x * scale), offsetY + (gridPos.y * scale)};
}

Position ADisplayModule::convertGridSize(Position gridSize, size_t screenHeight, size_t screenWidth)
{
    if (_renderContext.space != CoordinateSpace::GRID) {
        return gridSize;
    }
    if (_renderContext.width <= 0 || _renderContext.height <= 0) {
        return gridSize;
    }

    const float screenW = static_cast<float>(screenWidth);
    const float screenH = static_cast<float>(screenHeight);
    const float worldW = static_cast<float>(_renderContext.width);
    const float worldH = static_cast<float>(_renderContext.height);

    const float scaleX = screenW / worldW;
    const float scaleY = screenH / worldH;
    const float scale = (scaleX < scaleY) ? scaleX : scaleY;

    return Position{gridSize.x * scale, gridSize.y * scale};
}

}