#include <iostream>

#include "test.h"
#include "hexboard.h"

#ifndef TEST

int main()
{
    HexGame hexGame{11, 11};

    hexGame.StartGame();

    return 0;
}

#endif