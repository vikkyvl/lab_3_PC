#include "thread_pool.h"
#include <iostream>

#define THREADS_NUM 2
#define QUEUES_NUM 3

std::mutex ThreadPool::output_mutex;

ThreadPool::ThreadPool(): queues(QUEUES_NUM) {}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::start()
{
    for(int i = 0; i < QUEUES_NUM; i++)
    {
        for(int j = 0; j < THREADS_NUM; j++)
        {
            int threadId = i * THREADS_NUM + j;
            working_threads.emplace_back(&ThreadPool::serveQueue, this, i, threadId);
        }
    }
}

void ThreadPool::serveQueue(int queueIndex, int threadId)
{
    while(!isStopped.load())
    {
        {
            std::unique_lock<std::mutex> lock(control_mutex);
            control_cv.wait(lock,[this](){return !isPaused || isStopped.load();});
        }

        Task task = queues[queueIndex].pop();
        {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cout << "Thread " << threadId << " executing Task #"
                      << task.getId() << " (Duration: " << task.getExecutionTime() << " sec)\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(task.getExecutionTime()));
    }
}


void ThreadPool::add_task(const Task& task)
{
    int queueIndex = 0;
    int queueIndexDuration = queues[0].getTotalExecutionTime();

    for(int i = 1; i < QUEUES_NUM; i++)
    {
        int duration = queues[i].getTotalExecutionTime();
        if(duration < queueIndexDuration)
        {
            queueIndex = i;
            queueIndexDuration = duration;
        }
    }

    queues[queueIndex].push(task);
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        std::cout << "Task #" << task.getId() << "(Duration: " << task.getExecutionTime() << ") added to queue " << queueIndex << std::endl;
    }
}

void ThreadPool::pause()
{
    isPaused.store(true);
}

void ThreadPool::resume()
{
    isPaused.store(false);
    control_cv.notify_all();
}

void ThreadPool::stop()
{
    isStopped.store(true);
    control_cv.notify_all();

    for (auto& queue : queues)
    {
        queue.clear();
        queue.notifyAll();
    }

    for (auto& thread : working_threads)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
}

