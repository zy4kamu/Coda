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

#ifndef SIMPLE_SYNTAX_TREE_OPTIMIZABLE
#define SIMPLE_SYNTAX_TREE_OPTIMIZABLE

#include "IOptimizable.h"
#include "SyntaxTree.h"
#include "TreeOptimizableData.hpp"

#define SIMPLE_TREE_OPTIMIZABLE_TEMPLATE template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>

#define SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1 SimpleSyntaxTreeOptimizable< \
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
class SimpleSyntaxTreeOptimizable
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

    SimpleSyntaxTreeOptimizable(
        size_t indexInDataSet
        , const wstring& line
        , shared_ptr<Tools::Alphabet> alphabet
        , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator);

    SimpleSyntaxTreeOptimizable(SimpleSyntaxTreeOptimizable&& tree);

    virtual ~SimpleSyntaxTreeOptimizable(void);

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

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::SimpleSyntaxTreeOptimizable(
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

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::SimpleSyntaxTreeOptimizable(
    SimpleSyntaxTreeOptimizable&& tree)
    : TreeOptimizableDataType(std::move(tree))
{
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::~SimpleSyntaxTreeOptimizable(void)
{
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
double SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateEdgeLogWeight(
    int firstNode, int secondNode, const vector<double>& parameters)
{
    return this->edgeWeightCalculator.CalculateLogWeight(
        this->edgeFeatureIndexes[firstNode][secondNode], parameters);
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
double SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateRootLogWeight(
    int rootIndex, const vector<double>& parameters)
{
    return this->rootWeightCalculator.CalculateLogWeight(
        this->rootFeatureIndexes[rootIndex], parameters);
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
double SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateLogWeight(
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

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
double SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::CalculateValue(
    const vector<double>& parameters)
{
    return -calculateLogWeight(parameters) + calculateLogPartitionFunction(parameters);
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
void SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::CalculateGradient(
        const vector<double>& parameters,
        vector<double>* gradient)
{
    calculateGradLogWeight(parameters, gradient);
    calculateGradLogPartitionFunction(parameters, gradient);
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
double SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateLogPartitionFunction(
    const vector<double>& parameters)
{
    double sum = 0;
    int size = int(this->nodes.size());
    // Calculate for root
    for (int it = 0; it < size; ++it)
    {
        double rootWeight = calculateRootLogWeight(it, parameters);
        sum += std::log(1.0 + std::exp(rootWeight));
    }
    // Calculate for another edges
    for (int it = 0; it < size; ++it)
    {
        for (int jt = it + 1; jt < size; ++jt)
        {
            double logWeight = calculateEdgeLogWeight(it, jt, parameters);
            double inverseLogWeight = calculateEdgeLogWeight(jt, it, parameters);
            sum += std::log(1.0 + std::exp(logWeight) + std::exp(inverseLogWeight));
        }
    }
    return  sum;
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
void SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    int firstNode, int secondNode, 
    const vector<double>& parameters,
    vector<double>* gradient, double multiplicator)
{ 
    IntBatch& indexes = this->edgeFeatureIndexes[firstNode][secondNode];
    for (size_t it = 0; it < indexes.GetSize(); ++it)
    {
        int index = indexes.GetFeature(it);
        gradient->at(index) -=
            multiplicator * this->edgeWeightCalculator.CalculateLogWeightGradient(
            indexes, parameters, it);
    }
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
void SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    int rootIndex 
    , const vector<double>& parameters
    , vector<double>* gradient, double multiplicator)
{ 
    IntBatch& indexes = this->rootFeatureIndexes[rootIndex];
    for (size_t featureIndex = 0; featureIndex < indexes.GetSize(); ++featureIndex)
    {
        int index = indexes.GetFeature(featureIndex);
        gradient->at(index) -=
            multiplicator * this->rootWeightCalculator.CalculateLogWeightGradient(
            indexes, parameters, featureIndex);
    }
}

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
void SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
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

SIMPLE_TREE_OPTIMIZABLE_TEMPLATE
void SIMPLE_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogPartitionFunction(
    const vector<double>& parameters, vector<double>* gradient)
{
    int size = int(this->nodes.size());
    for (int it = 0; it < size - 1; ++it)
    {
        for (int jt = it + 1; jt < size; ++jt)
        {
            double weight = std::exp(calculateEdgeLogWeight(it, jt, parameters));
            double inverseWeight = std::exp(calculateEdgeLogWeight(jt, it, parameters));
            double multiplicator = weight / (1.0 + weight + inverseWeight);
            double inverseMultiplicator = inverseWeight / (1.0 + weight + inverseWeight);
            calculateGradLogWeight(it, jt, parameters, gradient, -multiplicator);
            calculateGradLogWeight(jt, it, parameters, gradient, -inverseMultiplicator);
        }
    }
    // root
    for (int it = 0; it < size; ++it)
    {
        double weight = std::exp(calculateRootLogWeight(it, parameters));
        double multiplicator = weight / (1.0 + weight);
        calculateGradLogWeight(it, parameters, gradient, -multiplicator);
    }
}

}

#endif // SIMPLE_SYNTAX_TREE_OPTIMIZABLE
