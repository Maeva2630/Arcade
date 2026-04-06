/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** SnakeModule.cpp
*/

#include "games/snake/Snake.hpp"
#include <functional>
#include <vector>
#include <random>
#include <map>

extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::SnakeModule();
}

namespace arcade {

using SnakeInputHandler = std::function<void(Key)>;

SnakeModule::FoodSystem::FoodSystem() : cell(-1.0f, -1.0f), hasFood(false), rng(std::random_device{}())
{
}

void SnakeModule::FoodSystem::spawn(const Snake &snake, int width, int height, GameState &state)
{
    std::vector<Position> freeCells;

    for (int y = 3; y < height - 2; ++y) {
        for (int x = 1; x < width - 1; ++x) {
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
    _currentState = GameState::MENU;
    _renderContext = {MAX_GRID_X + 1, MAX_GRID_Y + 1, CoordinateSpace::GRID};
}

SnakeModule::~SnakeModule()
{
    stop();
}

void SnakeModule::init()
{
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
}

const std::string& SnakeModule::getName() const
{
    static const std::string &name = "Snake";
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
            refreshUI();
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

void SnakeModule::refreshFrame()
{
    const size_t frameSize = 4;
    std::array<Entity, frameSize> frames;

    frames[0].pos = Position(0.0f, 2.0f);
    frames[0].size = Position(1.0f, MAX_GRID_Y - 3.0f);

    frames[1].pos = Position(0.0f, 2.0f);
    frames[1].size = Position(MAX_GRID_X - 1.0f, 1.0f);

    frames[2].pos = Position(0.0f, MAX_GRID_Y - 2.0f);
    frames[2].size = Position(MAX_GRID_X - 1.0f, 1.0f);

    frames[3].pos = Position(MAX_GRID_X - 1.0f, 2.0f);
    frames[3].size = Position(1.0f, MAX_GRID_Y - 3.0f);

    for (Entity &i : frames) {
        i.color = Colors::WHITE;
        i.backgroundColor = Colors::BLACK;
        i.character = '*';
        _entities.emplace_back(i);
    }
}

void SnakeModule::refreshEntities()
{
    _entities.clear();

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

    refreshFrame();
}

void SnakeModule::refreshUI()
{
    _uiText.clear();

    std::string title = "SNAKE";
    _uiText.emplace_back(title, MAX_GRID_X / 2 - 3, 0.0f, Colors::YELLOW, 32);
    
    std::string scoreStr = "Score: " + std::to_string(_score);
    _uiText.emplace_back(scoreStr, 1.0f, 1.0f, Colors::WHITE, 32);

    if (_currentState == GameState::PAUSED) {
        _uiText.emplace_back("PAUSED", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::YELLOW, 36);
    } else if (_currentState == GameState::GAME_OVER) {
        _uiText.emplace_back("GAME OVER", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::RED, 36);
    } else if (_currentState == GameState::WON) {
        _uiText.emplace_back("YOU WIN!", MAX_GRID_X / 2 - 3, MAX_GRID_Y / 2, Colors::GREEN, 36);
    }
    _uiText.emplace_back("[Arrows: move] [P: pause] [R: restart] [Esc: exit] [M: menu]",
                         1.0f, MAX_GRID_Y, Colors::CYAN, 36);
}

void SnakeModule::handleInput(Key key)
{
    static const std::map<Key, SnakeInputHandler> inputs = {
        {Key::LEFT,     [this](Key) { _snake.setFace(Snake::Face::WEST); }},
        {Key::RIGHT,    [this](Key) { _snake.setFace(Snake::Face::EAST); }},
        {Key::UP,       [this](Key) { _snake.setFace(Snake::Face::NORTH); }},
        {Key::DOWN,     [this](Key) { _snake.setFace(Snake::Face::SOUTH); }},
    };

    const auto it = inputs.find(key);
    if (it != inputs.end()) {
        it->second(key);
        return;
    }

    handleGameInput(key);
}

const std::vector<Entity>& SnakeModule::getEntities() const
{
    return _entities;
}

const std::vector<Text>& SnakeModule::getUIText() const
{
    return _uiText;
}

GameState SnakeModule::getState() const
{
    return _currentState;
}

int SnakeModule::getScore() const
{
    return _score;
}

void SnakeModule::restart()
{
    stop();
    init();
}

void SnakeModule::pause()
{
    if (_currentState == GameState::RUNNING) {
        _currentState = GameState::PAUSED;
        refreshUI();
    }
}

void SnakeModule::resume()
{
    if (_currentState == GameState::PAUSED) {
        _currentState = GameState::RUNNING;
        refreshUI();
    }
}

RenderContext SnakeModule::getRenderContext() const
{    
    return _renderContext;
}

}
