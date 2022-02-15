#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>

#include "hexcell.h"

class HexBoard
{
    public:
        HexCell CurrentPlayer;        

        HexBoard(int width, int height);
        HexBoard(HexBoard& hexBoard);
        ~HexBoard();        
        HexCell GetCell(int xCoordinate, int yCoordinate) const;
        bool MarkCell(int xCoordinate, int yCoordinate, HexCell mark);
        void StartGame();
        bool IsGameWon();
        friend std::ostream& operator<<(std::ostream& out, const HexBoard& HexBoard);

        const int Width;
        const int Height;

    private:                
        HexCell** AdjacencyMatrix;
        bool **VisitedCells;

        bool TraversePathsFromCell(int x, int y);
        void GetAIMove(std::pair<int, int>& move, int sampleCount);
};

#endif