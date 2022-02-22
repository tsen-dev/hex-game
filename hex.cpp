#include <iostream>
#include <ctype.h>

#include "hex.h"
#include "settings.h"
#include "hexboard.h"
#include "aiplayer.h"

void startGame()
{
    Settings settings{};
    HexBoard hexBoard{
        settings.BoardSize.first, settings.BoardSize.second, 
        settings.PlayerMarkers.first, settings.PlayerMarkers.second, 
        settings.PlayerNames.first, settings.PlayerNames.second
    };
    AIPlayer aiPlayer{1000, hexBoard};
    char currentPlayer = hexBoard.P1;
    std::pair<int, int> move;
    
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
            move = aiPlayer.GetMove();
        }
        
        while (hexBoard.MarkCell(move.first, move.second, currentPlayer) == false) 
            std::cin >> move.first >> move.second;

        aiPlayer.RemoveMove(move, currentPlayer);

        if (hexBoard.IsGameWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == hexBoard.P1) ? hexBoard.P2 : hexBoard.P1; 
    }

    std::cout << hexBoard << (currentPlayer == hexBoard.P1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}