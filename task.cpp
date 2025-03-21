#include "task.h"
#include <ctime>
#include <cstdlib>

std::atomic<int> Task::counter(0);

Task::Task() {
    id = counter++;
    executionTime = MIN + (rand() % (MAX - MIN + 1));
}

int Task::getId() const
{
    return id;
}

int Task::getExecutionTime() const
{
    return executionTime;
}

