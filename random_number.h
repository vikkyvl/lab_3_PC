#ifndef RANDOM_NUMBER_H
#define RANDOM_NUMBER_H

#include <random>

class RandomNumber
{
private:
    thread_local static std::mt19937 generator;
public:
    RandomNumber() = default;
    static int getRandomNumber(int min, int max);
};

#endif //RANDOM_NUMBER_H
