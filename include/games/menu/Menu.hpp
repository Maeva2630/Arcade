/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Menu.hpp
*/

/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** TestModule.hpp
*/

#ifndef MENUMODULE_HPP_
    #define MENUMODULE_HPP_

    #define FONTSIZE 50
    #define SPACING 4
    #define INDENTATION 10
    #define SEPARATOR ":"
    #define COMMENTS "#"
    #define SAVEPATH "highscores.txt"
    #define LASTSCOREPATH "last_player_score.txt"

    #include "games/AGameModule.hpp"
    #include "graphicals/IDisplayModule.hpp"
    #include <unordered_map>
    #include <filesystem>
    #include <functional>
    #include <iostream>
    #include <fstream>
    #include <map>

namespace arcade {

    class MenuModule : public AGameModule 
    {
        public:
            /**
             * @brief Construct a new Menu Module object
             * 
             * init current state and renderContext
             */
            MenuModule();
            ~MenuModule() override = default;
    
            /**
             * @brief Init game
             * 
             * write ???
             */
            void init() override;

            /**
             * @brief Stop game
             * 
             * write ???
             */
            void stop() override;

            /**
             * @brief Get the Name object
             * 
             * @return const std::string& 
             */
            const std::string& getName() const override;

            /**
             * @brief Update game states and displays
             * 
             * write ???
             * @param deltaTime 
             */
            void update(float deltaTime) override;

            /**
             * @brief Handle user inputs for the game
             * 
             * write ???
             * @param key 
             */
            void handleInput(Key key) override;

            /**
             * @brief Get entities
             * 
             * @return const std::vector<Entity>& 
             */
            const std::vector<Entity>& getEntities() const override;

            /**
             * @brief Get UI text from the game to display
             * 
             * @return const std::vector<Text>& 
             */
            const std::vector<Text>& getUIText() const override;

            /**
             * @brief Get the current game state
             * 
             * @return GameState 
             */
            GameState getState() const override;

            /**
             * @brief Get the current user score
             * 
             * @return int 
             */
            int getScore() const override;

            /**
             * @brief Do nothing, you can't restart a menu write ??? maybe restart from 0
             */
            void restart() override {};

            /**
             * @brief Do nothing, you can't pause the menu
             */
            void pause() override {};

            /**
             * @brief Do nothing, if menu can't be paused, you can't resume it
             */
            void resume() override {};

            /**
             * @brief Get the Render Context object
             * 
             * @return RenderContext 
             */
            RenderContext getRenderContext() const override;

            void setAvailableGames(const std::vector<std::string>& games);
            void setAvailableDisplays(const std::vector<std::string>& displays);

        private:
            /**
             * @brief Update all menu UI texts
             * 
             * clear and emplace back all texts needed for UI
             * (score, commands, menu states texts)
             */
            void refreshUI() override;

            void renderPlayerName(std::size_t& x, std::size_t& y);
            void renderHighScores(std::size_t& x, std::size_t& y);
            void renderGamesLib(std::size_t& x, std::size_t& y);
            void renderDisplaysLib(std::size_t& x, std::size_t& y);

            /**
             * @brief Handle name input
             * @param key The input key to handle
             */
            void handleNameInput(Key key);

            /**
             * @brief Load scores from a file
             */
            void loadScores();

            /**
             * @brief Save the player's score with the format "playerName:highscore" in a file
             */
            void saveScore();

            /**
             * @brief Add a new score for a player in a game
             */
            void addScore();

            std::string _name = "Welcome to my Arcade";

            std::vector<std::string> _games;
            std::vector<std::string> _displays;

            std::size_t _selectedGame = 0;
            std::size_t _selectedDisplay = 0;

            std::string _playerName;
            // std::string _bestPlayerName;
            // GameName --> PlayerName --> High Score
            std::map<std::string, std::unordered_map<std::string, std::size_t>> _scores;
    
            bool _launchWithoutName;
    };
};

#endif
