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
                Moves.push_back({col, row}); 
}

void AIPlayer::RemoveMove(std::pair<int, int>& move, char player)
{
    Board.MarkCell(move.first, move.second, player);

    Moves.erase(
        std::find_if(Moves.begin(), Moves.end(), [move](const std::pair<int, int>& currentMove){
            return move.first == currentMove.first && move.second == currentMove.second;})
    );
}

int AIPlayer::SampleMove(int move)
{
    std::vector<std::future<int>> winCounts;            
    int totalSamples = SampleCount;
    int samplerSamples;

    CopyBoardState(MoveBoard, Board);
    MoveBoard.MarkCell(Moves[move].first, Moves[move].second, Board.P2);

    // Remove move from remaining moves here instead of doing it in each Sampler's SampleMove
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
    std::pair<int, int> bestMove = Moves.front();
    int maxWins = 0;    
    int moveWins;

    for (int move = 0; move < Moves.size(); ++move)
    {       
        if ((moveWins = SampleMove(move)) > maxWins) 
        {
            maxWins = moveWins;
            bestMove = Moves[move];
        }        
    }

    std::cout << (double) (clock() - time) / CLOCKS_PER_SEC;

    return bestMove;
}