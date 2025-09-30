#include <iostream>
// Minimal SDL2 window example
#include <SDL2/SDL.h>
#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    if (!game.init("SDL2 Window", 800, 600)) {
        return 1;
    }
    game.run();
    return 0;
}

