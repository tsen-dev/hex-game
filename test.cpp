#include <iostream>
#include <cassert>

#include "hexboard.h"
#include "aiplayer.h"
#include "move.h"
#include "test.h"

#ifdef TEST

void testCreateEmptyBoard()
{
    // Create a 3x5 game and check that all cells are initialised to HexBoard::EMPTY
    
    int boardWidth = 3;
    int boardHeight = 5;    
    HexBoard hexBoard{boardWidth, boardHeight};
    
    for (int yCoordinate = 0; yCoordinate < boardHeight; ++yCoordinate)
    {
        for (int xCoordinate = 0; xCoordinate < boardWidth; ++xCoordinate)
        {
            assert(hexBoard.GetCell(xCoordinate, yCoordinate) == HexBoard::EMPTY);
        }        
    }
}

void testMarkCell()
{
    int boardWidth = 5;
    int boardHeight = 3;   
    HexBoard hexBoard{boardWidth, boardHeight};

    // Suppress error output so "(x, y) is occupied" messages don't clutter test output
    auto cerrBuffer = std::cerr.rdbuf();
    std::cerr.rdbuf(nullptr);

    // Check that a cell with the game's bounds can be marked
    // Check that the correct value is retrieved from that cell
    // Check that the same cell can't be marked again

    assert(hexBoard.MarkCell(0, 2, hexBoard.P1) == true);
    assert(hexBoard.GetCell(0, 2) == hexBoard.P1);
    assert(hexBoard.MarkCell(0, 2, hexBoard.P1) == false);

    assert(hexBoard.MarkCell(2, 0, hexBoard.P2) == true);
    assert(hexBoard.GetCell(2, 0) == hexBoard.P2);
    assert(hexBoard.MarkCell(2, 0, hexBoard.P2) == false);

    // Check that cells out of the game's bounds can't be marked

    assert(hexBoard.MarkCell(boardWidth, boardHeight, hexBoard.P1) == false);
    assert(hexBoard.MarkCell(-1, -1, hexBoard.P1) == false);
    assert(hexBoard.MarkCell(0, boardHeight, hexBoard.P1) == false);
    assert(hexBoard.MarkCell(boardWidth, 0, hexBoard.P1) == false);

    // Unsuppress error output
    std::cerr.rdbuf(cerrBuffer);
}

void testGetCell()
{
    int boardWidth = 5;
    int boardHeight = 3;

    HexBoard hexBoard{boardWidth, boardHeight};

    // Check that attempts to retrieve values of out of bounds cells are detected

    assert(hexBoard.GetCell(boardWidth, boardHeight) == HexBoard::OUT_OF_BOUNDS);
    assert(hexBoard.GetCell(-1, -1) == HexBoard::OUT_OF_BOUNDS);
    assert(hexBoard.GetCell(0, boardHeight) == HexBoard::OUT_OF_BOUNDS);
    assert(hexBoard.GetCell(boardWidth, 0) == HexBoard::OUT_OF_BOUNDS);
    
    // Check that the correct value is retrieved for a cell in the game's bounds

    assert(hexBoard.GetCell(boardWidth - 1, boardHeight - 1) == HexBoard::EMPTY);
}

