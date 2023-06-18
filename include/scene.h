#ifndef scene_h
#define scene_h

#include <vector>

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "../os/include/semaphore.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "thread_container.h"
#include "entity.h"

__USING_API

class Scene final : public ThreadContainer {
   public:
    Scene();
    ~Scene();
    void render(sf::RenderWindow *window);
    void handle_player_control_event(StateMachine::Event event);
    void start_game();
    inline void lock_scene() { this->_scene_sem->p(); }
    inline void unlock_scene() { this->_scene_sem->v(); }
    inline Player *get_player() { return this->_player; }

   private:
    static void update_scene(Scene *scene);
    void create_player();
    void create_enemy();
    void create_bullet();
    void solve_collisions(Entity *entity);
    bool solve_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y, int rotation);

   private:
    int _width;
    int _height;
    sf::Texture *_player_texture;
    sf::Texture *_enemy_texture;
    sf::Texture *_bullet_texture;
    Player *_player;
    std::vector<Enemy *> *_enemies;
    std::vector<Bullet *> *_bullets;
    Semaphore *_scene_sem;
};

#endif