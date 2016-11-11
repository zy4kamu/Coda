#include "L2Regularization.h"
#include <cstdlib>

namespace LinearCRF
{

L2Regularization::L2Regularization(double regularizationFactor)
    : regularizationFactor(regularizationFactor)
{
}

void L2Regularization::UpdateValue(const vector<double>& point
    , size_t numberOfPoints, double* value)
{
    // Calculate normFactor
    double normFactor = 0;
    for (size_t it = 0; it < point.size(); ++it)
    {
        normFactor += point[it] * point[it];
    }
    *value += numberOfPoints * normFactor / regularizationFactor;
}

void L2Regularization::UpdateGradient(const vector<double>&point
    , size_t numberOfPoints, vector<double>* gradient)
{
    // Add norm factor
    for (size_t it = 0; it < point.size(); ++it)
    {
        (*gradient)[it] += 2.0 * numberOfPoints * point[it]
            / regularizationFactor;
    }
}

}

