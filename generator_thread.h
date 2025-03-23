#ifndef GENERATOR_THREAD_H
#define GENERATOR_THREAD_H
#include "thread_pool.h"
#include <thread>


class GeneratorThread
{
private:
    ThreadPool& pool;
    std::thread task_thread;
    std::atomic<bool> running{true};
public:
    explicit GeneratorThread(ThreadPool& p);
    ~GeneratorThread();

    void run();
    void stop();
};


#endif //GENERATOR_THREAD_H
