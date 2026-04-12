/**
 * @file AGameModule.hpp
 * @brief Abstract base class for game modules
 */

#pragma once

#include "IGameModule.hpp"
#include <functional>
#include <map>

namespace arcade {

/**
 * @class AGameModule
 * @brief Abstract implementation of IGameModule that provides getType()
 *
 * This class implements the getType() method returning ModuleType::GAME.
 * All concrete game modules (Snake, Pacman, etc.) should inherit from
 * this class instead of IGameModule directly.
 */
class AGameModule : public IGameModule {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~AGameModule() = default;

        /**
         * @brief Get the type of this module
         * @return ModuleType::GAME
         */
        ModuleType getType() const override { return ModuleType::GAME; }
    
    protected:
        GameState _currentState;
        std::vector<Entity> _entities;
        std::vector<Text> _uiText;
        RenderContext _renderContext;
        int _score;

        /**
         * @brief Rebuild game entities from current state
         * 
         * Called after each game logic update to sync _entities
         * with the current game state for rendering.
         */
        virtual void refreshEntities() {}

        /**
         * @brief Rebuild UI text elements from current state
         * 
         * Called to update score, status messages, controls help, etc.
         * in the _uiText vector for rendering.
         */
        virtual void refreshUI() {}

        /**
         * @brief Handle common in game input
         *  
         * handle pause, restart and exit keys.
         * @param key 
         */
        void handleGameInput(Key key);


};

} // namespace arcade
