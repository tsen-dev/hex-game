#ifndef HEX_H
#define HEX_H

#include "hexboard.h"

bool offerSwap(HexBoard& hexBoard);
void getMove(std::pair<int, int>& move, HexBoard& hexBoard);
void playSinglePlayer();
void playSingleMultiplayer();
void startGame();

#endif