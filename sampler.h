#ifndef SAMPLER_H
#define SAMPLER_H

#include <vector>
#include <random>

#include "hexboard.h"

class Sampler
{
    public:
        // Create a sampler. The sampler's RandomEngine is seeded with sum of the current time and the specified id
        Sampler(int id, HexBoard& moveBoard, std::vector<std::pair<int, int>>& moves, char myPlayer);
        /* Simulate sampleCount many random games from MoveBoard's state and return the number of times MyPlayer won. 
        Setting move to TRY_SWAP signals that (1) a swap should be simulated i.e. there is no move to be removed as one 
        wasn't played, (2) the players of MoveBoard have been swapped */
        int SampleMove(int move, int sampleCount);

        static const int TRY_SWAP = -1;

    private:
        /* Play all remaining moves on the SampleBoard, alternating players at each move. moveIndices indexes into Moves, 
        and is played in front order */
        void PlayRemainingMoves(std::vector<int>& moveIndices, char currentPlayer);
        /* Play all remaining moves on the SampleBoard, alternating players at each move. moveIndices indexes into Moves, 
        and is played in reverse order */
        void PlayRemainingMovesReverse(std::vector<int>& moveIndices, char currentPlayer);

        int ID;
        char MyPlayer; // The player who's wins are counted
        HexBoard& MoveBoard; // A board that the move to be tested was played on
        HexBoard SampleBoard; // A board that the remaining moves are to be played on
        std::vector<std::pair<int, int>>& Moves; // Remaining moves that can be played
        std::default_random_engine RandomEngine;
};

#endif