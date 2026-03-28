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

namespace arcade
{
    class NCurses : public ADisplayModule
    {
    public:
        /**
         * @brief Construct a new NCurses object
         */
        NCurses();
        ~NCurses();

        /**
         * @brief Init all resources
         * 
         * initscr() to init the window
         * init keyMap to all NCurses bindings
         * put noecho() on window to disable writing 
         * (disable on menu when the user will put his username)
         */
        void init() override;
        void stop() override;
        void clear() override;
        void display() override;

        void drawEntity(const Entity &e) override;
        void drawText(const Text &t) override;
        void drawRectangle(const Rect &rect, const Color &color) override;

        const std::string &getName() const override;

        Key getKey() override;

        std::map<int, Key> initKeyFromEnum();
    
        MouseEvent getMouseEvent() override;

        void setTitle(const std::string &title) override;
        void setWindowSize(int w, int h) override;
        void setFullscreen(bool fullscreen) override;

        bool isOpen() const override;

        float getDeltaTime() const override;
        int getWidth() const override;
        int getHeight() const override;
        void setRenderContext(const RenderContext &context) override;

    private:
        bool _fullscreen = false;

        WINDOW *_window;
        int _cols;
        int _rows;

        // std::map<size_t, Color> _colorPairs;

        // Color getColor(size_t code);
    };
}

#endif
