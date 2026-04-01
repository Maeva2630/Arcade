/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeEntity.cpp
*/

#ifndef SNAKE_ENTITY_HPP
#define SNAKE_ENTITY_HPP
    #include <deque>
    #include "IDisplayModule.hpp"
    #define START_SNAKE_LENGTH 4
    #define START_HEAD_X 5.0f
    #define START_HEAD_Y 5.0f
    #define MAX_GRID_X 10
    #define MAX_GRID_Y 10

namespace arcade
{
    class Snake
    {
        public:
            Snake();
            ~Snake() = default;

            enum Face {
                EAST = 1,
                WEST = 2,
                NORTH = 3,
                SOUTH = 4,
            };

            struct SnakeSegment {
                Position pos;
                Face face;
            };

            SnakeSegment &getHeadReference();
            SnakeSegment getHead() const;

            SnakeSegment getNextHead() const;

            void setFace(Face f);

            size_t getSegmentSize() const;

            const std::deque<SnakeSegment> &getSegments() const;

            bool containsAtThisPos(Position pos) const;

            void setAlive(bool isAlive);

            void updateBody(bool hasEaten, SnakeSegment nextHead);

        private:
            bool _isALive;
            std::deque<SnakeSegment> _snakeSegment; 
    };
};


#endif