/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
    #define CORE_HPP
    #define SAVEPATH "last_player_score.txt"
    #define SEPARATOR ":"
    #include <map>
    #include <vector>
    #include <memory>
    #include <unordered_map>
    #include "games/IGameModule.hpp"
    #include "graphicals/IDisplayModule.hpp"
    #include "dlloader/DLLoader.hpp"

namespace arcade {
    class Core {
        public:
            /**
             * @brief Exception class for Core
             * 
             * if there is an error during the loading/ deletion of the library
             * it will be thrown with a CoreException
             * 
             */
            class CoreException : public std::exception
            {
                private:
                    std::string _message;
                public:
                    CoreException(std::string message) : _message(message) {};
                    const char* what() const noexcept override { return this->_message.c_str(); };
            };

            Core(std::string libPath);
            void run();
        
        private:
            void mainLoop();

            /**
             * @brief Handle global input
             * @param key The input key to handle
             */
            void handleGlobalInput(Key key);
    
            /**
             * @brief Handle name input
             * @param key The input key to handle
             */
            void handleNameInput(Key key);

            /**
             * @brief Switch the active display module
             * @param idx The index of the display module to switch to
             */
            void switchDisplay(std::size_t idx);

            /**
             * @brief Switch the active game module
             * @param idx The index of the game module to switch to
             */
            void switchGame(std::size_t idx);

            /**
             * @brief Load all libraries from a directory
             * @param dir The directory to load libraries from
             */
            void loadLibs(std::string dir);

            void renderGame();

            /**
             * @brief Extract the library name from its path
             * @param path The path to the library
             * @return The library name
             */
            std::string libName(const std::string& path);

            /**
             * @brief Initialize all modules (display and game)
             */
            void initAll();

            /**
             * @brief Stop all modules (display and game)
             */
            void stopAll();

            /**
             * @brief Apply the current menu selection to switch game/display or start the game
             */
            void applyMenuSelection();

            /**
             * @brief Formats a vector of libraries, keeping only the library name
             * @return vector of formatted libraries name
             */
            std::vector<std::string> formatLib(const std::vector<std::string> unformatted);

            /**
             * @brief Save the player's score for a game
             */
            void saveScore();

            std::unique_ptr<DLLoader<IDisplayModule>> _displayLoader;
            std::unique_ptr<DLLoader<IGameModule>> _gameLoader;
            std::unique_ptr<DLLoader<IGameModule>> _menuLoader;

            std::unique_ptr<IDisplayModule> _display;
            std::unique_ptr<IGameModule> _game;
            std::unique_ptr<IGameModule> _menu;

            std::vector<std::string> _displayLibs;
            std::vector<std::string> _gameLibs;

            std::size_t _currentDisplayIdx;
            std::size_t _currentGameIdx;
            std::size_t _currentModule;

            std::size_t _selectedGameIdx;
            std::size_t _selectedDisplayIdx;

            GameState _previousState;

            std::string _playerName;

            bool _running;
            bool _inCoreMenu;
    };
}

#endif
