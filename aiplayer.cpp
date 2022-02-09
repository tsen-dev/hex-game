#include <random>

#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(int sampleCount) : SampleCount{sampleCount} {}

std::pair<int, int> AIPlayer::GetMove(HexBoard& board)
{
    std::pair<int, int> move;

    HexBoard boardCopy{board};
    std::default_random_engine rng{};
    
    int maxWins = 0;
    std::vector<std::pair<int, int>> remainingMoves;

    for (int row = 0; row < board.Height; ++row)
        for (int col = 0; col < board.Width; ++col)
            if (board.GetCell(col, row) == HexCell::EMPTY)
                remainingMoves.push_back(std::pair<int, int>{col, row});                    

    clock_t movescpy = 0;
    clock_t boardcpy = 0;
    clock_t erasemove = 0;
    clock_t shufflemoves = 0;
    clock_t playmoves = 0;
    clock_t iswoncheck = 0;
    clock_t starttime;

    for (int i = 0; i < remainingMoves.size(); ++i)
    {       
        int wins = 0; 

        for (int sample = 0; sample < SampleCount; ++sample)
        {
            starttime = clock();
            std::vector<std::pair<int, int>> sampleRemainingMoves{remainingMoves};
            movescpy += clock() - starttime;
            starttime = clock();
            HexBoard sampleBoard{boardCopy}; 
            boardcpy += clock() - starttime;
            // Creating a new board here at every iteration which calls OS to allocate memory:
            // Could declare a board before the for loop and set its initial value at each iteration using assignment operator             
            sampleBoard.MarkCell(sampleRemainingMoves[i].first, sampleRemainingMoves[i].second, sampleBoard.CurrentPlayer);
            sampleBoard.CurrentPlayer = (sampleBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;

            // This erasing is inefficient as it'll cause many moves.
            // Can instead move the item to be removed to the end before removing so we can just pop it off         
            starttime = clock();
            sampleRemainingMoves.erase(sampleRemainingMoves.begin() + i); 
            erasemove += clock() - starttime;
            starttime = clock();
            // shuffle using fisher yates algorithm
            // shuffle indices rather than pair structs
            std::shuffle(sampleRemainingMoves.begin(), sampleRemainingMoves.end(), rng);
            shufflemoves += clock() - starttime;

            starttime = clock();
            for (int j = 0; j < sampleRemainingMoves.size(); ++j)
            {
                std::pair<int, int>& currentMove = sampleRemainingMoves[j];
                sampleBoard.MarkCell(currentMove.first, currentMove.second, sampleBoard.CurrentPlayer);
                sampleBoard.CurrentPlayer = (sampleBoard.CurrentPlayer == HexCell::PLAYER1) ? HexCell::PLAYER2 : HexCell::PLAYER1;
            }
            playmoves += clock() - starttime;

            sampleBoard.CurrentPlayer = HexCell::PLAYER2;

            starttime = clock();
            if (sampleBoard.IsGameWon()) 
                ++wins;
            iswoncheck += clock() - starttime;
        }

        if (wins > maxWins) 
        {
            maxWins = wins;
            move = remainingMoves[i];
        }
    }

    clock_t tot = movescpy + boardcpy + erasemove + shufflemoves + playmoves + iswoncheck;

    std::cout << 
    "movescpy: " << (double)(movescpy) / CLOCKS_PER_SEC << ' ' << 100*(double)(movescpy)/tot << "%\n" <<             // 3 (3)
    "boardcpy: " << (double)(boardcpy) / CLOCKS_PER_SEC << ' ' << 100*(double)(boardcpy)/tot << "%\n" <<             // 5 (4)
    "erasemove: " << (double)(erasemove) / CLOCKS_PER_SEC << ' ' << 100*(double)(erasemove)/tot << "%\n" <<          // 6 (6)
    "shufflemoves: " << (double)(shufflemoves) / CLOCKS_PER_SEC << ' ' << 100*(double)(shufflemoves)/tot << "%\n" << // 4 (1)
    "playmoves: " << (double)(playmoves) / CLOCKS_PER_SEC << ' ' << 100*(double)(playmoves)/tot << "%\n" <<          // 3 (5)
    "iswoncheck: " << (double)(iswoncheck) / CLOCKS_PER_SEC << ' ' << 100*(double)(iswoncheck)/tot << "%\n";         // 1 (2)

    /* Profiler results:
    movescpy: 0.085 11.5332%
    boardcpy: 0.086 11.6689%
    erasemove: 0.057 7.73406%
    shufflemoves: 0.339 45.9973%
    playmoves: 0.082 11.1262%
    iswoncheck: 0.088 11.9403% */

    return move;
}