#include <iostream>

#include "test.h"
#include "hex.h"
#include "threadpool.h"

#ifndef TEST

int main()
{
    ThreadPool tp(100);

    std::future<void> x = tp.EnqueueWork([](){std::cout << 10;});

    x.get();

    startGame();

    return 0;
}

#endif