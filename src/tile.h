#pragma once
#include <SDL2/SDL.h>
#include <string>

enum class TileType {
    Grass,
    Water,
    Wall
};

class Tile {
public:
    Tile(int x, int y, TileType type);
    void render(SDL_Renderer* renderer) const;
    TileType getType() const;
private:
    int x, y;
    TileType type;
    static constexpr int SIZE = 32;
};
