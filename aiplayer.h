#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(int sampleCount);
        std::pair<int, int> GetMove(HexBoard& board);
    private:
        int SampleCount;
};

#endif