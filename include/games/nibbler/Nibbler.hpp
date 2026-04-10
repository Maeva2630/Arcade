/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Nibbler.hpp
*/

#ifndef NIBBLERMODULE_HPP_
# define NIBBLERMODULE_HPP_
    #include "games/AGameModule.hpp"
    #include "games/nibbler/NibblerEntity.hpp"
    #include "games/nibbler/NibblerStaticMap.hpp"
    #include <iostream>
    #include <unordered_map>
    #include <random>
    #include <array>

    #define MOVE_INTERVAL_SECONDS 0.12

    #define FONT_SIZE 36

namespace arcade {

    class NibblerModule : public AGameModule 
    {
        public:
            /**
             * @brief Construct a new Nibbler Module object
             * 
             * init current state and renderContext
             */
            NibblerModule();
            ~NibblerModule();
    
            /**
             * @brief Init game
             * 
             * init _Nibbler entity, generate food and place them on screen
             */
            void init() override;

            /**
             * @brief Update the current difficulty level
             */
            bool updateLevel();

            /**
             * @brief Stop game
             * 
             * clear all entities
             */
            void stop() override;

            /**
             * @brief Get the Name object
             * 
             * @return const std::string& 
             */
            const std::string& getName() const override;

            /**
             * @brief Update game states and displays
             * 
             * update Nibbler position and length, game state, update ui texts 
             * and check food available
             * @param deltaTime 
             */
            void update(float deltaTime) override;

            /**
             * @brief Handle user inputs for the game
             * 
             * check for global inputs (pause, restart, exit) and specific inputs 
             * (arrows to move)
             * @param key 
             */
            void handleInput(Key key) override;

            /**
             * @brief Get entities
             * 
             * @return const std::vector<Entity>& 
             */
            const std::vector<Entity>& getEntities() const override;

            /**
             * @brief Get UI text from the game to display
             * 
             * @return const std::vector<Text>& 
             */
            const std::vector<Text>& getUIText() const override;

            /**
             * @brief Get the current game state
             * 
             * @return GameState 
             */
            GameState getState() const override;

            /**
             * @brief Get the current user score
             * 
             * @return int 
             */
            int getScore() const override;

            /**
             * @brief Restart the game from 0
             */
            void restart() override;

            /**
             * @brief Pause the game
             */
            void pause() override;

            /**
             * @brief Resume game from pause
             */
            void resume() override;

            /**
             * @brief Get the Render Context object
             * 
             * @return RenderContext 
             */
            RenderContext getRenderContext() const override;

        private:
            /**
             * @brief Make food spawn on the grid
             * 
             * using "uniform_int_distribution" to generate a random number
             * keeping the same amount of chance for each cell
             */
            void spawnFood();

            /**
             * @brief Load nibbler map from NibblerStaticMap
             */
            void loadStaticMap();

            /**
             * @brief Update all game entities
             * 
             * clear and emplace back all entities in vector to prepare them for display
             * (Nibbler, food)
             */
            void refreshEntities() override;

            /**
             * @brief Update all game UI texts
             * 
             * clear and emplace back all texts needed for UI
             * (score, commands, game states texts)
             */
            void refreshUI() override;

            /**
             * @brief Check if a wall is at the position
             * 
             * @param p 
             * @return true 
             * @return false 
             */
            bool isWallAt(const Position &p) const;

            /**
             * @brief Check if food is at the position
             * 
             * @param p 
             * @return true 
             * @return false 
             */
            bool isFoodAt(const Position &p) const;

            /**
             * @brief Check if the position is occupied
             * 
             * check if any entity can occup this position (nibbler, walls, foods)
             * @param p 
             * @return true 
             * @return false 
             */
            bool isOccupied(const Position &p) const;

            /**
             * @brief try to turn if nibbler is blocked
             * 
             * if nibbler is stuck because of a collision,
             * it will identify the right direction to take depending the context
             * @param blockedFace 
             * @return true 
             * @return false 
             */
            bool tryTurnFromFace(Nibbler::Face blockedFace);

            /**
             * @brief Handle wall collision
             * 
             * handle the delay from when the nibbler is blocked and when it
             * automatically turn
             * @param currentFace 
             * @return true 
             * @return false 
             */
            bool handleWallCollision(Nibbler::Face currentFace);

            /**
             * @brief Update the timer for blocked Nibbler
             * 
             * @param deltaTime 
             * @return true 
             * @return false 
             */
            bool updateWallBlockTimer(float deltaTime);

            /**
             * @brief Reset all stuck wall params
             */
            void resetWallStuckState();

            /**
             * @brief Remove available food
             * 
             * remove food from the _food vector
             * @param p 
             */
            void removeFoodAt(const Position &p);

            /**
             * @brief Get the Free Cells from the grid
             * 
             * @return std::vector<Position> 
             */
            std::vector<Position> getFreeCells();

            int _score;
            size_t _level;

            float _moveDelay;
            float _moveInterval;
    
            float _wallBlockTimer;
            float _wallStuckTime;
            Nibbler::Face _lastBlockedFace;
            bool wallStuck;
    
            std::vector<Position> _food;
            bool _hasFood;

            std::vector<Position> _walls;
            Nibbler _nibbler;

            std::mt19937 _rng;
    };
};

#endif
