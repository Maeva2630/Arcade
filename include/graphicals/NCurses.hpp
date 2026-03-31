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
         * init colorMap
         */
        void init() override;
        /**
         * @brief 
         * use of endwin function to close current window and isOpen = false
         */
        void stop() override;
        /**
         * @brief 
         * use nCurses clear function
         */
        void clear() override;
        /**
         * @brief 
         * use the refresh function to show any changements on the screen
         */
        void display() override;

        /**
         * @brief 
         * draw an entity sent from the GameModule
         * @param e 
         * Entity containing a position, a color, a caracter or a spritePath to be displayed
         */
        void drawEntity(const Entity &e) override;
        /**
         * @brief 
         * draw text from the GameModule
         * @param t 
         */
        void drawText(const Text &t) override;

        /**
         * @brief 
         * Draw rectangles to delimit zones
         * @param rect 
         * @param color 
         */
        void drawRectangle(const Rect &rect, const Color &color) override;

        /**
         * @brief Get the Name object
         * 
         * @return const std::string& 
         */
        const std::string &getName() const override;

        /**
         * @brief Get the input key
         * 
         * @return Key 
         */
        Key getKey() override;

        /**
         * @brief init the keyMap from the enum Key
         * 
         * @return std::map<int, Key> 
         */
        std::map<int, Key> initKeyFromEnum();
    
        /**
         * @brief Get the Mouse Event object
         * 
         * @return MouseEvent 
         */
        MouseEvent getMouseEvent() override;

        /**
         * @brief Set the Title object
         * 
         * @param title 
         */
        void setTitle(const std::string &title) override;

        /**
         * @brief Set the Window Size object
         * 
         * @param w (new width for the window)
         * @param h (new height for the window)
         */
        void setWindowSize(int w, int h) override;

        /**
         * @brief Set the Fullscreen object
         * 
         * @param fullscreen 
         */
        void setFullscreen(bool fullscreen) override;

        /**
         * @brief Check if the window is open or not
         * 
         * @return true 
         * @return false 
         */
        bool isOpen() const override;

        /**
         * @brief Get the Delta Time object
         * 
         * @return float 
         */
        float getDeltaTime() const override;

        /**
         * @brief Get the Width of the grid object
         * 
         * @return int 
         */
        int getWidth() const override;

        /**
         * @brief Get the Height of the grid object
         * 
         * @return int 
         */
        int getHeight() const override;
        void setRenderContext(const RenderContext &context) override;

    private:
        struct ColorComparator {
            bool operator()(const Color& lhs, const Color& rhs) const
            {
                if (lhs.r != rhs.r)
                    return lhs.r < rhs.r;
                if (lhs.g != rhs.g)
                    return lhs.g < rhs.g;
                if (lhs.b != rhs.b)
                    return lhs.b < rhs.b;
                return lhs.a < rhs.a;
            }
        };

        struct ColorPairComparator {
            bool operator()(const std::pair<Color, Color>& lhs, const std::pair<Color, Color>& rhs) const
            {
                ColorComparator comparator;

                if (comparator(lhs.first, rhs.first))
                    return true;
                if (comparator(rhs.first, lhs.first))
                    return false;
                return comparator(lhs.second, rhs.second);
            }
        };

        bool _fullscreen = false;

        WINDOW *_window;
        size_t _cols;
        size_t _rows;

        //foreground and background colors map 
        std::map<std::pair<Color, Color>, size_t, ColorPairComparator> _colorPairs;
        std::map<Color, size_t, ColorComparator> _colorMap;
        size_t _nbPairs;

        // Color getColor(size_t code) const;
        size_t checkColorPairs(Color background, Color foreground = Colors::WHITE);
        std::map<Color, size_t, ColorComparator> initColorMap();
    };
}

#endif
