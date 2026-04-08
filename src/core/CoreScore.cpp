/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** CoreScore.cpp
*/

#include "core/Core.hpp"
#include <iostream>
#include <fstream>

namespace arcade {

void Core::updateScore(std::size_t newScore)
{
    std::string game = libName(_gameLibs[_currentGameIdx]);
    auto& playerScores = _scores[game];

    if (playerScores[_playerName] < newScore)
        playerScores[_playerName] = newScore;
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
