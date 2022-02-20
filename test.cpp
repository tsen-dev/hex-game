#include <iostream>
#include <cassert>

#include "hexboard.h"
#include "hexcell.h"
#include "aiplayer.h"
#include "test.h"

#ifdef TEST

void testCreateEmptyBoard()
{
    // Create a 3x5 game and check that all cells are initialised to HexCell::EMPTY
    
    int boardWidth = 3;
    int boardHeight = 5;    
    HexGame hexGame{boardWidth, boardHeight};
    
    for (int yCoordinate = 0; yCoordinate < boardHeight; ++yCoordinate)
    {
        for (int xCoordinate = 0; xCoordinate < boardWidth; ++xCoordinate)
        {
            assert(hexGame.GetCell(xCoordinate, yCoordinate) == HexCell::EMPTY);
        }        
    }
}

void testMarkCell()
{
    int boardWidth = 5;
    int boardHeight = 3;   
    HexGame hexGame{boardWidth, boardHeight};

    HexCell mark1 = HexCell::PLAYER1;
    HexCell mark2 = HexCell::PLAYER2;

    // Suppress error output so "(x, y) is occupied" messages don't slow tests
    auto cerrBuffer = std::cerr.rdbuf();
    std::cerr.rdbuf(nullptr);

    // Check that a cell with the game's bounds can be marked
    // Check that the correct value is retrieved from that cell
    // Check that the same cell can't be marked again

    assert(hexGame.MarkCell(0, 2, mark1) == true);
    assert(hexGame.GetCell(0, 2) == mark1);
    assert(hexGame.MarkCell(0, 2, mark1) == false);

    assert(hexGame.MarkCell(2, 0, mark2) == true);
    assert(hexGame.GetCell(2, 0) == mark2);
    assert(hexGame.MarkCell(2, 0, mark2) == false);

    // Check that cells out of the game's bounds can't be marked

    assert(hexGame.MarkCell(boardWidth, boardHeight, mark1) == false);
    assert(hexGame.MarkCell(-1, -1, mark1) == false);
    assert(hexGame.MarkCell(0, boardHeight, mark1) == false);
    assert(hexGame.MarkCell(boardWidth, 0, mark1) == false);

    // Unsuppress error output
    std::cerr.rdbuf(cerrBuffer);
}

void testGetCell()
{
    int boardWidth = 5;
    int boardHeight = 3;

    HexGame hexGame{boardWidth, boardHeight};

    // Check that attempts to retrieve values of out of bounds cells are detected

    assert(hexGame.GetCell(boardWidth, boardHeight) == HexCell::NO_CELL);
    assert(hexGame.GetCell(-1, -1) == HexCell::NO_CELL);
    assert(hexGame.GetCell(0, boardHeight) == HexCell::NO_CELL);
    assert(hexGame.GetCell(boardWidth, 0) == HexCell::NO_CELL);
    
    // Check that the correct value of retrieved for cell in the game's bounds

    assert(hexGame.GetCell(boardWidth - 1, boardHeight - 1) == HexCell::EMPTY);
}

