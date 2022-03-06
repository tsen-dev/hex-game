#include <iostream>
#include <string>

#include "settings.h"
#include "hexboard.h"

/* Prompt user to enter game settings and read responses into member variables.
Also prompt for AI settings if user has selected singleplayer */
Settings::Settings()
{
    GetBoardSize();
    GetPlayerMode();
    if (SinglePlayer) 
    {
        GetAIDifficulty();
        GetAIThreadCount();
    }    
    GetPlayerMarkers();
    GetPlayerNames(SinglePlayer);
}

/* Prompt user to enter board dimensions (width and height, separated by spaces). 
Display prompt until a valid input is entered, and read the response into BoardSize */
void Settings::GetBoardSize()
{    
    BoardSize = std::pair<int, int>{0, 0};

    while (BoardSize.first <= 0 || BoardSize.second <= 0)
    {
        std::cout << "Enter the size (width and height) of the hex board (e.g. 7 7):\n\n";
        std::cin >> BoardSize.first >> BoardSize.second;
    }

    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin
    std::cout << '\n';
}

/* Prompt user to select singleplayer or multiplayer. 
Display prompt until a valid input is entered, and read the response into SinglePlayer */
void Settings::GetPlayerMode()
{
    char response = 0;

    while (response != 'Y' && response != 'N')
    {
        std::cout << "Play against AI (SinglePlayer)? (Y/N)\n\n";
        std::cin >> response;
    }

    SinglePlayer = (response == 'Y');

    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin
    std::cout << '\n';
}

/* Prompt user to enter the AI difficulty.
Display prompt until a valid input is entered, and read the response into AIDifficulty */
void Settings::GetAIDifficulty()
{
    AIDifficulty = -1;

    while (AIDifficulty < 0)
    {
        std::cout << "Enter AI difficulty: 0 (easy) --- 100 (medium) --- 1000+ (hard)\n\n";
        std::cin >> AIDifficulty;
    }
    
    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin
    std::cout << '\n';
}

/* Prompt user to enter the number of AI threads.
Display prompt until a valid input is entered, and read the response into AIThreadCount */
void Settings::GetAIThreadCount()
{
    AIThreadCount = -1;

    while (AIThreadCount < 0)
    {
        std::cout << "Enter number of AI threads (recommended 8):\n\n";
        std::cin >> AIThreadCount;
    }
    
    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin
    std::cout << '\n';
}

/* Prompt user to enter the markers for player 1 and player 2. Display prompts until valid inputs are entered i.e. markers 
are different for each player and don't use reserved EMPTY and OUT_OF_BOUNDS values. Read the responses into PlayerMarkers */
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
    
    std::getline(std::cin, std::string{}); // Consume trailing newline in std::cin   
    std::cout << '\n';
}

/* Prompt user to enter the names for player 1 and player 2. Player 2's name is set to "CPU" if in singleplayer mode.
Display prompts until valids inputs are entered, and read the response into PlayerNames */
void Settings::GetPlayerNames(bool singlePlayer)
{
    std::cout << "Enter player 1's name:\n\n";
    std::getline(std::cin, PlayerNames.first);

    if (singlePlayer)
        PlayerNames.second = "CPU";

    else
    {
        std::cout << "\nEnter player 2's name:\n\n";
        std::getline(std::cin, PlayerNames.second);
    }    

    std::cout << '\n';
}