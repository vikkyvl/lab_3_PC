#include "thread_pool.h"
#include <iostream>

#define THREADS_NUM 2
#define QUEUES_NUM 3


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
            control_cv.wait(lock,[this](){return !isPaused.load() || isStopped.load();});
        }

        if (isStopped.load())
        {
            break;
        }

        Task task = queues[queueIndex].pop(threadId);
        {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cout << "Thread " << threadId << " executing Task #"
                      << task.getId() << " (Duration: " << task.getExecutionTime() << " sec)\n";
        }

        int delay = task.getExecutionTime();
        std::this_thread::sleep_for(std::chrono::seconds(delay));

        {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cout << "Thread " << threadId << " finished Task #" << task.getId() << std::endl;
        }

        totalTasksExecuted.fetch_add(1);
        totalExecutionTime.fetch_add(delay);
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
    totalTasksAdded.fetch_add(1);
}

void ThreadPool::pause()
{
    isPaused.store(true);

    // std::lock_guard<std::mutex> lock(output_mutex);
    // std::cout << "ThreadPool is paused." << std::endl;
}

void ThreadPool::resume()
{
    isPaused.store(false);
    control_cv.notify_all();

    // std::lock_guard<std::mutex> lock(output_mutex);
    // std::cout << "ThreadPool is resumed." << std::endl;
}

void ThreadPool::stop()
{
    isStopped.store(true);
    control_cv.notify_all();

    for (auto& queue : queues)
    {
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

void ThreadPool::stopNow()
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

    // std::lock_guard<std::mutex> lock(output_mutex);
    // std::cout << "Thread pool stopped immediately" << std::endl;
}

void ThreadPool::getThreadPoolStatistics() const
{
    std::cout << "\n* Results *" << std::endl;
    std::cout << "\nTasks added: " << totalTasksAdded.load() << std::endl;
    std::cout << "Tasks executed: " << totalTasksExecuted.load() << "\n";

    if (totalTasksExecuted > 0)
    {
        std::cout << "Average task execution time: " << totalExecutionTime.load() / totalTasksExecuted.load() << " s\n";
    }

    std::cout << "\n--- Queue Statistics ---\n";
    for (int i = 0; i < queues.size(); i++)
    {
        queues[i].getQueueStatistics(i);
    }
}

