cmake_minimum_required(VERSION 3.20)
project(arcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-g3 -Wall -Wextra -rdynamic -fno-gnu-unique)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)
include(FetchContent)

option(ARCADE_ENABLE_SDL2 "Enable SDL backend dependency/build" OFF)

# =========================
# DEPENDENCIES
# =========================

# find_package(X11 REQUIRED)

# ---- ncurses ----
find_package(Curses REQUIRED)

# ---- SFML ----
find_package(SFML 2.5 QUIET COMPONENTS system window graphics)

if (NOT SFML_FOUND)
    message(STATUS "SFML not found: Fetching...")
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SFML)
endif()

# ---- SDL2 ----
find_package(SDL2 QUIET)

if (NOT SDL2_FOUND)
    message(STATUS "SDL2 not found: Fetching...")
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL2)

    if (NOT TARGET SDL2::SDL2)
        add_library(SDL2::SDL2 ALIAS SDL2)
    endif()
endif()

# ---- SDL2_ttf ----
find_package(SDL2_ttf QUIET)
if (NOT SDL2_ttf_FOUND AND NOT TARGET SDL2_ttf::SDL2_ttf)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
endif()

# ---- SDL2_image ----
find_package(SDL2_image QUIET)

if (NOT SDL2_image_FOUND AND NOT TARGET SDL2_image::SDL2_image)
    message(STATUS "SDL2_image not found: Fetching...")
    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL2_image)

    if (NOT TARGET SDL2_image::SDL2_image)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    endif()
endif()

# =========================
# CORE
# =========================

file(GLOB_RECURSE CORE_SRC src/core/*.cpp)

add_executable(arcade ${CORE_SRC} src/Main.cpp)
target_link_libraries(arcade dl)

# =========================
# GRAPHICAL LIBS
# =========================

macro(arcade_library target)
    set_target_properties(${target} PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    )
endmacro()

set(DISPLAY_COMMON_SRC src/graphicals/ADisplay.cpp)

# ---- ncurses ----
file(GLOB NCURSES_SRC src/graphicals/ncurses/*.cpp)
add_library(arcade_ncurses SHARED ${NCURSES_SRC} ${DISPLAY_COMMON_SRC})
target_include_directories(arcade_ncurses PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(arcade_ncurses ${CURSES_LIBRARIES})
arcade_library(arcade_ncurses)

# # ---- SFML ----
# file(GLOB SFML_SRC src/graphicals/sfml/*.cpp)
# add_library(arcade_sfml SHARED ${SFML_SRC})
# target_link_libraries(arcade_sfml sfml-graphics sfml-window sfml-system)
# arcade_library(arcade_sfml)

# ---- SDL2 ----
file(GLOB SDL2_SRC src/graphicals/sdl2/*.cpp)
add_library(arcade_sdl2 SHARED ${SDL2_SRC} ${DISPLAY_COMMON_SRC})
if (TARGET SDL2_ttf::SDL2_ttf AND TARGET SDL2_image::SDL2_image)
    target_link_libraries(arcade_sdl2 SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)
else()
    target_include_directories(arcade_sdl2 PRIVATE ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(arcade_sdl2 SDL2::SDL2 ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
endif()
arcade_library(arcade_sdl2)

# =========================
# GAMES LIBS
# =========================

set(GAME_COMMON_SRC src/games/AGame.cpp)

# ---- Snake ----
file(GLOB SNAKE_SRC src/games/snake/*.cpp)
add_library(arcade_snake SHARED ${SNAKE_SRC} ${GAME_COMMON_SRC})
arcade_library(arcade_snake)

# ---- Nibbler ----
file(GLOB NIBBLER_SRC src/games/nibbler/*.cpp)
add_library(arcade_nibbler SHARED ${NIBBLER_SRC} ${GAME_COMMON_SRC})
arcade_library(arcade_nibbler)
