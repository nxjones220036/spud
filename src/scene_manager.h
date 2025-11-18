#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class Scene;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Replace current scene with a new one (takes ownership)
    void setScene(Scene* scene);
    // Push a scene on top (e.g., pause menu)
    void pushScene(Scene* scene);
    // Pop the top scene
    void popScene();

    // Forwarding helpers
    void initCurrent();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer) const;
    void handleEvent(const SDL_Event& e);

    // Get pointer to current top scene (non-owning)
    Scene* getCurrent();

    void clear();
private:
    // We own the scenes
    std::vector<std::unique_ptr<Scene>> scenes;
};
