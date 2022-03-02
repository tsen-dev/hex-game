#ifndef HEX_H
#define HEX_H

#include "hexboard.h"

void swapPlayers(HexBoard& hexBoard);
bool offerSwap(HexBoard& hexBoard);
void getMove(std::pair<int, int>& move, HexBoard& hexBoard);
void startGame();

#endif