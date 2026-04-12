/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** IDLLoader.hpp
*/

#ifndef IDLLOADER_HPP
#define IDLLOADER_HPP
    #include <string>

namespace arcade
{
    template <typename T>
    class IDLLoader
    {
        public:
            /**
             * @brief Virtual destructor
             */
            virtual ~IDLLoader() = default;

            /**
             * @brief Open and load a library
             * 
             * @param libraryPath 
             */
            virtual void openLibrary(const std::string &libraryPath) = 0;

            /**
             * @brief Close a loaded library
             */
            virtual void closeLibrary() = 0;

            /**
             * @brief Get the instance from a library 
             * 
             * Get the entryPoint from a .so library
             *
             * @param entryPoint 
             * @return T* 
             */
            virtual T *getInstance(const std::string &entryPoint) const = 0;
    };
};

#endif