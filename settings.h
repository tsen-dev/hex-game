#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
{
    public:
        Settings();
        
        std::pair<int, int> BoardSize;
        std::pair<char, char> PlayerMarkers;
        std::pair<std::string, std::string> PlayerNames;        
        bool SinglePlayer;
        int AIDifficulty = 0;
        int AIThreadCount = 0;

    private:
        void GetBoardSize();        
        void GetPlayerMode();        
        void GetAIDifficulty();
        void GetAIThreadCount();
        void GetPlayerMarkers();
        void GetPlayerNames(bool singlePlayer);        
};

#endif