#include <iostream>

#include "hexboard.h"
#include "aiplayer.h"
#include "hexcell.h"

void startGame(int width, int height)
{
    std::pair<int, int> move;
    HexBoard hexBoard{width, height};
    AIPlayer aiPlayer{1000, hexBoard};
    HexCell currentPlayer = HexCell::PLAYER1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == HexCell::PLAYER1)
        {   
            std::cout << "Player 1's Turn:\n\n";         
            std::cin >> move.first >> move.second;            
        }

        else 
        {
            std::cout << "Player 2's Turn:\n\n";
            move = aiPlayer.GetMove();
        }
        
        while (hexBoard.MarkCell(move.first, move.second, currentPlayer) == false) 
            std::cin >> move.first >> move.second;

        aiPlayer.RemoveMove(move, currentPlayer);

        if (hexBoard.IsGameWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; 
    }

    std::cout << hexBoard << (currentPlayer == HexCell::PLAYER1 ? "Player 1 Wins!\n\n" : "Player 2 Wins!\n\n");
}