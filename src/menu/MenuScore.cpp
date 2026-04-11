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

    if (_games.empty() || _selectedGame >= _games.size())
        return;

    _uiText.emplace_back(_games[_selectedGame] + ":", x, y += SPACING, Colors::MAGENTA, FONTSIZE);

    auto it = _scores.find(_games[_selectedGame]);
    if (it == _scores.end()) {
        _uiText.emplace_back("No scores yet", x + 2, y += SPACING, Colors::WHITE, FONTSIZE - 10);
        return;
    }

    std::vector<std::pair<std::string, std::size_t>> sorted(it->second.begin(), it->second.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    y += SPACING;
    std::size_t offsetX = x + 2;
    std::size_t rank = 1;
    bool playerInTop = false;

    for (const auto& [playerName, score] : sorted) {
        if (rank > TOP_N)
            break;

        bool isPlayer = (playerName == _playerName);
        if (isPlayer)
            playerInTop = true;

        std::string entry = std::to_string(rank) + ". " + playerName + " - " + std::to_string(score);

        if (rank > 1) {
            _uiText.emplace_back("|", offsetX, y, Colors::WHITE, FONTSIZE - 10);
            offsetX += 2;
        }

        _uiText.emplace_back(entry, offsetX, y, isPlayer ? Colors::CYAN : Colors::WHITE, FONTSIZE - 10);
        offsetX += entry.size() + 2;
        rank++;
    }

    if (!playerInTop && !_playerName.empty()) {
        auto playerIt = it->second.find(_playerName);
        if (playerIt != it->second.end()) {
            std::string line = "You: " + _playerName + " - " + std::to_string(playerIt->second);
            _uiText.emplace_back(line, x + 2, y += SPACING, Colors::CYAN, FONTSIZE - 10);
        }
    }
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
    if (!_nameFocused && !gameName.empty() && !_playerName.empty() && score > 0)
        _scores[gameName][_playerName] = std::max(_scores[gameName][_playerName], score);

    refreshUI();
}

}
