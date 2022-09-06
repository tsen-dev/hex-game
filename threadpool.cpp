#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>




#include <iostream>

class ThreadPool
{
    public:
        ThreadPool(int threadCount)
        {
            Threads.reserve(threadCount);

            for (int i = 0; i < threadCount; ++i)
                Threads.push_back(std::thread(ThreadLoop));
        }

        ~ThreadPool()
        {
            {
                std::lock_guard<std::mutex> queueLockQuard{QueueLock};

                for (std::thread& thread : Threads)
                    Queue.push(nullptr);
            }
            
            QueueNotEmptyCondition.notify_all();    

            for (std::thread& thread : Threads) 
                thread.join();
        }

        void QueueWork(std::function<void(void)> work)
        {
            {
                std::unique_lock<std::mutex> queueLockQuard{QueueLock};
                Queue.push(work);
            }
            QueueNotEmptyCondition.notify_one();
        }

    private:
        std::vector<std::thread> Threads;
        std::queue<std::function<void(void)>> Queue;
        std::mutex QueueLock;
        std::condition_variable QueueNotEmptyCondition;
        std::function<void(void)> ThreadLoop = [this](){
            while (true)
            {
                std::function<void(void)> work;
                {
                    std::unique_lock<std::mutex> guard{QueueLock};
                    QueueNotEmptyCondition.wait(guard, [this](){return !Queue.empty();});    
                    work = Queue.front();
                    Queue.pop();
                }

                if (!work) break;
                else work();
            } 
        };
};

int main()
{
    ThreadPool threadPool{1000};

    std::mutex coutlock;

    for (int i = 0; i < 100; ++i)
    {
        threadPool.QueueWork([&coutlock, i](){
            std::unique_lock<std::mutex> guard{coutlock};
            std::cout << i << '\n';
        });
    }
    return 0;
}