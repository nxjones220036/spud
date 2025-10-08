#pragma once
#include <vector>
#include <string>
#include "tile.h"

class TileMap {
public:
    TileMap();
    bool loadFromFile(const std::string& filename);
    void render(SDL_Renderer* renderer) const;
    int getWidth() const;
    int getHeight() const;
private:
    std::vector<Tile> tiles;
    int width;
    int height;
};
