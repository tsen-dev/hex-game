#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(int sampleCount, HexBoard& board);
        std::pair<int, int> GetMove(HexBoard& board);
        void RemovePlayedMove(std::pair<int, int>& move, int boardWidth);
        std::vector<int> RemainingMoves;

    private:
        void GetRemainingMoves(HexBoard& board);

        int SampleCount;
        
};

#endif