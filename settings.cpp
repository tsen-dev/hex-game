#include <iostream>

#include "settings.h"
#include "hexboard.h"

Settings::Settings()
{
    GetBoardSize();
    GetPlayerMode();
    if (SinglePlayer == true) 
    {
        GetAIDifficulty();
        GetAIThreadCount();
    }    
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

    std::cout << '\n';
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

    std::cout << '\n';
}

void Settings::GetAIDifficulty()
{
    AIDifficulty = -1;

    while (AIDifficulty < 0)
    {
        std::cout << "Enter AI difficulty: 0 (easy) --- 100 (medium) --- 1000+ (hard)\n\n";
        std::cin >> AIDifficulty;
    }
    
    std::cout << '\n';
}

void Settings::GetAIThreadCount()
{
    AIThreadCount = -1;

    while (AIThreadCount < 0)
    {
        std::cout << "Enter number of AI threads (recommended 8):\n\n";
        std::cin >> AIThreadCount;
    }
    
    std::cout << '\n';
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

    std::cout << "\nEnter player 2's marker:\n\n";
    std::cin >> PlayerMarkers.second;

    while (PlayerMarkers.second == HexBoard::EMPTY || PlayerMarkers.second == HexBoard::OUT_OF_BOUNDS || PlayerMarkers.second == PlayerMarkers.first)
    {
        std::cout << HexBoard::EMPTY << " (empty cell) or " << PlayerMarkers.first << " (player 1) can't be used\n\n";
        std::cin >> PlayerMarkers.second;
    }

    std::cout << '\n';
}

void Settings::GetPlayerNames()
{
    std::cout << "Enter player 1's name:\n\n";
    std::cin >> PlayerNames.first;

    if (SinglePlayer == true)
        PlayerNames.second = "CPU";

    else
    {
        std::cout << "\nEnter player 2's name:\n\n";
        std::cin >> PlayerNames.second;
    }    

    std::cout << '\n';
}