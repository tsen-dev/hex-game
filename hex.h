#ifndef HEX_H
#define HEX_H

#include "hexboard.h"
#include "aiplayer.h"

// Prompt the user to enter a move. Display input error and wait for valid input if move is invalid. Return entered move
std::pair<int, int> getMove(HexBoard& hexBoard);
/* Prompt player 2 to select whether to swap with player 1 and display prompt until a valid response is entered.
Return true if player 2 selected to swap, false otherwise */
bool offerSwap(HexBoard& hexBoard);
// Prompt player 1 for a valid move and play that move on the board. Return the played move
std::pair<int, int> playP1Turn(std::string& p1Name, char p1Marker, HexBoard& hexBoard);
/* Prompt player 2 for a valid move and play that move on the board. Offer player 2 a swap before prompting for a move 
if it's their first move. If player 2 swaps, swap the players on the board and return SWAP, otherwise play the move and return it */
std::pair<int, int> playHumanP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove);
/* Prompt the AI player 2 for a move and play that move on the board. The AI player 2 can swap if this is its first move. 
If AI player swaps, return SWAP, otherwise play the move and return it */
std::pair<int, int> playAIP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove, AIPlayer& aiPlayer);
/* Prompts the user to enter the game's settings (board size, player names, etc), then starts a hex game loop. At each 
iteration, the other player is prompted for a move, then the board is checked for a win. The loop exits when a player wins. 
On their first move, player 2 can swap with player 1 instead of playing a move */
void startGame();

#endif