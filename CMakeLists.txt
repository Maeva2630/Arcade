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

# =========================
# DEPENDENCIES
# =========================

find_package(X11 REQUIRED)

# ---- ncurses ----

find_package(Curses REQUIRED)

# include(ExternalProject)
# set(NCURSES_INSTALL_DIR ${CMAKE_BINARY_DIR}/ncurses)

# ExternalProject_Add(ncurses_external
#     URL https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.4.tar.gz
#     CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${NCURSES_INSTALL_DIR}
#     BUILD_COMMAND make -j
#     INSTALL_COMMAND make install
# )

# add_library(ncurses SHARED IMPORTED GLOBAL)
# add_dependencies(ncurses ncurses_external)

# set_target_properties(ncurses PROPERTIES
#     IMPORTED_LOCATION ${NCURSES_INSTALL_DIR}/lib/libncursesw.so
# )

# ---- SFML ----
find_package(SFML 2.5 QUIET COMPONENTS system window graphics)

if (NOT SFML_FOUND)
    message(STATUS "Fetching SFML")
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
    )
    FetchContent_MakeAvailable(SFML)
endif()

# ---- SDL2 ----
find_package(SDL2 QUIET)

if (NOT SDL2_FOUND)
    message(STATUS "Fetching SDL2")
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.2
    )
    FetchContent_MakeAvailable(SDL2)
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

# ---- NCURSES ----
file(GLOB NCURSES_SRC src/graphicals/ncurses/*.cpp)

add_library(arcade_ncurses SHARED ${NCURSES_SRC})
# add_dependencies(arcade_ncurses ncurses_external)

# target_include_directories(arcade_ncurses PRIVATE ${NCURSES_INSTALL_DIR}/include/ncursesw)
# target_link_libraries(arcade_ncurses ncurses dl)

target_link_libraries(arcade_ncurses ${CURSES_LIBRARIES})
target_include_directories(arcade_ncurses PRIVATE ${CURSES_INCLUDE_DIR})

# # ---- SFML ----
# file(GLOB SFML_SRC src/graphicals/sfml/*.cpp)
# add_library(arcade_sfml SHARED ${SFML_SRC})
# target_link_libraries(arcade_sfml sfml-graphics sfml-window sfml-system dl)

# # ---- SDL2 ----
# file(GLOB SDL2_SRC src/graphicals/sdl2/*.cpp)
# add_library(arcade_sdl2 SHARED ${SDL2_SRC})
# target_link_libraries(arcade_sdl2 SDL2 dl)

# =========================
# GAMES LIBS
# =========================

# # ---- Snake ----
# file(GLOB SNAKE_SRC src/games/snake/*.cpp)
# add_library(arcade_snake SHARED ${SNAKE_SRC})

# # ---- Nibbler ----
# file(GLOB NIBBLER_SRC src/games/nibbler/*.cpp)
# add_library(arcade_nibbler SHARED ${NIBBLER_SRC})
