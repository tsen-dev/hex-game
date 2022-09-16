#include <random>
#include <numeric>
#include <future>

#include "aiplayer.h"
#include "hexboard.h"
#include "move.h"

const Move AIPlayer::SWAP = {-1, -1}; // Initialised here as this type can't have an in-class initialiser

AIPlayer::AIPlayer(int sampleCount, int samplerCount) : 
    SampleCount{sampleCount}, Board{0, 0}, MoveBoard{0, 0}, ThreadPool{samplerCount}
{
    for (int sampler = 0; sampler < samplerCount; ++sampler)
        Samplers.push_back(Sampler{sampler, MyPlayer, MoveBoard, Moves});    
}

// Clear Moves and place all availables moves on the specified board into Moves
void AIPlayer::GetAvailableMoves(const HexBoard& hexBoard)
{
    Moves.clear();

    for (int row = 0; row < hexBoard.Height; ++row)
        for (int col = 0; col < hexBoard.Width; ++col)
            if (hexBoard.GetCell(col, row) == HexBoard::EMPTY) 
                Moves.push_back({col, row}); 
}

/* If move is TRY_SWAP, swap MoveBoard's players, otherwise play the move to be sampled on MoveBoard. Then simulate 
SampleCount many random games from MoveBoard's state and return the number of wins. move is an index into Moves */
int AIPlayer::SampleMove(int move)
{
    std::vector<std::future<int>> winCounts;            
    int totalSamples = SampleCount;
    int samplerSamples;

    MoveBoard = Board;
    
    if (move == TRY_SWAP) MoveBoard.SwapPlayers();
    else MoveBoard.MarkCell(Moves[move].X, Moves[move].Y, MyPlayer);

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

/* Simulate SampleCount many random games from hexBoard for each of the remaining moves. If firstMove is true, also try swapping the players 
instead of playing a move. Return the move resulting in the most number of wins, or SWAP if swapping was more successful */      
Move AIPlayer::GetMove(const HexBoard& hexBoard, char myPlayer, bool firstMove)
{    
    Board = hexBoard;
    GetAvailableMoves(Board);
    MyPlayer = myPlayer;    

    Move bestMove = Moves.front();
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