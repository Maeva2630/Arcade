/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** AMenuuModuule.hpp
*/

#ifndef AMENUMODULE_HPP
    #define AMENUMODULE_HPP
    #include "menu/IMenuModule.hpp"
    #include "graphicals/IDisplayModule.hpp"
    #include "games/IGameModule.hpp"
    #include <unordered_map>
    #include <map>

namespace arcade {

class AMenuModule : public IMenuModule {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~AMenuModule() = default;

        /**
         * @brief Get the type of this module
         * @return ModuleType::MENU
         */
        virtual ModuleType getType() const  override { return ModuleType::MENU; }
        
        /**
         * @brief Get the ui text elements to render
         * @return ui text elements to render
         */
        const std::vector<Text>& getUIText() const override { return _uiText; }

        /**
         * @brief Get the current state
         * @return Current state (menu, running, paused, game over, won)
         */
        GameState getState() const override { return _currentState; }

        /**
         * @brief Get the player's name
         * @return Player's name
         */
        const std::string& getPlayerName() const override { return _playerName; }

        /**
         * @brief Set the list of available games and displays for the menu
         */
        void setAvailableGames(const std::vector<std::string>& games) override { _games = games; }
        void setAvailableDisplays(const std::vector<std::string>& displays) override { _displays = displays; }

    protected:
        /**
         * @brief Rebuild UI text elements from current state
         * 
         * Called to update score, status messages, controls help, etc.
         * in the _uiText vector for rendering.
         */
        virtual void refreshUI() {}

        GameState _currentState = GameState::MENU;
        std::vector<Text> _uiText;
        /**
         * @brief Vector of available games to display in the menu
         */
        std::vector<std::string> _games;

        /**
         * @brief Vector of available display libraries to display in the menu
         */
        std::vector<std::string> _displays;

        /**
         * @brief Index of the currently selected game in the menu
         */
        std::size_t _selectedGame = 0;

        /**
         * @brief Index of the currently selected display library in the menu
         */
        std::size_t _selectedDisplay = 0;

        /**
         * @brief Player's name
         */
        std::string _playerName;

        /**
         * @brief Scores map to store all scores for each game and player
         * The outer map's key is the game name, and its value is an unordered map where:
         * - The key is the player's name (std::string).
         * - The value is the player's high score (std::size_t) for that game
         */  
        std::map<std::string, std::unordered_map<std::string, std::size_t>> _scores;

        bool _launchWithoutName;
};

} // namespace arcade

#endif
