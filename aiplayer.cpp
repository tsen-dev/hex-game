#include <random>
#include <array>

#include "aiplayer.h"
#include "hexboard.h"

void AIPlayer::RemovePlayedMove(std::pair<int, int>& move, int boardWidth)
{
    RemainingMoves.erase(
        std::find(RemainingMoves.begin(), RemainingMoves.end(), move.second * boardWidth + move.first));
}

void AIPlayer::GetRemainingMoves(HexBoard& board)
{
    for (int row = 0; row < board.Height; ++row)
        for (int col = 0; col < board.Width; ++col)
            if (board.GetCell(col, row) == HexCell::EMPTY)
                RemainingMoves.push_back(row * board.Width + col); 
}

AIPlayer::AIPlayer(int sampleCount, HexBoard& board) : SampleCount{sampleCount} 
{
    GetRemainingMoves(board);
}

std::pair<int, int> AIPlayer::GetMove(HexBoard& board)
{
    // Rng could be a member of aiplayer
    std::default_random_engine rng{static_cast<unsigned int>(time(nullptr))};
    int maxWins = 0;
    std::pair<int, int> move;

    for (int i = 0; i < RemainingMoves.size(); ++i)
    {            
        std::vector<int> sampleRemainingMoves{RemainingMoves};        
        std::swap(sampleRemainingMoves[i], sampleRemainingMoves[sampleRemainingMoves.size() - 1]); // Moving the move to the end before removing to make removal constant time        
        int moveWinCount = 0; 

        HexBoard moveBoard{board};
        moveBoard.MarkCell(sampleRemainingMoves.back() % moveBoard.Width, sampleRemainingMoves.back() / moveBoard.Width, moveBoard.CurrentPlayer);
        moveBoard.CurrentPlayer = (moveBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; // A (simulated move was played, so change current player)

        sampleRemainingMoves.pop_back();

        for (int sample = 0; sample < SampleCount; ++sample)
        {   
            // Creating a new board here at every iteration which calls OS to allocate memory:
            // Could declare a board before the for loop and set its initial value at each iteration using assignment operator                      
            HexBoard sampleBoard{moveBoard};    

            // shuffle using fisher yates algorithm
            std::shuffle(sampleRemainingMoves.begin(), sampleRemainingMoves.end(), rng);

            for (int j = 0; j < sampleRemainingMoves.size(); ++j)
            {
                sampleBoard.MarkCell(sampleRemainingMoves[j] % moveBoard.Width, sampleRemainingMoves[j] / moveBoard.Width, sampleBoard.CurrentPlayer);
                sampleBoard.CurrentPlayer = (sampleBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;
            }

            // This might need to stay to make it start with the same player at each iteration
            sampleBoard.CurrentPlayer = HexCell::PLAYER2;

            if (sampleBoard.IsGameWon()) 
                ++moveWinCount;
        }

        if (moveWinCount > maxWins) 
        {
            maxWins = moveWinCount;
            move.first = RemainingMoves[i] % moveBoard.Width;
            move.second = RemainingMoves[i] / moveBoard.Width;
        }
    }         

    return move;
}