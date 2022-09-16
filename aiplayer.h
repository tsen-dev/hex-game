#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <vector>
#include <random>

#include "hexboard.h"
#include "sampler.h"
#include "threadpool.h"
#include "move.h"

class AIPlayer
{
    public:
        // Creates an AIPlayer and writes all of hexBoard's unplayed moves into Moves
        AIPlayer(HexBoard& hexBoard, int sampleCount = 1000, int samplerCount = 8);          
        /* Simulate SampleCount many random games from hexBoard for each of the remaining moves. If firstMove is true, also try swapping the players 
        instead of playing a move. Return the move resulting in the most number of wins, or SWAP if swapping was more successful */      
        Move GetMove(const HexBoard& hexBoard, bool firstMove);

        // Test functions
        friend void testAIPlayerConstructor();
        friend void testRemoveMove();

        static const Move SWAP;        
        static const int TRY_SWAP = Sampler::TRY_SWAP;

    private:
        // Clear Moves and place all availables moves on the specified board into Moves
        void GetAvailableMoves(const HexBoard& hexBoard);
        /* If move is TRY_SWAP, swap MoveBoard's players, otherwise play the move to be sampled on MoveBoard. Then simulate 
        SampleCount many random games from MoveBoard's state and return the number of wins. move is an index into Moves */
        int SampleMove(int move);

        int SampleCount;        
        char MyPlayer;
        std::vector<Move> Moves; // The remaining moves that can be played
        HexBoard& Board; // The main game board
        HexBoard MoveBoard; // A board that a move to be tested will be played on
        std::vector<Sampler> Samplers;   
        ThreadPool ThreadPool;

};

#endif