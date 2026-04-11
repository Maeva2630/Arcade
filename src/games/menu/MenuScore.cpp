/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** MenuScore.cpp
*/

#include "menu/Menu.hpp"

namespace arcade {

void MenuModule::renderHighScores(std::size_t& x, std::size_t& y)
{
    _uiText.emplace_back("=== HIGH SCORES ===", x, y += SPACING, Colors::YELLOW, FONTSIZE + 10);

    if (_games.empty() || _selectedGame > _games.size())
        return;

    _uiText.emplace_back(_games[_selectedGame] + ":", x, y += SPACING, Colors::MAGENTA, FONTSIZE);

    std::string bestPlayer;
    std::size_t bestScore = 0;
    std::size_t myScore = 0;

    auto it = _scores.find(_games[_selectedGame]);
    if (it == _scores.end()) {
        _uiText.emplace_back("Best: ~~~", x + 2, y += SPACING, Colors::WHITE, FONTSIZE - 10);
        _uiText.emplace_back("You: ~~~", x + 2, y += SPACING, Colors::CYAN, FONTSIZE - 10);
        return;
    }

    for (const auto& playerScores : it->second) {
        if (bestPlayer.empty() || playerScores.second > bestScore) {
            bestPlayer = playerScores.first;
            bestScore = playerScores.second;
        }
        if (playerScores.first == _playerName)
            myScore = playerScores.second;
    }

    _uiText.emplace_back(
        "Best: " + (bestPlayer.empty() ? "~~~" : bestPlayer + " - " + std::to_string(bestScore)),
        x + 2, y += SPACING,
        Colors::WHITE,
        FONTSIZE - 10
    );

    _uiText.emplace_back(
        "You: " + (_playerName.empty() ? "~~~" : _playerName + " - " + std::to_string(myScore)),
        x + 2, y += SPACING,
        Colors::CYAN,
        FONTSIZE - 10
    );
}

void MenuModule::loadScores()
{
    std::ifstream file(SAVEPATH);
    std::string line;
    std::string gameName = "";

    if (!file.is_open()) {
        std::cerr << "Failed to open " << SAVEPATH << " for loading scores." << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::size_t delimiter = line.find(SEPARATOR);

        if (line.empty() || line.find(COMMENTS) == 0)
            continue;

        if (delimiter == line.npos) {
            if (std::find(_games.begin(), _games.end(), line) != _games.end())
                gameName = line;
            else
                gameName.clear();
            continue;
        }

        if (gameName.empty())
            continue;

        std::string playerName = line.substr(0, line.find(SEPARATOR));

        std::size_t highScore = 0;
        try {
            highScore = std::stoul(line.substr(delimiter + 1));
        } catch (...) {
            continue;
        }

        if (playerName.empty() || highScore == 0) {
            std::cerr << "Invalid data:" << std::endl;
            std::cerr << "   Player Name: " << playerName << std::endl;
            std::cerr << "   High Score: " << highScore << std::endl;
            continue;
        }

        _scores[gameName][playerName] = std::max(_scores[gameName][playerName], highScore);
    }

    file.close();
}

void MenuModule::saveScore()
{
    std::ofstream file(SAVEPATH);

    if (!file.is_open()) {
        std::cerr << "Failed to open " << SAVEPATH << " for saving scores." << std::endl;
        return;
    }

    file << COMMENTS << "[Game Name]" << std::endl;
    file << COMMENTS << "[Player Name]:[High Score]\n" << std::endl;

    for (const auto& scores : _scores) {
        file << scores.first << std::endl;

        for (const auto& playerscores : scores.second) {
            if (!playerscores.first.empty() && playerscores.second > 0)
                file << playerscores.first << SEPARATOR << playerscores.second << std::endl;
        }

        file << std::endl;
    }

    file.close();
}

void MenuModule::addScore(std::string gameName, std::size_t score)
{
    if (!gameName.empty() && !_playerName.empty() && score > 0)
        _scores[gameName][_playerName] = std::max(_scores[gameName][_playerName], score);
}

}
