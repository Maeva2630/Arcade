/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NCurses.hpp
*/

#ifndef NCURSES_HPP
    #define NCURSES_HPP
    #include <ncurses.h>
    #include "ADisplayModule.hpp"

namespace arcade {
    class NCurses : public ADisplayModule {
        public:
            NCurses();
            ~NCurses();

            void init() override;
            void stop() override;
            void clear() override;
            void display() override;

            void drawEntity(const Entity& e) override;
            void drawText(const Text& t) override;
            void drawRectangle(const Rect& rect, const Color& color) override;

            const std::string& getName() const override;

            Key getKey() override;
            MouseEvent getMouseEvent() override;
            
            void setTitle(const std::string& title) override;
            void setWindowSize(int w, int h) override;
            void setFullscreen(bool fullscreen) override;

            bool isOpen() const override;

            float getDeltaTime() const override;
            int getWidth() const override;
            int getHeight() const override;
            void setRenderContext(const RenderContext& context) override;
    
        private:
            Key keyToEnum(int ch);
            int colorPair(Color c);
            bool _fullscreen = false;

            WINDOW* _window;
            int _cols;
            int _rows;
    };
}

#endif
