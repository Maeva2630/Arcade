# Arcade — Retro Gaming Platform

> **Epitech Tek2 — Object-Oriented Programming (C++)**  
> A modular retro gaming platform in **C++** allowing games and graphical libraries to be dynamically loaded and hot-swapped at runtime.

---

## Architecture & Features

The **Core** engine is completely decoupled from both games and rendering backends via shared interfaces (`IDisplay` / `IGame`) and a dynamic library loader (`dlopen`):
- **Games (`.so` plugins)**:
  - **Snake**
  - **Nibbler**
- **Graphical Libraries (`.so` plugins)**:
  - **NCurses** (terminal)
  - **SDL2** (2D hardware-accelerated)
  - **SFML** (2D hardware-accelerated)
- **Interactive Menu & High Scores**: Switch games and graphics libraries live without restarting the program, with persistent score tracking.

---

## Build & Usage

### Prerequisites
- C++20 compiler (`g++` / `clang++`)
- CMake
- `ncurses`, `SDL2`, and `SFML` development libraries

### Compile
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run
From the repository root, pass any graphical library plugin as argument:
```bash
./arcade ./lib/arcade_ncurses.so
# or ./lib/arcade_sdl2.so / ./lib/arcade_sfml.so
```

---

## Credits

- **Core, Games & Graphics Implementation**: **Maëva Arroniz** ([@Maeva2630](https://github.com/Maeva2630)) & **Julie Vincent**
- **Shared Interface Design Collaborators**: Gustave Binet, Alban Lembrez, Nielsen Combe-Bracciale
