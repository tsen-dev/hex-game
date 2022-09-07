#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

#include "threadpool.h"

// Create a thread pool with the specidied number of threads
ThreadPool::ThreadPool(int threadCount)
{
    Threads.reserve(threadCount);
    
    for (int i = 0; i < threadCount; ++i)
    {
        Threads.push_back(std::thread([this](){
            while (true)
            {
                std::function<void(void)> work;
                {
                    std::unique_lock<std::mutex> queueGuard{QueueLock};
                    QueueNotEmptyCondition.wait(queueGuard, [this](){return !Queue.empty();});    
                    work = Queue.front();
                    Queue.pop();
                }
                if (work == nullptr) break;
                else work();
            } 
        }));
    }
}

// Terminate all threads in the pool after all outstanding work items have been processed
ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> queueGuard{QueueLock};
        for (std::thread& thread : Threads)
            Queue.push(nullptr);
    }
    
    QueueNotEmptyCondition.notify_all();   

    for (std::thread& thread : Threads) 
        thread.join();
}