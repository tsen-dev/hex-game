#include <random>
#include <numeric>
#include <future>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(HexBoard& hexBoard, int sampleCount, int samplerCount) : 
    SampleCount{sampleCount}, Board{hexBoard}, MoveBoard{hexBoard}
{
    for (int sampler = 0; sampler < samplerCount; ++sampler)
        Samplers.push_back(Sampler{sampler, MoveBoard, Moves});

    for (int row = 0; row < hexBoard.Height; ++row)
        for (int col = 0; col < hexBoard.Width; ++col)
            if (hexBoard.GetCell(col, row) == HexBoard::EMPTY)
                Moves.push_back(row * hexBoard.Width + col); 
}

void AIPlayer::RemoveMove(std::pair<int, int>& move, char player)
{
    Board.MarkCell(move.first, move.second, player);
    Moves.erase(std::find(Moves.begin(), Moves.end(), move.second * Board.Width + move.first));
}

int AIPlayer::SampleMove(int move)
{
    std::vector<std::future<int>> winCounts;            
    int totalSamples = SampleCount;
    int samplerSamples;

    CopyBoardState(MoveBoard, Board);
    MoveBoard.MarkCell(Moves[move] % Board.Width, Moves[move] / Board.Width, Board.P2);

    for (int sampler = 0; sampler < Samplers.size(); ++sampler)
    {
        samplerSamples = totalSamples / (Samplers.size() - sampler);
        winCounts.push_back(std::async(
            std::launch::async, &Sampler::SampleMove, &Samplers[sampler], move, samplerSamples));
        totalSamples -= samplerSamples;
    }
        
    int wins = 0;

    for (int sampler = 0; sampler < Samplers.size(); ++sampler)
    {
        wins += winCounts.back().get();
        winCounts.pop_back();
    }
        
    return wins;
}

std::pair<int, int> AIPlayer::GetMove()
{
    clock_t time = clock();
    std::pair<int, int> bestMove = std::pair<int, int>{Moves.front() % MoveBoard.Width, Moves.front() / MoveBoard.Width};   
    int maxWins = 0;    
    int moveWins;

    for (int move = 0; move < Moves.size(); ++move)
    {       
        if ((moveWins = SampleMove(move)) > maxWins) 
        {
            maxWins = moveWins;
            bestMove.first = Moves[move] % MoveBoard.Width;
            bestMove.second = Moves[move] / MoveBoard.Width;
        }        
    }

    std::cout << (double) (clock() - time) / CLOCKS_PER_SEC;

    return bestMove;
}