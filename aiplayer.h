#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"
#include "sampler.h"

class AIPlayer
{
    public:
        // Creates an AIPlayer and writes all of hexBoard's unplayed moves into Moves
        AIPlayer(HexBoard& hexBoard, int sampleCount = 1000, int samplerCount = 8);  
        /* Simulate SampleCount many random games for each of the remaining moves. If firstMove is true, also try swapping 
        the players instead of playing a move. Return the move resulting in the most number of wins, or SWAP if swapping was 
        more successful */      
        std::pair<int, int> GetMove(bool firstMove);
        // Remove the specified move from Moves, if it exists
        void RemoveMove(const std::pair<int, int>& move);        

        // Test functions
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

        static const std::pair<int, int> SWAP;        
        static const int TRY_SWAP = Sampler::TRY_SWAP;

    private:
        /* If move is TRY_SWAP, swap MoveBoard's players, otherwise play the move to be sampled on MoveBoard. Then simulate 
        SampleCount many random games from MoveBoard's state and return the number of wins. move is an index into Moves */
        int SampleMove(int move);

        int SampleCount;        
        char MyPlayer;
        std::vector<std::pair<int, int>> Moves; // The remaining moves that can be played
        HexBoard& Board; // The main game board
        HexBoard MoveBoard; // A board that a move to be tested will be played on
        std::vector<Sampler> Samplers;                
};

#endif