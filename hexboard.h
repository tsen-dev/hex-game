#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>
#include <vector>

#include "settings.h"

class HexBoard
{
    public:
        /* Create a hex board of size width x height. Throws an std::invalid_argument exception if player markers (p1/p2) 
        are a reserved value (HexBoaard::EMPTY/HexBoard::OUT_OF_BOUNDS) */
        HexBoard(int width = 7, int height = 7, char p1 = 'X', char p2 = 'O', const std::string& p1Name = "P1", const std::string& p2Name = "P2");        
        // Create a deep copy of the specified board
        HexBoard(const HexBoard& hexBoard);         
        /* Create a board using the configuration specified in settings. Throws an std::invalid_argument exception if player markers 
        are a reserved value (HexBoaard::EMPTY/HexBoard::OUT_OF_BOUNDS) */
        HexBoard(const Settings& settings); 
        // Return the state of the cell (x, y) if (x, y) is in the bounds of the board. Otherwise return HexBoard::OUT_OF_BOUNDS
        char GetCell(int x, int y) const; 
        /* Change the state of the cell (x, y) to player. If (x, y) is occupied (i.e. not HexBoard::EMPTY) or out of bounds, display
        an error instead. Return true if a cell's state was changed, false otherwise */
        bool MarkCell(int x, int y, char player);
        /* Swaps the board's players such that player 1 plays vertically and player 2 plays horizontally. The board state is 
        also inverted such that player 1 marks become player 2 marks, and vice versa */
        void SwapPlayers();
        /* Check if player has a winning path. A winning path is horizontal if player is player 1, vertical otherwise. 
        Return true if a winning path is found, false otherwise */
        bool HasPlayerWon(char player);
        // Create a deep copy of the specified board
        HexBoard& operator=(const HexBoard& hexBoard); 
        // Print the board, player names, and headers for each row and column
        friend std::ostream& operator<<(std::ostream& out, const HexBoard& HexBoard);
                
        int Width;
        int Height;
        std::vector<char> BoardState; // 1D array containing the board's state
        // Used when determining whether to check for a vertical or horizontal path in HasPlayerWon
        char P1;
        char P2;
        // Used when printing the board in operator<<
        std::string P1Name;
        std::string P2Name;

        // Reserved cell values with special meanings
        static const char EMPTY = '.';
        static const char OUT_OF_BOUNDS = '\0';

    private:                        
        std::vector<bool> VisitedCells; // Memory used by the TraversePathsFromCell function 

        /* Find a winning path starting from cell (x, y). If player is player 1, a winning path is a path that reaches the right
        side of the board, otherwise it is a path that reaches the bottom side of the board. Return true if a winning path is 
        found, otherwise return false */
        bool TraversePathsFromCell(int x, int y, char player);
};

#endif