void testGameWonByPlayer1()
{
    // Player 1 marks a horizontal path
    // Check that Player 1 wins when the path is complete, and doesn't until it isn't

    int board1Width = 7; 
    int board1Height = 7;

    HexBoard hexBoard1{board1Width, board1Height};

    for (int col = 0; col < board1Width - 1; ++col)
    {
        hexBoard1.MarkCell(col, 0, hexBoard1.P1);
        assert(hexBoard1.HasPlayerWon(hexBoard1.P1) == false);
    }    

    hexBoard1.MarkCell(board1Width - 1, 0, hexBoard1.P1);
    assert(hexBoard1.HasPlayerWon(hexBoard1.P1) == true);    

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

    HexBoard hexBoard2{board2Width, board2Width};

    for (int col = 0; col < board2Width - 1; ++col)
    {
        hexBoard2.MarkCell(col, col, hexBoard2.P1);
        hexBoard2.MarkCell(col, col + 1, hexBoard2.P1);
        assert(hexBoard2.HasPlayerWon(hexBoard2.P1) == false);
    }

    hexBoard2.MarkCell(board2Width - 1, board2Width - 1, hexBoard2.P1);
    assert(hexBoard2.HasPlayerWon(hexBoard2.P1) == true);

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

    HexBoard hexBoard3{board3Width, board3Width};

    for (int row = 0; row < board3Width; ++row) hexBoard3.MarkCell(0, row, hexBoard3.P1);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P1) == false);
    for (int col = 1; col < 5; ++col) hexBoard3.MarkCell(col, 1, hexBoard3.P1);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P1) == false);
    for (int row = 2; row < 4; ++row) hexBoard3.MarkCell(4, row, hexBoard3.P1);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P1) == false);
    hexBoard3.MarkCell(3, 4, hexBoard3.P1);
    hexBoard3.MarkCell(2, 5, hexBoard3.P1);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P1) == false);
    for (int col = 2; col < board3Width; ++col) hexBoard3.MarkCell(col, board3Width - 1, hexBoard3.P1);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P1) == true);

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

    HexBoard hexBoard4{board4Width, board4Width};

    for (int col = 0; col < board4Width; ++col)
    {
        hexBoard4.MarkCell(col, col, hexBoard4.P1);
        assert(hexBoard4.HasPlayerWon(hexBoard4.P1) == false);
    }
}

void testGameWonByPlayer2()
{
    // Player 2 marks a vertical path
    // Check that Player 2 wins when the path is complete, and doesn't until it isn't

    int board1Width = 7; 
    int board1Height = 7;

    HexBoard hexBoard1{board1Width, board1Height};

    for (int row = 0; row < board1Height - 1; ++row)
    {
        hexBoard1.MarkCell(0, row, hexBoard1.P2);
        assert(hexBoard1.HasPlayerWon(hexBoard1.P2) == false);
    }    

    hexBoard1.MarkCell(0, board1Height - 1, hexBoard1.P2);
    assert(hexBoard1.HasPlayerWon(hexBoard1.P2) == true);

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

    HexBoard hexBoard2{board2Height, board2Height};

    for (int row = 0; row < board2Height - 1; ++row)
    {
        hexBoard2.MarkCell(row, row, hexBoard2.P2);
        hexBoard2.MarkCell(row + 1, row, hexBoard2.P2);
        assert(hexBoard2.HasPlayerWon(hexBoard2.P2) == false);
    }

    hexBoard2.MarkCell(board2Height - 1, board2Height - 1, hexBoard2.P2);
    assert(hexBoard2.HasPlayerWon(hexBoard2.P2) == true);

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

    HexBoard hexBoard3{board3Height, board3Height};

    for (int col = 0; col < board3Height; ++col) hexBoard3.MarkCell(col, 0, hexBoard3.P2);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P2) == false);
    for (int row = 1; row < 5; ++row) hexBoard3.MarkCell(1, row, hexBoard3.P2);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P2) == false);
    for (int col = 2; col < 4; ++col) hexBoard3.MarkCell(col, 4, hexBoard3.P2);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P2) == false);
    hexBoard3.MarkCell(4, 3, hexBoard3.P2);
    hexBoard3.MarkCell(5, 2, hexBoard3.P2);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P2) == false);
    for (int row = 2; row < board3Height; ++row) hexBoard3.MarkCell(board3Height - 1, row, hexBoard3.P2);
    assert(hexBoard3.HasPlayerWon(hexBoard3.P2) == true);

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

    HexBoard hexBoard4{board4Height, board4Height};

    for (int row = 0; row < board4Height; ++row)
    {
        hexBoard4.MarkCell(row, row, hexBoard4.P2);
        assert(hexBoard4.HasPlayerWon(hexBoard4.P2) == false);
    }
}

