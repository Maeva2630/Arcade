/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** TestModule.hpp
*/

#ifndef SNAKEMODULE_HPP_
# define SNAKEMODULE_HPP_
    #include "AGameModule.hpp"
    #include <iostream>

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
                GameState current_state;
                std::string name;
                std::vector<Entity> entities;
                std::vector<Text> ui_text;
                RenderContext render_context;
                int score;
    };
};

#endif
