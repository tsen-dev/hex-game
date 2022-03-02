#include <iostream>
#include <ctype.h>
#include <string>

#include "settings.h"
#include "hexboard.h"
#include "aiplayer.h"

void getMove(std::pair<int, int>& move, HexBoard& hexBoard)
{
    std::cin >> move.first >> move.second;

    while (hexBoard.GetCell(move.first, move.second) != HexBoard::EMPTY) 
    {
        hexBoard.MarkCell(move.first, move.second, hexBoard.P1); // Display the error
        std::cin >> move.first >> move.second;
    }   
}

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

void startGame()
{
    Settings settings{};
    HexBoard hexBoard{settings};
    AIPlayer aiPlayer{hexBoard, settings.AIDifficulty, settings.AIThreadCount};    
    std::pair<int, int> move;
    bool firstMove = true;
    char currentPlayer = hexBoard.P1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == hexBoard.P1)
        {   
            std::cout << hexBoard.P1Name << (tolower(hexBoard.P1Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         
            getMove(move, hexBoard);
            hexBoard.MarkCell(move.first, move.second, currentPlayer);
        }

        else 
        {
            std::cout << hexBoard.P2Name << (tolower(hexBoard.P2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                            

            if (settings.SinglePlayer)
            {
                move = aiPlayer.GetMove();
                hexBoard.MarkCell(move.first, move.second, currentPlayer);
            }     

            else
            {
                if (firstMove == true && offerSwap(hexBoard) == true) 
                {
                    hexBoard.SwapPlayers();
                    currentPlayer = hexBoard.P1;
                }
                    
                else
                {
                    std::cout << "Enter your move:\n\n";
                    getMove(move, hexBoard);                    
                    hexBoard.MarkCell(move.first, move.second, currentPlayer);
                }      

                firstMove = false;
            }            
        }

        aiPlayer.RemoveMove(move, currentPlayer);

        if (hexBoard.HasPlayerWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == hexBoard.P1) ? hexBoard.P2 : hexBoard.P1; 
    }

    std::cout << hexBoard << (currentPlayer == hexBoard.P1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}