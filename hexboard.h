#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>

#include "hexcell.h"

class HexBoard
{
    public:
        HexBoard(int width, int height);
        ~HexBoard();        
        HexCell GetCell(int x, int y) const;
        bool MarkCell(int x, int y, HexCell player);
        bool IsGameWon(HexCell player);
        friend bool CopyBoardState(HexBoard& dstBoard, HexBoard& srcBoard);
        friend std::ostream& operator<<(std::ostream& out, const HexBoard& HexBoard);

        const int Width;
        const int Height;

    private:                
        HexCell** BoardState;
        bool **VisitedCells;

        bool TraversePathsFromCell(int x, int y, HexCell player);
};

#endif