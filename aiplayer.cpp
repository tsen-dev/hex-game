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
    HexBoard boardCopy{board};
    std::default_random_engine rng{static_cast<unsigned int>(time(nullptr))};
    int maxWins = 0;
    std::pair<int, int> move;

    clock_t getfreemovs = 0;
    clock_t movescpy = 0;
    clock_t boardcpy = 0;
    clock_t erasemove = 0;
    clock_t shufflemoves = 0;
    clock_t playmoves = 0;
    clock_t iswoncheck = 0;
    clock_t starttime;

    #if true
    for (int i = 0; i < RemainingMoves.size(); ++i)
    {       
        int wins = 0; 

        for (int sample = 0; sample < SampleCount; ++sample)
        {
            // Recently changed Remainingmoves from vector<pair<int, int>> to vector<int>
            starttime = clock();
            std::vector<int> sampleRemainingMoves{RemainingMoves};
            movescpy += clock() - starttime;
            starttime = clock();
            HexBoard sampleBoard{boardCopy}; 
            boardcpy += clock() - starttime;
            // Creating a new board here at every iteration which calls OS to allocate memory:
            // Could declare a board before the for loop and set its initial value at each iteration using assignment operator             
            sampleBoard.MarkCell(sampleRemainingMoves[i] % boardCopy.Width, sampleRemainingMoves[i] / boardCopy.Width, sampleBoard.CurrentPlayer);
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
                sampleBoard.MarkCell(sampleRemainingMoves[j] % boardCopy.Width, sampleRemainingMoves[j] / boardCopy.Width, sampleBoard.CurrentPlayer);
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
            move.first = RemainingMoves[i] % boardCopy.Width;
            move.second = RemainingMoves[i] / boardCopy.Width;
        }
    }
    #endif

    clock_t tot = getfreemovs + movescpy + boardcpy + erasemove + shufflemoves + playmoves + iswoncheck;

    std::cout << 
    "getfreemovs: " << (double)(getfreemovs) / CLOCKS_PER_SEC << ' ' << 100*(double)(getfreemovs)/tot << "%\n" <<             
    "movescpy: " << (double)(movescpy) / CLOCKS_PER_SEC << ' ' << 100*(double)(movescpy)/tot << "%\n" <<             
    "boardcpy: " << (double)(boardcpy) / CLOCKS_PER_SEC << ' ' << 100*(double)(boardcpy)/tot << "%\n" <<             
    "erasemove: " << (double)(erasemove) / CLOCKS_PER_SEC << ' ' << 100*(double)(erasemove)/tot << "%\n" <<          
    "shufflemoves: " << (double)(shufflemoves) / CLOCKS_PER_SEC << ' ' << 100*(double)(shufflemoves)/tot << "%\n" << 
    "playmoves: " << (double)(playmoves) / CLOCKS_PER_SEC << ' ' << 100*(double)(playmoves)/tot << "%\n" <<          
    "iswoncheck: " << (double)(iswoncheck) / CLOCKS_PER_SEC << ' ' << 100*(double)(iswoncheck)/tot << "%\n";         

    /* Profiler results:
    movescpy: 0.085 11.5332%
    boardcpy: 0.086 11.6689%
    erasemove: 0.057 7.73406%
    shufflemoves: 0.339 45.9973%
    playmoves: 0.082 11.1262%
    iswoncheck: 0.088 11.9403% */

    return move;
}