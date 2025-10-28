#pragma once
#include <SDL2/SDL.h>
#include "player.h"
#include "tilemap.h"

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title, int width, int height);
    void run();
    void clean();
private:
    SDL_Window* window;
    bool running;
    Player* player;
    TileMap tilemap;
};
