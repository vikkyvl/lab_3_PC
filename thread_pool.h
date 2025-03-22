#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <vector>
#include "queue.h"


class ThreadPool
{
private:
    std::vector<Queue> queues;
    std::vector<std::thread> working_threads;

public:
    ThreadPool();
    ~ThreadPool();

    void initialize();
    void start();
    void pause();
    void resume();
    void stop();
};



#endif //THREAD_POOL_H
