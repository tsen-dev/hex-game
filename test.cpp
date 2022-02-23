#include <iostream>
#include <cassert>

#include "hexboard.h"
#include "aiplayer.h"
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

    // Suppress error output so "(x, y) is occupied" messages don't slow tests
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
    
    // Check that the correct value of retrieved for cell in the game's bounds

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
        assert(hexBoard1.IsGameWon(hexBoard1.P1) == false);
    }    

    hexBoard1.MarkCell(board1Width - 1, 0, hexBoard1.P1);
    assert(hexBoard1.IsGameWon(hexBoard1.P1) == true);    

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
        assert(hexBoard2.IsGameWon(hexBoard2.P1) == false);
    }

    hexBoard2.MarkCell(board2Width - 1, board2Width - 1, hexBoard2.P1);
    assert(hexBoard2.IsGameWon(hexBoard2.P1) == true);

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
    assert(hexBoard3.IsGameWon(hexBoard3.P1) == false);
    for (int col = 1; col < 5; ++col) hexBoard3.MarkCell(col, 1, hexBoard3.P1);
    assert(hexBoard3.IsGameWon(hexBoard3.P1) == false);
    for (int row = 2; row < 4; ++row) hexBoard3.MarkCell(4, row, hexBoard3.P1);
    assert(hexBoard3.IsGameWon(hexBoard3.P1) == false);
    hexBoard3.MarkCell(3, 4, hexBoard3.P1);
    hexBoard3.MarkCell(2, 5, hexBoard3.P1);
    assert(hexBoard3.IsGameWon(hexBoard3.P1) == false);
    for (int col = 2; col < board3Width; ++col) hexBoard3.MarkCell(col, board3Width - 1, hexBoard3.P1);
    assert(hexBoard3.IsGameWon(hexBoard3.P1) == true);

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
        assert(hexBoard4.IsGameWon(hexBoard4.P1) == false);
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
        assert(hexBoard1.IsGameWon(hexBoard1.P2) == false);
    }    

    hexBoard1.MarkCell(0, board1Height - 1, hexBoard1.P2);
    assert(hexBoard1.IsGameWon(hexBoard1.P2) == true);

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
        assert(hexBoard2.IsGameWon(hexBoard2.P2) == false);
    }

    hexBoard2.MarkCell(board2Height - 1, board2Height - 1, hexBoard2.P2);
    assert(hexBoard2.IsGameWon(hexBoard2.P2) == true);

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
    assert(hexBoard3.IsGameWon(hexBoard3.P2) == false);
    for (int row = 1; row < 5; ++row) hexBoard3.MarkCell(1, row, hexBoard3.P2);
    assert(hexBoard3.IsGameWon(hexBoard3.P2) == false);
    for (int col = 2; col < 4; ++col) hexBoard3.MarkCell(col, 4, hexBoard3.P2);
    assert(hexBoard3.IsGameWon(hexBoard3.P2) == false);
    hexBoard3.MarkCell(4, 3, hexBoard3.P2);
    hexBoard3.MarkCell(5, 2, hexBoard3.P2);
    assert(hexBoard3.IsGameWon(hexBoard3.P2) == false);
    for (int row = 2; row < board3Height; ++row) hexBoard3.MarkCell(board3Height - 1, row, hexBoard3.P2);
    assert(hexBoard3.IsGameWon(hexBoard3.P2) == true);

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
        assert(hexBoard4.IsGameWon(hexBoard4.P2) == false);
    }
}

