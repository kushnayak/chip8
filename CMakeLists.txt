cmake_minimum_required(VERSION 3.18.3)

project(chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")


add_executable(${PROJECT_NAME} src/main.cpp
                               src/chip8.cpp
                               src/graphics.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/sdl2-cmake-scripts")

find_package(SDL2 REQUIRED)

include_directories(${PROJECT_NAME} include/ ${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

message("SDL INCLUDE DIR: ${SDL2_INCLUDE_DIR}")
message("SDL LIBRARY: ${SDL2_LIBRARY}")
