#include "random.hpp"

using namespace std;

Random::Random()
    : rng(random_device{}())
{}

Random& Random::inst() //static
{
    static Random instance;
    return instance;
}

double Random::roll(double low, double high) //static
{
    return uniform_real_distribution<double>(low, high)(inst().rng);
}
