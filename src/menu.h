#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

struct MenuItem {
    std::string text;
    SDL_Texture* texture; // cached normal (unselected) texture
    SDL_Rect rect;
    MenuItem() : texture(nullptr), rect{0,0,0,0} {}
};

class Menu {
    std::vector<MenuItem> items;
    int selectedIndex;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool activated;
    int activatedIndex;
    int posX, posY;
    int spacing;
public:
    Menu();
    ~Menu();

    // initialize with renderer + font; must be called before adding/rendering
    void init(SDL_Renderer* renderer, TTF_Font* font);

    void addItem(const std::string& text);
    void removeItem(int index);
    void clear();

    // render menu at current pos (default bottom-left); call setPosition to change
    void render(SDL_Renderer* renderer);

    // keyboard handling: changes selectedIndex and sets activation flag on Enter
    void handleInput(const SDL_Event& event);

    int getSelectedIndex() const { return selectedIndex; }

    // If an action was activated, returns true and sets outIndex, then clears the flag.
    bool consumeActivate(int &outIndex);

    void setPosition(int x, int y) { posX = x; posY = y; }
    void setSpacing(int s) { spacing = s; }
};
