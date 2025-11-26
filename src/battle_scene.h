#pragma once
#include "scene.h"
#include "player.h"
#include "menu.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

class BattleScene : public Scene {
public:
    BattleScene(Player* player = nullptr);
    ~BattleScene() override;

    void init() override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    void handleEvent(const SDL_Event& e) override;
    void destroy() override;

private:
    // simple state for a placeholder scene
    float hue;

    // HUD values
    int maxHP;
    int curHP;
    int atk;
    int def;

    // reference to player (non-owning). If set, HUD shows real player stats.
    Player* playerRef;

    // font used for HUD text (owned)
    TTF_Font* font;

    // action/menu for battle ( Attack / Magic / Defend / Run )
    Menu menu;

    // defend state
    bool defending;
    int defendTurns;
};
