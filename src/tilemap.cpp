#include "tilemap.h"
#include <fstream>
#include <iostream>

TileMap::TileMap() : width(0), height(0) {}

bool TileMap::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open tilemap file: " << filename << std::endl;
        return false;
    }
    tiles.clear();
    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        width = line.length();
        for (int x = 0; x < width; ++x) {
            TileType type;
            switch (line[x]) {
                case 'G': type = TileType::Grass; break;
                case 'W': type = TileType::Water; break;
                case 'X': type = TileType::Wall; break;
                default:  type = TileType::Grass; break;
            }
            tiles.emplace_back(x * 32, y * 32, type);
        }
        ++y;
    }
    height = y;
    return true;
}

void TileMap::render(SDL_Renderer* renderer) const {
    for (const auto& tile : tiles) {
        tile.render(renderer);
    }
}

int TileMap::getWidth() const { return width; }
int TileMap::getHeight() const { return height; }
