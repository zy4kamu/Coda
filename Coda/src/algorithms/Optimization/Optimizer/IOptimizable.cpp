#include "IOptimizable.h"

using namespace Optimization;

IOptimizable::IOptimizable() :
    dimension(0)
{
}

IOptimizable::IOptimizable(int dimension) :
    dimension(dimension)
{
}


IOptimizable::~IOptimizable(void)
{
}
