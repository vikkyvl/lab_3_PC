#include "queue.h"

void Queue::push(const Task task)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    task_queue.push(task);
    totalExecutionTime.fetch_add(task.getExecutionTime());
    queue_notifier.notify_one();
}

Task Queue::pop()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    queue_notifier.wait(lock, [this] {return !task_queue.empty();});
    Task task = task_queue.front();
    task_queue.pop();
    totalExecutionTime.fetch_sub(task.getExecutionTime());
    return task;
}

void Queue::clear()
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    while(!task_queue.empty())
    {
        totalExecutionTime.fetch_sub(task_queue.front().getExecutionTime());
        task_queue.pop();
    }
}

int Queue::getTotalExecutionTime() const
{
    return totalExecutionTime.load();
}

bool Queue::empty()
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return task_queue.empty();
}

