/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <fstream>
#include <dlfcn.h>

namespace arcade {

static const char* LIB_DIR = "lib";

Core::Core(std::string libPath)
{
    bool found = false;

    loadLibs(LIB_DIR);

    if (_displayLibs.empty())
        throw CoreException("No display librairies found.");

    if (_gameLibs.empty())
        throw CoreException("No game librairies found.");

    if (libPath.substr(0, 2) == "./")
        libPath = libPath.substr(2);

    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        if (_displayLibs[i] == libPath) {
            _currentDisplayIdx = i;
            found = true;
            break;
        }
    }

    if (!found)
        throw CoreException(libPath + " is not a graphics library.");

    _currentGameIdx = 0;
    _selectedGameIdx = _currentGameIdx;
    _selectedDisplayIdx = _currentDisplayIdx;
    
    switchDisplay(_currentDisplayIdx);
    switchGame(_currentGameIdx);
    
    loadScores();
    
    _previousState = GameState::MENU;

    _running = true;
    _inCoreMenu = true;
    _launchWithoutName = false;
}

void Core::initAll()
{
    _display->init();
    _game->init();
    _display->setRenderContext(_game->getRenderContext());
    _display->setTitle(_game->getName());
}

void Core::stopAll()
{
    _display->stop();
    _game->stop();
}

void Core::run()
{
    initAll();
    mainLoop();
    saveScore();
    stopAll();
}

void Core::applyMenuSelection()
{
    if (!_inCoreMenu)
        return;
    if (_playerName.empty()) {
        _launchWithoutName = true;
        return;
    }
    if (_game->getState() == GameState::PAUSED)
        _game->resume();
    _inCoreMenu = false;
    _launchWithoutName = false;
}

void Core::handleNameInput(Key key)
{
    static const std::size_t maxNameLength = 20;
    static const std::map<Key, char> nameInputs = {
        {Key::NUM_0, '0'}, {Key::NUM_1, '1'}, {Key::NUM_2, '2'},
        {Key::NUM_3, '3'}, {Key::NUM_4, '4'}, {Key::NUM_5, '5'},
        {Key::NUM_6, '6'}, {Key::NUM_7, '7'}, {Key::NUM_8, '8'},
        {Key::NUM_9, '9'},
        {Key::A, 'a'}, {Key::B, 'b'}, {Key::C, 'c'}, {Key::D, 'd'},
        {Key::E, 'e'}, {Key::F, 'f'}, {Key::G, 'g'}, {Key::H, 'h'},
        {Key::I, 'i'}, {Key::J, 'j'}, {Key::K, 'k'}, {Key::L, 'l'},
        {Key::M, 'm'}, {Key::N, 'n'}, {Key::O, 'o'}, {Key::P, 'p'},
        {Key::Q, 'q'}, {Key::R, 'r'}, {Key::S, 's'}, {Key::T, 't'},
        {Key::U, 'u'}, {Key::V, 'v'}, {Key::W, 'w'}, {Key::X, 'x'},
        {Key::Y, 'y'}, {Key::Z, 'z'},
    };

    if (key == Key::BACKSPACE && !_playerName.empty()) {
        _playerName.pop_back();
    } else {
        auto it = nameInputs.find(key);
        if (it != nameInputs.end() && _playerName.size() < maxNameLength) {
            _playerName += it->second;
        }
    }
}

void Core::handleGlobalInput(Key key)
{
    static const std::map<Key, std::function<void()>> coreInputs = {
        {Key::NEXT_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + 1) % _displayLibs.size(); switchDisplay(_selectedDisplayIdx); }},
        {Key::PREV_GRAPHICS, [this] { _selectedDisplayIdx = (_selectedDisplayIdx + _displayLibs.size() - 1) % _displayLibs.size(); switchDisplay(_selectedDisplayIdx); }},
        {Key::NEXT_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + 1) % _gameLibs.size(); switchGame(_selectedGameIdx); }},
        {Key::PREV_GAME,     [this] { _selectedGameIdx = (_selectedGameIdx + _gameLibs.size() - 1) % _gameLibs.size(); switchGame(_selectedGameIdx); }},
        {Key::MENU,          [this] { _game->pause(); _inCoreMenu = true; }},
        {Key::ENTER,         [this] { applyMenuSelection(); }},
        {Key::EXIT,          [this] { _running = false; }},
        {Key::ESCAPE,        [this] { _running = false; }},
    };
 
    const auto it = coreInputs.find(key);
    if (it != coreInputs.end()) {
        it->second();
        return;
    }
 
    if (_inCoreMenu) {
        handleNameInput(key);
    } else if (_game) {
        _game->handleInput(key);
    }
}

