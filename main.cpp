#include <iostream>

#include "test.h"
#include "hexboard.h"

#ifndef TEST

int main()
{
    HexBoard hexboard{5, 22};

    hexboard.StartGame();

    return 0;
}

#endif