/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Sfml.hpp
*/

#ifndef SMFL_HPP
#define SFML_HPP
#include "ADisplayModule.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <utility>

namespace arcade
{
    class Sfml : public ADisplayModule
    {
        public:
            /**
             * @brief Construct a new Sfml object
             * 
             */
            Sfml();
            ~Sfml();

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
             * draw text using the ttf extension of Sfml
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
             * get event from Sfml and check keys from keyMap
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
             * Set title for Sfml window
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
             * @brief Get the Sprite object
             * 
             * Check if the spite exists on the _spiteMap or create it
             * @param e 
             * @return SpriteDefinition 
             */
            SpriteDefinition getSprite(const Entity &e) const;

            /**
             * @brief Init keyMap from SDL inputs
             * 
             * @return std::map<int, Key> 
             */
            std::map<int, Key> initKeyFromEnum();

            std::map<std::string, SpriteDefinition> _spriteMap;

            sf::Window *_window;
            sf::Event _event;

            void updateDeltaTime();
    };
};

#endif
