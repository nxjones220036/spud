#include "entity.h"

Entity::Entity(float x, float y, int w, int h)
    : x(x), y(y), width(w), height(h) {}

Entity::~Entity() {}

void Entity::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Default: white
    SDL_RenderFillRect(renderer, &rect);
}

float Entity::getX() const { return x; }
float Entity::getY() const { return y; }
int Entity::getWidth() const { return width; }
int Entity::getHeight() const { return height; }

void Entity::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}
