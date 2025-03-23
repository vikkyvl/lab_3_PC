#ifndef QUEUE_H
#define QUEUE_H
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "task.h"

class Queue
{
private:
    std::queue<Task> task_queue;
    std::atomic<int> totalExecutionTime{0};
    std::mutex queue_mutex;
    std::condition_variable queue_notifier;
public:
    Queue() = default;
    ~Queue() { clear(); };

    void push(Task task);
    Task pop();
    void clear();
    bool empty();
    //int size();
    int getTotalExecutionTime() const;
};

#endif //QUEUE_H
