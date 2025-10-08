#pragma once
#include "entity.h"

class Enemy : public Entity {
public:
    Enemy(float x, float y, int w, int h, int hp, int atk, int def);
    ~Enemy() override = default;

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;

    int getHP() const;
    int getATK() const;
    int getDEF() const;
    void setHP(int hp);
    void setATK(int atk);
    void setDEF(int def);

private:
    int hp;
    int atk;
    int def;
    float speed;
};
