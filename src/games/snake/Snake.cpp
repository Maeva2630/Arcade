/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeModule.cpp
*/

#include "games/Snake.hpp"

extern "C" arcade::IModule *entryPoint(void)
{
    std::cout << "Snake entryPoint\n";
    return new arcade::SnakeModule();
}

namespace arcade {

SnakeModule::SnakeModule() : current_state(GameState::MENU), name("Snake"), score(0)
{
    std::cout << "SnakeModule constructor" << std::endl;
    render_context = {800, 600, CoordinateSpace::PIXEL};
}

SnakeModule::~SnakeModule()
{
    std::cout << "Snake destructor" << std::endl;
}

void SnakeModule::init()
{
    std::cout << "SnakeModule init" << std::endl;
    current_state = GameState::MENU;
}

void SnakeModule::stop()
{
    current_state = GameState::MENU;
    score = 0;
    entities.clear();
    ui_text.clear();
    std::cout << "SnakeModule stop" << std::endl;
}

const std::string& SnakeModule::getName() const
{
    std::cout << "SnakeModule getName : " << name << std::endl;
    return name;
}

void SnakeModule::update(float deltaTime)
{
    std::cout << "SnakeModule update : deltaTime = " << deltaTime << std::endl;
}

void SnakeModule::handleInput(Key key)
{
    std::cout << "SnakeModule handleInput" << std::endl;
}

const std::vector<Entity>& SnakeModule::getEntities() const
{
    std::cout << "SnakeModule getEntities : " << entities.size() << " entities" << std::endl;
    return entities;
}

const std::vector<Text>& SnakeModule::getUIText() const
{
    std::cout << "SnakeModule getUIText : " << ui_text.size() << " text elements" << std::endl;
    return ui_text;
}

GameState SnakeModule::getState() const
{
    std::cout << "SnakeModule getState : " << static_cast<int>(current_state) << std::endl;
    return current_state;
}

int SnakeModule::getScore() const
{
    std::cout << "SnakeModule getScore : " << score << std::endl;
    return score;
}

void SnakeModule::restart()
{
    std::cout << "SnakeModule restart" << std::endl;
    stop();
    init();
}

void SnakeModule::pause()
{
    std::cout << "SnakeModule pause" << std::endl;
    if (current_state == GameState::RUNNING) {
        current_state = GameState::PAUSED;
    }
}

void SnakeModule::resume()
{
    std::cout << "SnakeModule resume" << std::endl;
    if (current_state == GameState::PAUSED) {
        current_state = GameState::RUNNING;
    }
}

RenderContext SnakeModule::getRenderContext() const
{    
    std::cout << "SnakeModule getRenderContext" << std::endl;
    return render_context;
}

}
