/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sdl2.hpp
*/

#ifndef SL2_HPP
#define SDL2_HPP
#include "ADisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <utility>

namespace arcade
{
    class Sdl2 : public ADisplayModule
    {
        public:
            /**
             * @brief Construct a new Sdl 2 object
             * 
             */
            Sdl2();
            ~Sdl2();

            /**
             * @brief Init all resources
             * 
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
            SDL_Color colorConvert(Color color);
            TTF_Font *getFontForText(const Text &t);

            std::map<int, Key> initKeyFromEnum();

            SDL_Window *_window;

            SDL_Renderer *_renderer;

            TTF_Font *_font;

            SDL_Event *_event;

            std::map<std::pair<std::string, int>, TTF_Font *> _fontMap;
    };
};

#endif
