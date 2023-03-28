#include "system.h"

__BEGIN_API

void System::init()
{
    setvbuf(stdout, 0, _IONBF, 0);
}

__END_API