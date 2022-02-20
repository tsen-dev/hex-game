#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "hexboard.h"
#include "hexcell.h"
#include "aiplayer.h"

HexGame::HexGame(int width, int height) : BoardWidth{width}, BoardHeight{height}
{
    CurrentPlayer = HexCell::PLAYER1;
    Board = new HexCell* [BoardHeight];
    VisitedCells = new bool*[BoardHeight];

    for (int row = 0; row < BoardHeight; ++row)
    {
        Board[row] = new HexCell[BoardWidth];
        VisitedCells[row] = new bool[BoardWidth];

        for (int col = 0; col < BoardWidth; ++col)
        {
            Board[row][col] = HexCell::EMPTY;
            VisitedCells[row][col] = false;
        }            
    }        
}

HexGame::HexGame(HexGame& hexBoard) : BoardWidth{hexBoard.BoardWidth}, BoardHeight{hexBoard.BoardHeight}, CurrentPlayer{hexBoard.CurrentPlayer}
{
    Board = new HexCell* [BoardHeight];
    VisitedCells = new bool*[BoardHeight];

    for (int row = 0; row < BoardHeight; ++row)
    {
        Board[row] = new HexCell[BoardWidth];
        VisitedCells[row] = new bool[BoardWidth];

        for (int col = 0; col < BoardWidth; ++col)
        {
            Board[row][col] = hexBoard.Board[row][col];
            VisitedCells[row][col] = false;
        }            
    }
}

HexGame::~HexGame()
{
    for (int row = 0; row < BoardHeight; ++row)
    {
        delete[] Board[row];
        delete[] VisitedCells[row];
    }
        
    delete[] Board;
    delete[] VisitedCells;
}

HexCell HexGame::GetCell(int xCoordinate, int yCoordinate) const
{
    if (xCoordinate < 0 || xCoordinate >= BoardWidth || yCoordinate < 0 || yCoordinate >= BoardHeight)    
        return HexCell::NO_CELL;
    else
        return Board[yCoordinate][xCoordinate];        
}

bool HexGame::MarkCell(int xCoordinate, int yCoordinate, HexCell mark)
{
    switch (GetCell(xCoordinate, yCoordinate))
    {
        case HexCell::EMPTY:
            Board[yCoordinate][xCoordinate] = mark;
            return true;
        case HexCell::NO_CELL:
            std::cerr << '(' << xCoordinate << ',' << yCoordinate << ") is out of bounds\n\n";
            return false;
        default:
            std::cerr << '(' << xCoordinate << ',' << yCoordinate << ") is occupied\n\n";
            return false;
    }
}

void HexGame::StartGame()
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

        aiPlayer.RemovePlayedMove(move, BoardWidth);

        if (IsGameWon() == true) break;
        else CurrentPlayer = (CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; 
    }

    std::cout << *this << (CurrentPlayer == HexCell::PLAYER1 ? "Player 1 Wins!\n\n" : "Player 2 Wins!\n\n");
}

bool HexGame::TraversePathsFromCell(int x, int y)
{
    switch (CurrentPlayer)
    {
        case HexCell::PLAYER1: if (x == BoardWidth - 1) return true; else break;
        case HexCell::PLAYER2: if (y == BoardHeight - 1) return true; else break;
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

bool HexGame::IsGameWon()
{
    int isGameWon = false;

    if (CurrentPlayer == HexCell::PLAYER1)
    {
        for (int row = 0; row < BoardHeight; ++row)
            if (GetCell(0, row) == CurrentPlayer && (isGameWon = TraversePathsFromCell(0, row)) == true)
                break;
    }

    else
    {
        for (int col = 0; col < BoardWidth; ++col)
            if (GetCell(col, 0) == CurrentPlayer && (isGameWon = TraversePathsFromCell(col, 0)) == true)
                break;
    }

    for (int row = 0; row < BoardHeight; ++row)
        for (int col = 0; col < BoardWidth; ++col)
            VisitedCells[row][col] = false;
    
    return isGameWon;
}

inline int numberOfDigits(int n) {return (n == 0) ? 1 : floor(log10(n)) + 1;}

std::ostream& operator<<(std::ostream& out, const HexGame& hexboard)
{   
    int colHeaderWidth = numberOfDigits(hexboard.BoardWidth - 1);    
    int rowHeaderWidth = numberOfDigits(hexboard.BoardHeight - 1);    
    int rowPadding = rowHeaderWidth + strlen("P1") + strlen(" ");
    
    // Print player 2's header
    std::cout << std::setw(rowPadding + ((hexboard.BoardWidth - 1) * 4) / 2 - strlen("P2") / 2) << "" << "P2\n";

    // Print top column headers
    for (int col = 0; col < hexboard.BoardWidth; ++col)
    {
        if (col == 0) std::cout << std::setw(rowPadding) << "";
        std::cout << std::setw(colHeaderWidth) << std::left << col << \
        std::setw(strlen("    ") - colHeaderWidth) << "";
    }
   
    std::cout << '\n';

    for (int row = 0; row < hexboard.BoardHeight; ++row)
    {
        if (row == hexboard.BoardHeight / 2) // Print player 1's header and left row header
            std::cout << std::setw(rowPadding - rowHeaderWidth - strlen(" ")) << std::right << "P1" << ' ' << std::setw(rowHeaderWidth) << row << ' ';
        else // Print left row header only
            std::cout << std::setw(rowPadding) << std::right << row << ' ';
        
        // Print row and horizontal links
        for (int col = 0; col < hexboard.BoardWidth - 1; ++col)
            std::cout << hexboard.GetCell(col, row) << " - ";
        // Print row's last cell and right row header
        std::cout << hexboard.GetCell(hexboard.BoardWidth - 1, row) << ' ' << std::left << row << '\n';        

        // Print links to cells below
        if (row < hexboard.BoardHeight - 1)
        {
            std::cout << std::setw(rowPadding += 2) << "";
            for (int col = 0; col < hexboard.BoardWidth - 1; ++col)
                std::cout << "\\ / ";
            std::cout << "\\ \n";
        }
    }

    // Print bottom column headers
    std::cout << std::setw(rowPadding + 2) << "" << std::left;
    for (int col = 0; col < hexboard.BoardWidth; ++col)
         std::cout << std::setw(colHeaderWidth) << col << std::setw(strlen("    ") - colHeaderWidth) << "";
    std::cout << "\n\n";

    return out;
}