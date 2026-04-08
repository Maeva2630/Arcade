/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** CoreLibraries.cpp
*/

#include "core/Core.hpp"
#include <filesystem>

namespace arcade {

std::string Core::libName(const std::string& path)
{
    std::size_t slash = path.rfind('/');
    std::string file;

    if (slash == std::string::npos) {
        file = path;
    } else {
        file = path.substr(slash + 1);
    }

    if (file.substr(0, 7) == "arcade_")
        file = file.substr(7);

    if (file.find(".") != std::string::npos)
        file = file.substr(0, file.find("."));

    return file;
}

void Core::loadLibs(std::string dir)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string lib_path = entry.path();

        try {
            DLLoader<IModule> loader(lib_path);
            std::unique_ptr<IModule> module(loader.getInstance("entryPoint"));

            switch (module->getType()) {
                case ModuleType::DISPLAY:
                    _displayLibs.push_back(lib_path);
                    break;
                case ModuleType::GAME:
                    _gameLibs.push_back(lib_path);
                    break;
                default:
                    throw CoreException(lib_path + " invalid module type.");
            }

        } catch (const std::exception& e) {
            std::cerr << "Skipping " << lib_path << ": " << e.what() << std::endl;
        }
    }
}

void Core::switchDisplay(std::size_t idx)
{
    if (idx >= _displayLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_display) {
        _display->stop();
        _display.reset();
        _displayLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IDisplayModule>>(_displayLibs[idx]);
    auto newDisplay = std::unique_ptr<IDisplayModule>(newLoader->getInstance("entryPoint"));
    
    _displayLoader = std::move(newLoader);
    _display = std::move(newDisplay);

    _display->init();
    if (_game)
        _display->setRenderContext(_game->getRenderContext());

    _currentDisplayIdx = idx;
}

void Core::switchGame(std::size_t idx)
{   
    if (idx >= _gameLibs.size()) {
        throw CoreException("Index out of range.");
    }

    if (_game) {
        _game->stop();
        _game.reset();
        _gameLoader.reset();
    }

    auto newLoader = std::make_unique<DLLoader<IGameModule>>(_gameLibs[idx]);
    auto newGame = std::unique_ptr<IGameModule>(newLoader->getInstance("entryPoint"));

    _gameLoader = std::move(newLoader);
    _game = std::move(newGame);

    _game->init();
    if (_display)
        _display->setRenderContext(_game->getRenderContext());

    _currentGameIdx = idx;
}

void Core::renderSelection()
{
    _menuY += SPACING;
    _display->drawText(Text("=== SELECT YOUR GAME AND GRAPHICS ===", _menuX, _menuY += SPACING, Colors::YELLOW, FONTSIZE + 5));

    _display->drawText(Text("Game  [F3/F4]:", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    for (std::size_t i = 0; i < _gameLibs.size(); i++) {
        Color c = (i == _selectedGameIdx) ? Colors::GREEN : Colors::WHITE;
        std::string marker = (i == _selectedGameIdx) ? "> " : "  ";
        _display->drawText(Text(marker + libName(_gameLibs[i]), _menuX + 2, _menuY += SPACING, c, FONTSIZE));
    }

    _display->drawText(Text("Graphics [F1/F2]:", _menuX, _menuY += SPACING, Colors::CYAN, FONTSIZE + 2));
    for (std::size_t i = 0; i < _displayLibs.size(); i++) {
        bool isSelected = (i == _selectedDisplayIdx);
        bool isActive = (i == _currentDisplayIdx);

        Color c = isSelected ? Colors::GREEN : Colors::WHITE;

        std::string marker = isSelected ? "> " : "  ";
        std::string active = isActive ? " [RUNNING]" : "";

        _display->drawText(Text(
            marker + libName(_displayLibs[i]) + active,
            2,
            _menuY += SPACING,
            c,
            FONTSIZE
        ));
    }
}

}
