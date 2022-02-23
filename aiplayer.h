#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(int sampleCount, HexBoard& hexBoard);
        std::pair<int, int> GetMove();
        void RemoveMove(std::pair<int, int>& move, char player);        
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

    private:
        int SampleMove(int moveIndex);

        int SampleCount;
        std::vector<int> RemainingMoves;        
        std::vector<int> ShuffledRemainingMoves;        
        HexBoard Board, MoveBoard, SampleBoard;
        std::default_random_engine RandomEngine;
};

#endif