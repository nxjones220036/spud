#include "enemy.h"
#include <SDL2/SDL.h>

Enemy::Enemy(float x, float y, int w, int h, int hp, int atk, int def)
    : Entity(x, y, w, h), hp(hp), atk(atk), def(def), speed(100.0f) {}

void Enemy::update(float deltaTime) {
    // Placeholder for enemy logic
}

void Enemy::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for enemy
    SDL_RenderFillRect(renderer, &rect);
}

int Enemy::getHP() const { return hp; }
int Enemy::getATK() const { return atk; }
int Enemy::getDEF() const { return def; }
void Enemy::setHP(int h) { hp = h; }
void Enemy::setATK(int a) { atk = a; }
void Enemy::setDEF(int d) { def = d; }
