#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "hexboard.h"
#include "hexcell.h"
#include "aiplayer.h"

HexBoard::HexBoard(int width, int height) : Width{width}, Height{height}
{
    CurrentPlayer = HexCell::PLAYER1;
    AdjacencyMatrix = new HexCell* [Height];
    VisitedCells = new bool*[Height];

    for (int row = 0; row < Height; ++row)
    {
        AdjacencyMatrix[row] = new HexCell[Width];
        VisitedCells[row] = new bool[Width];

        for (int col = 0; col < Width; ++col)
        {
            AdjacencyMatrix[row][col] = HexCell::EMPTY;
            VisitedCells[row][col] = false;
        }            
    }        
}

HexBoard::HexBoard(HexBoard& hexBoard) : Width{hexBoard.Width}, Height{hexBoard.Height}, CurrentPlayer{hexBoard.CurrentPlayer}
{
    AdjacencyMatrix = new HexCell* [Height];
    VisitedCells = new bool*[Height];

    for (int row = 0; row < Height; ++row)
    {
        AdjacencyMatrix[row] = new HexCell[Width];
        VisitedCells[row] = new bool[Width];

        for (int col = 0; col < Width; ++col)
        {
            AdjacencyMatrix[row][col] = hexBoard.AdjacencyMatrix[row][col];
            VisitedCells[row][col] = false;
        }            
    }
}

HexBoard::~HexBoard()
{
    for (int row = 0; row < Height; ++row)
    {
        delete[] AdjacencyMatrix[row];
        delete[] VisitedCells[row];
    }
        
    delete[] AdjacencyMatrix;
    delete[] VisitedCells;
}

HexCell HexBoard::GetCell(int xCoordinate, int yCoordinate) const
{
    if (xCoordinate < 0 || xCoordinate >= Width || yCoordinate < 0 || yCoordinate >= Height)    
        return HexCell::NO_CELL;
    else
        return AdjacencyMatrix[yCoordinate][xCoordinate];        
}

bool HexBoard::MarkCell(int xCoordinate, int yCoordinate, HexCell mark)
{
    switch (GetCell(xCoordinate, yCoordinate))
    {
        case HexCell::EMPTY:
            AdjacencyMatrix[yCoordinate][xCoordinate] = mark;
            return true;
        case HexCell::NO_CELL:
            std::cerr << '(' << xCoordinate << ',' << yCoordinate << ") is out of bounds\n\n";
            return false;
        default:
            std::cerr << '(' << xCoordinate << ',' << yCoordinate << ") is occupied\n\n";
            return false;
    }
}

void HexBoard::StartGame()
{
    std::pair<int, int> move;
    AIPlayer aiPlayer{1000, *this};

    while (true)
    {
        std::cout << *this;

        if (CurrentPlayer == HexCell::PLAYER1)
        {   
            std::cout << "Player 1's Turn:\n\n";         
            std::cin >> move.first >> move.second;            
        }

        else 
        {
            std::cout << "Player 2's Turn:\n\n";
            move = aiPlayer.GetMove(*this);
        }
        
        while (MarkCell(move.first, move.second, CurrentPlayer) == false) 
            std::cin >> move.first >> move.second;

        aiPlayer.RemovePlayedMove(move, Width);

        if (IsGameWon() == true) break;
        else CurrentPlayer = (CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; 
    }

    std::cout << *this << (CurrentPlayer == HexCell::PLAYER1 ? "Player 1 Wins!\n\n" : "Player 2 Wins!\n\n");
}

bool HexBoard::TraversePathsFromCell(int x, int y)
{
    switch (CurrentPlayer)
    {
        case HexCell::PLAYER1: if (x == Width - 1) return true; else break;
        case HexCell::PLAYER2: if (y == Height - 1) return true; else break;
    }

    VisitedCells[y][x] = true;

    if (GetCell(x - 1, y) == CurrentPlayer && VisitedCells[y][x - 1] == false)
        if (TraversePathsFromCell(x - 1, y) == true) return true;

    if (GetCell(x - 1, y + 1) == CurrentPlayer && VisitedCells[y + 1][x - 1] == false)
        if (TraversePathsFromCell(x - 1, y + 1) == true) return true;

    if (GetCell(x, y - 1) == CurrentPlayer && VisitedCells[y - 1][x] == false)
        if (TraversePathsFromCell(x, y - 1) == true) return true;

    if (GetCell(x, y + 1) == CurrentPlayer && VisitedCells[y + 1][x] == false)
        if (TraversePathsFromCell(x, y + 1) == true) return true;

    if (GetCell(x + 1, y - 1) == CurrentPlayer && VisitedCells[y - 1][x + 1] == false)
        if (TraversePathsFromCell(x + 1, y - 1) == true) return true;

    if (GetCell(x + 1, y) == CurrentPlayer && VisitedCells[y][x + 1] == false)
        if (TraversePathsFromCell(x + 1, y) == true) return true;

    return false;
}

bool HexBoard::IsGameWon()
{
    int isGameWon = false;

    if (CurrentPlayer == HexCell::PLAYER1)
    {
        for (int row = 0; row < Height; ++row)
            if (GetCell(0, row) == CurrentPlayer && (isGameWon = TraversePathsFromCell(0, row)) == true)
                break;
    }

    else
    {
        for (int col = 0; col < Width; ++col)
            if (GetCell(col, 0) == CurrentPlayer && (isGameWon = TraversePathsFromCell(col, 0)) == true)
                break;
    }

    for (int row = 0; row < Height; ++row)
        for (int col = 0; col < Width; ++col)
            VisitedCells[row][col] = false;
    
    return isGameWon;
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