/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeModule.cpp
*/

#include "games/snake/Snake.hpp"
#include <vector>
#include <random>


extern "C" arcade::IModule *entryPoint(void)
{
    // std::cout << "Snake entryPoint\n";
    return new arcade::SnakeModule();
}

namespace arcade {

SnakeModule::FoodSystem::FoodSystem() : cell(-1.0f, -1.0f), hasFood(false), rng(std::random_device{}())
{
}

void SnakeModule::FoodSystem::spawn(const Snake &snake, int width, int height, GameState &state)
{
    std::vector<Position> freeCells;
    freeCells.reserve(static_cast<size_t>(width * height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position candidate(static_cast<float>(x), static_cast<float>(y));
            if (!snake.containsAtThisPos(candidate))
                freeCells.emplace_back(candidate);
        }
    }

    if (freeCells.empty()) {
        hasFood = false;
        state = GameState::WON;
        return;
    }

    std::uniform_int_distribution<size_t> dist(0, freeCells.size() - 1);
    cell = freeCells[dist(rng)];
    hasFood = true;
}

SnakeModule::SnakeModule() : _score(0), _moveDelay(0.0f)
{
    // std::cout << "SnakeModule constructor" << std::endl;
    _currentState = GameState::MENU;
    _renderContext = {MAX_GRID_X, MAX_GRID_Y, CoordinateSpace::GRID};
}

SnakeModule::~SnakeModule()
{
    // std::cout << "Snake destructor" << std::endl;
}

void SnakeModule::init()
{
    // std::cout << "SnakeModule init" << std::endl;
    _currentState = GameState::RUNNING;

    _score = 0;
    _moveDelay = 0.0f;
    _snake = Snake();
    generateFood();
    refreshEntities();
    refreshUI();
}

void SnakeModule::stop()
{
    _currentState = GameState::MENU;
    _score = 0;
    _moveDelay = 0.0f;
    _entities.clear();
    _uiText.clear();
    // std::cout << "SnakeModule stop" << std::endl;
}

const std::string& SnakeModule::getName() const
{
    const std::string &name = "Snake";
    return name;
}

void SnakeModule::update(float deltaTime)
{
    if (_currentState != GameState::RUNNING)
        return;

    if (deltaTime <= 0.0f)
        return;

    _moveDelay += deltaTime;

    while (_moveDelay >= MOVE_INTERVAL_SECONDS && _currentState == GameState::RUNNING) {
        _moveDelay -= MOVE_INTERVAL_SECONDS;

        Snake::SnakeSegment nextHead = _snake.getNextHead();
        const bool hasEaten = _food.hasFood
            && nextHead.pos.x == _food.cell.x
            && nextHead.pos.y == _food.cell.y;

        if (_snake.containsAtThisPos(nextHead.pos) && !hasEaten) {
            _snake.setAlive(false);
            _currentState = GameState::GAME_OVER;
            return;
        } else {
            if (hasEaten) {
                _score++;
                generateFood();
            }
            _snake.updateBody(hasEaten, nextHead);
        }

        refreshEntities();
        refreshUI();
    }
}

void SnakeModule::generateFood()
{
    _food.spawn(_snake, MAX_GRID_X, MAX_GRID_Y, _currentState);
}

void SnakeModule::refreshEntities()
{
    _entities.clear();
    _entities.reserve(_snake.getSegmentSize() + (_food.hasFood ? 1U : 0U));

    const std::deque<Snake::SnakeSegment> &segments = _snake.getSegments();

    for (size_t i = 0; i < segments.size(); ++i) {
        Entity segmentEntity;
        segmentEntity.pos = segments[i].pos;
        segmentEntity.size = Position(1.0f, 1.0f);
        segmentEntity.character = (i + 1 == segments.size()) ? '@' : 'o';
        segmentEntity.color = (i + 1 == segments.size()) ? Colors::YELLOW : Colors::GREEN;
        _entities.emplace_back(segmentEntity);
    }

    if (_food.hasFood) {
        Entity foodEntity;
        foodEntity.pos = _food.cell;
        foodEntity.size = Position(1.0f, 1.0f);
        foodEntity.character = '*';
        foodEntity.color = Colors::RED;
        _entities.emplace_back(foodEntity);
    }
}

void SnakeModule::refreshUI()
{
    _uiText.clear();
    
    // Score and length in top-left
    std::string scoreStr = "Score: " + std::to_string(_score);
    _uiText.emplace_back(scoreStr, -0.5f, -1.5f, Colors::WHITE, 10);
    
    std::string lengthStr = "Length: " + std::to_string(_snake.getSegmentSize());
    _uiText.emplace_back(lengthStr, 3.5f, -1.5f, Colors::WHITE, 10);
    
    // State-specific messages
    if (_currentState == GameState::PAUSED) {
        _uiText.emplace_back("PAUSED", 4.0f, 5.0f, Colors::YELLOW, 12);
    } else if (_currentState == GameState::GAME_OVER) {
        _uiText.emplace_back("GAME OVER", 3.5f, 5.0f, Colors::RED, 12);
    } else if (_currentState == GameState::WON) {
        _uiText.emplace_back("YOU WIN!", 4.0f, 5.0f, Colors::GREEN, 12);
    }
    
    // Controls help in bottom
    _uiText.emplace_back("Arrows: move | P: pause | R: restart | M: menu | Esc: exit",
                         0.5f, 11.5f, Colors::CYAN, 9);
}

void SnakeModule::handleInput(Key key)
{
    // std::cout << "SnakeModule handleInput" << std::endl;
    switch (key) {
        case Key::LEFT:
            _snake.setFace(Snake::Face::WEST);
            break;
        case Key::RIGHT:
            _snake.setFace(Snake::Face::EAST);
            break;
        case Key::UP:
            _snake.setFace(Snake::Face::NORTH);
            break;
        case Key::DOWN:
            _snake.setFace(Snake::Face::SOUTH);
            break;
        default:
            break;
    }
}

const std::vector<Entity>& SnakeModule::getEntities() const
{
    // std::cout << "SnakeModule getEntities : " << _entities.size() << " entities" << std::endl;
    return _entities;
}

const std::vector<Text>& SnakeModule::getUIText() const
{
    // std::cout << "SnakeModule getUIText : " << _uiText.size() << " text elements" << std::endl;
    return _uiText;
}

GameState SnakeModule::getState() const
{
    // std::cout << "SnakeModule getState : " << static_cast<int>(_currentState) << std::endl;
    return _currentState;
}

int SnakeModule::getScore() const
{
    // std::cout << "SnakeModule getScore : " << _score << std::endl;
    return _score;
}

void SnakeModule::restart()
{
    // std::cout << "SnakeModule restart" << std::endl;
    stop();
    init();
}

void SnakeModule::pause()
{
    // std::cout << "SnakeModule pause" << std::endl;
    if (_currentState == GameState::RUNNING) {
        _currentState = GameState::PAUSED;
    }
}

void SnakeModule::resume()
{
    // std::cout << "SnakeModule resume" << std::endl;
    if (_currentState == GameState::PAUSED) {
        _currentState = GameState::RUNNING;
    }
}

RenderContext SnakeModule::getRenderContext() const
{    
    // std::cout << "SnakeModule getRenderContext" << std::endl;
    return _renderContext;
}

}
