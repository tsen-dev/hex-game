#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include <random>

#include "hexboard.h"

class Sampler
{
    public:
        Sampler(int id, HexBoard& moveBoard, std::vector<int>& moves);
        int SampleMove(int move, int sampleCount);
    private:
        int ID;
        HexBoard& MoveBoard;
        HexBoard SampleBoard;
        std::vector<int>& Moves;
        std::vector<int> ShuffledMoves;
        std::default_random_engine RandomEngine;


};

#endif