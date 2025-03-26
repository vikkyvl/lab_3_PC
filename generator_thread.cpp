#include "generator_thread.h"
#include "random_number.h"
#include <chrono>

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
            delay = RandomNumber::getRandomNumber(DELAY_MIN, DELAY_MAX);
            std::this_thread::sleep_for(std::chrono::seconds(delay));
        }
    });
}

void GeneratorThread::stop()
{
    running.store(false);
}

