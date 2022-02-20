#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(int sampleCount, HexGame& board);
        std::pair<int, int> GetMove(HexGame& board);
        void RemovePlayedMove(std::pair<int, int>& move, int boardWidth);
        std::vector<int> RemainingMoves;

    private:
        void GetRemainingMoves(HexGame& game);

        int SampleCount;
        
};

#endif