void Core::mainLoop()
{
    static const std::map<GameState, std::function<void()>> renderHandlers = {
        {GameState::MENU,       [this]() { renderMenu(); }},
        {GameState::RUNNING,    [this]() { renderGame(); }},
        {GameState::PAUSED,     [this]() { renderGame(); }},
        {GameState::GAME_OVER,  [this]() { renderGame(); }},
        {GameState::WON,        [this]() { renderGame(); }},
    };

    while (_running && _display->isOpen()) {
        Key key = _display->getKey();
        if (key != Key::UNKNOWN)
            handleGlobalInput(key);

        if (_inCoreMenu) {
            renderMenu();
        } else {
            GameState currentState = _game->getState();

            if (currentState != _previousState) {
                if (currentState == GameState::GAME_OVER || currentState == GameState::WON)
                    saveScore();
                _previousState = currentState;
            }

            const auto it = renderHandlers.find(currentState);
            if (it != renderHandlers.end())
                it->second();
        }

        updateScore(_game->getScore());
        _display->display();
    }
}

void Core::renderGame()
{
    _display->clear();
    _game->update(_display->getDeltaTime());

    for (Entity entity : _game->getEntities())
        _display->drawEntity(entity);

    for (Text text : _game->getUIText())
        _display->drawText(text);
}

static std::string libName(const std::string& path)
{
    std::size_t slash = path.rfind('/');
    std::string file;

    if (slash == std::string::npos) {
        file = path;
    } else {
        file = path.substr(slash + 1);
    }

    if (file.substr(0, 7) == "arcade_")
        file = file.substr(7);

    if (file.find(".") != std::string::npos)
        file = file.substr(0, file.find("."));

    return file;
}

