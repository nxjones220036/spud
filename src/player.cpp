#include "player.h"
#include <SDL2/SDL.h>

Player::Player(float x, float y, int w, int h, int hp, int atk, int def)
    : Entity(x, y, w, h), hp(hp), atk(atk), def(def), speed(200.0f) {}

void Player::update(float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    float dx = 0, dy = 0;
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])    dy -= 1;
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])  dy += 1;
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])  dx -= 1;
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) dx += 1;
    if (dx != 0 || dy != 0) {
        float len = SDL_sqrtf(dx*dx + dy*dy);
        dx /= len;
        dy /= len;
        x += dx * speed * deltaTime;
        y += dy * speed * deltaTime;
    }
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for player
    SDL_RenderFillRect(renderer, &rect);
}

int Player::getHP() const { return hp; }
int Player::getATK() const { return atk; }
int Player::getDEF() const { return def; }
void Player::setHP(int h) { hp = h; }
void Player::setATK(int a) { atk = a; }
void Player::setDEF(int d) { def = d; }
