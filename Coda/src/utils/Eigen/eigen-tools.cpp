/**
 * @file	eigen-tools.cpp
 * @brief	source file containing all header with eigen API and tools functions
 */

#include "eigen-tools.h"

using namespace eigentools;

ScalarType eigentools::cosineMeasure(DenseMat &a,DenseMat &b)
{
    if (a.cols()!=b.cols() || a.rows()!=b.rows())
        return (ScalarType)0;
    if (a.cols()==1 || a.rows() == 1)
    {
        eigentools::ScalarType rval = a.cwiseProduct(b).array().sum();
        if (!a.norm() || !b.norm())
        {
            std::cerr<<"Norm of vector is 0\n";
            return 0;
        }
        rval /= a.norm();
        rval /= b.norm();
        return rval;
    }
    else
        return 0;
}
