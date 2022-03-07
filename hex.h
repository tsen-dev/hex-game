#ifndef HEX_H
#define HEX_H

#include "hexboard.h"

/* Prompts the user to enter the game's settings (board size, player names, etc), then starts a hex game loop. At each 
iteration, the other player is prompted for a move, then the board is checked for a win. The loop exits when a player wins. 
On their first move, player 2 can swap with player 1 instead of playing a move */
void startGame();

#endif