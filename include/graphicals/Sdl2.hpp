/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sdl2.hpp
*/

#ifndef SDL2_HPP
#define SDL2_HPP
#include "ADisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
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
             * Sdl init, init window, renderer, font and keyMap
             */
            void init() override;

            /**
             * @brief Stop and clear all resources
             * 
             * close font, destroy all textures/ renderers/ window and sdl quit, 
             */
            void stop() override;

            /**
             * @brief Clear screen
             * 
             * reset current renderer color and clear renderer
             */
            void clear() override;

            /**
             * @brief Display all element on screen
             * 
             * update deltaTime for display and render present
             */
            void display() override;

            /**
             * @brief Draw entity from game
             * 
             * If the entity has a sprite path it draws the sprite, otherwise it draws rectangle 
             * @param e 
             */
            void drawEntity(const Entity &e) override;

            /**
             * @brief Draw text
             * 
             * draw text using the ttf extension of sdl2
             * @param t 
             */
            void drawText(const Text &t) override;

            /**
             * @brief Draw rectangle
             * 
             * draw a rectangle using renderFillRect
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
             * @brief Get the Key from event
             * 
             * get event from sdl2 and check keys from keyMap
             * @return Key 
             */
            Key getKey() override;

            /**
             * @brief Get the Mouse Event object
             * 
             * get mouse clicks on buttons
             * @return MouseEvent 
             */
            MouseEvent getMouseEvent() override;

            /**
             * @brief Set the Title object
             * 
             * Set title for sdl2 window
             * @param title 
             */
            void setTitle(const std::string &title) override;

            /**
             * @brief Set the Window Size object
             * 
             * resize window
             * @param w 
             * @param h 
             */
            void setWindowSize(int w, int h) override;

            /**
             * @brief Set the Fullscreen object
             * 
             * setf full screen for window
             * @param fullscreen 
             */
            void setFullscreen(bool fullscreen) override;

            /**
             * @brief Check isOpen
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
             * @brief Get the window width
             * 
             * @return int 
             */
            int getWidth() const override;

            /**
             * @brief Get the window Height
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
            /**
             * @brief Sprite definition struct
             * 
             * define what is needed for a sprite (sprite, texture and rect)
             */
            struct SpriteDefinition {
                std::string texturePath;
                Rect srcRect;
                bool useSrcRect;

                SpriteDefinition() : texturePath(""), srcRect(0, 0, 0, 0), useSrcRect(false) {}
                SpriteDefinition(const std::string &path)
                    : texturePath(path), srcRect(0, 0, 0, 0), useSrcRect(false) {}
                SpriteDefinition(const std::string &path, const Rect &source)
                    : texturePath(path), srcRect(source), useSrcRect(true) {}
            };

            /**
             * @brief convert color to SDL_Color
             * 
             * @param color 
             * @return SDL_Color 
             */
            SDL_Color colorConvert(Color color);

            /**
             * @brief Get the Font For Text object
             * 
             * Check if the font exists on the _fontMap or create and use it
             * @param t 
             * @return TTF_Font* 
             */
            TTF_Font *getFontForText(const Text &t);

            /**
             * @brief Get the Texture object
             * 
             * Check if the texture exists on the _textureMap or create and use it
             * @param texturePath 
             * @return SDL_Texture* 
             */
            SDL_Texture *getTexture(const std::string &texturePath);

            /**
             * @brief Get the Sprite object
             * 
             * Check if the spite exists on the _spiteMap or create it
             * @param e 
             * @return SpriteDefinition 
             */
            SpriteDefinition getSprite(const Entity &e);

            /**
             * @brief Init keyMap from SDL inputs
             * 
             * @return std::map<int, Key> 
             */
            std::map<int, Key> initKeyFromEnum();

            SDL_Window *_window;

            SDL_Renderer *_renderer;

            TTF_Font *_font;

            SDL_Event _event;
            Uint64 _lastTicks;

            std::map<std::pair<std::string, int>, TTF_Font *> _fontMap;
            std::map<std::string, SDL_Texture *> _textureMap;
            std::map<std::string, SpriteDefinition> _spriteMap;

            void updateDeltaTime();
    };
};

#endif
