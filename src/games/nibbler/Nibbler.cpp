/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Nibbler.cpp
*/

#include "games/nibbler/Nibbler.hpp"
#include <vector>
#include <random>


extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::NibblerModule();
}

namespace arcade {

void NibblerModule::spawnFood()
{
    std::vector<Position> freeCells;

    for (int y = 3; y < MAX_GRID_Y - 2; ++y) {
        for (int x = 1; x < MAX_GRID_X - 1; ++x) {
            Position candidate(static_cast<float>(x), static_cast<float>(y));
            if (!isOccupied(candidate))
                freeCells.emplace_back(candidate);
        }
    }

    if (freeCells.empty()) {
        _hasFood = false;
        _currentState = GameState::WON;
        return;
    }

    std::uniform_int_distribution<size_t> dist(0, freeCells.size() - 1);
    _food = freeCells[dist(_rng)];
    _hasFood = true;
}

std::vector<Position> NibblerModule::getFreeCells()
{
    std::vector<Position> freeCells;

    for (int y = 3; y < MAX_GRID_Y - 2; ++y) {
        for (int x = 1; x < MAX_GRID_X - 1; ++x) {
            Position candidate(static_cast<float>(x), static_cast<float>(y));
            if (!isOccupied(candidate))
                freeCells.emplace_back(candidate);
        }
    }

    return freeCells;
}

bool isSamePos(const Position &a, const Position &b)
{
    return a.x == b.x && a.y == b.y;
}

bool NibblerModule::isWallAt(const Position &p) const
{
    for (const auto &w : _walls)
        if (isSamePos(w, p))
            return true;
    return false;
}

void NibblerModule::spawnWallSegment()
{
    constexpr int maxAttempts = 20;
    constexpr int minLength = 4;
    constexpr int maxLength = 8;

    std::vector<Position> freeCells = getFreeCells();

    if (freeCells.empty())
        return;

    std::uniform_int_distribution<size_t> distCell(0, freeCells.size() - 1);
    std::uniform_int_distribution<size_t> distLen(minLength, maxLength);

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        const bool horizontal = (_rng() % 2 == 0);
        const size_t length = distLen(_rng);
        const Position start = freeCells[distCell(_rng)];

        std::vector<Position> candidateLine;
        candidateLine.reserve(length);
        bool valid = true;

        for (size_t i = 0; i < length; ++i) {
            const int x = horizontal ? static_cast<int>(start.x) + static_cast<int>(i)
                                     : static_cast<int>(start.x);
            const int y = horizontal ? static_cast<int>(start.y)
                                     : static_cast<int>(start.y) + static_cast<int>(i);

            if (x < 1 || x > MAX_GRID_X - 2 || y < 3 || y > MAX_GRID_Y - 3) {
                valid = false;
                break;
            }

            const Position p(static_cast<float>(x), static_cast<float>(y));
            if (isOccupied(p)) {
                valid = false;
                break;
            }

            candidateLine.push_back(p);
        }

        if (!valid)
            continue;

        _walls.insert(_walls.end(), candidateLine.begin(), candidateLine.end());
        return;
    }
}

void NibblerModule::updateLevel()
{
    constexpr float minMoveInterval = 0.06f;

    if (_score < static_cast<int>(_nextLevelScore))
        return;

    ++_level;
    _nextLevelScore += 5;

    if (_moveInterval > minMoveInterval)
        _moveInterval -= 0.01f;

    spawnWallSegment();
}

bool NibblerModule::isOccupied(const Position &p) const
{
    if (_nibbler.containsAtThisPos(p))
        return true;
    if (_hasFood && isSamePos(_food, p))
        return true;
    if (isWallAt(p))
        return true;
    return false;
}

NibblerModule::NibblerModule() : _score(0), _level(1), _nextLevelScore(5), _moveDelay(0.0f), _moveInterval(MOVE_INTERVAL_SECONDS), _hasFood(false), _rng(std::random_device{}())
{
    _currentState = GameState::MENU;
    _renderContext = {MAX_GRID_X + 1, MAX_GRID_Y + 1, CoordinateSpace::GRID};
}

NibblerModule::~NibblerModule()
{
    stop();
}

void NibblerModule::init()
{
    _currentState = GameState::RUNNING;

    _score = 0;
    _level = 1;
    _nextLevelScore = 5;
    _moveDelay = 0.0f;
    _moveInterval = MOVE_INTERVAL_SECONDS;
    _hasFood = false;
    _food = Position(-1.0f, -1.0f);
    _walls.clear();

    _nibbler = Nibbler();
    spawnFood();
    spawnWallSegment();
    refreshEntities();
    refreshUI();
}

