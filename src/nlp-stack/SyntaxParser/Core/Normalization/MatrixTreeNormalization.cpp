#include "MatrixTreeNormalization.h"

namespace SyntaxParser
{

void MatrixTreeNormalization::Normalize(
    vector<vector<double> >* edgeLogWeights,
    vector<double>* rootLogWeights)
{
    normalizeRootWeights(rootLogWeights);
    normalizeEdgeWeights(edgeLogWeights, 0);
    getProbabilities(edgeLogWeights, rootLogWeights);
}

void MatrixTreeNormalization::normalizeRootWeights(
    vector<double>* rootLogWeights)
{
    double sum = 0;
    for (size_t nodeIndex = 0; nodeIndex < rootLogWeights->size(); ++nodeIndex)
    {
        (*rootLogWeights)[nodeIndex] = std::exp((*rootLogWeights)[nodeIndex]);
        sum += (*rootLogWeights)[nodeIndex];
    }
    for (size_t nodeIndex = 0; nodeIndex < rootLogWeights->size(); ++nodeIndex)
    {
        (*rootLogWeights)[nodeIndex] /= sum;
    }
}

void MatrixTreeNormalization::normalizeEdgeWeights(
    vector<vector<double> >* edgeLogWeights,
    double regularizationFactor)
{
    size_t size = edgeLogWeights->size();
    vector<double> sums(size);

    for (size_t leftNode = 0; leftNode < size; ++leftNode)
    {
        for (size_t rightNode = 0; rightNode < size; ++rightNode)
        {
            if (leftNode != rightNode)
            {
                (*edgeLogWeights)[leftNode][rightNode] =
                    std::exp((*edgeLogWeights)[leftNode][rightNode]) + regularizationFactor;
                sums[rightNode] += (*edgeLogWeights)[leftNode][rightNode];
            }
        }
    }

    for (size_t leftNode = 0; leftNode < size; ++leftNode)
    {
        for (size_t rightNode = 0; rightNode < size; ++rightNode)
        {
            if (leftNode != rightNode)
            {
                (*edgeLogWeights)[leftNode][rightNode] /=
                    sums[rightNode];
            }
        }
    }
}

matrix<double> MatrixTreeNormalization::getMatrix(
    const vector<vector<double> >& edgeExpWeights,
    const vector<double>& rootExpWeights)
{
    size_t size = rootExpWeights.size();
    matrix<double> data(size, size);
    // Set non-diagonal elements
    for (size_t headerIndex = 0; headerIndex < size; ++headerIndex)
    {
        for (size_t modifierIndex = 0; modifierIndex < size; ++modifierIndex)
        {
            if (headerIndex != modifierIndex)
            {
                data(headerIndex, modifierIndex)
                    = -edgeExpWeights[headerIndex][modifierIndex];
            }
        }
    }
    // Set diagonal elements
    for (size_t modifierIndex = 1; modifierIndex < size; ++modifierIndex)
    {
        double& currentDiagonalElement
            = data(modifierIndex, modifierIndex);
        currentDiagonalElement = 0;
        for (size_t headIndex = 0; headIndex < size; ++headIndex)
        {
            if (headIndex != modifierIndex)
            {
                currentDiagonalElement -= data(
                    headIndex, modifierIndex);
            }
        }
    }
    // Replace first raw
    for (size_t modifierIndex = 0; modifierIndex < size; ++modifierIndex)
    {
        data(0, modifierIndex) =
            rootExpWeights[modifierIndex];
    }
    return data;
}

void MatrixTreeNormalization::getProbabilities(
    vector<vector<double> >* edgeWeights,
    vector<double>* rootWeights)
{
    size_t size = rootWeights->size();
    matrix<double> matr = getMatrix(*edgeWeights, *rootWeights);
    matrix<double> inversedMartix(size, size);
    InvertMatrix(matr, &inversedMartix);
    // Edge probabilities (first row)
    (*edgeWeights)[0][0] = 0;
    for (size_t colIndex = 1; colIndex < size; ++colIndex)
    {
        (*edgeWeights)[0][colIndex] *=
            inversedMartix(colIndex, colIndex);
    }
    // Edge probabilities (first column)
    for (size_t rowIndex = 1; rowIndex < size; ++rowIndex)
    {
        (*edgeWeights)[rowIndex][0] *=
            -inversedMartix(0, rowIndex);
    }
    // Edge probabilities (other rows)
    for (size_t rowIndex = 1; rowIndex < size; ++rowIndex)
    {
        for (size_t colIndex = 1; colIndex < size; ++colIndex)
        {
            if (rowIndex == colIndex) {
                (*edgeWeights)[rowIndex][colIndex] = 0;
            } else {
                (*edgeWeights)[rowIndex][colIndex] *=
                    -inversedMartix(colIndex, rowIndex) +
                    inversedMartix(colIndex, colIndex);
            }
        }
    }
    // Root probabilities
    for (size_t rootIndex = 0; rootIndex < size; ++rootIndex)
    {
        (*rootWeights)[rootIndex] *= inversedMartix(rootIndex, 0);
    }
    // Take log
    for (size_t rootIndex = 0; rootIndex < size; ++rootIndex)
    {
        (*rootWeights)[rootIndex] = std::log((*rootWeights)[rootIndex]);
    }
    for (size_t rowIndex = 0; rowIndex < size; ++rowIndex)
    {
        for (size_t colIndex = 0; colIndex < size; ++colIndex)
        {
            if (rowIndex == colIndex) {
                (*edgeWeights)[rowIndex][colIndex] = -10000000;
            } else {
                (*edgeWeights)[rowIndex][colIndex] =
                    std::log((*edgeWeights)[rowIndex][colIndex]);
            }
        }
    }
}

}
