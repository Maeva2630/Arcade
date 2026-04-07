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
    #include <iostream>
    #include <list>
    #include <random>
    #include <array>

    #define MOVE_INTERVAL_SECONDS 0.12

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
            void generateFood();

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
             * @brief Emplace frame entity
             * 
             * place a frame to delimit the game area
             */
            void refreshFrame();

            struct FoodSystem {
                Position cell;
                bool hasFood;
                std::mt19937 rng;

                /**
                 * @brief Construct a new Food System object
                 */
                FoodSystem();
        
                /**
                 * @brief Spawn food on game area
                 * 
                 * @param nibbler 
                 * @param width 
                 * @param height 
                 * @param state 
                 */
                void spawn(const Nibbler &nibbler, int width, int height, GameState &state);
            };

            int _score;
            float _moveDelay;
            FoodSystem _food;
            Nibbler _nibbler;
    };
};

#endif
