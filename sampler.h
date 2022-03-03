#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include <random>

#include "hexboard.h"

class Sampler
{
    public:
        Sampler(int id, HexBoard& moveBoard, std::vector<std::pair<int, int>>& moves, char myPlayer);
        int SampleMove(int move, int sampleCount);
    private:
        void PlayRemainingMoves(std::vector<int>& moveIndices, char& currentPlayer);
        void PlayRemainingMovesReverse(std::vector<int>& moveIndices, char& currentPlayer);

        int ID;
        char MyPlayer;
        HexBoard& MoveBoard;
        HexBoard SampleBoard;
        std::vector<std::pair<int, int>>& Moves;
        std::default_random_engine RandomEngine;
};

#endif