#include "../include/scene.h"

__USING_API

Scene::Scene(sf::Color entities_color, int entities_scale, int width, int height) {
    this->_entities_color = entities_color;
    this->_entities_scale = entities_scale;
    this->_space_matrix = new Matrix<Entity::Type>(width, height, Entity::Type::VOID);
    this->_player = nullptr;
    this->_enemies = std::vector<Enemy *>();
    this->_bullets = std::vector<Bullet *>();

    // TODO: Checar erros
    this->_player_texture.loadFromFile("assets/sprites/player.png");
    this->_enemy_texture.loadFromFile("assets/sprites/enemy.png");
    this->_bullet_texture.loadFromFile("assets/sprites/bullet.png");
}

Scene::~Scene() {
    if (this->_space_matrix) {
        delete this->_space_matrix;
        this->_space_matrix = nullptr;
    }

    if (this->_player) {
        delete this->_player;
        this->_player = nullptr;
    }
}

void Scene::init() {
    this->thread = new Thread(update_scene, this);

    this->create_player();

    this->create_enemy();

    // for (int i = 0; i < 4; i++) {
    //     this->create_enemy();
    // }
}

void Scene::update_scene(Scene *scene) {
    while (true) {
        Game::lock_state();
        if (Game::get_state() == StateMachine::State::EXIT) {
            Game::unlock_state();
            break;
        }
        Game::unlock_state();

        Player *player = scene->get_player();

        player->lock_target_move();
        if (player->has_target_movement()) {
            
            // TODO: Obter a intenção do jogador
        }
        player->unlock_target_move();

        // TODO: Resolver as colisões

        Thread::yield();
    }

    // TODO: Limpar tudo que foi alocado

    scene->get_thread()->thread_exit(0);
}

void Scene::create_player() {
    this->_player = new Player(1, 1, &this->_player_texture, this->_entities_color, this->_entities_scale);
    this->_player->init();
}

void Scene::create_enemy() {
    this->_enemies.push_back(
        new Enemy(4, 4, 0, &this->_enemy_texture, this->_entities_color, this->_entities_scale, 0.0f));
}

void Scene::create_bullet() {
    // this->_bullets.push_back(Bullet());
}

void Scene::render(sf::RenderWindow *window) {
    this->_player->render(window);

    // TODO: Verificar o caso em que alguma entidade é destruída enquanto a renderização acontece
    for (size_t i = 0; i < this->_enemies.size(); i++) {
        this->_enemies.at(i)->render(window);
    }

    for (size_t i; i < this->_bullets.size(); i++) {
        this->_bullets.at(i)->render(window);
    }
}

void Scene::handle_player_control_event(StateMachine::Event event) { this->_player->set_control_event(event); }
