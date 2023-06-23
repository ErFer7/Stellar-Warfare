#include "../include/game.h"
#include "../os/include/system.h"

__USING_API

int main(void) {
    System::init((void (*)(void*)) & Game::run);

    return 0;
}
