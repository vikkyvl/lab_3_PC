#include "task.h"
#include "random_number.h"
#include <ctime>
#include <cstdlib>
#include <random>

std::atomic<int> Task::counter(0);

Task::Task()
{
    id = counter++;
    executionTime = RandomNumber::getRandomNumber(MIN, MAX);
}

int Task::getId() const
{
    return id;
}

int Task::getExecutionTime() const
{
    return executionTime;
}

