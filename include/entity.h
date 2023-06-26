#ifndef entity_h
#define entity_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "../os/include/semaphore.h"
#include "../os/include/traits.h"

__USING_API

class Entity {
   public:
    enum Type { VOID, PLAYER, ENEMY, PLAYER_BULLET, ENEMY_BULLET };

    Entity() {
        this->_sprite = nullptr;
        this->_clock = nullptr;
    }
    Entity(int x,
           int y,
           int rotation,
           float speed,
           Type type,
           int size,
           float scale,
           int scene_offset_x,
           int scene_offset_y);
    ~Entity();

    inline unsigned int get_id() { return this->_id; }
    inline unsigned int get_index() { return this->_index; }
    inline int *get_position() { return this->position; }
    inline int *get_target_move() { return this->_target_move; }
    inline int get_size() { return this->size; }
    inline int get_rotation() { return this->rotation; }
    inline bool has_target_move() { return this->_has_target_move; }
    inline void set_index(unsigned int index) { this->_index = index; }
    inline Type get_type() { return this->_type; }
    inline void set_speed(float speed) { this->_speed = speed; }
    void set_position_and_rotation(int x, int y, int rotation);
    void render(sf::RenderWindow *window);
    void reset_target_move();

   protected:
    void set_graphics(sf::Texture *texture);
    bool can_move();
    void set_target_move(int x, int y);

   protected:
    int position[2];
    int rotation;
    int size;

   private:
    void update_sprite();

   private:
    static unsigned int _id_counter;
    unsigned int _id;
    unsigned int _index;
    int _target_move[2];
    int _scene_offset[2];
    bool _has_target_move;
    float _scale;
    float _speed;
    float _time_accumulator;
    Type _type;
    sf::Color _color;
    sf::Sprite *_sprite;
    sf::Clock *_clock;
};

#endif
