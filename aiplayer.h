#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"

class AIPlayer
{
    public:
        AIPlayer(int sampleCount, HexBoard& hexBoard, int threads = 8);
        std::pair<int, int> GetMove();
        void RemoveMove(std::pair<int, int>& move, char player);        
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

    private:
        int SampleMove(int moveIndex);
        void TryMove(int thread, int moveIndex, std::vector<int>& winCounts);

        int SampleCount;
        static const int THREAD_COUNT = 8;
        std::vector<int> RemainingMoves;        
        std::vector<int> ShuffledRemainingMoves[THREAD_COUNT];        
        HexBoard Board, MoveBoard;
        std::vector<HexBoard> SampleBoards;
};

#endif