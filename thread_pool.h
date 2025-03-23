#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "queue.h"


class ThreadPool
{
private:
    std::vector<Queue> queues;
    std::vector<std::thread> working_threads;
    std::atomic<bool> isPaused{false};
    std::atomic<bool> isStopped{false};
    static std::mutex output_mutex;
    std::mutex control_mutex;
    std::condition_variable control_cv;

public:
    ThreadPool();
    ~ThreadPool();

    void start();
    void serveQueue(int queueIndex, int threadId);
    void add_task(const Task& task);
    void pause();
    void resume();
    void stop();
};



#endif //THREAD_POOL_H
