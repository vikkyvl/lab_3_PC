#include <iostream>
#include <iomanip>
#include "queue.h"

void Queue::push(const Task task)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    task_queue.push(task);
    totalExecutionTime.fetch_add(task.getExecutionTime());
    queue_notifier.notify_one();
}

Task Queue::pop(int threadIndex)
{
    std::unique_lock<std::mutex> lock(queue_mutex);

    auto start = std::chrono::high_resolution_clock::now();
    queue_notifier.wait(lock, [this] { return !task_queue.empty(); });
    auto end = std::chrono::high_resolution_clock::now();

    auto waitTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    threadWaitTimes[threadIndex] += waitTime.count() * 1e-9;

    Task task = task_queue.front();
    task_queue.pop();

    sumQueueLength.fetch_add(task_queue.size());
    numMeasurements.fetch_add(1);

    totalExecutionTime.fetch_sub(task.getExecutionTime());
    return task;
}

void Queue::clear()
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    while(!empty())
    {
        totalExecutionTime.fetch_sub(task_queue.front().getExecutionTime());
        task_queue.pop();
    }
}

void Queue::notifyAll()
{
    queue_notifier.notify_all();
}

int Queue::getTotalExecutionTime() const
{
    return totalExecutionTime.load();
}

bool Queue::empty()
{
    return task_queue.empty();
}

double Queue::getAverageQueueLength() const
{
    if (numMeasurements == 0)
    {
        return 0.0;
    }
    return static_cast<double>(sumQueueLength.load()) / numMeasurements.load();
}

void Queue::getQueueStatistics(int queueIndex) const
{
    std::cout << "\nQueue " << queueIndex << ":\n";
    std::cout << "Average length: " << getAverageQueueLength() << "\n";
    std::cout << "Worker thread wait times:\n";
    for (const auto& entry : threadWaitTimes)
    {
        std::cout << "Thread " << entry.first << ", total wait time: " << entry.second << " s\n";
    }
}


