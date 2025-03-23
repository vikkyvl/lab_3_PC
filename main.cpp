#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>
#include "generator_thread.h"
#include "queue.h"
#include "thread_pool.h"

#define NUM_THREADS 4

int main()
{
    srand(time(NULL));

    ThreadPool pool;
    pool.start();

    std::vector<GeneratorThread*> task_threads;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        task_threads.push_back(new GeneratorThread(pool));
        task_threads.back()->run();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        task_threads[i]->stop();
        delete task_threads[i];
    }

    pool.stop();

    return 0;
}
