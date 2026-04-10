/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerEntities.hpp
*/

#ifndef NIBBLER_ENTITIES_HPP
#define NIBBLER_ENTITIES_HPP
#include "games/IGameModule.hpp"
#include "games/nibbler/NibblerPlayer.hpp"

namespace arcade
{
    class NibblerEntities
    {
        public:

            /**
             * @brief Build entities
             * 
             * Construct all current entities to make them usable for display modules
             * @param entities 
             * @param segments 
             * @param food 
             * @param walls 
             */
            static void buildEntities(std::vector<Entity> &entities,
                const std::deque<Nibbler::NibblerSegment> &segments,
                const std::vector<Position> &food,
                const std::vector<Position> &walls);

            /**
             * @brief Build UI text
             * 
             * Construct all UI texts to make them usable for display module (score,
             * level, title, usage) 
             * @param uiText 
             * @param score 
             * @param state 
             * @param fontSize 
             * @param level 
             */
            static void buildUI(std::vector<Text> &uiText,
                int score,
                GameState state,
                float fontSize, size_t level);
    };
};

#endif