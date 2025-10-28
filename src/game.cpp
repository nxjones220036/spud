#include "game.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

Game::Game() : window(nullptr), running(false), player(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
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
    // Try several candidate paths for the map file so it works when running from build/ or project root
    std::vector<std::string> candidates = {
        "assets/map.txt",
        "./assets/map.txt",
        "../assets/map.txt"
    };
    // Also try SDL_GetBasePath if available
    char* base = SDL_GetBasePath();
    if (base) {
        std::string basePath(base);
        SDL_free(base);
        candidates.push_back(basePath + "assets/map.txt");
        candidates.push_back(basePath + "../assets/map.txt");
    }

    bool loaded = false;
    for (const auto& p : candidates) {
        std::ifstream f(p);
        if (f.good()) {
            f.close();
            if (tilemap.loadFromFile(p)) {
                std::cout << "Loaded tilemap from: " << p << std::endl;
                loaded = true;
                break;
            }
        }
    }
    if (!loaded) {
        std::cerr << "Warning: failed to find/load assets/map.txt (tried multiple paths)" << std::endl;
    }

    // Create player at center of window
    player = new Player(width / 2.0f - 25, height / 2.0f - 25, 50, 50, 100, 10, 5);
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
        }

    // Update player
    if (player) player->update(deltaTime);

    // Render
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Render tilemap first
    tilemap.render(renderer);
    // Then render player
    if (player) player->render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }
}

void Game::clean() {
    if (player) {
        delete player;
        player = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
