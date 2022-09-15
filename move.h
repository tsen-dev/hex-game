#ifndef MOVE_H
#define MOVE_H

struct Move
{
    public:
        int X;
        int Y;

    Move();

    Move(int x, int y);

    bool operator==(const Move& move) const;
};

#endif