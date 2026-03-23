/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.hpp
*/

#ifndef CORE_HPP
    #define CORE_HPP
    #define LIB_DIR "../lib"
    #include <vector>
    #include <map>
    #include "games/IGameModule.hpp"
    #include "graphicals/IDisplayModule.hpp"

namespace arcade {
    class Core {
        public:
            Core(std::string libPath);
            void run();
        
        private:
            void mainLoop();
            void handleGlobalInput(Key key);

            void switchDisplay(std::size_t idx);
            void switchGame(std::size_t idx);

            void loadLibs(std::string dir);
            void saveScore(std::string name, int score);

            void renderMenu();
            void renderGame();

            IDisplayModule* _display;
            IGameModule* _game;

            std::vector<std::string> _displayLibs;
            std::vector<std::string> _gameLibs;

            std::map<std::string, int> _scores;
            std::string playerName;

            std::size_t currentDisplayIdx;
            std::size_t currentGameIdx;

            bool _running;
    };
}

#endif
