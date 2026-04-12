/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerPlayer.cpp
*/

#ifndef NIBBLER_PLAYER_HPP
#define NIBBLER_PLAYER_HPP
    #include <deque>
    #include "graphicals/IDisplayModule.hpp"

    #define MAX_GRID_X 50
    #define MAX_GRID_Y 20
    
    #define START_ROW 3
    #define START_COL 1

    #define START_NIBBLER_LENGTH 4
    #define START_HEAD_X ((MAX_GRID_X / 2) + START_COL + 1)
    #define START_HEAD_Y (MAX_GRID_Y / 2)

    #define END_ROW MAX_GRID_Y - 2
    #define END_COL MAX_GRID_X - 1


namespace arcade
{
    class Nibbler
    {
        public:
            Nibbler();
            ~Nibbler() = default;

            /**
             * @brief
             * Enum to know which direction the Nibbler is facing
             */
            enum Face {
                EAST = 0,
                NORTH = 1,
                WEST = 2,
                SOUTH = 3,
            };

            /**
             * @brief 
             * NibblerSegment struct regrouping one position and a face
             */
            struct NibblerSegment {
                Position pos;
                Face face;
            };

            /**
             * @brief init Nibbler segments and position
             */
            void setStarter();
            /**
             * @brief Get the Head Reference object
             * To modify the head of the Nibbler
             * @return NibblerSegment& 
             */
            NibblerSegment &getHeadReference();
            /**
             * @brief Get the Head object
             * To obtain information on the head of the Nibbler
             * @return NibblerSegment 
             */
            NibblerSegment getHead() const;

            /**
             * @brief Get the Next Head object
             * Get the next head information depending on user input and current head informations
             * @return NibblerSegment 
             */
            NibblerSegment getNextHead() const;

            /**
             * @brief Set the Face object
             * Set the new facing of the Nibbler depending on current face
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
             * @return const std::deque<NibblerSegment>& 
             */
            const std::deque<NibblerSegment> &getSegments() const;

            /**
             * @brief 
             * 
             * See if one segment of the Nibbler covers this position
             * @param pos 
             * @return true 
             * @return false 
             */
            bool containsAtThisPos(Position pos) const;

            /**
             * @brief Set the Alive object
             * Change the isAlive boolean of the Nibbler
             * @param isAlive 
             */
            void setAlive(bool isAlive);

            /**
             * @brief 
             * 
             * update the body size of the Nibbler
             * @param hasEaten 
             * @param nextHead 
             */
            void updateBody(bool hasEaten, NibblerSegment nextHead);

        private:
            bool _isALive;
            std::deque<NibblerSegment> _nibblerSegment; 
    };
};


#endif
