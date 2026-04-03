/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeEntity.cpp
*/

#include "games/snake/SnakeEntity.hpp"

namespace arcade
{

Snake::Snake() : _isALive(true)
{
    _snakeSegment.clear();

    for (int i = 0; i < START_SNAKE_LENGTH; ++i) {
        SnakeSegment segment;
        segment.pos = { START_HEAD_X - i, START_HEAD_Y };
        segment.face = Face::NORTH;
        _snakeSegment.emplace_back(segment);
    }
}

Snake::SnakeSegment &Snake::getHeadReference()
{
    return _snakeSegment.back();
}

Snake::SnakeSegment Snake::getHead() const
{
    return _snakeSegment.back();
}

void Snake::setFace(Face f) 
{ 
    Face current = getHead().face;
    int diff = static_cast<int>(current) - static_cast<int>(f);
    if (diff != 2 && diff != -2) {
        getHeadReference().face = f;
    }
}

Snake::SnakeSegment Snake::getNextHead() const
{
    SnakeSegment head = getHead();
    int x = static_cast<int>(head.pos.x);
    int y = static_cast<int>(head.pos.y);

    if (head.face == Face::EAST)
        x = (x >= MAX_GRID_X - 1) ? 1 : x + 1;
    else if (head.face == Face::WEST)
        x = (x <= 1) ? MAX_GRID_X - 1 : x - 1;
    else if (head.face == Face::NORTH)
        y = (y <= 1) ? MAX_GRID_Y - 1 : y - 1;
    else if (head.face == Face::SOUTH)
        y = (y >= MAX_GRID_Y - 1) ? 1 : y + 1;

    return SnakeSegment{ Position{ static_cast<float>(x), static_cast<float>(y) }, head.face };
}

size_t Snake::getSegmentSize() const
{
    return _snakeSegment.size();
}

const std::deque<Snake::SnakeSegment> &Snake::getSegments() const
{
    return _snakeSegment;
}

bool Snake::containsAtThisPos(Position pos) const
{
    for (const auto &segment : _snakeSegment) {
        if (segment.pos.x == pos.x && segment.pos.y == pos.y)
            return true;
    }
    return false;
}

void Snake::setAlive(bool isAlive)
{
    _isALive = isAlive;
}

void Snake::updateBody(bool hasEaten, SnakeSegment nextHead)
{
    if (!hasEaten) {
        _snakeSegment.pop_front();
    }
    _snakeSegment.push_back(nextHead);
}

}