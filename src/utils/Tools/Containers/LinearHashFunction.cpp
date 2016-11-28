#include "LinearHashFunction.h"


LinearHashFunction::LinearHashFunction() :
    slope(DEFAULT_HASH_SLOPE), intercept(DEFAULT_HASH_COERCIDENT)
{
}

LinearHashFunction::LinearHashFunction(int slope, int intercept) :
    slope(slope), intercept(intercept)
{
}

int LinearHashFunction::operator ()(int input) const
{
    long long hash = 
        static_cast<long long>(slope) * static_cast<long long>(input) + 
        static_cast<long long>(intercept);
    hash %= PRIME;
    if (hash < 0)
    {
        hash += PRIME;
    }
    return static_cast<int>(hash);
}

LinearHashFunction RandomizeHashFunction(
    std::default_random_engine& generator)
{
    std::uniform_int_distribution<int> distribution(1, PRIME);
    return LinearHashFunction(
        distribution(generator), 
        distribution(generator));
}
