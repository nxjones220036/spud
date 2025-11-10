#pragma once
#include <SDL2/SDL.h>
#include "scene_manager.h"

// forward
class Scene;

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
    // Scene manager handles scenes (including player/tilemap now)
    SceneManager sceneManager;
};
