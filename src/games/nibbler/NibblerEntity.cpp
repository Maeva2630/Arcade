/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerEntity.cpp
*/

#include "games/nibbler/NibblerEntity.hpp"

namespace arcade
{

Nibbler::Nibbler() : _isALive(true)
{
    setStarter();
}

void Nibbler::setStarter()
{
    _nibblerSegment.clear();

    for (int i = 0; i < START_NIBBLER_LENGTH; ++i) {
        NibblerSegment segment;
        segment.pos = { START_HEAD_X - static_cast<float>(i), START_HEAD_Y };
        segment.face = Face::NORTH;
        _nibblerSegment.emplace_back(segment);
    }
}

Nibbler::NibblerSegment &Nibbler::getHeadReference()
{
    return _nibblerSegment.back();
}

Nibbler::NibblerSegment Nibbler::getHead() const
{
    return _nibblerSegment.back();
}

void Nibbler::setFace(Face f) 
{ 
    Face current = getHead().face;
    int diff = static_cast<int>(current) - static_cast<int>(f);
    if (diff != 2 && diff != -2) {
        getHeadReference().face = f;
    }
}

Nibbler::NibblerSegment Nibbler::getNextHead() const
{
    NibblerSegment head = getHead();
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

    return NibblerSegment{ Position{ static_cast<float>(x), static_cast<float>(y) }, head.face };
}

size_t Nibbler::getSegmentSize() const
{
    return _nibblerSegment.size();
}

const std::deque<Nibbler::NibblerSegment> &Nibbler::getSegments() const
{
    return _nibblerSegment;
}

bool Nibbler::containsAtThisPos(Position pos) const
{
    for (const auto &segment : _nibblerSegment) {
        if (segment.pos.x == pos.x && segment.pos.y == pos.y)
            return true;
    }
    return false;
}

void Nibbler::setAlive(bool isAlive)
{
    _isALive = isAlive;
}

void Nibbler::updateBody(bool hasEaten, NibblerSegment nextHead)
{
    if (!hasEaten) {
        _nibblerSegment.pop_front();
    }
    _nibblerSegment.push_back(nextHead);
}

}