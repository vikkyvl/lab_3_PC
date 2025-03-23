#ifndef TASK_H
#define TASK_H

#include <atomic>
#define MIN 5
#define MAX 20

class Task
{
private:
    int id;
    int executionTime;
    static std::atomic<int> counter;

public:
    Task();
    int getId() const;
    int getExecutionTime() const;
};

#endif //TASK_H
