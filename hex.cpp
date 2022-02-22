#include <iostream>
#include <ctype.h>

#include "hexboard.h"
#include "aiplayer.h"
#include "hexcell.h"

void startGame(int width, int height)
{
    std::pair<int, int> move;
    HexBoard hexBoard{width, height, "TCS", "adadsafsafsadfasdfsdafiufhsdafoiaushofiasuhdfoih"};
    AIPlayer aiPlayer{1000, hexBoard};
    const char P1 = 'T', const P2 = 'H';
    HexCell currentPlayer = HexCell::PLAYER1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == HexCell::PLAYER1)
        {   
            std::cout << hexBoard.P1Name << (tolower(hexBoard.P1Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         
            std::cin >> move.first >> move.second;            
        }

        else 
        {
            std::cout << hexBoard.P2Name << (tolower(hexBoard.P2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         
            move = aiPlayer.GetMove();
        }
        
        while (hexBoard.MarkCell(move.first, move.second, currentPlayer) == false) 
            std::cin >> move.first >> move.second;

        aiPlayer.RemoveMove(move, currentPlayer);

        if (hexBoard.IsGameWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; 
    }

    std::cout << hexBoard << (currentPlayer == HexCell::PLAYER1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}