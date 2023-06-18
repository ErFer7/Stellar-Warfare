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
    enum Type { VOID = 0b0000, PLAYER = 0b0001, ENEMY = 0b0010, PLAYER_BULLET = 0b0100, ENEMY_BULLET = 0b1000 };

    Entity() {
        this->_sprite = nullptr;
        this->_shape = nullptr;
    }
    Entity(int x, int y, int rotation, float speed);
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
    inline Matrix<Type> *get_shape() { return this->_shape; }
    inline bool is_destroyed() { return this->_destroyed; }
    inline void destroy() { this->_destroyed = true; }

   protected:
    void set_graphics(sf::Texture *texture);
    void set_shape(int width, int height, Type *type);

   private:
    void update_sprite();

   private:
    static unsigned int _id_counter;
    unsigned int _id;
    int _position[2];
    int _rotation;
    int _target_direction;
    int _target_rotation;
    float _speed;
    int _scale;
    sf::Color _color;
    sf::Sprite *_sprite;
    Matrix<Type> *_shape;
    bool _destroyed;
};

#endif
