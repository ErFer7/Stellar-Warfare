#ifndef user_interface_h
#define user_interface_h

#include <stdexcept>

#include "../os/include/semaphore.h"
#include "../os/include/traits.h"
#include "image.h"
#include "state_machine.h"
#include "text.h"

__USING_API

class UserInterface {
   public:
    UserInterface();
    ~UserInterface();

    void render(sf::RenderWindow *window);
    void handle_event(StateMachine::Event event);
    void update_score(int score);
    void update_health(int health);
    void update_level(int speed);

   private:
    int _window_size[2];
    sf::Texture *_border_texture;
    sf::Texture *_full_heart_texture;
    sf::Texture *_empty_heart_texture;
    sf::Font *_font;
    Image *_border;
    Image *_heart1;
    Image *_heart2;
    Image *_heart3;
    Text *_title;
    Text *_state_info;
    Text *_score_label;
    Text *_score;
    Text *_health_label;
    Text *_level_label;
    Text *_level;
    Text *_info;
    Text *_authors;
    Semaphore *_user_interface_sem;
    StateMachine::State _internal_state;
};

#endif
