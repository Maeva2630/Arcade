/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** Core.cpp
*/

#include "core/Core.hpp"
#include <iostream>
#include <filesystem>
#include <dlfcn.h>

namespace arcade {

typedef ModuleType (*fptr)();

Core::Core(std::string libPath)
{
    std::cout << "Core constructor, lib: " << libPath << "\n";
    for (const auto& entry : std::filesystem::directory_iterator(LIB_DIR)) {
        std::string lib_path = entry.path().c_str();
        std::cout << "lib:" << lib_path << std::endl;
        _loaders.emplace_back(lib_path);

        try {
            IModule *module = this->_loaders.back().getInstance("entryPoint");
            std::cout << "Loaded module: " << module->getName() << " from " << lib_path.c_str() << std::endl;
            switch (module->getType()) {
                case ModuleType::DISPLAY:
                    std::cout << "type of " << lib_path << ": DISPLAY\n";
                    _displayLibs.push_back(lib_path.c_str());
                    break;
                case ModuleType::GAME:
                    std::cout << "type of " << lib_path << ": GAME\n";
                    _gameLibs.push_back(lib_path.c_str());
                    break;
                default:
                    std::cerr << "Unknown type: " << lib_path << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error loading " << lib_path << ": " << e.what() << std::endl;
        }
    }
}

void Core::run()
{

}

void Core::mainLoop()
{

}

void Core::handleGlobalInput(Key key)
{

}

void Core::switchDisplay(std::size_t idx)
{
   
}

void Core::switchGame(std::size_t idx)
{
   
}

void Core::loadLibs(std::string dir)
{
   
}

void Core::saveScore(std::string name, int score)
{
   
}

void Core::renderMenu()
{
   
}

void Core::renderGame()
{
   
}

}
