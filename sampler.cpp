#include "sampler.h"

Sampler::Sampler(int id, HexBoard& moveBoard, std::vector<int>& moves) :
ID{id}, MoveBoard{moveBoard}, SampleBoard{moveBoard}, ShuffledMoves{moves}, Moves{moves}, RandomEngine{time(nullptr) + id} {}

int Sampler::SampleMove(int move, int sampleCount)
{
    ShuffledMoves = Moves;
    std::swap(ShuffledMoves[move], ShuffledMoves[Moves.size() - 1]);                    
    ShuffledMoves.pop_back();

    char currentPlayer;
    int wins = 0;

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        currentPlayer = MoveBoard.P1;
        
        CopyBoardState(SampleBoard, MoveBoard);  
        std::shuffle(ShuffledMoves.begin(), ShuffledMoves.end(), RandomEngine);  

        for (int move = 0; move < ShuffledMoves.size(); ++move)
        {
            SampleBoard.MarkCell(ShuffledMoves[move] % MoveBoard.Width, ShuffledMoves[move] / MoveBoard.Width, currentPlayer);
            currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
        }

        if (SampleBoard.HasPlayerWon(SampleBoard.P2) == true)
            ++wins;
    }

    return wins;
}