#include "battle_scene.h"
#include "globals.h"
#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

BattleScene::BattleScene(Player* player)
: hue(0.0f), maxHP(100), curHP(100), atk(12), def(5),
  playerRef(player), font(nullptr), defending(false), defendTurns(0)
{
}

BattleScene::~BattleScene() { destroy(); }

void BattleScene::init() {
    SDL_Log("BattleScene: init called");
    // HUD defaults
    if (maxHP <= 0) maxHP = 100;
    if (curHP < 0) curHP = maxHP;

    // Font: open font here. Game manages TTF lifecycle.
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

    // Initialize action menu (uses globals::renderer and opened font)
    menu.init(renderer, font); // renderer is global from globals.h
    menu.clear();
    menu.addItem("Attack");
    menu.addItem("Magic");
    menu.addItem("Defend");
    menu.addItem("Run");
    menu.setPosition(12, WINDOW_HEIGHT - 120);
    menu.setSpacing(8);
}

void BattleScene::update(float deltaTime) {
    hue += deltaTime * 60.0f;
    if (hue >= 360.0f) hue -= 360.0f;

    // defend turns decrement
    if (defending && defendTurns > 0) {
        defendTurns--;
        if (defendTurns == 0) {
            defending = false;
            SDL_Log("BattleScene: defend ended");
        }
    }
}

static void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    if (!font || text.empty()) return;
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
    // Render translucent overlay so underlying scene remains visible
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_Rect overlay = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    // Battle area
    SDL_Rect battleRect;
    battleRect.w = 400; battleRect.h = 240;
    battleRect.x = (WINDOW_WIDTH / 2) - battleRect.w / 2;
    battleRect.y = (WINDOW_HEIGHT / 2) - battleRect.h / 2;
    SDL_SetRenderDrawColor(renderer, 120, 40, 140, 220);
    SDL_RenderFillRect(renderer, &battleRect);

    // HUD box
    int hudX = 12;
    int hudY = 12;
    int hudW = 220;
    int hudH = 90;
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

    // Render name and stats
    if (font) {
        // name
        SDL_Color nameClr = { 220, 220, 220, 255 };
        SDL_Surface* sName = TTF_RenderText_Blended(font, name.c_str(), nameClr);
        if (sName) {
            SDL_Texture* tName = SDL_CreateTextureFromSurface(renderer, sName);
            SDL_Rect dst = { hudX + 8, hudY + 6, sName->w, sName->h };
            SDL_FreeSurface(sName);
            if (tName) {
                SDL_RenderCopy(renderer, tName, NULL, &dst);
                SDL_DestroyTexture(tName);
            }
        }

        // HP, MP, Lv
        char buf[64];
        snprintf(buf, sizeof(buf), "HP: %d", hp);
        renderText(renderer, font, buf, hudX + 8, hudY + 30);
        snprintf(buf, sizeof(buf), "MP: %d", mp);
        renderText(renderer, font, buf, hudX + 8, hudY + 50);
        snprintf(buf, sizeof(buf), "Lv: %d", level);
        renderText(renderer, font, buf, hudX + hudW - 48, hudY + 50);
    }

    // Render menu on top
    // menu.render(renderer);

    // Optionally show defend indicator
    if (defending && font) {
        renderText(renderer, font, "Defending", battleRect.x + 12, battleRect.y + 12);
    }
}

void BattleScene::handleEvent(const SDL_Event& e) {
    // First give the menu a chance to handle keys
    menu.handleInput(e);

    // If menu activated, consume and act
    int activatedIndex = -1;
    if (menu.consumeActivate(activatedIndex)) {
        SDL_Log("BattleScene: menu action %d selected", activatedIndex);
        switch (activatedIndex) {
            case 0: // Attack
                SDL_Log("BattleScene: Attack selected");
                // For demo, reduce a dummy enemy or just log. Reduce player's HP slightly to visualize.
                if (playerRef) {
                    // In a real game you'd apply to enemy; here just log.
                    SDL_Log("BattleScene: (demo) player attacks (no enemy implemented)");
                }
                break;
            case 1: // Magic
                SDL_Log("BattleScene: Magic selected");
                if (playerRef) {
                    int newMP = playerRef->getMP() - 5;
                    if (newMP < 0) newMP = 0;
                    playerRef->setMP(newMP);
                    SDL_Log("BattleScene: player MP now %d", playerRef->getMP());
                } else {
                    SDL_Log("BattleScene: no player bound to scene");
                }
                break;
            case 2: // Defend
                SDL_Log("BattleScene: Defend selected");
                defending = true;
                defendTurns = 3; // example duration
                break;
            case 3: // Run
                SDL_Log("BattleScene: Run selected, will request scene pop");
                // Simulate pressing E so Game will pop the battle scene (existing E handling pops)
                {
                    SDL_Event ev;
                    SDL_zero(ev);
                    ev.type = SDL_KEYDOWN;
                    ev.key.keysym.sym = SDLK_e;
                    SDL_PushEvent(&ev);
                }
                break;
            default:
                break;
        }
    }

    // Allow other key handling for debug: K/L to change HP (kept for convenience)
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode k = e.key.keysym.sym;
        if (k == SDLK_k) {
            if (playerRef) {
                int newHP = playerRef->getHP() - 10;
                playerRef->setHP(newHP < 0 ? 0 : newHP);
                SDL_Log("BattleScene: player HP decreased to %d", playerRef->getHP());
            } else {
                curHP -= 10; if (curHP < 0) curHP = 0;
                SDL_Log("BattleScene: HP decreased to %d", curHP);
            }
        } else if (k == SDLK_l) {
            if (playerRef) {
                int newHP = playerRef->getHP() + 10;
                if (newHP > maxHP) newHP = maxHP;
                playerRef->setHP(newHP);
                SDL_Log("BattleScene: player HP increased to %d", playerRef->getHP());
            } else {
                curHP += 10; if (curHP > maxHP) curHP = maxHP;
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
    menu.clear();
    // TTF_Quit is managed by Game
}
