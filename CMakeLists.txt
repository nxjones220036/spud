cmake_minimum_required(VERSION 3.10)
project(spud)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(spud src/main.cpp src/game.cpp src/globals.cpp)
target_link_libraries(spud ${SDL2_LIBRARIES})
