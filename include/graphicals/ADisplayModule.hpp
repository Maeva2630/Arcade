/**
 * @file ADisplayModule.hpp
 * @brief Abstract base class for display modules
 */

#pragma once

#include "IDisplayModule.hpp"
#include <map>
#include <memory>

namespace arcade {

/**
 * @class ADisplayModule
 * @brief Abstract implementation of IDisplayModule that provides getType()
 *
 * This class implements the getType() method returning ModuleType::DISPLAY.
 * All concrete display modules (SDL2, SFML, nCurses, etc.) should inherit from
 * this class instead of IDisplayModule directly.
 */
class ADisplayModule : public IDisplayModule {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~ADisplayModule() = default;

        /**
         * @brief Get the type of this module
         * @return ModuleType::DISPLAY
         */
        ModuleType getType() const override { return ModuleType::DISPLAY; }

    protected:
        /**
         * @brief KeyMap
         * Associate during init every key from the graphical library with our
         * commun Key enum.
         */
        std::map<int, Key> _keyMap;
        bool _isOpen = false;

        /**
         * @brief 
         * Delta time to handle different fps for each graphic library
         */
        float _deltaTime;

        /**
         * @brief 
         * Render context from the graphical library
         */
        RenderContext _renderContext;

        /**
         * @brief 
         * 
         * Convert to int one cell
         * 
         * @param value 
         * @return int 
         */
        static int convertToCell(float value);

        /**
         * @brief 
         * 
         * Convert a grid from a game into a pixel position for the display
         * 
         * @param gridPos 
         * @param screenHeight 
         * @param screenWidth 
         * @return Position 
         */
        Position convertGridPosition(Position gridPos, size_t screenHeight, size_t screenWidth);

        /**
         * @brief 
         * 
         * Convert a grid entity size to a pixel size
         * 
         * @param gridSize 
         * @param screenHeight 
         * @param screenWidth 
         * @return Position 
         */
        Position convertGridSize(Position gridSize, size_t screenHeight, size_t screenWidth);
};

} // namespace arcade
