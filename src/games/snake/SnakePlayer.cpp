/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeEntity.cpp
*/

#include "games/snake/SnakePlayer.hpp"

namespace arcade
{

static float normValue(int d)
{
    if (d > 1)
        return -1;
    if (d < -1)
        return 1;
    return d;
}

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

Snake::SnakeSegment Snake::getTail() const
{
    return _snakeSegment.front();
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
        x = (x >= MAX_GRID_X - 2) ? 1 : x + 1;
    else if (head.face == Face::WEST)
        x = (x <= 1) ? MAX_GRID_X - 2 : x - 1;
    else if (head.face == Face::NORTH)
        y = (y <= 3) ? MAX_GRID_Y - 3 : y - 1;
    else if (head.face == Face::SOUTH)
        y = (y >= MAX_GRID_Y - 3) ? 3 : y + 1;

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

std::string Snake::getSnakeHeadSprite() const
{
    Snake::SnakeSegment head = getHead();

    if (head.face == Snake::Face::EAST)
        return "assets/snake/green/head_right.png";
    else if (head.face == Snake::Face::WEST)
        return "assets/snake/green/head_left.png";
    else if (head.face == Snake::Face::NORTH)
        return "assets/snake/green/head_up.png";
    else if (head.face == Snake::Face::SOUTH)
        return "assets/snake/green/head_down.png";

    return "assets/snake/green/head_right.png";
}

std::string Snake::getSnakeTailSprite() const
{
    Snake::SnakeSegment tail = getTail();

    if (tail.face == Snake::Face::EAST)
        return "assets/snake/green/tail_right.png";
    else if (tail.face == Snake::Face::WEST)
        return "assets/snake/green/tail_left.png";
    else if (tail.face == Snake::Face::NORTH)
        return "assets/snake/green/tail_up.png";
    else if (tail.face == Snake::Face::SOUTH)
        return "assets/snake/green/tail_down.png";

    return "assets/snake/green/tail_right.png";
}

std::string Snake::getSnakeBodySprite(const Snake::SnakeSegment &prev,
    const Snake::SnakeSegment &current, const Snake::SnakeSegment &next) const
{
    Position prevNorm = {normValue(static_cast<int>(prev.pos.x - current.pos.x)),
        normValue(static_cast<int>(prev.pos.y - current.pos.y))};
    Position nextNorm = {normValue(static_cast<int>(next.pos.x - current.pos.x)),
        normValue(static_cast<int>(next.pos.y - current.pos.y))};

    if (prevNorm.y == 0 && nextNorm.y == 0)
        return "assets/snake/green/body_straight_horizontal.png";
    else if (prevNorm.x == 0 && nextNorm.x == 0)
        return "assets/snake/green/body_straight_vertical.png";

    const bool isLeft = (prevNorm.x == -1 || nextNorm.x == -1);
    const bool isRight = (prevNorm.x == 1 || nextNorm.x == 1);
    const bool isUp = (prevNorm.y == -1 || nextNorm.y == -1);
    const bool isDown = (prevNorm.y == 1 || nextNorm.y == 1);

    if (isLeft && isUp)
        return "assets/snake/green/body_turn_left_up.png";
    if (isLeft && isDown)
        return "assets/snake/green/body_turn_left_down.png";
    if (isRight && isUp)
        return "assets/snake/green/body_turn_right_up.png";
    if (isRight && isDown)
        return "assets/snake/green/body_turn_right_down.png";
    return "assets/snake/green/body_straight_horizontal.png";
}

}