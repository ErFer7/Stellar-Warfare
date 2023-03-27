#include "cpu.h"
#include <iostream>
#include "main_class.h"

__USING_API 

int main(void)
{
    System::init();
    Main *m = new Main();
    delete m;
}
