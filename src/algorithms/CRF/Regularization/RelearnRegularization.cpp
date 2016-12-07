#include "RelearnRegularization.h"

namespace LinearCRF
{

RelearnRegularization::RelearnRegularization(
    double regularizationFactor)
    : regularizationFactor(regularizationFactor)
{
}

void RelearnRegularization::Initialize(const vector<double>& _stableValues)
{
    stableValues = _stableValues;
}

void RelearnRegularization::UpdateValue(const vector<double>& point
    , size_t numberOfPoints, double* value)
{
    // Calculate normFactor
    double normFactor = 0;
    for (size_t it = 0; it < point.size(); ++it)
    {
        double difference = point[it] - stableValues[it];
        normFactor += difference * difference;
    }
    *value += numberOfPoints * normFactor / regularizationFactor;
}

void RelearnRegularization::UpdateGradient(
    const vector<double>&point, size_t numberOfPoints
    , vector<double>* gradient)
{
    // Add norm factor
    for (size_t it = 0; it < point.size(); ++it)
    {
        (*gradient)[it] +=
            2.0 * numberOfPoints * (point[it] - stableValues[it])
            / regularizationFactor;
    }
}

void RelearnRegularization::SetRegularizationFactor(double factor)
{
    regularizationFactor = factor;
}

double RelearnRegularization::GetRegularizationFactor()
{
    return regularizationFactor;
}

}
