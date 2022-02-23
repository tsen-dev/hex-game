#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>

#include "settings.h"

class HexBoard
{
    public:
        HexBoard(int width, int height, char p1 = 'X', char p2 = 'O', std::string p1Name = "P1", std::string p2Name = "P2");
        HexBoard(Settings& settings);
        ~HexBoard();        
        char GetCell(int x, int y) const;
        bool MarkCell(int x, int y, char player);
        bool IsGameWon(char player);
        friend bool CopyBoardState(HexBoard& dstBoard, HexBoard& srcBoard);
        friend std::ostream& operator<<(std::ostream& out, const HexBoard& HexBoard);

        const int Width;
        const int Height;
        const char P1;
        const char P2;
        const std::string P1Name;
        const std::string P2Name;

        static const char EMPTY = '.';
        static const char OUT_OF_BOUNDS = '\0';

    private:                
        char** BoardState;
        bool **VisitedCells;        
        void InitialiseVisitedCells();
        void InitialiseBoardState();
        bool TraversePathsFromCell(int x, int y, char player);
};

#endif