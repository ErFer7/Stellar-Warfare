#ifndef scene_h
#define scene_h

#include <math.h>

#include "../os/include/semaphore.h"
#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "bullet.h"
#include "dynamic_array.h"
#include "enemy.h"
#include "entity.h"
#include "game.h"
#include "player.h"
#include "thread_container.h"

__USING_API

class Scene final : public ThreadContainer {
   public:
    Scene();
    ~Scene();

    void render(sf::RenderWindow *window);
    void handle_event(StateMachine::Event event);

   private:
    static void update_scene(Scene *scene);
    inline void lock_scene() { this->_scene_sem->p(); }
    inline void unlock_scene() { this->_scene_sem->v(); }
    inline StateMachine::State get_internal_state() { return this->_internal_state; }
    inline float level_speed() { return 2.0f * expf(0.693147f * this->_level); }  // x = 1 => 4, x = 2 => 8, x = 3 => 16
    bool check_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y);
    bool check_corner_collision(int x1, int y1, int x2, int y2, int size1, int size2);
    bool solve_boundary_collision(Entity *entity, int new_x, int new_y);
    bool solve_entity_collision(Entity *entity1, Entity *entity2);
    void create_player();
    void create_enemy(int spot = -1);
    void create_bullet(int x, int y, int rotation, Entity::Type type);
    void destroy_player();
    void destroy_bullet(unsigned int i);
    void destroy_enemy(unsigned int i);
    void update_all_entities();
    bool solve_collisions(Entity *entity);
    void update_bullets_behavior();
    void spawn_enemies();
    void start_game();
    void end_game();
    void update_enemies_speed();
    void render_background(sf::RenderWindow *window, int noise_range);

   private:
    int _width;
    int _height;
    int _score;
    int _enemy_spawn_count;
    int _enemy_kill_count;
    int _level;
    int _scene_offset[2];
    float _scale;
    bool _skip_time;
    sf::Texture *_player_texture;
    sf::Texture *_enemy_texture;
    sf::Texture *_cell_texture;
    sf::Sprite *_background_cell;
    sf::Clock *_clock;
    Player *_player;
    DynamicArray<Enemy *> *_enemies;
    DynamicArray<Bullet *> *_bullets;
    DynamicArray<float> *_enemy_spawn_times;
    Semaphore *_scene_sem;
    StateMachine::State _internal_state;
};

#endif