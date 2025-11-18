#include "game.h"
#include "globals.h"
#include <SDL2/SDL_ttf.h>
#include "main_scene.h"
#include "battle_scene.h"
#include <iostream>

Game::Game() : window(nullptr), running(false) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // initialize SDL_ttf for text rendering
    if (TTF_WasInit() == 0) {
        if (TTF_Init() != 0) {
            std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
            // not fatal, continue without text
        }
    }
    window = SDL_CreateWindow(title, 100, 100, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    // Set initial scene (MainScene owns its own tilemap and player)
    sceneManager.setScene(new MainScene());
    sceneManager.initCurrent();
    running = true;
    return true;
}

void Game::run() {
    SDL_Event e;
    Uint32 lastTick = SDL_GetTicks();
    while (running) {
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // allow quick scene switching with B (battle) and E (main)
            if (e.type == SDL_KEYDOWN) {
                SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_b) {
                    // switch to battle scene
                    std::cout << "Game: B pressed, switching to BattleScene" << std::endl;
                    // attempt to pass current MainScene's player to BattleScene
                    Scene* cur = sceneManager.getCurrent();
                    Player* p = nullptr;
                    if (cur) {
                        MainScene* ms = dynamic_cast<MainScene*>(cur);
                        if (ms) p = ms->getPlayer();
                    }
                    sceneManager.setScene(new BattleScene(p));
                    sceneManager.initCurrent();
                    continue; // don't forward this key to scene
                } else if (k == SDLK_e) {
                    // switch back to main scene
                    std::cout << "Game: E pressed, switching to MainScene" << std::endl;
                    sceneManager.setScene(new MainScene());
                    sceneManager.initCurrent();
                    continue;
                }
            }

            // forward other events to current scene
            sceneManager.handleEvent(e);
        }

        // Update current scene
        sceneManager.update(deltaTime);

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        sceneManager.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }
}

void Game::clean() {
    // clear scenes first
    sceneManager.clear();
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (TTF_WasInit()) TTF_Quit();
    SDL_Quit();
}
