#ifndef GENERATOR_THREAD_H
#define GENERATOR_THREAD_H
#include "thread_pool.h"


class GeneratorThread
{
private:
    ThreadPool& pool;
    std::thread task_thread;
    std::atomic<bool> running{true};
public:
    GeneratorThread(ThreadPool& p);
    ~GeneratorThread();

    void run();
    void stop();
};



#endif //GENERATOR_THREAD_H
