#include "scene_manager.h"
#include "scene.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
    clear();
}

void SceneManager::setScene(Scene* scene) {
    clear();
    if (scene) {
        scenes.emplace_back(scene);
    }
}

void SceneManager::pushScene(Scene* scene) {
    if (scene) scenes.emplace_back(scene);
}

void SceneManager::popScene() {
    if (!scenes.empty()) {
        scenes.pop_back();
    }
}

void SceneManager::initCurrent() {
    if (!scenes.empty()) scenes.back()->init();
}

void SceneManager::update(float deltaTime) {
    if (!scenes.empty()) scenes.back()->update(deltaTime);
}

void SceneManager::render(SDL_Renderer* renderer) const {
    if (!scenes.empty()) scenes.back()->render(renderer);
}

void SceneManager::handleEvent(const SDL_Event& e) {
    if (!scenes.empty()) scenes.back()->handleEvent(e);
}

void SceneManager::clear() {
    scenes.clear();
}
