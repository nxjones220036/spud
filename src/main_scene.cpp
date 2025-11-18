#include "main_scene.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

MainScene::MainScene() : player(nullptr) {}

MainScene::~MainScene() {
    destroy();
}

void MainScene::init() {
    // Try a few candidate locations for the map file so it works when running from build/
    std::vector<std::string> candidates = {
        "assets/map.txt",
        "./assets/map.txt",
        "../assets/map.txt"
    };
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
                std::cout << "MainScene: loaded map from " << p << std::endl;
                loaded = true;
                break;
            }
        }
    }
    if (!loaded) {
        std::cerr << "MainScene: failed to find/load assets/map.txt (tried multiple paths)" << std::endl;
    }

    // spawn player roughly center of screen based on tilemap size
    float px = (tilemap.getWidth() * 32) / 2.0f - 25;
    float py = (tilemap.getHeight() * 32) / 2.0f - 25;
    player = new Player(px, py, 50, 50, 100, 10, 5);
    // set additional HUD values
    player->setName("Hero");
    player->setMP(50);
    player->setLevel(1);
}

void MainScene::update(float deltaTime) {
    if (player) player->update(deltaTime);
}

void MainScene::render(SDL_Renderer* renderer) const {
    tilemap.render(renderer);
    if (player) player->render(renderer);
}

void MainScene::handleEvent(const SDL_Event& e) {
    // no special handling yet
}

void MainScene::destroy() {
    if (player) {
        delete player;
        player = nullptr;
    }
}

Player* MainScene::getPlayer() const {
    return player;
}
