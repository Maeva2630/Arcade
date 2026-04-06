# ARCADE  
### <u>A RETRO PLATFORM</u>

## Description

**Arcade** is a retro gaming platform developed in **C++**.  
It allows different games and graphics libraries to be dynamically loaded at runtime.

The program is designed as a system of **plugins** :
- **Games** are dynamic libraries (`.so`)
- **Graphical interefaces** are also dynamic libraries

The core of the program does not depend on any specific graphics library.

---

## Compilation

### CMake

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Use

In the source folder:
```
./arcade ./lib/arcade_ncurses.so
```

---

## Games

- Snake

---

## Graphical interfaces

- NCurses
- SDL2
- SFML

---

## Interface collaborator

- gustave.binet@epitech.eu
- alban.lembrez@epitech.eu
- nielsen.combe-bracciale@epitech.eu

---

## Contributors

- VINCENT Julie
- ARRONIZ Maëva

---

## Licence

Academic project - Epitech