void NibblerModule::stop()
{
    _currentState = GameState::MENU;
    _score = 0;
    _level = 1;
    _nextLevelScore = 5;
    _moveDelay = 0.0f;
    _moveInterval = MOVE_INTERVAL_SECONDS;
    _hasFood = false;
    _food = Position(-1.0f, -1.0f);
    _entities.clear();
    _uiText.clear();
    _walls.clear();
}

const std::string& NibblerModule::getName() const
{
    const std::string &name = "Nibbler";
    return name;
}

void NibblerModule::update(float deltaTime)
{   
    if (_currentState != GameState::RUNNING)
        return;

    if (deltaTime <= 0.0f)
        return;

    _moveDelay += deltaTime;

    while (_moveDelay >= _moveInterval && _currentState == GameState::RUNNING) {
        _moveDelay -= _moveInterval;

        Nibbler::NibblerSegment nextHead = _nibbler.getNextHead();
        const bool hasEaten = _hasFood
            && nextHead.pos.x == _food.x
            && nextHead.pos.y == _food.y;

        if ((_nibbler.containsAtThisPos(nextHead.pos) || isWallAt(nextHead.pos))
        && !hasEaten) {
            _nibbler.setAlive(false);
            _currentState = GameState::GAME_OVER;
            refreshUI();
            return;
        } else {
            if (hasEaten) {
                _score++;
                updateLevel();
                spawnFood();
            }
            _nibbler.updateBody(hasEaten, nextHead);
        }

        refreshEntities();
        refreshUI();
    }
}

void NibblerModule::refreshFrame()
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

void NibblerModule::refreshEntities()
{
    _entities.clear();

    const std::deque<Nibbler::NibblerSegment> &segments = _nibbler.getSegments();

    for (size_t i = 0; i < segments.size(); ++i) {
        Entity segmentEntity;
        segmentEntity.pos = segments[i].pos;
        segmentEntity.size = Position(1.0f, 1.0f);
        segmentEntity.character = (i + 1 == segments.size()) ? '@' : 'o';
        segmentEntity.color = (i + 1 == segments.size()) ? Colors::CYAN : Colors::MAGENTA;
        _entities.emplace_back(segmentEntity);
    }

    if (_hasFood) {
        Entity foodEntity;
        foodEntity.pos = _food;
        foodEntity.size = Position(1.0f, 1.0f);
        foodEntity.character = '*';
        foodEntity.color = Colors::RED;
        _entities.emplace_back(foodEntity);
    }

    if (!_walls.empty()) {
        for (const Position &i : _walls) {
            Entity wallEntity;
            wallEntity.pos = i;
            wallEntity.size = Position(1.0f, 1.0f);
            wallEntity.character = '#';
            wallEntity.color = Colors::WHITE;
            _entities.emplace_back(wallEntity);
        }
    }
    refreshFrame();
}

void NibblerModule::refreshUI()
{
    _uiText.clear();

    std::string title = "Nibbler";
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
    _uiText.emplace_back("[Arrows: move] [P: pause] [R: restart] [Esc: exit]",
                         1.0f, MAX_GRID_Y, Colors::CYAN, 36);
}

void NibblerModule::handleInput(Key key)
{
    handleGameInput(key);
    switch (key) {
        case Key::LEFT:
            _nibbler.setFace(Nibbler::Face::WEST);
            break;
        case Key::RIGHT:
            _nibbler.setFace(Nibbler::Face::EAST);
            break;
        case Key::UP:
            _nibbler.setFace(Nibbler::Face::NORTH);
            break;
        case Key::DOWN:
            _nibbler.setFace(Nibbler::Face::SOUTH);
            break;
        default:
            break;
    }
}

const std::vector<Entity>& NibblerModule::getEntities() const
{
    return _entities;
}

const std::vector<Text>& NibblerModule::getUIText() const
{
    return _uiText;
}

GameState NibblerModule::getState() const
{
    return _currentState;
}

int NibblerModule::getScore() const
{
    return _score;
}

void NibblerModule::restart()
{
    stop();
    init();
}

void NibblerModule::pause()
{
    if (_currentState == GameState::RUNNING) {
        _currentState = GameState::PAUSED;
        refreshUI();
    }
}

void NibblerModule::resume()
{
    if (_currentState == GameState::PAUSED) {
        _currentState = GameState::RUNNING;
        refreshUI();
    }
}

RenderContext NibblerModule::getRenderContext() const
{    
    return _renderContext;
}

}
