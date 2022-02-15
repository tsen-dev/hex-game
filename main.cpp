#include <iostream>

#include "test.h"
#include "hexboard.h"

#ifndef TEST

int main()
{
    HexBoard hexboard{11, 11};

    hexboard.StartGame();

    return 0;
}

#endif