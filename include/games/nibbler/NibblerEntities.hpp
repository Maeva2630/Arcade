/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerEntities.hpp
*/

#ifndef NIBBLER_ENTITIES_HPP
#define NIBBLER_ENTITIES_HPP
#include "games/IGameModule.hpp"
#include "games/nibbler/NibblerEntity.hpp"

namespace arcade
{
    class NibblerEntities
    {
        public:

            static void buildEntities(std::vector<Entity> &entities,
                const std::deque<Nibbler::NibblerSegment> &segments,
                const std::vector<Position> &food,
                const std::vector<Position> &walls);

            static void buildUI(std::vector<Text> &uiText,
                int score,
                GameState state,
                float fontSize, size_t level);
    };
};

#endif