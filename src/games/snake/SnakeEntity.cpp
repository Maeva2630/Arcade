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
    if (2 != abs(getHead().face - f)) {
        getHeadReference().face = f;
    } 
}

Snake::SnakeSegment Snake::getNextHead() const
{
    SnakeSegment head = getHead();

    int x = static_cast<int>(head.pos.x) % MAX_GRID_X;
    int y = static_cast<int>(head.pos.y) % MAX_GRID_Y;
    
    if (Face::EAST  == head.face) { x =  MAX_GRID_X <= x+1 ? 0 : x+1; }
    else if (Face::WEST  == head.face) { x =  x <= 0 ? MAX_GRID_X -1 : x-1; }
    else if (Face::NORTH == head.face) { y =  y <= 0 ? MAX_GRID_Y -1 : y-1; }
    else if (Face::SOUTH == head.face) { y =  MAX_GRID_Y <= y+1 ? 0 : y+1; }
    
    return SnakeSegment{ Position{x, y},  head.face};
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