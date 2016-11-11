#include "SimpleFunction.h"

using namespace Optimization;

#include <cstddef>
SimpleFunction::SimpleFunction(int dimension) : 
    IOptimizable(dimension)
{
}

SimpleFunction::~SimpleFunction(void)
{
}

double SimpleFunction::CalculateValue(const vector<double>& point)
{
    double retval = 0;
    for (size_t it = 0; it < point.size(); ++it)
    {
        retval += (point[it] - it - 1) * (point[it] - it - 1);
    }
    return retval;
}

void SimpleFunction::CalculateGradient(
        const vector<double>& point, 
        vector<double>* gradient)
{
    gradient->resize(point.size());
    for (size_t it = 0; it < point.size(); ++it)
    {
        (*gradient)[it] = 2.0 * (point[it] - it - 1);
    }
}
