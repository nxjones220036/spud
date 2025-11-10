#pragma once
#include <SDL2/SDL.h>

class Scene {
public:
    Scene();
    virtual ~Scene();

    // lifecycle hooks - override in derived scenes
    virtual void init();

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* renderer) const;
    virtual void handleEvent(const SDL_Event& e);
    virtual void destroy();
private:
    // Add scene-specific members here
};