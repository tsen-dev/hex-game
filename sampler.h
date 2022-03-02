#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include <random>

#include "hexboard.h"

class Sampler
{
    public:
        Sampler(int id, HexBoard& moveBoard, std::vector<std::pair<int, int>>& moves);
        int SampleMove(int move, int sampleCount);
    private:
        int ID;
        HexBoard& MoveBoard;
        HexBoard SampleBoard;
        std::vector<std::pair<int, int>>& Moves;
        std::default_random_engine RandomEngine;
};

#endif