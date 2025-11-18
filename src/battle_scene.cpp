#include "battle_scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

BattleScene::BattleScene(Player* player) : hue(0.0f), maxHP(100), curHP(100), atk(12), def(5), playerRef(player), font(nullptr) {}
BattleScene::~BattleScene() { destroy(); }

void BattleScene::init() {
    SDL_Log("BattleScene: init called");
    // HUD defaults (ensure initialized)
    if (maxHP <= 0) maxHP = 100;
    if (curHP < 0) curHP = maxHP;

    // Initialize TTF and load a font (try assets then system paths)
    if (TTF_WasInit() == 0) {
        if (TTF_Init() != 0) {
            SDL_Log("TTF_Init failed: %s", TTF_GetError());
            return;
        }
    }

    std::vector<std::string> fontCandidates = {
        "assets/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
    };

    for (const auto &p : fontCandidates) {
        std::ifstream f(p);
        if (f.good()) {
            f.close();
            font = TTF_OpenFont(p.c_str(), 14);
            if (font) {
                SDL_Log("BattleScene: loaded font %s", p.c_str());
                break;
            }
        }
    }
    if (!font) {
        SDL_Log("BattleScene: failed to load a font for HUD text");
    }
}

void BattleScene::update(float deltaTime) {
    hue += deltaTime * 60.0f;
    if (hue >= 360.0f) hue -= 360.0f;
}

static void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    if (!font) return;
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }
    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void BattleScene::render(SDL_Renderer* renderer) const {
    // Draw a simple distinctive background (purple-ish)
    SDL_SetRenderDrawColor(renderer, 90, 30, 120, 255);
    SDL_RenderClear(renderer);

    // Draw a center rectangle to show it's the battle scene
    SDL_Rect r;
    r.w = 200; r.h = 120;
    r.x = 400 - r.w/2; r.y = 300 - r.h/2; // assumes 800x600 window; fine as placeholder
    SDL_SetRenderDrawColor(renderer, 200, 80, 200, 255);
    SDL_RenderFillRect(renderer, &r);

    // HUD box (black)
    int hudX = 12;
    int hudY = 12;
    int hudW = 200;
    int hudH = 80;
    SDL_Rect hudBg = { hudX, hudY, hudW, hudH };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_RenderFillRect(renderer, &hudBg);

    // Get displayed values from playerRef if present, otherwise local
    std::string name = "Player";
    int hp = curHP;
    int mp = 0;
    int level = 1;
    if (playerRef) {
        name = playerRef->getName();
        hp = playerRef->getHP();
        mp = playerRef->getMP();
        level = playerRef->getLevel();
    }

    // Render name at top of box
    renderText(renderer, font, name, hudX + 6, hudY + 4);

    // Render HP/MP/Level lines
    char buf[64];
    snprintf(buf, sizeof(buf), "HP: %d/%d", hp, (playerRef ? playerRef->getHP() : maxHP));
    renderText(renderer, font, buf, hudX + 6, hudY + 24);
    snprintf(buf, sizeof(buf), "MP: %d", mp);
    renderText(renderer, font, buf, hudX + 6, hudY + 44);
    snprintf(buf, sizeof(buf), "Lv: %d", level);
    renderText(renderer, font, buf, hudX + 120, hudY + 44);
}

void BattleScene::handleEvent(const SDL_Event& e) {
    // allow keys to mutate HUD for testing
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode k = e.key.keysym.sym;
        if (k == SDLK_k) {
            // damage
            if (playerRef) {
                int newHP = playerRef->getHP() - 10;
                playerRef->setHP(newHP < 0 ? 0 : newHP);
                SDL_Log("BattleScene: player HP decreased to %d", playerRef->getHP());
            } else {
                curHP -= 10;
                if (curHP < 0) curHP = 0;
                SDL_Log("BattleScene: HP decreased to %d", curHP);
            }
        } else if (k == SDLK_l) {
            if (playerRef) {
                int newHP = playerRef->getHP() + 10;
                if (newHP > maxHP) newHP = maxHP;
                playerRef->setHP(newHP);
                SDL_Log("BattleScene: player HP increased to %d", playerRef->getHP());
            } else {
                curHP += 10;
                if (curHP > maxHP) curHP = maxHP;
                SDL_Log("BattleScene: HP increased to %d", curHP);
            }
        }
    }
}

void BattleScene::destroy() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    // Note: we do not call TTF_Quit here; Game will manage global TTF lifecycle if desired
}
