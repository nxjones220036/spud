#include "menu.h"
#include "globals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <cstdio>

Menu::Menu()
: selectedIndex(0), renderer(nullptr), font(nullptr),
  activated(false), activatedIndex(-1), posX(12), posY(WINDOW_HEIGHT - 120), spacing(6) {}

Menu::~Menu() {
    clear();
}

void Menu::init(SDL_Renderer* ren, TTF_Font* fnt) {
    renderer = ren;
    font = fnt;
    // refresh cached textures for existing items (if any)
    for (auto &it : items) {
        if (it.texture) {
            SDL_DestroyTexture(it.texture);
            it.texture = nullptr;
        }
        if (font && renderer) {
            SDL_Color white = {255,255,255,255};
            SDL_Surface* surf = TTF_RenderText_Blended(font, it.text.c_str(), white);
            if (surf) {
                it.texture = SDL_CreateTextureFromSurface(renderer, surf);
                it.rect.w = surf->w;
                it.rect.h = surf->h;
                SDL_FreeSurface(surf);
            }
        }
    }
}

void Menu::addItem(const std::string& text) {
    MenuItem mi;
    mi.text = text;
    mi.texture = nullptr;
    if (font && renderer) {
        SDL_Color white = {255,255,255,255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), white);
        if (surf) {
            mi.texture = SDL_CreateTextureFromSurface(renderer, surf);
            mi.rect.w = surf->w;
            mi.rect.h = surf->h;
            SDL_FreeSurface(surf);
        }
    }
    items.push_back(mi);
}

void Menu::removeItem(int index) {
    if (index < 0 || index >= (int)items.size()) return;
    if (items[index].texture) {
        SDL_DestroyTexture(items[index].texture);
        items[index].texture = nullptr;
    }
    items.erase(items.begin() + index);
    if (selectedIndex >= (int)items.size()) selectedIndex = std::max(0, (int)items.size() - 1);
}

void Menu::clear() {
    for (auto &it : items) {
        if (it.texture) {
            SDL_DestroyTexture(it.texture);
            it.texture = nullptr;
        }
    }
    items.clear();
    selectedIndex = 0;
    activated = false;
    activatedIndex = -1;
}

void Menu::render(SDL_Renderer* ren) {
    if (!font || !ren) return;
    int x = posX;
    int y = posY;
    // draw each item; selected item rendered in yellow
    for (size_t i = 0; i < items.size(); ++i) {
        const MenuItem &mi = items[i];
        // compute rect position
        SDL_Rect dst = { x, y, mi.rect.w, mi.rect.h };
        // create temporary texture for selected item in yellow, otherwise use cached
        if ((int)i == selectedIndex) {
            SDL_Color yellow = {255, 215, 0, 255};
            SDL_Surface* surf = TTF_RenderText_Blended(font, mi.text.c_str(), yellow);
            if (surf) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
                SDL_FreeSurface(surf);
                if (tex) {
                    SDL_RenderCopy(ren, tex, NULL, &dst);
                    SDL_DestroyTexture(tex);
                }
            }
        } else {
            if (mi.texture) {
                SDL_RenderCopy(ren, mi.texture, NULL, &dst);
            } else {
                // fallback, create white temp texture
                SDL_Color white = {255,255,255,255};
                SDL_Surface* surf = TTF_RenderText_Blended(font, mi.text.c_str(), white);
                if (surf) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
                    SDL_FreeSurface(surf);
                    if (tex) {
                        SDL_RenderCopy(ren, tex, NULL, &dst);
                        SDL_DestroyTexture(tex);
                    }
                }
            }
        }
        y += mi.rect.h + spacing;
    }
}

void Menu::handleInput(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) return;
    SDL_Keycode k = event.key.keysym.sym;
    if (k == SDLK_UP) {
        if (items.empty()) return;
        selectedIndex = (selectedIndex - 1 + (int)items.size()) % (int)items.size();
    } else if (k == SDLK_DOWN) {
        if (items.empty()) return;
        selectedIndex = (selectedIndex + 1) % (int)items.size();
    } else if (k == SDLK_LEFT) {
        if (items.empty()) return;
        selectedIndex = (selectedIndex - 1 + (int)items.size()) % (int)items.size();
    } else if (k == SDLK_RIGHT) {
        if (items.empty()) return;
        selectedIndex = (selectedIndex + 1) % (int)items.size();
    } else if (k == SDLK_RETURN || k == SDLK_KP_ENTER) {
        activated = true;
        activatedIndex = selectedIndex;
    }
}

bool Menu::consumeActivate(int &outIndex) {
    if (!activated) return false;
    outIndex = activatedIndex;
    activated = false;
    activatedIndex = -1;
    return true;
}