void testGameWonByPlayer1()
{
    // Player 1 marks a horizontal path
    // Check that Player 1 wins when the path is complete, and doesn't until it isn't

    int board1Width = 7; 
    int board1Height = 7;

    HexGame hexGame1{board1Width, board1Height};
    hexGame1.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board1Width - 1; ++col)
    {
        hexGame1.MarkCell(col, 0, HexCell::PLAYER1);
        assert(hexGame1.IsGameWon() == false);
    }    

    hexGame1.MarkCell(board1Width - 1, 0, HexCell::PLAYER1);
    assert(hexGame1.IsGameWon() == true);    

    /* 
    Player 1 marks *connected* a diagonal path:
            0 X - . - . - . 
                \ / \ / \ / \ 
               1 X - X - . - . 
                  \ / \ / \ / \  ...
                 2 . - X - X - . 
                    \ / \ / \ / \ 
                   3 . - . - X - . 
                        ... 
    Check that Player 1 wins when the path is complete, and doesn't until it isn't
    */

    int board2Width = 7;

    HexGame hexGame2{board2Width, board2Width};
    hexGame2.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board2Width - 1; ++col)
    {
        hexGame2.MarkCell(col, col, HexCell::PLAYER1);
        hexGame2.MarkCell(col, col + 1, HexCell::PLAYER1);
        assert(hexGame2.IsGameWon() == false);
    }

    hexGame2.MarkCell(board2Width - 1, board2Width - 1, HexCell::PLAYER1);
    assert(hexGame2.IsGameWon() == true);

    /*
    Player 1 marks a complex path with branching and reversing:
          X - . - . - . - . - . - .     
           \ / \ / \ / \ / \ / \ / \     
            X - X - X - X - X - . - .  
             \ / \ / \ / \ / \ / \ / \   
              X - . - . - . - X - . - . 
               \ / \ / \ / \ / \ / \ / \ 
                X - . - . - . - X - . - . 
                 \ / \ / \ / \ / \ / \ / \
                  X - . - . - X - . - . - . 
                   \ / \ / \ / \ / \ / \ / \
                    X - . - X - . - . - . - . 
                     \ / \ / \ / \ / \ / \ / \
                      X - . - X - X - X - X - X  
    Check that Player 1 wins when the path is complete, and doesn't until it isn't
    */

    int board3Width = 7;

    HexGame hexGame3{board3Width, board3Width};
    hexGame3.CurrentPlayer = HexCell::PLAYER1;

    for (int row = 0; row < board3Width; ++row) hexGame3.MarkCell(0, row, HexCell::PLAYER1);
    assert(hexGame3.IsGameWon() == false);
    for (int col = 1; col < 5; ++col) hexGame3.MarkCell(col, 1, HexCell::PLAYER1);
    assert(hexGame3.IsGameWon() == false);
    for (int row = 2; row < 4; ++row) hexGame3.MarkCell(4, row, HexCell::PLAYER1);
    assert(hexGame3.IsGameWon() == false);
    hexGame3.MarkCell(3, 4, HexCell::PLAYER1);
    hexGame3.MarkCell(2, 5, HexCell::PLAYER1);
    assert(hexGame3.IsGameWon() == false);
    for (int col = 2; col < board3Width; ++col) hexGame3.MarkCell(col, board3Width - 1, HexCell::PLAYER1);
    assert(hexGame3.IsGameWon() == true);

    /* 
    Player 1 marks a disconnected diagonal path:
            0 X - . - . - . 
                \ / \ / \ / \ 
               1 . - X - . - . 
                  \ / \ / \ / \  ...
                 2 . - . - X - . 
                    \ / \ / \ / \ 
                   3 . - . - . - X 
                        ... 
    Check that Player 1 does not win with this path
    */

    int board4Width = 7;

    HexGame hexGame4{board4Width, board4Width};
    hexGame4.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board4Width; ++col)
    {
        hexGame4.MarkCell(col, col, HexCell::PLAYER1);
        assert(hexGame4.IsGameWon() == false);
    }
}

void testGameWonByPlayer2()
{
    // Player 2 marks a vertical path
    // Check that Player 2 wins when the path is complete, and doesn't until it isn't

    int board1Width = 7; 
    int board1Height = 7;

    HexGame hexGame1{board1Width, board1Height};
    hexGame1.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board1Height - 1; ++row)
    {
        hexGame1.MarkCell(0, row, HexCell::PLAYER2);
        assert(hexGame1.IsGameWon() == false);
    }    

    hexGame1.MarkCell(0, board1Height - 1, HexCell::PLAYER2);
    assert(hexGame1.IsGameWon() == true);

    /* 
    Player 2 marks *connected* a diagonal path:
            0 O - O - . - . 
                \ / \ / \ / \ 
               1 . - O - O - . 
                  \ / \ / \ / \  ...
                 2 . - . - O - O 
                    \ / \ / \ / \ 
                   3 . - . - . - . 
                        ... 
    Check that Player 2 wins when the path is complete, and doesn't until it isn't
    */

    int board2Height = 7;

    HexGame hexGame2{board2Height, board2Height};
    hexGame2.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board2Height - 1; ++row)
    {
        hexGame2.MarkCell(row, row, HexCell::PLAYER2);
        hexGame2.MarkCell(row + 1, row, HexCell::PLAYER2);
        assert(hexGame2.IsGameWon() == false);
    }

    hexGame2.MarkCell(board2Height - 1, board2Height - 1, HexCell::PLAYER2);
    assert(hexGame2.IsGameWon() == true);

    /*
    Player 2 marks a complex path with branching and reversing:
           O - O - O - O - O - O - O 
            \ / \ / \ / \ / \ / \ / \ 
             . - O - . - . - . - . - . 
              \ / \ / \ / \ / \ / \ / \
               . - O - . - . - . - O - O 
                \ / \ / \ / \ / \ / \ / \
                 . - O - . - . - O - . - O 
                  \ / \ / \ / \ / \ / \ / \
                   . - O - O - O - . - . - O 
                    \ / \ / \ / \ / \ / \ / \
                     . - . - . - . - . - . - O 
                      \ / \ / \ / \ / \ / \ / \ 
                       . - . - . - . - . - . - O 
    Check that Player 2 wins when the path is complete, and doesn't until it isn't
    */

    int board3Height = 7;

    HexGame hexGame3{board3Height, board3Height};
    hexGame3.CurrentPlayer = HexCell::PLAYER2;

    for (int col = 0; col < board3Height; ++col) hexGame3.MarkCell(col, 0, HexCell::PLAYER2);
    assert(hexGame3.IsGameWon() == false);
    for (int row = 1; row < 5; ++row) hexGame3.MarkCell(1, row, HexCell::PLAYER2);
    assert(hexGame3.IsGameWon() == false);
    for (int col = 2; col < 4; ++col) hexGame3.MarkCell(col, 4, HexCell::PLAYER2);
    assert(hexGame3.IsGameWon() == false);
    hexGame3.MarkCell(4, 3, HexCell::PLAYER2);
    hexGame3.MarkCell(5, 2, HexCell::PLAYER2);
    assert(hexGame3.IsGameWon() == false);
    for (int row = 2; row < board3Height; ++row) hexGame3.MarkCell(board3Height - 1, row, HexCell::PLAYER2);
    assert(hexGame3.IsGameWon() == true);

    /* 
    Player 2 marks a disconnected diagonal path:
            0 O - . - . - . 
                \ / \ / \ / \ 
               1 . - O - . - . 
                  \ / \ / \ / \  ...
                 2 . - . - O - . 
                    \ / \ / \ / \ 
                   3 . - . - . - O 
                        ... 
    Check that Player 2 does not win with this path
    */

    int board4Height = 7;

    HexGame hexGame4{board4Height, board4Height};
    hexGame4.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board4Height; ++row)
    {
        hexGame4.MarkCell(row, row, HexCell::PLAYER2);
        assert(hexGame4.IsGameWon() == false);
    }
}

