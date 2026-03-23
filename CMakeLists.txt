cmake_minimum_required(VERSION 3.20)
project(arcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -rdynamic -Wextra -fno-gnu-unique)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(include)

# =========================
# CORE
# =========================
file(GLOB_RECURSE CORE_SRC
    src/core/*.cpp
)

add_executable(arcade ${CORE_SRC} src/Main.cpp)

target_link_libraries(arcade dl)

# =========================
# GRAPHICAL LIBS
# =========================

# NCURSES
file(GLOB NCURSES_SRC src/graphicals/ncurses/*.cpp)
add_library(arcade_ncurses SHARED ${NCURSES_SRC})
target_link_libraries(arcade_ncurses ncurses)

# # SDL2
# file(GLOB SDL2_SRC src/graphicals/sdl2/*.cpp)
# add_library(arcade_sdl2 SHARED ${SDL2_SRC})
# target_link_libraries(arcade_sdl2 SDL2)

# # SFML
# file(GLOB SFML_SRC src/graphicals/sfml/*.cpp)
# add_library(arcade_sfml SHARED ${SFML_SRC})
# target_link_libraries(arcade_sfml sfml-graphics sfml-window sfml-system)

# =========================
# GAMES LIBS
# =========================

# Snake
file(GLOB SNAKE_SRC src/games/snake/*.cpp)
add_library(arcade_snake SHARED ${SNAKE_SRC})

# # Nibbler
# file(GLOB NIBBLER_SRC src/games/nibbler/*.cpp)
# add_library(arcade_nibbler SHARED ${NIBBLER_SRC})
