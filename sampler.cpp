#include "sampler.h"

Sampler::Sampler(int id, HexBoard& moveBoard, std::vector<std::pair<int, int>>& moves) :
ID{id}, MoveBoard{moveBoard}, SampleBoard{moveBoard}, Moves{moves}, RandomEngine{time(nullptr) + id} {}

int Sampler::SampleMove(int move, int sampleCount)
{
    std::vector<int> moveIndices(Moves.size());
    for (int move = 0; move < Moves.size(); ++move) moveIndices[move] = move;
    std::swap(moveIndices[move], moveIndices[Moves.size() - 1]);                    
    moveIndices.pop_back();

    char currentPlayer;
    bool reverseUsed = true;
    int wins = 0;

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        currentPlayer = MoveBoard.P1;        
        CopyBoardState(SampleBoard, MoveBoard);  

        if (reverseUsed == true)
        {
            std::shuffle(moveIndices.begin(), moveIndices.end(), RandomEngine);              
            for (int move = 0; move < moveIndices.size(); ++move)
            {
                SampleBoard.MarkCell(Moves[moveIndices[move]].first, Moves[moveIndices[move]].second, currentPlayer);
                currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
            }            
            reverseUsed = false;
        }

        else
        {
            for (int move = moveIndices.size() - 1; move >= 0; --move)
            {
                SampleBoard.MarkCell(Moves[moveIndices[move]].first, Moves[moveIndices[move]].second, currentPlayer);
                currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
            }            
            reverseUsed = true;
        }

        if (SampleBoard.HasPlayerWon(SampleBoard.P2) == true)
            ++wins;
    }

    return wins;
}