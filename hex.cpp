#include <iostream>
#include <ctype.h>
#include <string>

#include <chrono>

#include "settings.h"
#include "hexboard.h"
#include "aiplayer.h"
#include "move.h"

const Move SWAP = AIPlayer::SWAP;

// Prompt the user to enter a move. Display input error and wait for valid input if move is invalid. Return entered move
Move getMove(HexBoard& hexBoard)
{
    Move move{0, 0};

    std::cout << "Enter your move: \n\n";
    std::cin >> move.X >> move.Y;

    while (hexBoard.GetCell(move.X, move.Y) != HexBoard::EMPTY) // Input cell is occupied or out-of-bounds
    {
        hexBoard.MarkCell(move.X, move.Y, hexBoard.P1); // Display the input error
        std::cin >> move.X >> move.Y;
    }   

    return move;
}

/* Prompt player 2 to select whether to swap with player 1 and display prompt until a valid response is entered.
Return true if player 2 selected to swap, false otherwise */
bool offerSwap(HexBoard& hexBoard)
{
    char swapResponse = 0;

    while (swapResponse != 'Y' && swapResponse != 'N')
    {
        std::cout << "Swap with " << hexBoard.P1Name << "?: (Y/N)\n\n";
        std::cin >> swapResponse;
    }

    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin
    
    std::cout << '\n';

    return swapResponse == 'Y';
}

// Prompt player 1 for a valid move and play that move on the board. Return the played move
Move playP1Turn(std::string& p1Name, char p1Marker, HexBoard& hexBoard)
{
    std::cout << p1Name << (tolower(p1Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         

    Move move = getMove(hexBoard);

    hexBoard.MarkCell(move.X, move.Y, p1Marker);
    
    return move;
}

/* Prompt player 2 for a valid move and play that move on the board. Offer player 2 a swap before prompting for a move 
if it's their first move. If player 2 swaps, swap the players on the board and return SWAP, otherwise play the move and return it */
Move playHumanP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove)
{
    std::cout << p2Name << (tolower(p2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                               
    
    if (firstMove == true && offerSwap(hexBoard) == true) 
    {
        hexBoard.SwapPlayers();                    
        return SWAP;
    }

    else
    {
        Move move = getMove(hexBoard);                    
        hexBoard.MarkCell(move.X, move.Y, p2Marker);
        return move;
    }
}

/* Prompt the AI player 2 for a move and play that move on the board. The AI player 2 can swap if this is its first move. 
If AI player swaps, return SWAP, otherwise play the move and return it */
Move playAIP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove, AIPlayer& aiPlayer)
{    
    std::cout << p2Name << (tolower(p2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                           

    Move move = aiPlayer.GetMove(firstMove);

    if (move == AIPlayer::SWAP) 
    {
        hexBoard.SwapPlayers();
        return SWAP;
    }

    else
    {
        hexBoard.MarkCell(move.X, move.Y, p2Marker);
        return move;
    }    
}

/* Prompts the user to enter the game's settings (board size, player names, etc), then starts a hex game loop. At each 
iteration, the other player is prompted for a move, then the board is checked for a win. The loop exits when a player wins. 
On their first move, player 2 can swap with player 1 instead of playing a move */
void startGame()
{
    Settings settings{};
    HexBoard hexBoard{settings};
    AIPlayer aiPlayer{hexBoard, settings.AIDifficulty, settings.AIThreadCount};    
    Move move;
    bool p2FirstMove = true; // Whether it is player 2's first move 
    char currentPlayer = hexBoard.P1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == settings.PlayerMarkers.first)
            move = playP1Turn(settings.PlayerNames.first, settings.PlayerMarkers.first, hexBoard);

        else 
        {
            if (settings.SinglePlayer) move = playAIP2Turn(
                settings.PlayerNames.second, settings.PlayerMarkers.second, hexBoard, p2FirstMove, aiPlayer);                
            else move = playHumanP2Turn(
                settings.PlayerNames.second, settings.PlayerMarkers.second, hexBoard, p2FirstMove);

            p2FirstMove = false;
        }

        aiPlayer.RemoveMove(move);

        if (hexBoard.HasPlayerWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == hexBoard.P1) ? hexBoard.P2 : hexBoard.P1; 
    }

    std::cout << hexBoard << (currentPlayer == hexBoard.P1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}