void testAIPlayerConstructor()
{
    int board1Width = 3;
    int board1Height = 5;

    HexBoard hexBoard{board1Width, board1Height};
    
    // Player 1 marks some cells i.e. plays some moves

    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
        hexBoard.MarkCell(i, i, hexBoard.P1); 

    AIPlayer aiPlayer{hexBoard};

    // Check that all unoccupied cells are available to aiPlayer as a move, and no occupied cells are

    for (int row = 0; row < hexBoard.Height; ++row)
    {
        for (int col = 0; col < hexBoard.Width; ++col)
        {            
            auto move = std::find_if(aiPlayer.Moves.begin(), aiPlayer.Moves.end(), 
                [col, row](const Move& move){return move.X == col && move.Y == row;}); 
            bool moveFound = move != aiPlayer.Moves.end();
            assert((hexBoard.GetCell(col, row) == HexBoard::EMPTY && moveFound) || (hexBoard.GetCell(col, row) != HexBoard::EMPTY && !moveFound));
        }
    }
}

void testAIPlayerGetMove()
{
    /*
    Check that the AIPlayer chooses to finish off a left-side downward path
           O - . - . - . - . - . - . 
            \ / \ / \ / \ / \ / \ / \ 
             O - . - . - . - . - . - . 
              \ / \ / \ / \ / \ / \ / \
               O - . - . - . - . - . - . 
                \ / \ / \ / \ / \ / \ / \
                 O - . - . - . - . - . - .
                  \ / \ / \ / \ / \ / \ / \
                   O - . - . - . - . - . - . 
                    \ / \ / \ / \ / \ / \ / \
                     O - . - . - . - . - . - . 
                      \ / \ / \ / \ / \ / \ / \ 
                       . - . - . - . - . - . - . 
    */

    int board1Width = 11;
    int board1Height = 11;

    HexBoard hexBoard{board1Width, board1Height};
    AIPlayer aiPlayer{hexBoard};

    for (int i = 0; i < board1Height - 1; ++i)
        hexBoard.MarkCell(0, i, hexBoard.P2);
    
    Move move = aiPlayer.GetMove(hexBoard, false);
    hexBoard.MarkCell(move.X, move.Y, hexBoard.P2);

    assert(hexBoard.HasPlayerWon(hexBoard.P2));

    /*
    Check that the AIPlayer chooses to finish off a right-side upward path
           . - . - . - . - . - . - . 
            \ / \ / \ / \ / \ / \ / \ 
             . - . - . - . - . - . - O 
              \ / \ / \ / \ / \ / \ / \
               . - . - . - . - . - . - O 
                \ / \ / \ / \ / \ / \ / \
                 . - . - . - . - . - . - O
                  \ / \ / \ / \ / \ / \ / \
                   . - . - . - . - . - . - O 
                    \ / \ / \ / \ / \ / \ / \
                     . - . - . - . - . - . - O 
                      \ / \ / \ / \ / \ / \ / \ 
                       . - . - . - . - . - . - O 
    */

    int board2Width = 11;
    int board2Height = 11;

    HexBoard hexBoard2{board2Width, board2Height};
    AIPlayer aiPlayer2{hexBoard2};

    for (int i = board2Height - 1; i > 0; --i)
        hexBoard2.MarkCell(board2Width - 1, i, hexBoard2.P2);
    
    move = aiPlayer2.GetMove(hexBoard2, false);
    hexBoard2.MarkCell(move.X, move.Y, hexBoard2.P2);

    assert(hexBoard2.HasPlayerWon(hexBoard2.P2));

    // Add swap test case
}

void runTests()
{
    testCreateEmptyBoard();
    testMarkCell();  
    testGetCell();
    testGameWonByPlayer1();
    testGameWonByPlayer2();
    testAIPlayerGetMove();
}

int main()
{
    runTests();

    std::cout << "All tests passed" << std::endl;

    return 0;
}

#endif