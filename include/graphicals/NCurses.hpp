/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NCurses.hpp
*/

#ifndef NCURSES_CPP
    #define NCURSES_CPP
    #include <ncurses.h>
    #include "ADisplayModule.hpp"

namespace arcade {
    class NCurses : public ADisplayModule {
        public:
            NCurses();
            ~NCurses();

            void init();
            void stop();
            void clear();
            void display();

            // void drawEntity(Entity& e);
            // void drawText(Text& t);
            // void drawRectangle(int r, int c);

            Key getKey();
            MouseEvent getMouseEvent();
            
            void setTitle(std::string& title);
            void setWindowSize(int w, int h);
            void setFullscreeen();

        private:
            Key keyToEnum(int ch);
            int colorPair(Color c);

            WINDOW* _window;
            int _cols;
            int _rows;
    };
}

#endif
