#include <iostream>
#include <ctype.h>

#include "hex.h"
#include "settings.h"
#include "hexboard.h"
#include "aiplayer.h"

void startGame()
{
    Settings settings{};
    HexBoard hexBoard{settings};
    AIPlayer aiPlayer{hexBoard, settings.AIDifficulty, settings.AIThreadCount};    
    std::pair<int, int> move;
    char currentPlayer = hexBoard.P1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == hexBoard.P1)
        {   
            std::cout << hexBoard.P1Name << (tolower(hexBoard.P1Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         
            std::cin >> move.first >> move.second;            
        }

        else 
        {
            std::cout << hexBoard.P2Name << (tolower(hexBoard.P2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                            
            if (settings.SinglePlayer == true) move = aiPlayer.GetMove();
            else std::cin >> move.first >> move.second;             
        }
        
        while (hexBoard.MarkCell(move.first, move.second, currentPlayer) == false) 
            std::cin >> move.first >> move.second;

        aiPlayer.RemoveMove(move, currentPlayer);

        if (hexBoard.HasPlayerWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == hexBoard.P1) ? hexBoard.P2 : hexBoard.P1; 
    }

    std::cout << hexBoard << (currentPlayer == hexBoard.P1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}