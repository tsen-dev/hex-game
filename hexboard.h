#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>

#include "hexcell.h"

class HexBoard
{
    public:
        HexBoard(int width, int height, std::string p1Name = "P1", std::string p2Name = "P2");
        ~HexBoard();        
        HexCell GetCell(int x, int y) const;
        bool MarkCell(int x, int y, HexCell player);
        bool IsGameWon(HexCell player);
        friend bool CopyBoardState(HexBoard& dstBoard, HexBoard& srcBoard);
        friend std::ostream& operator<<(std::ostream& out, const HexBoard& HexBoard);

        const int Width;
        const int Height;
        const std::string P1Name;
        const std::string P2Name;

    private:                
        HexCell** BoardState;
        bool **VisitedCells;        
        bool TraversePathsFromCell(int x, int y, HexCell player);
};

#endif