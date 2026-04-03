/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** TestModule.hpp
*/

#ifndef SNAKEMODULE_HPP_
# define SNAKEMODULE_HPP_
    #include "games/AGameModule.hpp"
    #include "games/snake/SnakeEntity.hpp"
    #include <iostream>
    #include <list>
    #include <random>
    #include <array>

    #define MOVE_INTERVAL_SECONDS 0.12

namespace arcade {

    class SnakeModule : public AGameModule 
    {
        public:
            SnakeModule();
            ~SnakeModule();
    
            void init() override;
            void stop() override;
            const std::string& getName() const override;
            void update(float deltaTime) override;
            void handleInput(Key key) override;
            const std::vector<Entity>& getEntities() const override;
            const std::vector<Text>& getUIText() const override;
            GameState getState() const override;
            int getScore() const override;
            void restart() override;
            void pause() override;
            void resume() override;
            RenderContext getRenderContext() const override;

        private:
            void generateFood();
            void refreshEntities() override;
            void refreshUI() override;
            void refreshFrame();

            struct FoodSystem {
                Position cell;
                bool hasFood;
                std::mt19937 rng;

                FoodSystem();
                void spawn(const Snake &snake, int width, int height, GameState &state);
            };

            int _score;
            float _moveDelay;
            FoodSystem _food;
            Snake _snake;
    };
};

#endif
