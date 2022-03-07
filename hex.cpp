#include <iostream>
#include <ctype.h>
#include <string>

#include "settings.h"
#include "hexboard.h"
#include "aiplayer.h"

const std::pair<int, int> SWAP = AIPlayer::SWAP;

// Prompt the user to enter a move. Display input error and wait for valid input if move is invalid. Return entered move
std::pair<int, int> getMove(HexBoard& hexBoard)
{
    std::pair<int, int> move;

    std::cout << "Enter your move: \n\n";
    std::cin >> move.first >> move.second;

    while (hexBoard.GetCell(move.first, move.second) != HexBoard::EMPTY) // Input cell is occupied or out-of-bounds
    {
        hexBoard.MarkCell(move.first, move.second, hexBoard.P1); // Display the input error
        std::cin >> move.first >> move.second;
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

std::pair<int, int> playP1Turn(std::string& p1Name, char p1Marker, HexBoard& hexBoard)
{
    std::cout << p1Name << (tolower(p1Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";         

    std::pair<int, int> move = getMove(hexBoard);

    hexBoard.MarkCell(move.first, move.second, p1Marker);
    
    return move;
}

std::pair<int, int> playHumanP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove)
{
    std::cout << p2Name << (tolower(p2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                               
    
    if (firstMove == true && offerSwap(hexBoard) == true) 
    {
        hexBoard.SwapPlayers();                    
        return SWAP;
    }

    else
    {
        std::pair<int, int> move = getMove(hexBoard);                    
        hexBoard.MarkCell(move.first, move.second, p2Marker);
        return move;
    }
}

std::pair<int, int> playAIP2Turn(std::string& p2Name, char p2Marker, HexBoard& hexBoard, bool firstMove, AIPlayer& aiPlayer)
{    
    std::cout << p2Name << (tolower(p2Name.back()) == 's' ? "'" : "'s") << " Turn:\n\n";                           

    std::pair<int, int> move = aiPlayer.GetMove(firstMove);

    if (move == AIPlayer::SWAP) 
    {
        hexBoard.SwapPlayers();
        return SWAP;
    }

    else
    {
        hexBoard.MarkCell(move.first, move.second, p2Marker);
        return move;
    }    
}

void startGame()
{
    Settings settings{};
    HexBoard hexBoard{settings};
    AIPlayer aiPlayer{hexBoard, settings.AIDifficulty, settings.AIThreadCount};    
    std::pair<int, int> move;
    bool firstMove = true; // Whether player 2 can swap with player 1
    char currentPlayer = hexBoard.P1;
    
    while (true)
    {
        std::cout << hexBoard;

        if (currentPlayer == settings.PlayerMarkers.first)
            move = playP1Turn(settings.PlayerNames.first, settings.PlayerMarkers.first, hexBoard);

        else 
        {
            if (settings.SinglePlayer) move = playAIP2Turn(
                settings.PlayerNames.second, settings.PlayerMarkers.second, hexBoard, firstMove, aiPlayer);                
            else move = playHumanP2Turn(
                settings.PlayerNames.second, settings.PlayerMarkers.second, hexBoard, firstMove);

            firstMove = false;
        }

        aiPlayer.RemoveMove(move);

        if (hexBoard.HasPlayerWon(currentPlayer) == true) break;
        else currentPlayer = (currentPlayer == hexBoard.P1) ? hexBoard.P2 : hexBoard.P1; 
    }

    std::cout << hexBoard << (currentPlayer == hexBoard.P1 ? hexBoard.P1Name : hexBoard.P2Name) << " Wins!\n\n";
}