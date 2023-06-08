#include "../include/thread.h"
#include "../include/system.h"
#include "../include/main_class.h"

__USING_API 

int main(void)
{
    System::init(&Main::run);
}
