/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NibblerStaticMap.hpp
*/

#ifndef NIBBLER_STATIC_MAP_HPP_
#define NIBBLER_STATIC_MAP_HPP_
    #include <array>
    #include <string_view>
    #include "games/nibbler/NibblerPlayer.hpp"

namespace arcade {

constexpr std::size_t NIBBLER_MAP_WIDTH = static_cast<std::size_t>(END_COL - START_COL);
constexpr std::size_t NIBBLER_MAP_HEIGHT = static_cast<std::size_t>(END_ROW - START_ROW);

using NibblerStaticMap = std::array<std::array<int, NIBBLER_MAP_WIDTH>, NIBBLER_MAP_HEIGHT>;
using MapRows = std::array<std::string_view, NIBBLER_MAP_HEIGHT>;


class NibblerLevels
{
    public:
        NibblerLevels() = default;
        ~NibblerLevels() = default;

        /**
         * @brief Get Nibbler Static Map depending on level
         * 
         * @param level 
         * @return const NibblerStaticMap& 
         */
        static const NibblerStaticMap &getNibblerStaticMap(std::size_t level);

        /**
         * @brief Convert static map to NibblerStaticMap type
         * 
         * @param rows 
         * @return NibblerStaticMap 
         */
        static NibblerStaticMap convertStaticMap(const MapRows &rows);
};

}

#endif
