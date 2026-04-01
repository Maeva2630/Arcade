/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** NCurses.hpp
*/

#ifndef NCURSES_HPP
#define NCURSES_HPP
#include <ncurses.h>
#include <chrono>
#include <unordered_map>
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
        /**
         * @brief Set the Render Context object
         * 
         * @param context 
         */
        void setRenderContext(const RenderContext &context) override;

    private:
        bool _fullscreen = false;

        WINDOW *_window;

        /**
         * @brief 
         * List color pairs for NCurses
         */
        std::unordered_map<uint64_t, size_t> _colorPairs;
        std::unordered_map<uint32_t, size_t> _colorMap;
        size_t _nbPairs;

        /**
         * @brief 
         * 
         * Get the demanded color pair or create it if it doesn't exist
         * @param background 
         * @param foreground 
         * @return size_t 
         */
        size_t checkColorPairs(Color background, Color foreground = Colors::WHITE);

        /**
         * @brief 
         * Init the colorMap from the NCurses enum
         * @return std::unordered_map<uint32_t, size_t> 
         */
        std::unordered_map<uint32_t, size_t> initColorMap();
        /**
         * @brief 
         * convert a Color to a uint32_t
         * @param color 
         * @return uint32_t 
         */
        static uint32_t colorToKey(const Color &color);
        /**
         * @brief 
         * convert a Color Pair to uint64_t
         * @param foreground 
         * @param background 
         * @return uint64_t 
         */
        static uint64_t colorPairToKey(const Color &foreground, const Color &background);

        /**
         * @brief 
         * Handle delta time for graphical library
         * 
         */
        std::chrono::steady_clock::time_point _lastTime;

        /**
         * @brief update delta time for graphical display 
         * 
         * @return float 
         */
        float updateDeltaTime();
    };
}

#endif
