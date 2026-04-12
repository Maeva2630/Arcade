/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Menu.hpp
*/

#ifndef MENUMODULE_HPP_
    #define MENUMODULE_HPP_

    #define FONTSIZE 50
    #define SPACING 4
    #define INDENTATION 10

    #define SEPARATOR ":"
    #define COMMENTS "#"
    #define SAVEPATH "highscores.txt"

    #define TOP_N 3
    #define NAMEUSAGE "Click here to enter your name..."

    #include "menu/AMenuModule.hpp"
    #include <filesystem>
    #include <functional>
    #include <iostream>
    #include <fstream>

namespace arcade {

class MenuModule : public AMenuModule
{
    public:
        /**
         * @brief Construct a new Menu Module object
         * init current state and renderContext
         */
        MenuModule();
        ~MenuModule() override = default;

        /**
         * @brief Get the Name object
         * @return const std::string& 
         */
        const std::string& getName() const override;

        /**
         * @brief Init menu
         */
        void init() override;

        /**
         * @brief Stop menu
         */
        void stop() override;

        /**
         * @brief Handle user mouses inputs for the menu
         * player name input
         * @param event
         */
        void handleMouseInput(const MouseEvent& event);

        /**
         * @brief Handle user inputs for the menu
         * player name input, game and display selection
         * @param key
         */
        void handleInput(Key key) override;

        /**
         * @brief Integrate score for the current player
         * @param gameName The name of the game for which to add the score (not the path)
         * @param score The score to add
         */
        void addScore(std::string gameName, std::size_t score) override;

        /**
         * @brief Get the Render Context object
         * @return RenderContext 
         */
        RenderContext getRenderContext() const override;

    private:
        // Rendu UI
        void refreshUI() override;
        void renderPlayerName(std::size_t& x, std::size_t& y);
        void renderHighScores(std::size_t& x, std::size_t& y);
        void renderGamesLib(std::size_t& x, std::size_t& y);
        void renderDisplaysLib(std::size_t& x, std::size_t& y);

        // Inputs
        void applyEnter();
        void handleNameInput(Key key);

        // Scores
        void loadScores();
        void saveScore();

        const std::string _name = "Welcome to my Arcade";
        
        bool _nameFocused = false;
        Position _namePosition;
};

} // namespace arcade

#endif
