#include <iostream>

#include "settings.h"
#include "hexboard.h"

Settings::Settings()
{
    GetBoardSize();
    GetPlayerMode();
    GetPlayerMarkers();
    GetPlayerNames();
}

void Settings::GetBoardSize()
{    
    BoardSize = std::pair<int, int>{0, 0};

    while (BoardSize.first <= 0 || BoardSize.second <= 0)
    {
        std::cout << "Enter the size (width and height) of the hex board (e.g. 7 7):\n\n";
        std::cin >> BoardSize.first >> BoardSize.second;
    }
}

void Settings::GetPlayerMode()
{
    char response = 0;

    while (response != 'Y' && response != 'N')
    {
        std::cout << "Play against AI (SinglePlayer)? (Y/N)\n\n";
        std::cin >> response;
    }

    SinglePlayer = (response == 'Y');
}

void Settings::GetPlayerMarkers()
{
    std::cout << "Enter player 1's marker:\n\n";
    std::cin >> PlayerMarkers.first;

    while (PlayerMarkers.first == HexBoard::EMPTY || PlayerMarkers.first == HexBoard::OUT_OF_BOUNDS)
    {
        std::cout << HexBoard::EMPTY << " (empty cell) can't be used\n\n";
        std::cin >> PlayerMarkers.first;
    }

    std::cout << "Enter player 2's marker:\n\n";
    std::cin >> PlayerMarkers.second;

    while (PlayerMarkers.second == HexBoard::EMPTY || PlayerMarkers.second == HexBoard::OUT_OF_BOUNDS || PlayerMarkers.second == PlayerMarkers.first)
    {
        std::cout << HexBoard::EMPTY << " (empty cell) or " << PlayerMarkers.first << " (player 1) can't be used\n\n";
        std::cin >> PlayerMarkers.second;
    }
}

void Settings::GetPlayerNames()
{
    std::cout << "Enter player 1's name:\n\n";
    std::cin >> PlayerNames.first;

    if (SinglePlayer == true)
        PlayerNames.second = "CPU";

    else
    {
        std::cout << "Enter player 2's name:\n\n";
        std::cin >> PlayerNames.second;
    }    
}