#include <random>
#include <numeric>
#include <thread>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(HexBoard& hexBoard, int sampleCount, int samplerCount) : 
    SampleCount{sampleCount}, SamplerCount{samplerCount}, ShuffledRemainingMoves{samplerCount},
    Board{hexBoard}, MoveBoard{hexBoard}, SampleBoards{samplerCount, HexBoard{hexBoard}}, RandomEngines{SamplerCount}
{
    RemainingMoves.reserve(Board.Width * Board.Height);

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

void AIPlayer::TryMove(int thread, int moveIndex, std::vector<int>& winCounts)
{
    ShuffledRemainingMoves[thread] = RemainingMoves;
    std::swap(ShuffledRemainingMoves[thread][moveIndex], ShuffledRemainingMoves[thread][ShuffledRemainingMoves[thread].size() - 1]);                    
    ShuffledRemainingMoves[thread].pop_back();
    int samples = (SampleCount / SamplerCount) + (thread == SamplerCount - 1 ? SampleCount % SamplerCount : 0); 
    char currentPlayer;
    RandomEngines[thread].seed(time(nullptr) + thread);

    for (int sample = 0; sample < samples; ++sample)
    {
        currentPlayer = Board.P1;
        CopyBoardState(SampleBoards[thread], MoveBoard);  
        std::shuffle(ShuffledRemainingMoves[thread].begin(), ShuffledRemainingMoves[thread].end(), RandomEngines[thread]);  
        for (int moveIndex = 0; moveIndex < ShuffledRemainingMoves[thread].size(); ++moveIndex)
        {
            SampleBoards[thread].MarkCell(ShuffledRemainingMoves[thread][moveIndex] % Board.Width, ShuffledRemainingMoves[thread][moveIndex] / Board.Width, currentPlayer);
            currentPlayer = (currentPlayer == Board.P1) ? Board.P2 : Board.P1;
        }
        if (SampleBoards[thread].HasPlayerWon(SampleBoards[thread].P2) == true)
            ++winCounts[thread];
    }
}

int AIPlayer::SampleMove(int moveIndex)
{
    std::vector<int> winCounts(SamplerCount, 0);        
    char currentPlayer = Board.P2;
    std::vector<std::thread> threads;

    CopyBoardState(MoveBoard, Board);
    MoveBoard.MarkCell(RemainingMoves[moveIndex] % Board.Width, RemainingMoves[moveIndex] / Board.Width, currentPlayer);

    for (int thread = 0; thread < SamplerCount; ++thread)
        threads.push_back(std::thread{&AIPlayer::TryMove, this, thread, moveIndex, std::ref(winCounts)});

    for (int thread = 0; thread < SamplerCount; ++thread)
        threads[thread].join();

    return accumulate(winCounts.begin(), winCounts.end(), 0);
}

std::pair<int, int> AIPlayer::GetMove()
{
    clock_t time = clock();
    std::pair<int, int> bestMove;   
    char currentPlayer = Board.P2;
    int maxWins = 0;    
    int moveWinCount;

    for (int moveIndex = 0; moveIndex < RemainingMoves.size(); ++moveIndex)
    {       
        if ((moveWinCount = SampleMove(moveIndex)) > maxWins) 
        {
            maxWins = moveWinCount;
            bestMove.first = RemainingMoves[moveIndex] % MoveBoard.Width;
            bestMove.second = RemainingMoves[moveIndex] / MoveBoard.Width;
        }        
    }

    std::cout << (double) (clock() - time) / CLOCKS_PER_SEC;

    return bestMove;
}