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

    private:
        void GetBoardSize();
        void GetPlayerMarkers();
        void GetPlayerNames();
        void GetPlayerMode();
};

#endif