void testHexBoardCopyConstructor()
{
    // An empty hexGame (board1) is used to copy construct another hexGame (board2)
    // Player 1 marks all the cells of board1, which is then used to copy construct a new hexGame (board3)
    // Check that the state of board1 and hexGame 2 are not equal, and the state of board1 and board3 are equal

    int board1Width = 3;

    HexGame hexGame1{board1Width, board1Width};
    hexGame1.CurrentPlayer = HexCell::PLAYER1;
    
    HexGame hexGame2{hexGame1};

    for (int row = 0; row < board1Width; ++row) 
        for (int col = 0; col < board1Width; ++col) 
            hexGame1.MarkCell(col, row, HexCell::PLAYER1);

    HexGame hexGame3{hexGame1};

    for (int row = 0; row < board1Width; ++row)
    {
        for (int col = 0; col < board1Width; ++col)
        {
            assert(hexGame1.GetCell(col, row) != hexGame2.GetCell(col, row));
            assert(hexGame1.GetCell(col, row) == hexGame3.GetCell(col, row));
        }
    } 
}

void testGetRemainingMoves()
{
    int board1Width = 3;
    int board1Height = 5;

    HexGame hexGame{board1Width, board1Height};
    
    // Player 1 marks some cells i.e. plays some moves

    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
        hexGame.MarkCell(i, i, HexCell::PLAYER1); 

    AIPlayer aiPlayer{1000, hexGame};

    // Check that:
    // (1) all unoccupied cells are available to aiPlayer as a move, and no occupied cells are
    // (2) the encodings of the cells' (x, y) pairs as single integers are correct 

    for (int row = 0; row < hexGame.BoardHeight; ++row)
    {
        for (int col = 0; col < hexGame.BoardWidth; ++col)
        {
            auto move = std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * hexGame.BoardWidth + col);
            bool moveFound = move != aiPlayer.RemainingMoves.end();
            if (hexGame.GetCell(col, row) == HexCell::EMPTY) 
                assert(col == *move % hexGame.BoardWidth && row == *move / hexGame.BoardWidth);
            else 
                assert(move == aiPlayer.RemainingMoves.end());
        }
    }
}

void testRemovePlayedMove()
{
    int board1Width = 3;
    int board1Height = 5;

    HexGame board{board1Width, board1Height};
    AIPlayer aiPlayer{1000, board};

    // Check that each move made by Player 1 is removed from aiPlayer's list of available moves

    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
    {
        board.MarkCell(i, i, HexCell::PLAYER1);
        aiPlayer.RemovePlayedMove(std::pair<int, int>{i, i}, board.BoardWidth);
    }

    for (int row = 0; row < board.BoardHeight; ++row)
    {
        for (int col = 0; col < board.BoardWidth; ++col)
        {
            bool moveFound = 
                std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * board.BoardWidth + col) 
                != aiPlayer.RemainingMoves.end();
            assert((board.GetCell(col, row) == HexCell::EMPTY && moveFound) || (board.GetCell(col, row) != HexCell::EMPTY && !moveFound));
        }            
    }
}

void runTests()
{
    testCreateEmptyBoard();
    testMarkCell();  
    testGetCell();
    testGameWonByPlayer1();
    testGameWonByPlayer2();
    testHexBoardCopyConstructor();
    testGetRemainingMoves();
    testRemovePlayedMove();
}

int main()
{
    runTests();

    std::cout << "All tests passed" << std::endl;

    return 0;
}

#endif