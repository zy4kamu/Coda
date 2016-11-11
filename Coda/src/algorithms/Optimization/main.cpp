#include "IOptimizable.h"
#include "LBFGS.h"

class SimpleFunction : public Optimization::IOptimizable
{
public:
    SimpleFunction(int dimension);
    ~SimpleFunction(void);
    double CalculateValue(const vector<double>& point);
    void CalculateGradient(const vector<double>& point,
        vector<double>* gradient);
};

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

int main()
{
    SimpleFunction func(10);
    Optimization::LBFGS lbfgs;
    vector<double> initialPoint(10, 1);
    vector<double> minPoint;
    Optimization::ICallable eventHandler;
    lbfgs.Optimize(func, &eventHandler, initialPoint, &minPoint, 1000);
}
