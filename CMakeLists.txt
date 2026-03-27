cmake_minimum_required(VERSION 3.20)
project(arcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -rdynamic -Wextra -fno-gnu-unique)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(include)
include(FetchContent)

# =========================
# DEPENDENCIES
# =========================

if (UNIX)
    find_package(X11 REQUIRED)
endif()

# ---- ncurses ----
find_package(Curses QUIET) # Execute the else branch if ncurses is not found

if (CURSES_FOUND)
    message(STATUS "OK - NCurses found")
else()
    message(STATUS "KO - NCurses not found: Fetching")
    
    include(ExternalProject) # To build external projects that are not CMake-based
    set(NCURSES_INSTALL_DIR ${CMAKE_BINARY_DIR}/ncurses-install)

    ExternalProject_Add(ncurses_external
        URL https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.4.tar.gz
        URL_HASH SHA256=6931283d9ac87c5073f30b6290c4c75f21632bb4fc3603ac8100812bed248159
        CONFIGURE_COMMAND <SOURCE_DIR>/configure
            --prefix=${NCURSES_INSTALL_DIR}
        BUILD_COMMAND make -j${CMAKE_BUILD_PARALLEL_LEVEL} # Launch with the max of threads (cores) available
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${NCURSES_INSTALL_DIR}/lib/libncursesw.so # Success if this file is generated
    )

    add_library(ncurses SHARED IMPORTED GLOBAL)
    add_dependencies(ncurses ncurses_external)
    set_target_properties(ncurses PROPERTIES
        IMPORTED_LOCATION ${NCURSES_INSTALL_DIR}/lib/libncursesw.so
        INTERFACE_INCLUDE_DIRECTORIES ${NCURSES_INSTALL_DIR}/include/ncursesw
    )
endif()

# ---- SFML ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB_IMAGE QUIET xcb-image)
if (NOT XCB_IMAGE_FOUND)
    message(FATAL_ERROR 
        "xcb-image manquant. Installe avec:\n"
        "  sudo apt install libxcb-image0-dev libxcb-randr0-dev "
        "libxcb-render-util0-dev libx11-xcb-dev"
    )
endif()

find_package(SFML 2.5 QUIET COMPONENTS system window graphics)

if (SFML_FOUND)
    message(STATUS "OK - Found SFML")
else()
    message(STATUS "KO - SFML not found: Fetching")

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW TRUE # Only the latest commit, not the whole history
    )

    set(SFML_BUILD_AUDIO ON CACHE BOOL "Build SFML audio module" FORCE)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

    FetchContent_MakeAvailable(SFML)
    message(STATUS "OK - SFML fetched and built")
endif()

# ---- SDL2 ----
find_package(SDL2 QUIET)

if (SDL2_FOUND)
    message(STATUS "OK - Found SDL2")
else()
    message(STATUS "KO - SDL2 not found: Fetching")

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.2
        GIT_SHALLOW TRUE
    )

    set(SDL_SHARED ON CACHE BOOL "Build shared libraries" FORCE)

    FetchContent_MakeAvailable(SDL2)
    message(STATUS "OK - SDL2 fetched and built")
endif()

# ---- SDL2 ----
# find_package(SDL2 QUIET)
# if (SDL2_FOUND)
#     message(STATUS "Found SDL2")
# else()
#     message(STATUS "Fetching SDL2")
#     FetchContent_Declare(
#         SDL2
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#         GIT_TAG release-3.4.2
#         GIT_SHALLOW TRUE
#     )
#     set(SDL2_DISABLE_INSTALL ON)
#     FetchContent_MakeAvailable(SDL2)
# endif()

# ---- SFML ----
# find_package(SFML QUIET)
# if (SFML_FOUND)
#     message(STATUS "Found SFML")
# else()
#     message(STATUS "Fetching SFML")

#     FetchContent_Declare(
#         SFML
#         GIT_REPOSITORY https://github.com/SFML/SFML.git
#         GIT_TAG 2.5.1
#         GIT_SHALLOW TRUE
#     )
    
#     # set(SFML_DISABLE_INSTALL ON)
#     set(SFML_BUILD_AUDIO OFF)
#     set(SFML_BUILD_NETWORK OFF)
#     set(SFML_BUILD_EXAMPLES OFF)
#     set(SFML_BUILD_DOC OFF)

#     FetchContent_MakeAvailable(SFML)
# endif()

# =========================
# CORE
# =========================

file(GLOB_RECURSE CORE_SRC src/core/*.cpp)

add_executable(arcade ${CORE_SRC} src/Main.cpp)

target_link_libraries(arcade dl)

# =========================
# GRAPHICAL LIBS
# =========================

# # NCURSES
# file(GLOB NCURSES_SRC src/graphicals/ncurses/*.cpp)
# add_library(arcade_ncurses SHARED ${NCURSES_SRC})
# target_link_libraries(arcade_ncurses ncurses)

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

# # Snake
# file(GLOB SNAKE_SRC src/games/snake/*.cpp)
# add_library(arcade_snake SHARED ${SNAKE_SRC})

# # Nibbler
# file(GLOB NIBBLER_SRC src/games/nibbler/*.cpp)
# add_library(arcade_nibbler SHARED ${NIBBLER_SRC})
