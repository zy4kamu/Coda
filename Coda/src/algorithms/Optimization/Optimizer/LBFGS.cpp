#include "LBFGS.h"

#include <cassert>
#include <iostream>

using std::wcout;

using namespace Optimization;

size_t LBFGS::currentIteration = 0;
size_t LBFGS::maxIteration = 100000;
double LBFGS::currentTime = 0;
ICallable* LBFGS::eventHandler = NULL;
bool LBFGS::shouldPrintInfo = true;

lbfgsfloatval_t LBFGS::Evaluate(
    void *instance,
    const lbfgsfloatval_t *x,
    lbfgsfloatval_t *g,
    const int n,
    const lbfgsfloatval_t step)
{
    vector<double> point(x, x + n);
    IOptimizable* function = reinterpret_cast<IOptimizable*>(instance);
    vector<double> grad;
    grad.resize(n);
    function->CalculateGradient(point, &grad);
    ++currentIteration;
    if (currentIteration < maxIteration)
    {
        for (int it = 0; it < n; ++it)
        {
            g[it] = grad[it];
        }
    }
    else
    {
        for (int it = 0; it < n; ++it)
        {
            g[it] = 0;
        }
    }
    return function->CalculateValue(point);
}

int LBFGS::Progress(
    void *instance,
    const lbfgsfloatval_t *x,
    const lbfgsfloatval_t *g,
    const lbfgsfloatval_t fx,
    const lbfgsfloatval_t xnorm,
    const lbfgsfloatval_t gnorm,
    const lbfgsfloatval_t step,
    int n,
    int k,
    int ls)
{
if (shouldPrintInfo)
{
    #if defined(MSVC)
        printf("Iteration %d:\n", k);
        printf("F = %f,  xnorm = %f, gnorm = %f, step = %f, time = %f\n",
            fx, xnorm, gnorm, step,
            omp_get_wtime() - currentTime);
    #else
        wcout
            << "Iteration " << k
            << std::endl
            << "F = "
            << fx << ", xnorm = "
            << xnorm << ", gnorm = "
            << gnorm << ", step = "
            << step << ", time = "
            << (double)(clock() - currentTime) / CLOCKS_PER_SEC
            << std::endl;
     #endif
}
    currentTime = omp_get_wtime();
    if (eventHandler != NULL)
    {
        bool shouldTerminate = eventHandler->Call(k, x, n);
        if (shouldTerminate) {
            if (shouldPrintInfo)
            {
                printf("\n");
            }
            return 1;
        }
    }
    if (shouldPrintInfo)
    {
        printf("\n");
    }
    return 0;
}

LBFGS::LBFGS(void)
{
}

LBFGS::~LBFGS(void)
{
}

double LBFGS::Optimize(
        const IOptimizable& function,
        ICallable* _eventHandler,
        const vector<double>& initialPoint, 
        vector<double>* minPoint, int maxIter,
        bool printInfo)
{
    shouldPrintInfo = printInfo;
    eventHandler = _eventHandler;
    currentTime = omp_get_wtime();

    currentIteration = 0;
    maxIteration = maxIter;
    int dimension = function.GetDimension();
    lbfgsfloatval_t* startPoint = lbfgs_malloc(dimension);
    assert(static_cast<size_t>(dimension) == initialPoint.size());
    for (int it = 0; it < dimension; ++it)
    {
        startPoint[it] = initialPoint[it];
    }
    lbfgsfloatval_t fx;

    // Initialize the parameters for the L-BFGS optimization.
    lbfgs_parameter_t param;
    lbfgs_parameter_init(&param);

    // Start the L-BFGS optimization; this will invoke the callback functions
    // evaluate() and progress() when necessary.
    lbfgsfloatval_t ret = lbfgs(
        dimension, startPoint, &fx, LBFGS::Evaluate, LBFGS::Progress,
        const_cast<void*>(reinterpret_cast<const void*>(&function)), 
        &param);

    minPoint->resize(dimension);
    for (int it = 0; it < dimension; ++it)
    {
        (*minPoint)[it] = startPoint[it];
    }
    return ret;
}
