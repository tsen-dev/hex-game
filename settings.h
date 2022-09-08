#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
    public:
        /* Prompt user to enter game settings and read responses into member variables.
        Also prompt for AI settings if user has selected singleplayer */
        Settings();
        
        std::pair<int, int> BoardSize;
        std::pair<char, char> PlayerMarkers;
        std::pair<std::string, std::string> PlayerNames;        
        bool SinglePlayer;
        int AIDifficulty = 0;
        int AIThreadCount = 0;

    private:
        /* Prompt user to enter board dimensions (width and height, separated by spaces). 
        Display prompt until a valid input is entered, and read the response into BoardSize */
        void GetBoardSize();        
        /* Prompt user to select singleplayer or multiplayer. 
        Display prompt until a valid input is entered, and read the response into SinglePlayer */
        void GetPlayerMode();  
        /* Prompt user to enter the AI difficulty.
        Display prompt until a valid input is entered, and read the response into AIDifficulty */      
        void GetAIDifficulty();
        /* Prompt user to enter the number of AI threads.
        Display prompt until a valid input is entered, and read the response into AIThreadCount */
        void GetAIThreadCount();
        /* Prompt user to enter the markers for player 1 and player 2. Display prompts until valid inputs are entered i.e. markers 
        are different for each player and don't use reserved EMPTY and OUT_OF_BOUNDS values. Read the responses into PlayerMarkers */
        void GetPlayerMarkers();
        /* Prompt user to enter the names for player 1 and player 2. Player 2's name is set to "CPU" if in singleplayer mode.
        Display prompts until valids inputs are entered, and read the response into PlayerNames */
        void GetPlayerNames(bool singlePlayer);        
};

#endif