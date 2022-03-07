#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"
#include "sampler.h"

class AIPlayer
{
    public:
        AIPlayer(HexBoard& hexBoard, int sampleCount = 1000, int samplerCount = 8);
        std::pair<int, int> GetMove(bool firstMove);
        int SampleSwap();
        void RemoveMove(std::pair<int, int>& move);        
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

        static const std::pair<int, int> SWAP;        
        static const int TRY_SWAP = Sampler::TRY_SWAP;

    private:
        int SampleMove(int move);

        int SampleCount;        
        char MyPlayer;
        std::vector<std::pair<int, int>> Moves;
        HexBoard& Board;
        HexBoard MoveBoard;
        std::vector<Sampler> Samplers;                
};

#endif