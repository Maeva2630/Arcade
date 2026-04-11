/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** IMenuModule.hpp
*/

#ifndef IMENUMODULE_HPP
    #define IMENUMODULE_HPP
    #include "IModule.hpp"
    #include "graphicals/IDisplayModule.hpp"
    #include "games/IGameModule.hpp"

namespace arcade {

/**
 * @class IMenuModule
 * @brief Interface for menu libraries
 *
 * This interface must be implemented by the menu library, which is responsible for
 * displaying the main menu, handling user input for game/display selection, and
 * showing high scores.
 * 
 * The menu module is a special type of game module that is active
 * when the user is navigating the menu. It should provide a way to switch to
 * the selected game and display modules when the user starts the game.
 * 
 */
class IMenuModule : public IModule {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~IMenuModule() = default;

        /**
         * @brief Initialize the menu
         *
         * Prepares the menu for display and interaction, loads score..
         * This method should be called once before starting the game.
         *
         * @throw ModuleException if initialization fails
         */
        virtual void init() = 0;

        /**
         * @brief Stop and cleanup the menu
         *
         * Releases all menu resources and resets state.
         * This method should be called once when exiting the menu.
         */
        virtual void stop() = 0;

        /**
         * @brief Get the name of this menu
         * @return Name of the menu
         */
        virtual const std::string& getName() const = 0;

        /**
         * @brief Handle user mouses inputs for the menu
         * player name input
         * @param event
         */
        virtual void handleMouseInput(const MouseEvent& event) = 0;

        /**
         * @brief Handle keyboard input
         *
         * Process a key event to update menu state accordingly.
         * The core program will call this method when a key is pressed.
         *
         * @param key The key that was pressed
         */
        virtual void handleInput(Key key) = 0;

        /**
         * @brief Get UI text elements to be rendered
         *
         * Returns text elements for title, player name, score, libraries, etc.
         *
         * @return Const reference to vector of text elements to render
         */
        virtual const std::vector<Text>& getUIText() const = 0;

        /**
         * @brief Get the current state
         * @return Current state (menu, running, paused, game over, won)
         */
        virtual GameState getState() const = 0;

        /**
         * @brief Get the render context for this game
         *
         * Defines the logical rendering space and coordinate system used by
         * entities and text returned by the game.
         *
         * @return Logical render context
         */
        virtual RenderContext getRenderContext() const = 0;

        /**
         * @brief Provide the list of available game libraries (optional)
         * Only used by the menu module. Other games can ignore this.
         */
        virtual void setAvailableGames(const std::vector<std::string>&) = 0;
        
        /**
         * @brief Provide the list of available display libraries (optional)
         * Only used by the menu module. Other games can ignore this.
         */
        virtual void setAvailableDisplays(const std::vector<std::string>&) = 0;

        /**
         * @brief Get the current player name
         */
        virtual const std::string& getPlayerName() const = 0;

        /**
         * @brief Integrate score from the last played game
         * @param gameName The name of the game for which to add the score (not the path)
         * @param score The score to add
         */
        virtual void addScore(std::string gameName, std::size_t score) = 0;
};

} // namespace arcade

#endif
