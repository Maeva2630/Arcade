/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Main.cpp
*/

#include <iostream>
#include "core/Core.hpp"

int main(int argc, char *const *argv) {
    if (argc != 2)
        return 84;

    std::cout << "Hello, Arcade!" << std::endl;
    arcade::Core core(argv[1]);
    return 0;
}
