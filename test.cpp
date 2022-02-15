#include <iostream>
#include <cassert>

#include "hexboard.h"
#include "hexcell.h"
#include "aiplayer.h"
#include "test.h"

#ifdef TEST

void testCreateEmptyBoard()
{
    int boardWidth = 3;
    int boardHeight = 5;    
    HexBoard hexBoard{boardWidth, boardHeight};
    
    for (int yCoordinate = 0; yCoordinate < boardHeight; ++yCoordinate)
    {
        for (int xCoordinate = 0; xCoordinate < boardWidth; ++xCoordinate)
        {
            assert(hexBoard.GetCell(xCoordinate, yCoordinate) == HexCell::EMPTY);
        }        
    }
}

void testMarkCell()
{
    int boardWidth = 5;
    int boardHeight = 3;   
    HexBoard hexBoard{boardWidth, boardHeight};

    HexCell mark1 = HexCell::PLAYER1;
    HexCell mark2 = HexCell::PLAYER2;

    auto cerrBuffer = std::cerr.rdbuf();
    std::cerr.rdbuf(nullptr);

    assert(hexBoard.MarkCell(0, 2, mark1) == true);
    assert(hexBoard.GetCell(0, 2) == mark1);
    assert(hexBoard.MarkCell(0, 2, mark1) == false);

    assert(hexBoard.MarkCell(2, 0, mark2) == true);
    assert(hexBoard.GetCell(2, 0) == mark2);
    assert(hexBoard.MarkCell(2, 0, mark2) == false);

    assert(hexBoard.MarkCell(boardWidth, boardHeight, mark1) == false);
    assert(hexBoard.MarkCell(-1, -1, mark1) == false);
    assert(hexBoard.MarkCell(0, boardHeight, mark1) == false);
    assert(hexBoard.MarkCell(boardWidth, 0, mark1) == false);

    std::cerr.rdbuf(cerrBuffer);
}

void testGetCell()
{
    int boardWidth = 5;
    int boardHeight = 3;

    HexBoard hexBoard{boardWidth, boardHeight};

    assert(hexBoard.GetCell(boardWidth, boardHeight) == HexCell::NO_CELL);
    assert(hexBoard.GetCell(-1, -1) == HexCell::NO_CELL);
    assert(hexBoard.GetCell(0, boardHeight) == HexCell::NO_CELL);
    assert(hexBoard.GetCell(boardWidth, 0) == HexCell::NO_CELL);
    
    assert(hexBoard.GetCell(boardWidth - 1, boardHeight - 1) == HexCell::EMPTY);
}

void testGameWonByPlayer1()
{
    int board1Width = 7; 
    int board1Height = 7;

    HexBoard hexBoard1{board1Width, board1Height};
    hexBoard1.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board1Width - 1; ++col)
    {
        hexBoard1.MarkCell(col, 0, HexCell::PLAYER1);
        assert(hexBoard1.IsGameWon() == false);
    }    

    hexBoard1.MarkCell(board1Width - 1, 0, HexCell::PLAYER1);
    assert(hexBoard1.IsGameWon() == true);    

    int board2Width = 7;

    HexBoard hexBoard2{board2Width, board2Width};
    hexBoard2.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board2Width - 1; ++col)
    {
        hexBoard2.MarkCell(col, col, HexCell::PLAYER1);
        hexBoard2.MarkCell(col, col + 1, HexCell::PLAYER1);
        assert(hexBoard2.IsGameWon() == false);
    }

    hexBoard2.MarkCell(board2Width - 1, board2Width - 1, HexCell::PLAYER1);
    assert(hexBoard2.IsGameWon() == true);

    int board3Width = 7;

    HexBoard hexBoard3{board3Width, board3Width};
    hexBoard3.CurrentPlayer = HexCell::PLAYER1;

    for (int row = 0; row < board3Width; ++row) hexBoard3.MarkCell(0, row, HexCell::PLAYER1);
    assert(hexBoard3.IsGameWon() == false);
    for (int col = 1; col < 5; ++col) hexBoard3.MarkCell(col, 1, HexCell::PLAYER1);
    assert(hexBoard3.IsGameWon() == false);
    for (int row = 2; row < 4; ++row) hexBoard3.MarkCell(4, row, HexCell::PLAYER1);
    assert(hexBoard3.IsGameWon() == false);
    hexBoard3.MarkCell(3, 4, HexCell::PLAYER1);
    hexBoard3.MarkCell(2, 5, HexCell::PLAYER1);
    assert(hexBoard3.IsGameWon() == false);
    for (int col = 2; col < board3Width; ++col) hexBoard3.MarkCell(col, board3Width - 1, HexCell::PLAYER1);
    assert(hexBoard3.IsGameWon() == true);

    int board4Width = 7;

    HexBoard hexBoard4{board4Width, board4Width};
    hexBoard4.CurrentPlayer = HexCell::PLAYER1;

    for (int col = 0; col < board4Width; ++col)
    {
        hexBoard4.MarkCell(col, col, HexCell::PLAYER1);
        assert(hexBoard4.IsGameWon() == false);
    }
}

