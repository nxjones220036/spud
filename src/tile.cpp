#include "tile.h"

Tile::Tile(int x, int y, TileType type)
    : x(x), y(y), type(type) {}

void Tile::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { x, y, SIZE, SIZE };
    switch (type) {
        case TileType::Grass:
            SDL_SetRenderDrawColor(renderer, 34, 177, 76, 255); // Green
            break;
        case TileType::Water:
            SDL_SetRenderDrawColor(renderer, 0, 162, 232, 255); // Blue
            break;
        case TileType::Wall:
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); // Gray
            break;
    }
    SDL_RenderFillRect(renderer, &rect);
}

TileType Tile::getType() const {
    return type;
}
