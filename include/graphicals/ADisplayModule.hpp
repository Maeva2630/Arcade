/**
 * @file ADisplayModule.hpp
 * @brief Abstract base class for display modules
 */

#pragma once

#include "IDisplayModule.hpp"
#include <map>

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
        bool _isOpen;
};

} // namespace arcade
