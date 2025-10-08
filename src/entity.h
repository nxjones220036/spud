#pragma once
#include <SDL2/SDL.h>
#include <string>

class Entity {
public:
    Entity(float x, float y, int w, int h);
    virtual ~Entity();

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) const;

    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;

    void setPosition(float x, float y);
protected:
    float x, y;
    int width, height;
};
