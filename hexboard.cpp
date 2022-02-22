#include <iostream>
#include <iomanip>

#include "hexboard.h"
#include "hexcell.h"

HexBoard::HexBoard(int width, int height) : Width{width}, Height{height}
{
    BoardState = new HexCell* [Height];
    VisitedCells = new bool* [Height];

    for (int row = 0; row < Height; ++row)
    {
        BoardState[row] = new HexCell[Width];
        VisitedCells[row] = new bool[Width];

        for (int col = 0; col < Width; ++col)
        {
            BoardState[row][col] = HexCell::EMPTY;
            VisitedCells[row][col] = false;
        }            
    }        
}

HexBoard::~HexBoard()
{
    for (int row = 0; row < Height; ++row)
    {
        delete[] BoardState[row];
        delete[] VisitedCells[row];
    }
        
    delete[] BoardState;
    delete[] VisitedCells;
}

HexCell HexBoard::GetCell(int x, int y) const
{
    if (x < 0 || x >= Width || y < 0 || y >= Height)    
        return HexCell::NO_CELL;
    else
        return BoardState[y][x];        
}

bool HexBoard::MarkCell(int x, int y, HexCell player)
{
    switch (GetCell(x, y))
    {
        case HexCell::EMPTY:
            BoardState[y][x] = player;
            return true;
        case HexCell::NO_CELL:
            std::cerr << '(' << x << ',' << y << ") is out of bounds\n\n";
            return false;
        default:
            std::cerr << '(' << x << ',' << y << ") is occupied\n\n";
            return false;
    }
}

bool HexBoard::TraversePathsFromCell(int x, int y, HexCell player)
{
    switch (player)
    {
        case HexCell::PLAYER1: if (x == Width - 1) return true; else break;
        case HexCell::PLAYER2: if (y == Height - 1) return true; else break;
    }

    VisitedCells[y][x] = true;

    if (GetCell(x - 1, y) == player && VisitedCells[y][x - 1] == false)
        if (TraversePathsFromCell(x - 1, y, player) == true) return true;

    if (GetCell(x - 1, y + 1) == player && VisitedCells[y + 1][x - 1] == false)
        if (TraversePathsFromCell(x - 1, y + 1, player) == true) return true;

    if (GetCell(x, y - 1) == player && VisitedCells[y - 1][x] == false)
        if (TraversePathsFromCell(x, y - 1, player) == true) return true;

    if (GetCell(x, y + 1) == player && VisitedCells[y + 1][x] == false)
        if (TraversePathsFromCell(x, y + 1, player) == true) return true;

    if (GetCell(x + 1, y - 1) == player && VisitedCells[y - 1][x + 1] == false)
        if (TraversePathsFromCell(x + 1, y - 1, player) == true) return true;

    if (GetCell(x + 1, y) == player && VisitedCells[y][x + 1] == false)
        if (TraversePathsFromCell(x + 1, y, player) == true) return true;

    return false;
}

bool HexBoard::IsGameWon(HexCell player)
{
    int isGameWon = false;

    if (player == HexCell::PLAYER1)
    {
        for (int row = 0; row < Height; ++row)
            if (GetCell(0, row) == player && (isGameWon = TraversePathsFromCell(0, row, player)) == true)
                break;
    }

    else
    {
        for (int col = 0; col < Width; ++col)
            if (GetCell(col, 0) == player && (isGameWon = TraversePathsFromCell(col, 0, player)) == true)
                break;
    }

    for (int row = 0; row < Height; ++row)
        for (int col = 0; col < Width; ++col)
            VisitedCells[row][col] = false;
    
    return isGameWon;
}

bool CopyBoardState(HexBoard& dstBoard, HexBoard& srcBoard)
{
    if (dstBoard.Width != srcBoard.Width || dstBoard.Height != srcBoard.Height)
        return false;

    for (int row = 0; row < dstBoard.Height; ++row)
        for (int col = 0; col < dstBoard.Width; ++col)
            dstBoard.BoardState[row][col] = srcBoard.BoardState[row][col];

    return true;
}

inline int numberOfDigits(int n) {return (n == 0) ? 1 : floor(log10(n)) + 1;}

std::ostream& operator<<(std::ostream& out, const HexBoard& hexboard)
{   
    int colHeaderWidth = numberOfDigits(hexboard.Width - 1);    
    int rowHeaderWidth = numberOfDigits(hexboard.Height - 1);    
    int rowPadding = rowHeaderWidth + strlen("P1") + strlen(" ");
    
    // Print player 2's header
    std::cout << std::setw(rowPadding + ((hexboard.Width - 1) * 4) / 2 - strlen("P2") / 2) << "" << "P2\n";

    // Print top column headers
    for (int col = 0; col < hexboard.Width; ++col)
    {
        if (col == 0) std::cout << std::setw(rowPadding) << "";
        std::cout << std::setw(colHeaderWidth) << std::left << col << \
        std::setw(strlen("    ") - colHeaderWidth) << "";
    }
   
    std::cout << '\n';

    for (int row = 0; row < hexboard.Height; ++row)
    {
        if (row == hexboard.Height / 2) // Print player 1's header and left row header
            std::cout << std::setw(rowPadding - rowHeaderWidth - strlen(" ")) << std::right << "P1" << ' ' << std::setw(rowHeaderWidth) << row << ' ';
        else // Print left row header only
            std::cout << std::setw(rowPadding) << std::right << row << ' ';
        
        // Print row and horizontal links
        for (int col = 0; col < hexboard.Width - 1; ++col)
            std::cout << hexboard.GetCell(col, row) << " - ";
        // Print row's last cell and right row header
        std::cout << hexboard.GetCell(hexboard.Width - 1, row) << ' ' << std::left << row << '\n';        

        // Print links to cells below
        if (row < hexboard.Height - 1)
        {
            std::cout << std::setw(rowPadding += 2) << "";
            for (int col = 0; col < hexboard.Width - 1; ++col)
                std::cout << "\\ / ";
            std::cout << "\\ \n";
        }
    }

    // Print bottom column headers
    std::cout << std::setw(rowPadding + 2) << "" << std::left;
    for (int col = 0; col < hexboard.Width; ++col)
         std::cout << std::setw(colHeaderWidth) << col << std::setw(strlen("    ") - colHeaderWidth) << "";
    std::cout << "\n\n";

    return out;
}