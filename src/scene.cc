#include "../include/scene.h"

__USING_API

Scene::Scene() { this->_thread = nullptr; }

Scene::~Scene() {
    if (!this->_thread) {
        delete this->_thread;
    }
}

void Scene::init() { this->_thread = new Thread(update_scene, this); }

void Scene::stop() { this->_thread->join(); }

void Scene::update_scene(Scene *scene) {
    sf::RenderWindow *window = Game::get_window();

    while (window->isOpen()) {
        Thread::yield();
    }

    scene->get_thread()->thread_exit(0);
}
