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
        std::pair<int, int> GetMove();
        void RemoveMove(std::pair<int, int>& move, char player);        
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

    private:
        int SampleMove(int move);

        int SampleCount;        
        std::vector<std::pair<int, int>> Moves;
        HexBoard Board, MoveBoard;
        std::vector<Sampler> Samplers;
};

#endif