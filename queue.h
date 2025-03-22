#ifndef QUEUE_H
#define QUEUE_H
#include <queue>
#include "task.h"

class Queue
{
private:
    std::queue<Task> queue;
public:
    Queue();
    ~Queue();

    void pop();
    void push();
    void clear();
    bool empty();
};



#endif //QUEUE_H