void testCopyBoardState()
{
    // An empty hexBoard (board1) is copied into another hexBoard (board2)
    // Player 1 marks all the cells of board1, which is then copied into a new hexBoard (board3)
    // Check that (1) the state of board1 and hexBoard 2 are not equal, (2) the state of board1 and board3 are equal

    int board1Width = 3;

    HexBoard hexBoard1{board1Width, board1Width};
    HexBoard hexBoard2{board1Width, board1Width};

    // Check that copying boards of equal size returns no errors
    assert(CopyBoardState(hexBoard2, hexBoard1) == true);

    for (int row = 0; row < board1Width; ++row) 
        for (int col = 0; col < board1Width; ++col) 
            hexBoard1.MarkCell(col, row, hexBoard1.P1);

    HexBoard hexBoard3{board1Width, board1Width};

    // Check that copying boards of equal size returns no errors
    assert(CopyBoardState(hexBoard3, hexBoard1) == true);

    for (int row = 0; row < board1Width; ++row)
    {
        for (int col = 0; col < board1Width; ++col)
        {
            assert(hexBoard1.GetCell(col, row) != hexBoard2.GetCell(col, row));
            assert(hexBoard1.GetCell(col, row) == hexBoard3.GetCell(col, row));
        }
    }

    // Check that copying boards of unequal size returns an error

    HexBoard hexBoard4{board1Width + 1, board1Width};
    HexBoard hexBoard5{board1Width, board1Width + 1};
    HexBoard hexBoard6{board1Width + 1, board1Width + 1};

    assert(CopyBoardState(hexBoard3, hexBoard4) == false);
    assert(CopyBoardState(hexBoard3, hexBoard5) == false);
    assert(CopyBoardState(hexBoard3, hexBoard6) == false);
}

void testAIPlayerConstructor()
{
    int board1Width = 3;
    int board1Height = 5;

    HexBoard hexBoard{board1Width, board1Height};
    
    // Player 1 marks some cells i.e. plays some moves

    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
        hexBoard.MarkCell(i, i, hexBoard.P1); 

    AIPlayer aiPlayer{1000, hexBoard};

    // Check that:
    // (1) all unoccupied cells are available to aiPlayer as a move, and no occupied cells are
    // (2) the encodings of the cells' (x, y) pairs as single integers are correct 

    for (int row = 0; row < hexBoard.Height; ++row)
    {
        for (int col = 0; col < hexBoard.Width; ++col)
        {
            auto move = std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * hexBoard.Width + col);
            bool moveFound = move != aiPlayer.RemainingMoves.end();
            if (hexBoard.GetCell(col, row) == HexBoard::EMPTY) 
                assert(col == *move % hexBoard.Width && row == *move / hexBoard.Width);
            else 
                assert(move == aiPlayer.RemainingMoves.end());
        }
    }
}

void testRemoveMove()
{
    int board1Width = 3;
    int board1Height = 5;

    HexBoard hexBoard{board1Width, board1Height};
    AIPlayer aiPlayer{1000, hexBoard};

    // Check that:
    // (1) Each played move is removed from aiPlayer's list of available moves
    // (2) the state of aiPlayer's board is kept consistent with the main board


    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
    {
        hexBoard.MarkCell(i, i, hexBoard.P1);
        aiPlayer.RemoveMove(std::pair<int, int>{i, i}, hexBoard.P1);
    }

    hexBoard.MarkCell(board1Width - 1, board1Height - 1, hexBoard.P2);
    aiPlayer.RemoveMove(std::pair<int, int>{board1Width - 1, board1Height - 1}, hexBoard.P2);

    for (int row = 0; row < hexBoard.Height; ++row)
    {
        for (int col = 0; col < hexBoard.Width; ++col)
        {
            bool moveFound = 
                std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * hexBoard.Width + col) 
                != aiPlayer.RemainingMoves.end();
            assert((hexBoard.GetCell(col, row) == HexBoard::EMPTY && moveFound) || (hexBoard.GetCell(col, row) != HexBoard::EMPTY && !moveFound));
            assert(hexBoard.GetCell(col, row) == aiPlayer.Board.GetCell(col, row));
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
    testCopyBoardState();
    testAIPlayerConstructor();
    testRemoveMove();
}

int main()
{
    runTests();

    std::cout << "All tests passed" << std::endl;

    return 0;
}

#endif