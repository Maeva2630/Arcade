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

constexpr std::size_t NIBBLER_MAP_WIDTH = static_cast<std::size_t>(END_COL - START_COL);
constexpr std::size_t NIBBLER_MAP_HEIGHT = static_cast<std::size_t>(END_ROW - START_ROW);

using NibblerStaticMap = std::array<std::array<int, NIBBLER_MAP_WIDTH>, NIBBLER_MAP_HEIGHT>;

const NibblerStaticMap &getNibblerStaticMap(std::size_t level);

}

#endif
