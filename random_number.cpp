#include "random_number.h"

thread_local std::mt19937 RandomNumber::generator(std::random_device{}());

int RandomNumber::getRandomNumber(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}