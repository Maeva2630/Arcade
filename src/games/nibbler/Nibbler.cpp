/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Nibbler.cpp
*/

#include "games/nibbler/Nibbler.hpp"
#include "games/nibbler/NibblerStaticMap.hpp"
#include "games/nibbler/NibblerEntities.hpp"
#include <vector>
#include <random>
#include <algorithm>


extern "C" arcade::IModule *entryPoint(void)
{
    return new arcade::NibblerModule();
}

namespace {
    constexpr size_t foodBaseAmount = 3;
    constexpr size_t levelNb = 5;
    constexpr float wallBlockTimer = 0.2f;
    constexpr float autoTurnAfterSeconds = 0.8f;
}

namespace arcade {

void NibblerModule::spawnFood()
{
    std::vector<Position> freeCells;

    for (int y = START_ROW; y < END_ROW; ++y) {
        for (int x = START_COL; x < END_COL; ++x) {
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

    for (size_t i = 0; i < foodBaseAmount; i++) {
        std::uniform_int_distribution<size_t> dist(0, freeCells.size() - 1);
        _food.emplace_back(freeCells[dist(_rng)]);
        _hasFood = true;
    }
}

std::vector<Position> NibblerModule::getFreeCells()
{
    std::vector<Position> freeCells;

    for (int y = START_ROW; y < END_ROW; ++y) {
        for (int x = START_COL; x < END_COL; ++x) {
            Position candidate(static_cast<float>(x), static_cast<float>(y));
            if (!isOccupied(candidate))
                freeCells.emplace_back(candidate);
        }
    }

    return freeCells;
}

void NibblerModule::loadStaticMap()
{
    _walls.clear();
    const NibblerStaticMap &map = getNibblerStaticMap(static_cast<std::size_t>(_level));

    for (std::size_t row = 0; row < END_ROW - START_ROW; ++row) {
        for (std::size_t col = 0; col < END_COL - START_COL; ++col) {
            if (map[row][col] == 1) {
                _walls.emplace_back(
                    static_cast<float>(START_COL + static_cast<int>(col)),
                    static_cast<float>(START_ROW + static_cast<int>(row))
                );
            }
        }
    }
}

bool isAtPosition(const Position &a, const Position &b)
{
    return a.x == b.x && a.y == b.y;
}

bool NibblerModule::isWallAt(const Position &p) const
{
    for (const auto &w : _walls)
        if (isAtPosition(w, p))
            return true;
    return false;
}

bool NibblerModule::isFoodAt(const Position &p) const
{
    for (const auto &f : _food)
        if (isAtPosition(f, p))
            return true;
    return false;
}

void NibblerModule::updateLevel()
{
    constexpr float minMoveInterval = 0.06f;

    if (!_food.empty())
        return;

    _level++;

    if (_level >= levelNb) {
        _currentState = GameState::WON;
        return;
    }
    spawnFood();
    loadStaticMap();

    if (_moveInterval > minMoveInterval)
        _moveInterval -= 0.01f;
}

bool NibblerModule::isOccupied(const Position &p) const
{
    if (_nibbler.containsAtThisPos(p))
        return true;
    if (_hasFood && isFoodAt(p))
        return true;
    if (isWallAt(p))
        return true;
    return false;
}

NibblerModule::NibblerModule() : _score(0), _level(1), _moveDelay(0.0f),
_moveInterval(MOVE_INTERVAL_SECONDS), _wallBlockTimer(0.0f), _wallStuckTime(0.0f),
_lastBlockedFace(Nibbler::Face::EAST), wallStuck(false), _hasFood(false), _rng(std::random_device{}())
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
    _moveDelay = 0.0f;
    _moveInterval = MOVE_INTERVAL_SECONDS;
    _wallBlockTimer = 0.0f;
    _wallStuckTime = 0.0f;
    _lastBlockedFace = Nibbler::Face::EAST;
    wallStuck = false;
    _hasFood = false;
    loadStaticMap();
    _nibbler = Nibbler();
    _food.clear();
    spawnFood();
    refreshEntities();
    refreshUI();
}

void NibblerModule::stop()
{
    _currentState = GameState::MENU;
    _score = 0;
    _level = 1;
    _moveDelay = 0.0f;
    _moveInterval = MOVE_INTERVAL_SECONDS;
    _wallBlockTimer = 0.0f;
    _wallStuckTime = 0.0f;
    _lastBlockedFace = Nibbler::Face::EAST;
    wallStuck = false;
    _hasFood = false;
    _food.clear();
    _entities.clear();
    _uiText.clear();
    _walls.clear();
}

const std::string& NibblerModule::getName() const
{
    const std::string &name = "Nibbler";
    return name;
}

void NibblerModule::removeFoodAt(const Position &p)
{
    auto it = std::find_if(_food.begin(), _food.end(),
        [&](const Position &f) {
        return isAtPosition(f, p);
    });

    if (it == _food.end())
        return;

    _food.erase(it);
    _hasFood = !_food.empty();
}

bool NibblerModule::tryTurnFromFace(Nibbler::Face currentFace)
{
    Nibbler::Face firstChoice = Nibbler::Face::NORTH;
    Nibbler::Face secondChoice = Nibbler::Face::SOUTH;

    switch (currentFace) {
        case Nibbler::Face::EAST:
            firstChoice = Nibbler::Face::NORTH;
            secondChoice = Nibbler::Face::SOUTH;
            break;
        case Nibbler::Face::WEST:
            firstChoice = Nibbler::Face::SOUTH;
            secondChoice = Nibbler::Face::NORTH;
            break;
        case Nibbler::Face::NORTH:
            firstChoice = Nibbler::Face::WEST;
            secondChoice = Nibbler::Face::EAST;
            break;
        case Nibbler::Face::SOUTH:
            firstChoice = Nibbler::Face::EAST;
            secondChoice = Nibbler::Face::WEST;
            break;
    }

    for (Nibbler::Face nwFace : {firstChoice, secondChoice}) {
        _nibbler.setFace(nwFace);
        const Nibbler::NibblerSegment newHead = _nibbler.getNextHead();
        if (!isWallAt(newHead.pos) && !_nibbler.containsAtThisPos(newHead.pos))
            return true;
    }

    _nibbler.setFace(currentFace);
    return false;
}

bool NibblerModule::handleWallCollision(Nibbler::Face currentFace)
{
    if (wallStuck && currentFace == _lastBlockedFace)
        _wallStuckTime += wallBlockTimer;
    else {
        wallStuck = true;
        _lastBlockedFace = currentFace;
        _wallStuckTime = wallBlockTimer;
    }

    if (_wallStuckTime >= autoTurnAfterSeconds && tryTurnFromFace(currentFace)) {
        _wallStuckTime = 0.0f;
        wallStuck = false;
        _wallBlockTimer = 0.0f;
        return true;
    }
    _wallBlockTimer = wallBlockTimer;
    _moveDelay = 0.0f;
    refreshUI();
    return false;
}

void NibblerModule::resetWallStuckState()
{
    wallStuck = false;
    _wallStuckTime = 0.0f;
}

bool NibblerModule::updateWallBlockTimer(float deltaTime)
{
    if (_wallBlockTimer > 0.0f) {
        _wallBlockTimer = (_wallBlockTimer - deltaTime > 0) ?
        _wallBlockTimer - deltaTime : 0;
        return false;
    }
    return true;
}

void NibblerModule::update(float deltaTime)
{
    if (_currentState != GameState::RUNNING || deltaTime <= 0.0f)
        return;

    if (!updateWallBlockTimer(deltaTime))
        return;
    _moveDelay += deltaTime;
    while (_moveDelay >= _moveInterval && _currentState == GameState::RUNNING) {
        _moveDelay -= _moveInterval;

        Nibbler::NibblerSegment nextHead = _nibbler.getNextHead();
        const bool hasEaten = (_hasFood && isFoodAt(nextHead.pos)) ? true : false;

        if (isWallAt(nextHead.pos)) {
            const Nibbler::Face currentFace = _nibbler.getHead().face;
            if (handleWallCollision(currentFace))
                continue;
            break;
        }
        resetWallStuckState();

        if ((_nibbler.containsAtThisPos(nextHead.pos))
        && !hasEaten) {
            _nibbler.setAlive(false);
            _currentState = GameState::GAME_OVER;
            refreshUI();
            return;
        } else {
            if (hasEaten) {
                _score++;
                removeFoodAt(nextHead.pos);
                updateLevel();
            }
            _nibbler.updateBody(hasEaten, nextHead);
        }
        refreshEntities();
        refreshUI();
    }
}

void NibblerModule::refreshFrame()
{
    NibblerEntities::appendFrame(_entities);
}

void NibblerModule::refreshEntities()
{
    const std::deque<Nibbler::NibblerSegment> &segments = _nibbler.getSegments();
    NibblerEntities::buildEntities(_entities, segments, _food, _walls);
}

void NibblerModule::refreshUI()
{
    NibblerEntities::buildUI(_uiText, _score, _currentState, FONT_SIZE);
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
