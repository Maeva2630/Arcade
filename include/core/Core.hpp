/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
    #define CORE_HPP
    #include <vector>
    #include <map>
    #include <memory>
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
            
            /**
             * @brief Save the player's score
             * @param name The player's name
             * @param score The player's score
             */
            void saveScore(std::string name, int score);

            void renderMenu();
            void renderGame();

            void initAll();
            void stopAll();

            std::unique_ptr<DLLoader<IDisplayModule>> _displayLoader;
            std::unique_ptr<DLLoader<IGameModule>> _gameLoader;

            std::unique_ptr<IDisplayModule> _display;
            std::unique_ptr<IGameModule> _game;

            std::vector<std::string> _displayLibs;
            std::vector<std::string> _gameLibs;

            std::map<std::string, int> _scores;
            std::string _playerName;

            std::size_t _currentDisplayIdx;
            std::size_t _currentGameIdx;

            bool _running;
    };
}

#endif
