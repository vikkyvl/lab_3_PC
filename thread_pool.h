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
    std::mutex control_mutex;
    std::condition_variable control_cv;

    std::mutex output_mutex;
    std::atomic<int> totalTasksAdded{0};
    std::atomic<int> totalTasksExecuted{0};
    std::atomic<double> totalExecutionTime{0};
public:
    ThreadPool();
    ~ThreadPool();

    void start();
    void serveQueue(int queueIndex, int threadId);
    void add_task(const Task& task);
    void pause();
    void resume();
    void stop();
    void stopNow();
    void getThreadPoolStatistics() const;
};

#endif //THREAD_POOL_H
