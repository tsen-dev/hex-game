#include <iostream>
#include <iomanip>

#include "hexboard.h"

void HexBoard::InitialiseBoardState()
{
    BoardState = new char* [Height];

    for (int row = 0; row < Height; ++row)
    {
        BoardState[row] = new char[Width];

        for (int col = 0; col < Width; ++col)
        {
            BoardState[row][col] = HexBoard::EMPTY;
        }            
    }
}

void HexBoard::InitialiseVisitedCells()
{
    VisitedCells = new bool* [Height];

    for (int row = 0; row < Height; ++row)
    {
        VisitedCells[row] = new bool[Width];

        for (int col = 0; col < Width; ++col)
        {
            VisitedCells[row][col] = false;
        }            
    }        
}

HexBoard::HexBoard(int width, int height, char p1, char p2, std::string p1Name, std::string p2Name) 
    : Width{width}, Height{height}, P1{p1}, P2{p2}, P1Name{p1Name}, P2Name{p2Name}
{
    InitialiseBoardState();
    InitialiseVisitedCells();
}

HexBoard::HexBoard(Settings& settings) :
    Width{settings.BoardSize.first}, Height{settings.BoardSize.second}, 
    P1{settings.PlayerMarkers.first}, P2{settings.PlayerMarkers.second}, 
    P1Name{settings.PlayerNames.first}, P2Name{settings.PlayerNames.second}
{
    InitialiseBoardState();
    InitialiseVisitedCells();    
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

char HexBoard::GetCell(int x, int y) const
{
    if (x < 0 || x >= Width || y < 0 || y >= Height)    
        return HexBoard::OUT_OF_BOUNDS;
    else
        return BoardState[y][x];        
}

bool HexBoard::MarkCell(int x, int y, char player)
{
    switch (GetCell(x, y))
    {
        case HexBoard::EMPTY:
            BoardState[y][x] = player;
            return true;
        case HexBoard::OUT_OF_BOUNDS:
            std::cerr << '(' << x << ',' << y << ") is out of bounds\n\n";
            return false;
        default:
            std::cerr << '(' << x << ',' << y << ") is occupied\n\n";
            return false;
    }
}

bool HexBoard::TraversePathsFromCell(int x, int y, char player)
{
    if (player == P1 && x == Width - 1) return true;
    else if (player == P2 && y == Height - 1) return true;

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

bool HexBoard::HasPlayerWon(char player)
{
    bool isGameWon = false;
    int row = 0, col = 0;
    int end = (player == P1 ? Height : Width);

    for (int i = 0; i < end; ++i)
    {
        if (GetCell(col, row) == player && TraversePathsFromCell(col, row, player) == true)
        {
            isGameWon = true;
            break;
        }                    
        player == P1 ? ++col : ++row;
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
    int rowPadding = rowHeaderWidth + hexboard.P1Name.size() + strlen(" ");
    
    // Print player 2's header
    std::cout << std::setw(rowPadding + ((hexboard.Width - 1) * 4) / 2 - hexboard.P2Name.size() / 2) << "" << hexboard.P2Name << '\n';

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
            std::cout << std::setw(rowPadding - rowHeaderWidth - strlen(" ")) << std::right << hexboard.P1Name << ' ' << std::setw(rowHeaderWidth) << row << ' ';
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