#include "functions.hpp"

#include <random>

#include "constants.hpp"

namespace rcl
{

double RandomDouble01()
{
    static std::uniform_real_distribution<double> distribution(0.0, 0.999);
    static std::mt19937 generator;
    return distribution(generator);
}

double RandomDoubleMinMax(double min, double max)
{
    return min + (max-min) * RandomDouble01();
}

int RandomIntMinMax(int min, int max)
{
    return int(RandomDoubleMinMax(min, max+1));
}

double LinearToGamma(double value)
{
    if (value > 0)
        return std::sqrt(value);

    return 0;
}

}