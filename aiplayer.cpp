#include <random>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(int sampleCount) : SampleCount{sampleCount} {}

std::pair<int, int> AIPlayer::GetMove(HexBoard& board)
{
    std::pair<int, int> move;

    HexBoard currentBoard{board};
    std::default_random_engine rng{};
    
    int maxWins = 0;
    std::vector<std::pair<int, int>> remainingMoves;

    for (int row = 0; row < board.Height; ++row)
        for (int col = 0; col < board.Width; ++col)
            if (board.GetCell(col, row) == HexCell::EMPTY)
                remainingMoves.push_back(std::pair<int, int>{col, row});                    

    for (int i = 0; i < remainingMoves.size(); ++i)
    {       
        int wins = 0; 

        for (int sample = 0; sample < SampleCount; ++sample)
        {
            std::vector<std::pair<int, int>> sampleRemainingMoves{remainingMoves};
            HexBoard sampleBoard{currentBoard};
            
            sampleBoard.MarkCell(sampleRemainingMoves[i].first, sampleRemainingMoves[i].second, sampleBoard.CurrentPlayer);
            sampleBoard.CurrentPlayer = (sampleBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;

            sampleRemainingMoves.erase(sampleRemainingMoves.begin() + i);            
            std::shuffle(sampleRemainingMoves.begin(), sampleRemainingMoves.end(), rng);

            for (int j = 0; j < sampleRemainingMoves.size(); ++j)
            {
                std::pair<int, int>& currentMove = sampleRemainingMoves[j];
                sampleBoard.MarkCell(currentMove.first, currentMove.second, sampleBoard.CurrentPlayer);
                sampleBoard.CurrentPlayer = (sampleBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;
            }

            sampleBoard.CurrentPlayer = HexCell::PLAYER2;

            if (sampleBoard.IsGameWon()) 
                ++wins;
        }

        if (wins > maxWins) 
        {
            maxWins = wins;
            move = remainingMoves[i];
        }
    }
    
    return move;
}