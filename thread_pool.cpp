#include "thread_pool.h"
#define THREADS_NUM 2
#define QUEUES_NUM 3

ThreadPool::ThreadPool(): queues(QUEUES_NUM) {}

ThreadPool::~ThreadPool() {stop();}

void ThreadPool::start()
{

}

void ThreadPool::add_task(Task& task)
{

}

void ThreadPool::stop()
{

}

