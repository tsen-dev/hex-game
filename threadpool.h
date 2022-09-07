#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <type_traits>
#include <future>

class ThreadPool
{
    public:
        // Create a thread pool with the specidied number of threads
        ThreadPool(int threadCount);
        // Terminate all threads in the pool after all outstanding work items have been processed
        ~ThreadPool();
        /* Enqueue the specified function to be called with the specified arguments. Returns a future that will resolve with the function's return value after it is executed. 
        (Note: If the returned future is discarded, its destructor will be called, which automatically waits for the future to resolve) */
        template<typename Fun, typename ...Args>
	    auto EnqueueWork(Fun&& fun, Args&&... args)
	    {
            auto work = std::make_shared<std::packaged_task<std::result_of_t<Fun(Args...)>()>>(std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
            {
                std::unique_lock<std::mutex> queueLockQuard{QueueLock};
                Queue.push([work](){(*work)();});
            }
            QueueNotEmptyCondition.notify_one();
            return work->get_future();   
	    }

    private:
        std::vector<std::thread> Threads;
        std::queue<std::function<void(void)>> Queue;
        std::mutex QueueLock;
        std::condition_variable QueueNotEmptyCondition;
};

#endif