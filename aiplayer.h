#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(HexBoard& hexBoard, int sampleCount = 1000, int samplerCount = 8);
        std::pair<int, int> GetMove();
        void RemoveMove(std::pair<int, int>& move, char player);        
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

    private:
        int SampleMove(int moveIndex);
        void TryMove(int thread, int moveIndex, std::vector<int>& winCounts);

        int SampleCount;
        int SamplerCount;
        std::vector<int> RemainingMoves;        
        std::vector<std::vector<int>> ShuffledRemainingMoves;        
        HexBoard Board, MoveBoard;
        std::vector<HexBoard> SampleBoards;
        std::vector<std::default_random_engine> RandomEngines;
};

#endif