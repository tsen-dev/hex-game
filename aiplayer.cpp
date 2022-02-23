#include <random>
#include <array>
#include <algorithm>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(int sampleCount, HexBoard& hexBoard) 
    : SampleCount{sampleCount}, RandomEngine{time(nullptr)}, 
    Board{hexBoard.Width, hexBoard.Height, hexBoard.P1, hexBoard.P2}, 
    MoveBoard{hexBoard.Width, hexBoard.Height, hexBoard.P1, hexBoard.P2}, 
    SampleBoard{hexBoard.Width, hexBoard.Height, hexBoard.P1, hexBoard.P2}
{
    for (int row = 0; row < hexBoard.Height; ++row)
        for (int col = 0; col < hexBoard.Width; ++col)
            if (hexBoard.GetCell(col, row) == HexBoard::EMPTY)
                RemainingMoves.push_back(row * hexBoard.Width + col); 
}

void AIPlayer::RemoveMove(std::pair<int, int>& move, char player)
{
    Board.MarkCell(move.first, move.second, player);
    RemainingMoves.erase(std::find(RemainingMoves.begin(), RemainingMoves.end(), move.second * Board.Width + move.first));
}

int AIPlayer::SampleMove(int moveIndex)
{
    int winCount = 0;
    char currentPlayer = Board.P2;

    CopyBoardState(MoveBoard, Board);
    ShuffledRemainingMoves = RemainingMoves;

    std::swap(ShuffledRemainingMoves[moveIndex], ShuffledRemainingMoves[ShuffledRemainingMoves.size() - 1]);                    
    MoveBoard.MarkCell(ShuffledRemainingMoves.back() % Board.Width, ShuffledRemainingMoves.back() / Board.Width, currentPlayer);
    ShuffledRemainingMoves.pop_back();

    for (int sample = 0; sample < SampleCount; ++sample)
    {
        currentPlayer = Board.P1;
        CopyBoardState(SampleBoard, MoveBoard);  
        std::shuffle(ShuffledRemainingMoves.begin(), ShuffledRemainingMoves.end(), RandomEngine);  

        for (int moveIndex = 0; moveIndex < ShuffledRemainingMoves.size(); ++moveIndex)
        {
            SampleBoard.MarkCell(ShuffledRemainingMoves[moveIndex] % Board.Width, ShuffledRemainingMoves[moveIndex] / Board.Width, currentPlayer);
            currentPlayer = (currentPlayer == Board.P1) ? Board.P2 : Board.P1;
        }

        if (SampleBoard.HasPlayerWon(SampleBoard.P2) == true)
            ++winCount;
    }

    return winCount;
}

std::pair<int, int> AIPlayer::GetMove()
{
    clock_t time = clock();
    std::pair<int, int> move;   
    char currentPlayer = Board.P2;
    int maxWins = 0;    
    int moveWinCount;

    for (int moveIndex = 0; moveIndex < RemainingMoves.size(); ++moveIndex)
    {       
        if ((moveWinCount = SampleMove(moveIndex)) > maxWins) 
        {
            maxWins = moveWinCount;
            move.first = RemainingMoves[moveIndex] % MoveBoard.Width;
            move.second = RemainingMoves[moveIndex] / MoveBoard.Width;
        }        
    }

    std::cout << (double) (clock() - time) / CLOCKS_PER_SEC;

    return move;
}