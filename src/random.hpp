#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random
{
    std::mt19937 rng;

    Random();

  public:
    static Random& inst();

    static double roll(double low, double high);
};

#endif // RANDOM_HPP
