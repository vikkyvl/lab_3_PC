#include "generator_thread.h"
#include <chrono>
#include <cstdlib>

#define DELAY_MIN 1
#define DELAY_MAX 5

GeneratorThread::GeneratorThread(ThreadPool& p) : pool(p) {}

GeneratorThread::~GeneratorThread()
{
    stop();

    if(task_thread.joinable())
    {
        task_thread.join();
    }
}

void GeneratorThread::run()
{
    task_thread = std::thread([this]()
    {
        while(running.load())
        {
            Task task;
            pool.add_task(task);
            std::this_thread::sleep_for(std::chrono::seconds(DELAY_MIN + (rand() % (DELAY_MAX - DELAY_MIN + 1))));
        }
    });
}

void GeneratorThread::stop()
{
    running.store(false);
}

