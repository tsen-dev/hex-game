#include "sampler.h"
#include "aiplayer.h"
#include "move.h"

// Create a sampler. The sampler's RandomEngine is seeded with sum of the current CPU clock and the specified id
Sampler::Sampler(int id, HexBoard& moveBoard, std::vector<Move>& moves, char myPlayer) :
ID{id}, MyPlayer{myPlayer}, MoveBoard{moveBoard}, SampleBoard{moveBoard}, Moves{moves}, RandomEngine{clock() + id} {}

/* Play all remaining moves on the SampleBoard, alternating players at each move. moveIndices indexes into Moves, 
and is played in front order */
void Sampler::PlayRemainingMoves(std::vector<int>& moveIndices, char currentPlayer)
{
    for (int move = 0; move < moveIndices.size(); ++move)
    {
        SampleBoard.MarkCell(Moves[moveIndices[move]].X, Moves[moveIndices[move]].Y, currentPlayer);
        currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
    }          
}

/* Play all remaining moves on the SampleBoard, alternating players at each move. moveIndices indexes into Moves, 
and is played in reverse order */
void Sampler::PlayRemainingMovesReverse(std::vector<int>& moveIndices, char currentPlayer)
{
    for (int move = moveIndices.size() - 1; move >= 0; --move)
    {
        SampleBoard.MarkCell(Moves[moveIndices[move]].X, Moves[moveIndices[move]].Y, currentPlayer);
        currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
    }     
}

/* Simulate sampleCount many random games from MoveBoard's state and return the number of times MyPlayer won. 
Setting move to TRY_SWAP signals that (1) a swap should be simulated i.e. there is no move to be removed as one 
wasn't played, (2) the players of MoveBoard have been swapped */
int Sampler::SampleMove(int move, int sampleCount)
{    
    std::vector<int> moveIndices(Moves.size());
    for (int move = 0; move < Moves.size(); ++move) 
        moveIndices[move] = move;

    if (move != TRY_SWAP) // Remove the move to be sampled as it has been played, unless trying a swap
    {
        // Place the move to be removed at the back to make removal constant time
        std::swap(moveIndices[move], moveIndices[Moves.size() - 1]);                    
        moveIndices.pop_back();
    }

    char currentPlayer = (MyPlayer == MoveBoard.P1 ? MoveBoard.P2 : MoveBoard.P1);        
    bool reverseUsed = true; // Use every shuffle twice: once forward and once in reverse to reduce to number of shuffles
    int wins = 0;

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        CopyBoardState(SampleBoard, MoveBoard);          

        if (reverseUsed == true)
        {
            std::shuffle(moveIndices.begin(), moveIndices.end(), RandomEngine);                
            PlayRemainingMoves(moveIndices, currentPlayer);    
            reverseUsed = false;
        }

        else
        {
            PlayRemainingMovesReverse(moveIndices, currentPlayer);
            reverseUsed = true;
        }

        if (SampleBoard.HasPlayerWon(MyPlayer) == true)
            ++wins;
    }

    return wins;
}