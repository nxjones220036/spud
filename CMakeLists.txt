cmake_minimum_required(VERSION 3.10)
project(spud)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2_ttf for text rendering in HUD
find_library(SDL2_TTF_LIB SDL2_ttf)

add_executable(spud src/main.cpp src/game.cpp src/globals.cpp src/entity.cpp src/player.cpp src/enemy.cpp src/tile.cpp src/tilemap.cpp src/scene.cpp src/scene_manager.cpp src/main_scene.cpp src/battle_scene.cpp)
target_link_libraries(spud ${SDL2_LIBRARIES} ${SDL2_TTF_LIB})

# Copy assets into the build directory so the executable can find them when run from build/
add_custom_target(copy_assets ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		${CMAKE_BINARY_DIR}/assets
	COMMENT "Copying assets to build directory"
)
add_dependencies(spud copy_assets)

# add menu.cpp to the spud target (use this if you don't want to edit the add_executable list)
target_sources(spud PRIVATE
    src/menu.cpp
)
