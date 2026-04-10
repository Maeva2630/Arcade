/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerEntities.cpp
*/
#include "games/nibbler/NibblerEntities.hpp"
#include <array>
#include <string>

namespace arcade
{

void NibblerEntities::appendFrame(std::vector<Entity> &entities)
{
    const std::size_t frameSize = 4;
    std::array<Entity, frameSize> frames;

    frames[0].pos = Position(0.0f, 2.0f);
    frames[0].size = Position(1.0f, END_ROW - 1);

    frames[1].pos = Position(0.0f, 2.0f);
    frames[1].size = Position(END_COL, 1.0f);

    frames[2].pos = Position(0.0f, END_ROW);
    frames[2].size = Position(END_COL, 1.0f);

    frames[3].pos = Position(END_COL, 2.0f);
    frames[3].size = Position(1.0f, END_ROW - 1);

    for (Entity &i : frames) {
        i.color = Colors::WHITE;
        i.backgroundColor = Colors::BLACK;
        i.character = '#';
        entities.emplace_back(i);
    }
}

void NibblerEntities::buildEntities(std::vector<Entity> &entities,
    const std::deque<Nibbler::NibblerSegment> &segments,
    const std::vector<Position> &food,
    const std::vector<Position> &walls)
{
    entities.clear();

    for (std::size_t i = 0; i < segments.size(); ++i) {
        Entity segmentEntity;
        segmentEntity.pos = segments[i].pos;
        segmentEntity.size = Position(1.0f, 1.0f);
        segmentEntity.character = (i + 1 == segments.size()) ? '@' : 'o';
        segmentEntity.color = (i + 1 == segments.size()) ? Colors::CYAN : Colors::MAGENTA;
        entities.emplace_back(segmentEntity);
    }

    for (const Position &i : food) {
        Entity foodEntity;
        foodEntity.pos = i;
        foodEntity.size = Position(1.0f, 1.0f);
        foodEntity.character = '*';
        foodEntity.color = Colors::RED;
        entities.emplace_back(foodEntity);
    }

    for (const Position &i : walls) {
        Entity wallEntity;
        wallEntity.pos = i;
        wallEntity.size = Position(1.0f, 1.0f);
        wallEntity.character = '#';
        wallEntity.color = Colors::WHITE;
        entities.emplace_back(wallEntity);
    }
}

void NibblerEntities::buildUI(std::vector<Text> &uiText,
    int score,
    GameState state,
    float fontSize)
{
    uiText.clear();

    std::string title = "NIBBLER";
    uiText.emplace_back(title, MAX_GRID_X / 2 - 3, 0.0f, Colors::YELLOW, fontSize);

    std::string scoreStr = "Score: " + std::to_string(score);
    uiText.emplace_back(scoreStr, 1.0f, 1.0f, Colors::WHITE, fontSize);

    if (state == GameState::PAUSED) {
        uiText.emplace_back("PAUSED", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::YELLOW, fontSize);
    } else if (state == GameState::GAME_OVER) {
        uiText.emplace_back("GAME OVER", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::RED, fontSize);
    } else if (state == GameState::WON) {
        uiText.emplace_back("YOU WIN!", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::GREEN, fontSize);
    }

    uiText.emplace_back("[Arrows: move] [P: pause] [R: restart] [Esc: exit]",
        1.0f, MAX_GRID_Y, Colors::CYAN, fontSize);
}

}
