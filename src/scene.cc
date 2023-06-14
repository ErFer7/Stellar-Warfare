#include "../include/scene.h"

__USING_API

Scene::Scene() { this->_thread = nullptr; }

Scene::~Scene() {
    if (!this->_thread) {
        delete this->_thread;
    }
}

void Scene::init() {
    this->_thread = new Thread(update_scene, this);

    // TODO: Checar erros
    this->_player_texture.loadFromFile("assets/sprites/player.png");
    this->_enemy_texture.loadFromFile("assets/sprites/enemy.png");
    this->_bullet_texture.loadFromFile("assets/sprites/bullet.png");

    this->_enemies = std::vector<Enemy*>();
    this->_bullets = std::vector<Bullet*>();

    this->create_player();

    for (int i = 0; i < 4; i++) {
        this->create_enemy();
    }
}

void Scene::stop() { this->_thread->join(); }

void Scene::update_scene(Scene *scene) {
    sf::RenderWindow *window = Game::get_window();

    while (window->isOpen()) {
        Thread::yield();
    }

    scene->get_thread()->thread_exit(0);
}

void Scene::create_player() {
    this->_player = Player(0, 0, &this->_player_texture);
}

void Scene::create_enemy() {
    // this->_enemies.push_back(Enemy());
}

void Scene::create_bullet() {
    // this->_bullets.push_back(Bullet());
}

void Scene::render(sf::RenderWindow *window) {
    this->_player.render(window);

    // TODO: Verificar o caso em que alguma entidade é destruída enquanto a renderização acontece
    for (size_t i = 0; i < this->_enemies.size(); i++) {
        this->_enemies.at(i)->render(window);
    }

    for (size_t i; i < this->_bullets.size(); i++) {
        this->_bullets.at(i)->render(window);
    }
}
