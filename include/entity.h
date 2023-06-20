#ifndef entity_h
#define entity_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "../os/include/semaphore.h"
#include "../os/include/traits.h"
#include "matrix.h"

__USING_API

class Entity {
   public:
    enum Type { VOID, PLAYER, ENEMY, PLAYER_BULLET, ENEMY_BULLET };

    Entity() {
        this->_sprite = nullptr;
        this->_shape = nullptr;
        this->_clock = nullptr;
    }
    Entity(int x, int y, int rotation, float speed, Type type);
    ~Entity();
    inline unsigned int get_id() { return this->_id; }
    void render(sf::RenderWindow *window);
    void set_target_move(int direction, int rotation);
    inline int *get_position() { return this->_position; }
    inline int get_rotation() { return this->_rotation; }
    void set_position_and_rotation(int x, int y, int rotation);
    inline int get_target_direction() { return this->_target_direction; }
    inline int get_target_rotation() { return this->_target_rotation; }
    void reset_target_move();
    inline Matrix<bool> *get_shape() { return this->_shape; }
    inline Type get_type() { return this->_type; }
    inline bool has_target_move() { return this->_has_target_move; }
    inline void set_index(unsigned int index) { this->_index = index; }
    inline unsigned int get_index() { return this->_index; }
    inline void set_already_simulated(bool already_simulated) { this->_already_simulated = already_simulated; }
    inline bool already_simulated() { return this->_already_simulated; }
    bool can_move();

   protected:
    void set_graphics(sf::Texture *texture);
    void set_shape(int width, int height, bool *shape);

   private:
    void update_sprite();

   private:
    static unsigned int _id_counter;
    unsigned int _index;
    unsigned int _id;
    int _position[2];
    int _rotation;
    int _target_direction;
    int _target_rotation;
    bool _has_target_move;
    float _speed;
    float _time_accumulator;
    int _scale;
    sf::Color _color;
    sf::Sprite *_sprite;
    Matrix<bool> *_shape;
    Type _type;
    sf::Clock *_clock;
    bool _already_simulated;
};

#endif
