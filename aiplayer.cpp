#include <random>
#include <array>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(int sampleCount, HexBoard& hexBoard) 
    : SampleCount{sampleCount}, RandomEngine{time(nullptr)}, Board{hexBoard.Width, hexBoard.Height}, MoveBoard{hexBoard.Width, hexBoard.Height}, SampleBoard{hexBoard.Width, hexBoard.Height}
{
    for (int row = 0; row < hexBoard.Height; ++row)
        for (int col = 0; col < hexBoard.Width; ++col)
            if (hexBoard.GetCell(col, row) == HexCell::EMPTY)
                RemainingMoves.push_back(row * hexBoard.Width + col); 
}

void AIPlayer::RemoveMove(std::pair<int, int>& move, HexCell player)
{
    Board.MarkCell(move.first, move.second, player);

    RemainingMoves.erase(
        std::find(RemainingMoves.begin(), RemainingMoves.end(), move.second * Board.Width + move.first));
}

std::pair<int, int> AIPlayer::GetMove()
{
    std::pair<int, int> move;   
    HexCell currentPlayer = HexCell::PLAYER2;
    HexCell sampleCurrentPlayer;
    int maxWins = 0;

    for (int i = 0; i < RemainingMoves.size(); ++i)
    {            
        std::vector<int> sampleRemainingMoves{RemainingMoves};        
        std::swap(sampleRemainingMoves[i], sampleRemainingMoves[sampleRemainingMoves.size() - 1]); // Moving the move to the end before removing to make removal constant time        
        int moveWinCount = 0; 

        CopyBoardState(MoveBoard, Board);
        MoveBoard.MarkCell(sampleRemainingMoves.back() % MoveBoard.Width, sampleRemainingMoves.back() / MoveBoard.Width, currentPlayer);

        sampleRemainingMoves.pop_back();

        for (int sample = 0; sample < SampleCount; ++sample)
        {   
            sampleCurrentPlayer = (currentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1; // A (simulated move was played, so change current player);

            CopyBoardState(SampleBoard, MoveBoard);    

            std::shuffle(sampleRemainingMoves.begin(), sampleRemainingMoves.end(), RandomEngine);

            for (int j = 0; j < sampleRemainingMoves.size(); ++j)
            {
                SampleBoard.MarkCell(sampleRemainingMoves[j] % MoveBoard.Width, sampleRemainingMoves[j] / MoveBoard.Width, sampleCurrentPlayer);
                sampleCurrentPlayer = (sampleCurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;
            }

            if (SampleBoard.IsGameWon(HexCell::PLAYER2)) 
                ++moveWinCount;
        }

        if (moveWinCount > maxWins) 
        {
            maxWins = moveWinCount;
            move.first = RemainingMoves[i] % MoveBoard.Width;
            move.second = RemainingMoves[i] / MoveBoard.Width;
        }        
    }

    return move;
}