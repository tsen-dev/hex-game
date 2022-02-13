#ifndef HEXCELL_H
#define HEXCELL_H

#include <iostream>

enum class HexCell : char {NO_CELL = '_', EMPTY = '.', PLAYER1 = 'X', PLAYER2 = 'O'};

std::ostream& operator<<(std::ostream& out, const HexCell& hexCell);

#endif