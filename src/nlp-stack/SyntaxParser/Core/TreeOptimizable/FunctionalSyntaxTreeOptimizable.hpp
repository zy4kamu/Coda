/**
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */

#ifndef FUNCTIONAL_SYNTAX_TREE_OPTIMIZABLE
#define FUNCTIONAL_SYNTAX_TREE_OPTIMIZABLE

#include "IOptimizable.h"
#include "SyntaxTree.h"
#include "TreeOptimizableData.hpp"

#define FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>

#define FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1 FunctionalSyntaxTreeOptimizable< \
    EdgeFeatureCalculator \
    , EdgeWeightCalculator \
    , RootFeatureCalculator \
    , RootWeightCalculator \
    , saveToFile>

namespace SyntaxParser
{

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile = false>
class FunctionalSyntaxTreeOptimizable
    : public Optimization::IOptimizable
    , public TreeOptimizableData<
        EdgeFeatureCalculator
        , EdgeWeightCalculator
        , RootFeatureCalculator
        , RootWeightCalculator
        , saveToFile>
{
public:
    typedef EdgeFeatureCalculator EdgeFeatureCalculatorType;
    typedef EdgeWeightCalculator EdgeWeightCalculatorType;
    typedef RootFeatureCalculator RootFeatureCalculatorType;
    typedef RootWeightCalculator RootWeightCalculatorType;
    typedef TreeOptimizableData<
        EdgeFeatureCalculator, EdgeWeightCalculator
        , RootFeatureCalculator, RootWeightCalculator, saveToFile> TreeOptimizableDataType;
    typedef typename RootFeatureCalculatorType::IntBatch IntBatch;
    typedef typename RootFeatureCalculatorType::StringBatch StringBatch;

    FunctionalSyntaxTreeOptimizable(
        size_t indexInDataSet
        , const wstring& line
        , shared_ptr<Tools::Alphabet> alphabet
        , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator);

    FunctionalSyntaxTreeOptimizable(
        FunctionalSyntaxTreeOptimizable&& tree);

    FunctionalSyntaxTreeOptimizable();

    virtual ~FunctionalSyntaxTreeOptimizable(void);

    double CalculateValue(const vector<double>& parameters);

    void CalculateGradient(const vector<double>& parameters,
        vector<double>* gradient);
private:
    /*-------- Functions that are used for calculating value and gradient --------*/

    double calculateEdgeLogWeight(int firstNode, int secondNode
        , const vector<double>& parameters);

    double calculateRootLogWeight(int rootIndex
        , const vector<double>& parameters);

    double calculateLogWeight(const vector<double>& parameters);

    double calculateLogPartitionFunction(const vector<double>& parameters);

    void calculateGradLogWeight(int firstNode, int secondNode
        , const vector<double>& parameters
        , vector<double>* gradient, double multiplicator);

    void calculateGradLogWeight(int rootIndex
        , const vector<double>& parameters
        , vector<double>* gradient, double multiplicator);

    void calculateGradLogWeight(const vector<double>& parameters,
        vector<double>* gradient);

    void calculateGradLogPartitionFunction(const vector<double>& parameters,
        vector<double>* gradient);
};

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::FunctionalSyntaxTreeOptimizable(
    size_t indexInDataSet
    , const wstring& line
    , shared_ptr<Tools::Alphabet> alphabet
    , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator) :
    TreeOptimizableDataType(
        indexInDataSet
        , line
        , alphabet
        , edgeFeatureCalculator)
{
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::FunctionalSyntaxTreeOptimizable(
    FunctionalSyntaxTreeOptimizable&& tree)
    : TreeOptimizableDataType(std::move(tree))
{
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::~FunctionalSyntaxTreeOptimizable(void)
{
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
double FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateEdgeLogWeight(
    int firstNode, int secondNode, const vector<double>& parameters)
{
    return this->edgeWeightCalculator.CalculateLogWeight(
        this->edgeFeatureIndexes[firstNode][secondNode], parameters);
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
double FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateRootLogWeight(
    int rootIndex, const vector<double>& parameters)
{
    return this->rootWeightCalculator.CalculateLogWeight(
        this->rootFeatureIndexes[rootIndex], parameters);
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
double FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateLogWeight(
    const vector<double>& parameters)
{
    double sum = calculateRootLogWeight(this->rootIndex, parameters);
    for (int it = 0; it < static_cast<int>(this->nodes.size()); ++it)
    {
        for (int jt = 0; jt < static_cast<int>(this->nodes[it].neighbours.size()); ++jt)
        {
            sum += calculateEdgeLogWeight(it, this->nodes[it].neighbours[jt], parameters);
        }
    }
    return sum;
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
double FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::CalculateValue(
    const vector<double>& parameters)
{
    return -calculateLogWeight(parameters) + calculateLogPartitionFunction(parameters);
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
void FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::CalculateGradient(
        const vector<double>& parameters,
        vector<double>* gradient)
{
    calculateGradLogWeight(parameters, gradient);
    calculateGradLogPartitionFunction(parameters, gradient);
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
double FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateLogPartitionFunction(
    const vector<double>& parameters)
{
    double sum = 0;
    size_t size = this->nodes.size();
    for (size_t toIndex = 0; toIndex < size; ++toIndex)
    {
        double rootLogWeight = calculateRootLogWeight(toIndex, parameters);
        double weight = std::exp(rootLogWeight);
        for (size_t fromIndex = 0; fromIndex < size; ++fromIndex)
        {
            if (fromIndex != toIndex)
            {
                double edgeLogWeight = calculateEdgeLogWeight(
                    fromIndex, toIndex, parameters);
                weight += std::exp(edgeLogWeight);
            }
        }
        sum += std::log(weight);
    }
    return  sum;
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
void FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    int firstNode, int secondNode,
    const vector<double>& parameters,
    vector<double>* gradient, double multiplicator)
{
    IntBatch& indexes = this->edgeFeatureIndexes[firstNode][secondNode];
    size_t size = indexes.GetSize();
    for (size_t it = 0; it < size; ++it)
    {
        int index = indexes.GetFeature(it);
        gradient->at(index) -=
            multiplicator * this->edgeWeightCalculator.CalculateLogWeightGradient(
            indexes, parameters, it);
    }
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
void FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    int rootIndex
    , const vector<double>& parameters
    , vector<double>* gradient, double multiplicator)
{
    IntBatch& indexes = this->rootFeatureIndexes[rootIndex];
    size_t size = indexes.GetSize();
    for (size_t featureIndex = 0; featureIndex < size; ++featureIndex)
    {
        int index = indexes.GetFeature(featureIndex);
        gradient->at(index) -=
            multiplicator * this->rootWeightCalculator.CalculateLogWeightGradient(
            indexes, parameters, featureIndex);
    }
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
void FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    const vector<double>& parameters,
    vector<double>* gradient)
{
    for (int it = 0; it < int(this->nodes.size()); ++it)
    {
        for (int jt = 0; jt < int(this->nodes[it].neighbours.size()); ++jt)
        {
            calculateGradLogWeight(it, this->nodes[it].neighbours[jt], parameters, gradient, 1);
        }
    }
    calculateGradLogWeight(this->rootIndex, parameters, gradient, 1);
}

FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE
void FUNCTIONAL_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogPartitionFunction(
    const vector<double>& parameters, vector<double>* gradient)
{
    size_t size = this->nodes.size();
    // calculate root weights
    vector<double> rootWeights(size);
    for (size_t toIndex = 0; toIndex < size; ++toIndex)
    {
        rootWeights[toIndex] = std::exp(calculateRootLogWeight(toIndex, parameters));
    }
    // calculate edge weights
    vector<vector<double> > edgeWeights(size, vector<double>(size));
    for (size_t toIndex = 0; toIndex < size; ++toIndex)
    {
        for (size_t fromIndex = 0; fromIndex < size; ++fromIndex)
        {
            if (fromIndex != toIndex)
            {
                edgeWeights[fromIndex][toIndex] =
                    std::exp(calculateEdgeLogWeight(
                    fromIndex, toIndex, parameters));
            }
        }
    }
    // Calculate gradient
    for (size_t toIndex = 0; toIndex < size; ++toIndex)
    {
        double weight = rootWeights[toIndex];
        for (size_t fromIndex = 0; fromIndex < size; ++fromIndex)
        {
            if (fromIndex != toIndex)
            {
                weight += edgeWeights[fromIndex][toIndex];
            }
        }
        // for edges
        for (size_t fromIndex = 0; fromIndex < size; ++fromIndex)
        {
            double multiplicator = edgeWeights[fromIndex][toIndex] / weight;
            calculateGradLogWeight(fromIndex, toIndex, parameters, gradient, -multiplicator);
        }
        // for root
        double multiplicator = rootWeights[toIndex] / weight;
        calculateGradLogWeight(toIndex, parameters, gradient, -multiplicator);
    }
}

};

#endif // FUNCTIONAL_SYNTAX_TREE_OPTIMIZABLE

