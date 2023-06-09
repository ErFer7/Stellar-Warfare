#include "../include/scene.h"

__USING_API

Scene::Scene() { this->_thread = nullptr; }

Scene::~Scene() { delete this->_thread; }

void Scene::init() { this->_thread = new Thread(UpdateScene); }

void Scene::stop() { this->_thread->join(); }

void Scene::UpdateScene() { 
    while (Game::get_window()->isOpen()) {
        Thread::yield();
    }
}
