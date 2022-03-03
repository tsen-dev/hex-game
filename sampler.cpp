#include "sampler.h"
#include "aiplayer.h"

Sampler::Sampler(int id, HexBoard& moveBoard, std::vector<std::pair<int, int>>& moves, char myPlayer) :
ID{id}, MyPlayer{myPlayer}, MoveBoard{moveBoard}, SampleBoard{moveBoard}, Moves{moves}, RandomEngine{time(nullptr) + id} {}

void Sampler::PlayRemainingMoves(std::vector<int>& moveIndices, char& currentPlayer)
{
    for (int move = 0; move < moveIndices.size(); ++move)
    {
        SampleBoard.MarkCell(Moves[moveIndices[move]].first, Moves[moveIndices[move]].second, currentPlayer);
        currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
    }          
}

void Sampler::PlayRemainingMovesReverse(std::vector<int>& moveIndices, char& currentPlayer)
{
    for (int move = moveIndices.size() - 1; move >= 0; --move)
    {
        SampleBoard.MarkCell(Moves[moveIndices[move]].first, Moves[moveIndices[move]].second, currentPlayer);
        currentPlayer = (currentPlayer == MoveBoard.P1) ? MoveBoard.P2 : MoveBoard.P1;
    }     
}

int Sampler::SampleMove(int move, int sampleCount)
{
    std::vector<int> moveIndices(Moves.size());
    for (int move = 0; move < Moves.size(); ++move) moveIndices[move] = move;

    if (move != AIPlayer::TRY_SWAP)
    {
        std::swap(moveIndices[move], moveIndices[Moves.size() - 1]);                    
        moveIndices.pop_back();
    }

    char startPlayer = (MyPlayer == MoveBoard.P1 ? MoveBoard.P2 : MoveBoard.P1);
    char currentPlayer;    
    bool reverseUsed = true;
    int wins = 0;

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        currentPlayer = startPlayer;        
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