void testGameWonByPlayer2()
{
    int board1Width = 7; 
    int board1Height = 7;

    HexBoard hexBoard1{board1Width, board1Height};
    hexBoard1.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board1Height - 1; ++row)
    {
        hexBoard1.MarkCell(0, row, HexCell::PLAYER2);
        assert(hexBoard1.IsGameWon() == false);
    }    

    hexBoard1.MarkCell(0, board1Height - 1, HexCell::PLAYER2);
    assert(hexBoard1.IsGameWon() == true);

    int board2Height = 7;

    HexBoard hexBoard2{board2Height, board2Height};
    hexBoard2.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board2Height - 1; ++row)
    {
        hexBoard2.MarkCell(row, row, HexCell::PLAYER2);
        hexBoard2.MarkCell(row + 1, row, HexCell::PLAYER2);
        assert(hexBoard2.IsGameWon() == false);
    }

    hexBoard2.MarkCell(board2Height - 1, board2Height - 1, HexCell::PLAYER2);
    assert(hexBoard2.IsGameWon() == true);

    int board3Height = 7;

    HexBoard hexBoard3{board3Height, board3Height};
    hexBoard3.CurrentPlayer = HexCell::PLAYER2;

    for (int col = 0; col < board3Height; ++col) hexBoard3.MarkCell(col, 0, HexCell::PLAYER2);
    assert(hexBoard3.IsGameWon() == false);
    for (int row = 1; row < 5; ++row) hexBoard3.MarkCell(1, row, HexCell::PLAYER2);
    assert(hexBoard3.IsGameWon() == false);
    for (int col = 2; col < 4; ++col) hexBoard3.MarkCell(col, 4, HexCell::PLAYER2);
    assert(hexBoard3.IsGameWon() == false);
    hexBoard3.MarkCell(4, 3, HexCell::PLAYER2);
    hexBoard3.MarkCell(5, 2, HexCell::PLAYER2);
    assert(hexBoard3.IsGameWon() == false);
    for (int row = 2; row < board3Height; ++row) hexBoard3.MarkCell(board3Height - 1, row, HexCell::PLAYER2);
    assert(hexBoard3.IsGameWon() == true);

    int board4Height = 7;

    HexBoard hexBoard4{board4Height, board4Height};
    hexBoard4.CurrentPlayer = HexCell::PLAYER2;

    for (int row = 0; row < board4Height; ++row)
    {
        hexBoard4.MarkCell(row, row, HexCell::PLAYER2);
        assert(hexBoard4.IsGameWon() == false);
    }
}

void testHexBoardCopyConstructor()
{
    int board1Width = 3;

    HexBoard hexBoard1{board1Width, board1Width};
    hexBoard1.CurrentPlayer = HexCell::PLAYER1;
    
    HexBoard hexBoard2{hexBoard1};

    for (int row = 0; row < board1Width; ++row) 
        for (int col = 0; col < board1Width; ++col) 
            hexBoard1.MarkCell(col, row, HexCell::PLAYER1);

    HexBoard hexBoard3{hexBoard1};

    for (int row = 0; row < board1Width; ++row)
    {
        for (int col = 0; col < board1Width; ++col)
        {
            assert(hexBoard1.GetCell(col, row) != hexBoard2.GetCell(col, row));
            assert(hexBoard1.GetCell(col, row) == hexBoard3.GetCell(col, row));
        }
    } 
}

void testGetRemainingMoves()
{
    int board1Width = 3;
    int board1Height = 5;

    HexBoard board{board1Width, board1Height};
    
    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
        board.MarkCell(i, i, HexCell::PLAYER1); 

    AIPlayer aiPlayer{1000, board};

    for (int row = 0; row < board.Height; ++row)
    {
        for (int col = 0; col < board.Width; ++col)
        {
            auto move = std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * board.Width + col);
            bool moveFound = move != aiPlayer.RemainingMoves.end();
            if (board.GetCell(col, row) == HexCell::EMPTY) assert(col == *move % board.Width && row == *move / board.Width);
            else assert(move == aiPlayer.RemainingMoves.end());
        }
    }
}

void testRemovePlayedMove()
{
    int board1Width = 3;
    int board1Height = 5;

    HexBoard board{board1Width, board1Height};
    AIPlayer aiPlayer{1000, board};

    for (int i = 0; i < std::min(board1Width, board1Height); ++i)
    {
        board.MarkCell(i, i, HexCell::PLAYER1);
        aiPlayer.RemovePlayedMove(std::pair<int, int>{i, i}, board.Width);
    }

    for (int row = 0; row < board.Height; ++row)
    {
        for (int col = 0; col < board.Width; ++col)
        {
            bool moveFound = 
                std::find(aiPlayer.RemainingMoves.begin(), aiPlayer.RemainingMoves.end(), row * board.Width + col) 
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