void Core::renderPlayerName()
{
    _menuY += SPACING;
    _display->drawText(Text("Player Name: (authorized characters: a-z, 0-9)", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    if (_playerName.empty())
        _display->drawText(Text("Enter your name...", _menuX + 2, _menuY += SPACING, (_launchWithoutName) ? Colors::RED : Colors::WHITE, FONTSIZE));
    else
        _display->drawText(Text(_playerName, _menuX + 2, _menuY += SPACING, Colors::WHITE, FONTSIZE));

}

void Core::renderHighScores()
{
    _menuY += SPACING;
    _display->drawText(Text("=== HIGH SCORES ===", _menuX, _menuY += SPACING, Colors::YELLOW, FONTSIZE + 5));

    for (const auto& gameScore : _scores) {
        const std::string game = gameScore.first;
        if (game != libName(_gameLibs[_selectedGameIdx]))
            continue;

        _display->drawText(Text(game + ":", _menuX, _menuY += SPACING, Colors::MAGENTA, FONTSIZE));

        std::pair<std::string, std::size_t> bestPlayerEver;
        std::pair<std::string, std::size_t> player;
        for (const auto& playerScores : gameScore.second)
            if (bestPlayerEver.first.empty()|| playerScores.second > bestPlayerEver.second) {
                bestPlayerEver = playerScores;
            if (playerScores.first == _playerName)
                player = playerScores;
        }

        if (bestPlayerEver.second == 0) {
            _display->drawText(Text(
                "Best: ~~~ ",
                _menuX + 2,
                _menuY += SPACING,
                Colors::WHITE,
                FONTSIZE
            ));
        } else {
            _display->drawText(Text(
                "Best: " + bestPlayerEver.first + " - " +
                std::to_string(bestPlayerEver.second),
                _menuX + 2,
                _menuY += SPACING,
                Colors::WHITE,
                FONTSIZE
            ));
        }

        if (_playerName.empty()) {
            _display->drawText(Text(
                "You: ~~~ ",
                _menuX + 2,
                _menuY += SPACING,
                Colors::CYAN,
                FONTSIZE
            ));
        } else {
            _display->drawText(Text(
                "You: " + _playerName + " - " + std::to_string(player.second),
                _menuX + 2,
                _menuY += SPACING,
                Colors::CYAN,
                FONTSIZE
            ));
        }
    }
}

void Core::renderSelection()
{
    _menuY += SPACING;
    _display->drawText(Text("=== SELECT YOUR GAME AND GRAPHICS ===", _menuX, _menuY += SPACING, Colors::YELLOW, FONTSIZE + 5));

    _display->drawText(Text("Game  [F3/F4]:", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    for (std::size_t i = 0; i < _gameLibs.size(); i++) {
        Color c = (i == _selectedGameIdx) ? Colors::GREEN : Colors::WHITE;
        std::string marker = (i == _selectedGameIdx) ? "> " : "  ";
        _display->drawText(Text(marker + libName(_gameLibs[i]), _menuX + 2, _menuY += SPACING, c, FONTSIZE));
    }

    _display->drawText(Text("Graphics [F1/F2]:", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        bool isSelected = (i == _selectedDisplayIdx);
        bool isActive = (i == _currentDisplayIdx);

        Color c = isSelected ? Colors::GREEN : Colors::WHITE;

        std::string marker = isSelected ? "> " : "  ";
        std::string active = isActive ? " [RUNNING]" : "";

        _display->drawText(Text(
            marker + libName(_displayLibs[i]) + active,
            2,
            _menuY += SPACING,
            c,
            FONTSIZE
        ));
    }
}

void Core::renderMenu()
{
    _menuX = 0;
    _menuY = 0;

    _display->clear();
    _display->drawText(Text("=== ARCADE ===", _menuX, _menuY, Colors::YELLOW, FONTSIZE + 10));

    renderPlayerName();
    renderHighScores();
    renderSelection();
    
    // Helper
    _display->drawText(
        Text("Enter: launch | Escape: exit", _menuX, _menuY += SPACING, Colors::WHITE, FONTSIZE - 8));
}

void Core::switchDisplay(std::size_t idx)
{
    if (idx >= _displayLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_display) {
        _display->stop();
        _display.reset();
        _displayLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IDisplayModule>>(_displayLibs[idx]);
    auto newDisplay = std::unique_ptr<IDisplayModule>(newLoader->getInstance("entryPoint"));
    
    _displayLoader = std::move(newLoader);
    _display = std::move(newDisplay);

    _display->init();
    if (_game)
        _display->setRenderContext(_game->getRenderContext());

    _currentDisplayIdx = idx;
}

void Core::switchGame(std::size_t idx)
{
    if (idx >= _gameLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_game) {
        _game->stop();
        _game.reset();
        _gameLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IGameModule>>(_gameLibs[idx]);
    auto newGame = std::unique_ptr<IGameModule>(newLoader->getInstance("entryPoint"));

    _gameLoader = std::move(newLoader);
    _game = std::move(newGame);

    _game->init();
    if (_display)
        _display->setRenderContext(_game->getRenderContext());

    _currentGameIdx = idx;
}

void Core::loadLibs(std::string dir)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string lib_path = entry.path();

        try {
            DLLoader<IModule> loader(lib_path);
            std::unique_ptr<IModule> module(loader.getInstance("entryPoint"));

            switch (module->getType()) {
                case ModuleType::DISPLAY:
                    _displayLibs.push_back(lib_path);
                    break;
                case ModuleType::GAME:
                    _gameLibs.push_back(lib_path);
                    break;
                default:
                    throw CoreException(lib_path + " invalid module type.");
            }

        } catch (const std::exception& e) {
            std::cerr << "Skipping " << lib_path << ": " << e.what() << std::endl;
        }
    }
}

void Core::loadScores()
{
    std::ifstream file(SAVEPATH);
    std::string line;
    std::string gameName = "";

    if (!file.is_open()) {
        std::cerr << "Failed to open " << SAVEPATH << " for loading scores." << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::size_t delimiter = line.find(':');

        if (line.empty() || line.find('#') != line.npos)
            continue;

        if (delimiter == line.npos) {
            gameName = "";
            for (std::size_t i = 0; i < _gameLibs.size(); i++) {
                if (libName(_gameLibs[i]) == line)
                    gameName = line;
            }
            continue;
        }

        if (gameName == "")
            continue;

        std::string playerName = line.substr(0, line.find(':'));
        std::size_t highScore = atoi(line.substr(delimiter + 1, line.size()).c_str());

        if (playerName.empty() || highScore == 0) {
            std::cerr << "Invalid data:" << std::endl;
            std::cerr << "   Player Name: " << playerName << std::endl;
            std::cerr << "   High Score: " << highScore << std::endl;
            continue;
        }

        _scores[gameName].emplace(playerName, highScore);
    }

    file.close();
}

void Core::updateScore(std::size_t newScore)
{
    std::string game = libName(_gameLibs[_currentGameIdx]);
    auto& playerScores = _scores[game];

    if (playerScores[_playerName] < newScore)
        playerScores[_playerName] = newScore;
}

void Core::saveScore()
{
    std::ofstream file(SAVEPATH);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << SAVEPATH << " for saving scores." << std::endl;
        return;
    }

    for (const auto& scores : _scores) {
        file << scores.first << std::endl;

        for (const auto& playerscores : scores.second) {
            if (!playerscores.first.empty() && playerscores.second > 0)
                file << playerscores.first << ':' << playerscores.second << std::endl;
        }

        file << std::endl;
    }

    file.close();
}

}
