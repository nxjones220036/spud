#pragma once
#include "scene.h"
#include "tilemap.h"
#include "player.h"

class MainScene : public Scene {
public:
    MainScene();
    ~MainScene() override;

    void init() override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    void handleEvent(const SDL_Event& e) override;
    void destroy() override;

private:
    TileMap tilemap;
    Player* player;
};
