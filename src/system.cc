#include "system.h"

__BEGIN_API

void System::init()
{
    db<System>(TRC) << "System::init() called\n";
    setvbuf(stdout, 0, _IONBF, 0);
}

__END_API