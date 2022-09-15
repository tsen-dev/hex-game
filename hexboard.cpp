#include <iostream>
#include <iomanip>

#include "hexboard.h"

// Create a hex board of size width x height
HexBoard::HexBoard(int width, int height, char p1, char p2, const std::string& p1Name, const std::string& p2Name) : 
    BoardState(width * height, HexBoard::EMPTY), Width{width}, Height{height}, P1{p1}, P2{p2}, 
    P1Name{p1Name}, P2Name{p2Name}, VisitedCells(width * height, false) {}

// Create a deep copy of the specified board
HexBoard::HexBoard(const HexBoard& hexBoard) : 
    BoardState(hexBoard.BoardState), Width{hexBoard.Width}, Height{hexBoard.Height}, P1{hexBoard.P1}, P2{hexBoard.P2}, 
    P1Name{hexBoard.P1Name}, P2Name{hexBoard.P2Name}, VisitedCells(hexBoard.Width * hexBoard.Height, false) {}

// Create a board using the configuration specified in settings
HexBoard::HexBoard(const Settings& settings) :
    BoardState(settings.BoardSize.first * settings.BoardSize.first, HexBoard::EMPTY), Width{settings.BoardSize.first}, 
    Height{settings.BoardSize.second}, P1{settings.PlayerMarkers.first}, P2{settings.PlayerMarkers.second}, 
    P1Name{settings.PlayerNames.first}, P2Name{settings.PlayerNames.second}, VisitedCells(settings.BoardSize.first * settings.BoardSize.first, false) {}

// Return the state of the cell (x, y) if (x, y) is in the bounds of the board. Otherwise return HexBoard::OUT_OF_BOUNDS
char HexBoard::GetCell(int x, int y) const
{
    if (x < 0 || x >= Width || y < 0 || y >= Height)    
        return HexBoard::OUT_OF_BOUNDS;
    else
        return BoardState[x + y * Width];        
}

/* Change the state of the cell (x, y) to player. If (x, y) is occupied (i.e. not HexBoard::EMPTY) or out of bounds, display
an error instead. Return true if a cell's state was changed, false otherwise */
bool HexBoard::MarkCell(int x, int y, char player)
{
    switch (GetCell(x, y))
    {
        case HexBoard::EMPTY:
            BoardState[x + y * Width] = player;
            return true;
        case HexBoard::OUT_OF_BOUNDS:
            std::cerr << '(' << x << ',' << y << ") is out of bounds\n\n";
            return false;
        default:
            std::cerr << '(' << x << ',' << y << ") is occupied\n\n";
            return false;
    }
}

/* Swaps the board's players such that player 1 plays vertically and player 2 plays horizontally. The board state is 
also inverted such that player 1 marks become player 2 marks, and vice versa */
void HexBoard::SwapPlayers()
{
    std::swap(P1, P2);
    std::swap(P1Name, P2Name);

    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            if (BoardState[x + y * Width] == P1) BoardState[x + y * Width] = P2;
            else if (BoardState[x + y * Width] == P2) BoardState[x + y * Width] = P1;
        }
    }
}

/* Find a winning path starting from cell (x, y). If player is player 1, a winning path is a path that reaches the right
side of the board, otherwise it is a path that reaches the bottom side of the board. Return true if a winning path is 
found, otherwise return false */
bool HexBoard::TraversePathsFromCell(int x, int y, char player)
{
    // Check if (x, y) is at the right or bottom edge

    if (player == P1 && x == Width - 1) return true;
    else if (player == P2 && y == Height - 1) return true;

    VisitedCells[x + y * Width] = true;

    // Check cell (x, y)'s hexagonal neighbours    

    if (GetCell(x - 1, y) == player && VisitedCells[(x - 1) + y * Width] == false)
        if (TraversePathsFromCell(x - 1, y, player) == true) return true;

    if (GetCell(x - 1, y + 1) == player && VisitedCells[(x - 1) + (y + 1) * Width] == false)
        if (TraversePathsFromCell(x - 1, y + 1, player) == true) return true;

    if (GetCell(x, y - 1) == player && VisitedCells[x + (y - 1) * Width] == false)
        if (TraversePathsFromCell(x, y - 1, player) == true) return true;

    if (GetCell(x, y + 1) == player && VisitedCells[x + (y + 1) * Width] == false)
        if (TraversePathsFromCell(x, y + 1, player) == true) return true;

    if (GetCell(x + 1, y - 1) == player && VisitedCells[(x + 1) + (y - 1) * Width] == false)
        if (TraversePathsFromCell(x + 1, y - 1, player) == true) return true;

    if (GetCell(x + 1, y) == player && VisitedCells[(x + 1) + y * Width] == false)
        if (TraversePathsFromCell(x + 1, y, player) == true) return true;

    return false;
}

/* Check if player has a winning path. A winning path is horizontal if player is player 1, vertical otherwise. 
Return true if a winning path is found, false otherwise */
bool HexBoard::HasPlayerWon(char player)
{
    bool isGameWon = false;
    int y = 0, x = 0;
    int end = (player == P1 ? Height : Width);
    VisitedCells.assign(Width * Height, false);

    for (int i = 0; i < end; ++i) // Look for a path from the first column (P1) or y (P2) to the opposite side
    {
        if (GetCell(x, y) == player && TraversePathsFromCell(x, y, player) == true)
        {
            isGameWon = true;
            break;
        }   

        player == P1 ? ++y : ++x;
    }
    
    return isGameWon;
}

// Return the number of digits in n
inline int numberOfDigits(int n) {return (n == 0) ? 1 : floor(log10(n)) + 1;}

// Print the board, player names, and headers for each row and column
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