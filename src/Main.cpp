/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Main.cpp
*/

#include <iostream>
#include "core/Core.hpp"

int main(int argc, char *const *argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <graphics_library>" << std::endl;
        return 84;
    }

    try {
        arcade::Core core(argv[1]);
        core.run();
    } catch (const arcade::Core::CoreException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
