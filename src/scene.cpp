#include "scene.h"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::init() {
    // base init (override in derived scenes)
}

void Scene::update(float /*deltaTime*/) {
    // base update (override in derived scenes)
}

void Scene::render(SDL_Renderer* /*renderer*/) const {
    // base render (override in derived scenes)
}

void Scene::handleEvent(const SDL_Event& /*e*/) {
    // base event handler (override in derived scenes)
}

void Scene::destroy() {
    // base cleanup (override in derived scenes)
}
