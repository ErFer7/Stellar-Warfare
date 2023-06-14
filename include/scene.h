#ifndef scene_h
#define scene_h

#include <vector>

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"

__USING_API

class Scene {
   public:
    Scene();
    ~Scene();
    void init();
    void stop();
    // TODO: Decidir se deve-se usar inline
    inline Thread *get_thread() { return this->_thread; }
    void render(sf::RenderWindow *window);

   private:
    static void update_scene(Scene *scene);
    void create_player();
    void create_enemy();
    void create_bullet();

   private:
    Thread *_thread;
    // TODO: Decidir se as texturas ficam aqui mesmo. Atualmente estão aqui por motivos de serialização
    sf::Texture _player_texture;
    sf::Texture _enemy_texture;
    sf::Texture _bullet_texture;
    Player _player;
    std::vector<Enemy*> _enemies;
    std::vector<Bullet*> _bullets;
};

#endif