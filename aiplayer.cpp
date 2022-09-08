#include <random>
#include <numeric>
#include <future>

#include "aiplayer.h"
#include "hexboard.h"

const std::pair<int, int> AIPlayer::SWAP = {-1, -1}; // Initialised here as this type can't have an in-class initialiser

// Creates an AIPlayer and writes all of hexBoard's unplayed moves into Moves
AIPlayer::AIPlayer(HexBoard& hexBoard, int sampleCount, int samplerCount) : 
    SampleCount{sampleCount}, MyPlayer{hexBoard.P2}, Board{hexBoard}, MoveBoard{hexBoard}, ThreadPool{samplerCount}
{
    for (int sampler = 0; sampler < samplerCount; ++sampler)
        Samplers.push_back(Sampler{sampler, MoveBoard, Moves, hexBoard.P2});

    for (int row = 0; row < hexBoard.Height; ++row)
        for (int col = 0; col < hexBoard.Width; ++col)
            if (hexBoard.GetCell(col, row) == HexBoard::EMPTY) 
                Moves.push_back({col, row}); 
}

// Remove the specified move from Moves, if it exists
void AIPlayer::RemoveMove(const std::pair<int, int>& move)
{    
    auto moveItr = 
        std::find_if(Moves.begin(), Moves.end(), [move](const std::pair<int, int>& currentMove){
            return move.first == currentMove.first && move.second == currentMove.second;});

    if (moveItr != Moves.end()) 
        Moves.erase(moveItr);
}

/* If move is TRY_SWAP, swap MoveBoard's players, otherwise play the move to be sampled on MoveBoard. Then simulate 
SampleCount many random games from MoveBoard's state and return the number of wins. move is an index into Moves */
int AIPlayer::SampleMove(int move)
{
    std::vector<std::future<int>> winCounts;            
    int totalSamples = SampleCount;
    int samplerSamples;

    CopyBoardState(MoveBoard, Board);
    
    if (move == TRY_SWAP) MoveBoard.SwapPlayers();
    else MoveBoard.MarkCell(Moves[move].first, Moves[move].second, MyPlayer);

    for (int sampler = 0; sampler < Samplers.size(); ++sampler)
    {
        samplerSamples = totalSamples / (Samplers.size() - sampler);
        winCounts.push_back(ThreadPool.EnqueueWork(&Sampler::SampleMove, &Samplers[sampler], move, samplerSamples));
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

/* Simulate SampleCount many random games for each of the remaining moves. If firstMove is true, also try swapping 
the players instead of playing a move. Return the move resulting in the most number of wins, or SWAP if swapping was 
more successful */
std::pair<int, int> AIPlayer::GetMove(bool firstMove)
{
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

    if (firstMove && SampleMove(AIPlayer::TRY_SWAP) > maxWins) return SWAP;
    else return bestMove; 
}