#ifndef scene_h
#define scene_h

#include <vector>

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "matrix.h"
#include "player.h"
#include "thread_container.h"

__USING_API

class Scene final : public ThreadContainer {
   public:
    Scene(sf::Color entities_color, int entities_scale, int width, int height);
    ~Scene();
    void init();
    void render(sf::RenderWindow *window);
    void handle_player_control_event(StateMachine::Event event);
    inline Player *get_player() { return this->_player; }
    inline std::vector<Enemy *> *get_enemies() { return &this->_enemies; }
    inline std::vector<Bullet *> *get_bullets() { return &this->_bullets; }

   private:
    static void update_scene(Scene *scene);
    void create_player();
    void create_enemy();
    void create_bullet();

   private:
    // TODO: Decidir se as texturas ficam aqui mesmo. Atualmente estão aqui por motivos de serialização
    // TODO: Alocar as texturas no heap ou na stack?
    sf::Texture _player_texture;
    sf::Texture _enemy_texture;
    sf::Texture _bullet_texture;
    Player *_player;
    std::vector<Enemy *> _enemies;
    std::vector<Bullet *> _bullets;
    sf::Color _entities_color;
    int _entities_scale;
    Matrix<Entity::Type> *_space_matrix;
};

#endif