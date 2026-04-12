/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakePlayer.cpp
*/

#ifndef SNAKE_PLAYER_HPP
#define SNAKE_PLAYER_HPP
    #include <deque>
    #include "graphicals/IDisplayModule.hpp"
    #define START_SNAKE_LENGTH 4
    #define START_HEAD_X 5.0f
    #define START_HEAD_Y 5.0f
    #define MAX_GRID_X 50
    #define MAX_GRID_Y 20

namespace arcade
{
    class Snake
    {
        public:
            Snake();
            ~Snake() = default;

            /**
             * @brief
             * Enum to know which direction the snake is facing
             */
            enum Face {
                EAST = 0,
                NORTH = 1,
                WEST = 2,
                SOUTH = 3,
            };

            /**
             * @brief 
             * SnakeSegment struct regrouping one position and a face
             */
            struct SnakeSegment {
                Position pos;
                Face face;
            };

            /**
             * @brief Get the Head Reference object
             * To modify the head of the snake
             * @return SnakeSegment& 
             */
            SnakeSegment &getHeadReference();
            /**
             * @brief Get the Head object
             * To obtain information on the head of the snake
             * @return SnakeSegment 
             */
            SnakeSegment getHead() const;

            SnakeSegment getTail() const;

            /**
             * @brief Get the Next Head object
             * Get the next head information depending on user input and current head informations
             * @return SnakeSegment 
             */
            SnakeSegment getNextHead() const;

            /**
             * @brief Set the Face object
             * Set the new facing of the snake depending on current face
             * @param f 
             */
            void setFace(Face f);

            /**
             * @brief Get the Segment Size object
             * 
             * @return size_t 
             */
            size_t getSegmentSize() const;

            /**
             * @brief Get the Segments object reference
             * 
             * @return const std::deque<SnakeSegment>& 
             */
            const std::deque<SnakeSegment> &getSegments() const;

            /**
             * @brief 
             * 
             * See if one segment of the snake covers this position
             * @param pos 
             * @return true 
             * @return false 
             */
            bool containsAtThisPos(Position pos) const;

            /**
             * @brief Set the Alive object
             * Change the isAlive boolean of the snake
             * @param isAlive 
             */
            void setAlive(bool isAlive);

            /**
             * @brief 
             * 
             * update the body size of the snake
             * @param hasEaten 
             * @param nextHead 
             */
            void updateBody(bool hasEaten, SnakeSegment nextHead);

            std::string getSnakeHeadSprite() const;
            std::string getSnakeTailSprite() const;
            std::string getSnakeBodySprite(const Snake::SnakeSegment &prev,
                const Snake::SnakeSegment &current, const Snake::SnakeSegment &next)
                const;

        private:
            bool _isALive;
            std::deque<SnakeSegment> _snakeSegment; 
    };
};


#endif