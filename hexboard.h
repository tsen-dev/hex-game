#ifndef HEXGAME_H
#define HEXGAME_H

#include <iostream>

#include "hexcell.h"

class HexGame
{
    public:
        HexCell CurrentPlayer;        

        HexGame(int width, int height);
        HexGame(HexGame& HexGame);
        ~HexGame();        
        HexCell GetCell(int xCoordinate, int yCoordinate) const;
        bool MarkCell(int xCoordinate, int yCoordinate, HexCell mark);
        void StartGame();
        bool IsGameWon();
        friend std::ostream& operator<<(std::ostream& out, const HexGame& HexGame);

        const int BoardWidth;
        const int BoardHeight;

    private:                
        HexCell** Board;
        bool **VisitedCells;

        bool TraversePathsFromCell(int x, int y);
        std::pair<int, int> GetAIMove(int sampleCount);
};

#endif