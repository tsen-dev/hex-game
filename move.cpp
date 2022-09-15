#include "move.h"

Move::Move() : X{}, Y{} {}

Move::Move(int x, int y) : X{x}, Y{y} {}

bool Move::operator==(const Move& move) const {return X == move.X && Y == move.Y;}