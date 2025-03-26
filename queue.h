#ifndef QUEUE_H
#define QUEUE_H
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include "task.h"

class Queue
{
private:
    std::queue<Task> task_queue;
    std::atomic<int> totalExecutionTime{0};
    std::mutex queue_mutex;
    std::condition_variable queue_notifier;

    std::atomic<int> sumQueueLength{0};
    std::atomic<int> numMeasurements{0};
    std::unordered_map<int, double> threadWaitTimes;

public:
    Queue() = default;
    ~Queue() { clear(); };

    void push(Task task);
    Task pop(int threadIndex);
    void clear();
    bool empty();
    void notifyAll();

    int getTotalExecutionTime() const;
    void getQueueStatistics(int queueIndex) const;
    double getAverageQueueLength() const;
};

#endif //QUEUE_H
