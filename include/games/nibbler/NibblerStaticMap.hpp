/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerStaticMap.hpp
*/

#ifndef NIBBLER_STATIC_MAP_HPP_
#define NIBBLER_STATIC_MAP_HPP_
    #include <array>
    #include "games/nibbler/NibblerEntity.hpp"

namespace arcade {

using NibblerStaticMap = std::array<std::array<int, MAX_GRID_X>, MAX_GRID_Y>;

const NibblerStaticMap &getNibblerStaticMap(std::size_t level);

}

#endif
