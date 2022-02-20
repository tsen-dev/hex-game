#include <random>
#include <array>

#include "aiplayer.h"
#include "hexboard.h"

void AIPlayer::RemovePlayedMove(std::pair<int, int>& move, int boardWidth)
{
    RemainingMoves.erase(
        std::find(RemainingMoves.begin(), RemainingMoves.end(), move.second * boardWidth + move.first));
}

void AIPlayer::GetRemainingMoves(HexGame& game)
{
    for (int row = 0; row < game.BoardHeight; ++row)
        for (int col = 0; col < game.BoardWidth; ++col)
            if (game.GetCell(col, row) == HexCell::EMPTY)
                RemainingMoves.push_back(row * game.BoardWidth + col); 
}

AIPlayer::AIPlayer(int sampleCount, HexGame& game) : SampleCount{sampleCount} 
{
    GetRemainingMoves(game);
}

std::pair<int, int> AIPlayer::GetMove(HexGame& game)
{
    clock_t starttime;
    clock_t shuffletime = 0;
    clock_t getmovetime = clock();    

    // Rng could be a member of aiplayer
    std::default_random_engine rng{static_cast<unsigned int>(time(nullptr))};
    int maxWins = 0;
    std::pair<int, int> move;

    for (int i = 0; i < RemainingMoves.size(); ++i)
    {            
        std::vector<int> sampleRemainingMoves{RemainingMoves};        
        std::swap(sampleRemainingMoves[i], sampleRemainingMoves[sampleRemainingMoves.size() - 1]); // Moving the move to the end before removing to make removal constant time        
        int moveWinCount = 0; 

        HexGame moveGame{game};
        moveGame.MarkCell(sampleRemainingMoves.back() % moveGame.BoardWidth, sampleRemainingMoves.back() / moveGame.BoardWidth, moveGame.CurrentPlayer);
        moveGame.CurrentPlayer = (moveGame.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; // A (simulated move was played, so change current player)

        sampleRemainingMoves.pop_back();

        for (int sample = 0; sample < SampleCount; ++sample)
        {   
            // Creating a new game here at every iteration which calls OS to allocate memory:
            // Could declare a game before the for loop and set its initial value at each iteration using assignment operator                      
            HexGame sampleGame{moveGame};    

            starttime = clock();
            // shuffle using fisher yates algorithm
            std::shuffle(sampleRemainingMoves.begin(), sampleRemainingMoves.end(), rng);
            shuffletime += clock() - starttime;

            for (int j = 0; j < sampleRemainingMoves.size(); ++j)
            {
                sampleGame.MarkCell(sampleRemainingMoves[j] % moveGame.BoardWidth, sampleRemainingMoves[j] / moveGame.BoardWidth, sampleGame.CurrentPlayer);
                sampleGame.CurrentPlayer = (sampleGame.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;
            }

            // This might need to stay to make it start with the same player at each iteration
            sampleGame.CurrentPlayer = HexCell::PLAYER2;

            if (sampleGame.IsGameWon()) 
                ++moveWinCount;
        }

        if (moveWinCount > maxWins) 
        {
            maxWins = moveWinCount;
            move.first = RemainingMoves[i] % moveGame.BoardWidth;
            move.second = RemainingMoves[i] / moveGame.BoardWidth;
        }        
    }

    getmovetime = clock() - getmovetime;

    std::cout << "getmovetime: " << (double) getmovetime / CLOCKS_PER_SEC << '\n' 
              << "shuffletime: " << (double) shuffletime / CLOCKS_PER_SEC << '\n' 
              << (double) shuffletime * 100 / getmovetime << "%\n\n";

    return move;
}