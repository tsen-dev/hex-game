#include "aiplayer.h"
#include "hexboard.h"

AIPlayer::AIPlayer(int sampleCount) : SampleCount{sampleCount} {}

std::pair<int, int> AIPlayer::GetMove(HexBoard& board)
{
    return std::pair<int, int>{};
}
