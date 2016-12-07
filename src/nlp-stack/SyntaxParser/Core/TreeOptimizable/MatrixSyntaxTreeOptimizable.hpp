/** 
 * @file	MatrixSyntaxTreeOptimizable.h
 * @brief	header file for Collin's algorithm syntax tree optimizable
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


#ifndef MATRIX_SYNTAXTREE_OPTIMIZABLE
#define MATRIX_SYNTAXTREE_OPTIMIZABLE

#include "IOptimizable.h"
#include "Math/MatrixTools.h"
#include "SyntaxTree.h"
#include "TreeOptimizableData.hpp"
#include "UblasException.h"

#define M_TREE_OPTIMIZABLE_TEMPLATE template < \
    class EdgeFeatureCalculator \
    ,class EdgeWeightCalculator \
    ,class RootFeatureCalculator \
    ,class RootWeightCalculator \
    , bool saveToFile>

#define M_TREE_OPTIMIZABLE_TEMPLATE1 MatrixSyntaxTreeOptimizable< \
    EdgeFeatureCalculator \
    , EdgeWeightCalculator \
    , RootFeatureCalculator \
    , RootWeightCalculator \
    , saveToFile>

namespace SyntaxParser
{

/** 
* @class	MatrixSyntaxTreeOptimizable
* @brief	Optimizable syntax tree via matrix tree theorem
 */
template < \
    class EdgeFeatureCalculator \
    ,class EdgeWeightCalculator \
    ,class RootFeatureCalculator \
    ,class RootWeightCalculator \
    , bool saveToFile = false>
class MatrixSyntaxTreeOptimizable 
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

    /**
     * @brief	Constructor
     * @param[in]	line - special string representation of syntax tree
     * @param[in]	alphabet - contains feature to index correspondence (should be updated after adding to the collection)
     */
    MatrixSyntaxTreeOptimizable(
        size_t indexInDataSet
        , const wstring& line
        , shared_ptr<Tools::Alphabet> alphabet
        , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator);

    MatrixSyntaxTreeOptimizable(
        MatrixSyntaxTreeOptimizable&& tree);

    /**
     * @brief	Destructor
     */
    virtual ~MatrixSyntaxTreeOptimizable(void) {};

    /**
     * @brief	Calculates log weight of the tree
     */
    double CalculateValue(const vector<double>& parameters);

    /**
     * @brief	Calculates gradient of the log weight
     */
    void CalculateGradient(const vector<double>& parameters
        , vector<double>* gradient);

    /**
     * @brief	verifies if there were mistakes while evaluating this tree
     */
    bool IsValid() const
    {
        return !isTreeTerminated;
    }
private:
    /*-------- Data used for calculations value and gradient -------*/
    vector<vector<double> > currentEdgeLogWeights; /**< log weights for each edge in the full connected graph*/
    vector<vector<double> > currentEdgeExpWeights; /**< weights for each edge in the full connected graph*/
    vector<double> currentRootLogWeights; /**< log weights for each root pretendent in the full connected graph*/
    vector<double> currentRootExpWeights; /**< weights for each root pretendent in the full connected graph*/
    matrix<double> inversedPartitionFunctionMartix; /**< special matrix used for calculating gradient*/
    double currentLogPartitionFunction; /**< log partition function*/
    bool isTreeTerminated; /**< vierifies if there was exception during learning of this tree*/

    /*-------- Functions that are used for calculating value and gradient --------*/

    /**
     * @brief	calculates log weight of the edge
     * @param[in]	firstNode - index of the left node
     * @param[in]	secondNode - index of the right node
     * @param[in]	parameters - parameters of the model
     * @param[out]	logWeight - log weight of the edge
     */
    double calculateEdgeLogWeight(
        int firstNode, int secondNode, const vector<double>& parameters);

    /**
     * @brief	calculates log weight of the root pretendent
     * @param[in]	rootIndex - index of the root pretendent
     * @param[in]	parameters - parameters of the model
     */
    double calculateRootLogWeight(
        int rootIndex, const vector<double>& parameters);

    /**
     * @brief	calculates part of gradient which appears in nominator
     * @param[in]	parameters - parameters of the model
     * @param[in]	gradient - gradient to update
     */
    void calculateGradLogWeight(const vector<double>& parameters
        , vector<double>* gradient);

    /**
     * @brief	calculates part of gradient which appears in nominator for one edge
     * @param[in]	firstNode - head node of edge
     * @param[in]	secondNode - modifier  node of edge
     * @param[in]	parameters - parameters of the model
     * @param[in]	gradient - gradient to update
     * @param[in]	multiplicator - weight to add
     */
    void calculateGradLogWeight(int firstNode, int secondNode 
        , const vector<double>& parameters
        , vector<double>* gradient, double multiplicator);

    /**
     * @brief	calculates part of gradient which appears in nominator for one root pretendent
     * @param[in]	rootIndex - index of root pretendent
     * @param[in]	parameters - parameters of the model
     * @param[in]	gradient - gradient to update
     * @param[in]	multiplicator - weight to add
     */
    void calculateGradLogWeight(int rootIndex, 
        const vector<double>& parameters
        , vector<double>* gradient, double multiplicator);

    /**
     * @brief	calculates part of value which appears in nominator
     */
    double calculateLogWeight(const vector<double>& parameters);

    /**
     * @brief	before calculating value and gradient it calculates: weight and log weight matrices, inverse of partition function matrix, etc.
     * @param[in]	parameters - parameters of the model
     */
    void makeGradientAndValuePreparations(
        const vector<double>& parameters);

    /**
     * @brief	after calculations of gradient and value deletes all auxiliary data to decrease memory space
     */
    void clearCalculationData();

    /**
     * @brief	calculates weights and log weights for edges
     * @param[in]	parameters - parameters of the model
     */
    void calculateCurrentEdgeLogAndExpWeights(
        const vector<double>& parameters);

    /**
     * @brief	calculates weights and log weights for root pretendents
     * @param[in]	parameters - parameters of the model
     */
    void calculateCurrentRootLogAndExpWeights(
        const vector<double>& parameters);

    /**
     * @brief	calculates special matrix which first row consists of 
     *          root pretendents weights and other rows are transition weights
     * @param[out]	partititionFunctionMatrix - matrix to calculate
     * @param[in]	parameters - parameters of the model
     */
    void calculatePartitionFunctionMatrix(
        matrix<double>* partititionFunctionMatrix
        , const vector<double>& parameters);

    /**
     * @brief	calculates gradient of partitition function part
     * @param[in]	parameters - parameters of the model
     * @param[out]	gradient - gradient to update
     */
    void calculateGradPartitionFunction(
        const vector<double>& parameters
        , vector<double>* gradient);
};

M_TREE_OPTIMIZABLE_TEMPLATE
M_TREE_OPTIMIZABLE_TEMPLATE1::MatrixSyntaxTreeOptimizable(
    size_t indexInDataSet
    , const wstring& line
    , shared_ptr<Tools::Alphabet> alphabet
    , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator)
    : TreeOptimizableDataType(
        indexInDataSet
        , line
        , alphabet
        , edgeFeatureCalculator)
    , isTreeTerminated(false)
{
}

M_TREE_OPTIMIZABLE_TEMPLATE
M_TREE_OPTIMIZABLE_TEMPLATE1::MatrixSyntaxTreeOptimizable(
    MatrixSyntaxTreeOptimizable&& tree)
    : TreeOptimizableDataType(std::move(tree))
{
}

M_TREE_OPTIMIZABLE_TEMPLATE
double M_TREE_OPTIMIZABLE_TEMPLATE1::calculateEdgeLogWeight(
    int firstNode, int secondNode, const vector<double>& parameters)
{
    return this->edgeWeightCalculator.CalculateLogWeight(
        this->edgeFeatureIndexes[firstNode][secondNode], parameters);
}

M_TREE_OPTIMIZABLE_TEMPLATE
double M_TREE_OPTIMIZABLE_TEMPLATE1::calculateRootLogWeight(
    int rootIndex, const vector<double>& parameters)
{
    return this->rootWeightCalculator.CalculateLogWeight(
        this->rootFeatureIndexes[rootIndex], parameters);
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    const vector<double>& parameters
    , vector<double>* gradient)
{
    // Calculate gradient between edges
    for (int leftNodeIndex = 0
        ; leftNodeIndex < int(this->nodes.size())
        ; ++leftNodeIndex)
    {
        for (int rightNodeIndex = 0
            ; rightNodeIndex < int(this->nodes[leftNodeIndex].neighbours.size())
            ; ++rightNodeIndex)
        {
            calculateGradLogWeight(
                leftNodeIndex
                , this->nodes[leftNodeIndex].neighbours[rightNodeIndex]
                , parameters, gradient, 1);
        }
    }
    // Calculate gradient for roots
    calculateGradLogWeight(this->rootIndex, parameters, gradient, 1);
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
    int firstNode, int secondNode 
    , const vector<double>& parameters
    , vector<double>* gradient, double multiplicator)
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

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradLogWeight(
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

M_TREE_OPTIMIZABLE_TEMPLATE
double M_TREE_OPTIMIZABLE_TEMPLATE1::calculateLogWeight(
    const vector<double>& parameters)
{
    // Set value as for root;
    double sum = this->currentRootLogWeights[this->rootIndex];
    // Calculate value for edges
    for (int leftNodeIndex = 0
        ; leftNodeIndex < static_cast<int>(this->nodes.size())
        ; ++leftNodeIndex)
    {
        for (int rightNodeIndex = 0
            ; rightNodeIndex < static_cast<int>(this->nodes[leftNodeIndex].neighbours.size())
            ; ++rightNodeIndex)
        {
            int rightNeigbour = this->nodes[leftNodeIndex].neighbours[rightNodeIndex];
            sum += currentEdgeLogWeights[leftNodeIndex][rightNeigbour];
        }
    }
    return sum;
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::CalculateGradient(
    const vector<double>& parameters
    , vector<double>* gradient)
{
    if (isTreeTerminated)
    {
        return;
    }
    makeGradientAndValuePreparations(parameters);
    if (!isTreeTerminated)
    {
        calculateGradLogWeight(parameters, gradient);
        calculateGradPartitionFunction(parameters, gradient);
    }
}

M_TREE_OPTIMIZABLE_TEMPLATE
double M_TREE_OPTIMIZABLE_TEMPLATE1::CalculateValue(
    const vector<double>& parameters)
{
    if(isTreeTerminated)
    {
        return 0;
    }
    double value 
        = -calculateLogWeight(parameters) 
        + currentLogPartitionFunction;
    clearCalculationData();
    return value;
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::makeGradientAndValuePreparations(
    const vector<double>& parameters)
{
    int size = SyntaxTree::GetSize();
    // Calcualate needed data;
    calculateCurrentEdgeLogAndExpWeights(parameters);
    calculateCurrentRootLogAndExpWeights(parameters);
    // Calculate partition function matrix;
    matrix<double> partititionFunctionMatrix(size, size);
    calculatePartitionFunctionMatrix(
        &partititionFunctionMatrix, parameters);
    // Inverse partition function matrix and calculate partititon function;
    inversedPartitionFunctionMartix.resize(size, size);
    try
    {
        if (!CalculateLogDeterminantAndInverseMatrix(partititionFunctionMatrix
            , &inversedPartitionFunctionMartix
            , &currentLogPartitionFunction))
        {
            throw UblasException();
        }
    }
    catch(...)
    {
        std::wcout << L"UBLAS exception" << std::endl;
        isTreeTerminated = true;
    }
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::clearCalculationData()
{
    currentEdgeLogWeights.clear();
    currentRootLogWeights.clear();
    currentEdgeExpWeights.clear();
    currentRootExpWeights.clear();
    inversedPartitionFunctionMartix.clear();
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateCurrentEdgeLogAndExpWeights(
    const vector<double>& parameters)
{
    int size = SyntaxTree::GetSize();
    currentEdgeLogWeights.resize(size);
    currentEdgeExpWeights.resize(size);
    for (int leftNodeIndex = 0; leftNodeIndex < size; ++leftNodeIndex)
    {
        currentEdgeLogWeights[leftNodeIndex].resize(size);
        currentEdgeExpWeights[leftNodeIndex].resize(size);
        for (int rightNodeIndex = 0; rightNodeIndex < size; ++rightNodeIndex)
        {
            if (leftNodeIndex != rightNodeIndex)
            {
                currentEdgeLogWeights[leftNodeIndex][rightNodeIndex] 
                    = calculateEdgeLogWeight(
                        leftNodeIndex, rightNodeIndex, parameters);
                currentEdgeExpWeights[leftNodeIndex][rightNodeIndex]
                    = std::exp(currentEdgeLogWeights[leftNodeIndex][rightNodeIndex]);
            }
        }
    }
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateCurrentRootLogAndExpWeights(
        const vector<double>& parameters)
{
    int size = SyntaxTree::GetSize();
    currentRootLogWeights.resize(size);
    currentRootExpWeights.resize(size);
    for (int rootIndex = 0; rootIndex < size; ++rootIndex)
    {
        currentRootLogWeights[rootIndex] = calculateRootLogWeight(
            rootIndex, parameters);
        currentRootExpWeights[rootIndex] = std::exp(
            currentRootLogWeights[rootIndex]);
    }
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculatePartitionFunctionMatrix(
        matrix<double>* partititionFunctionMatrix
        , const vector<double>& parameters)
{
    int size = SyntaxTree::GetSize();
    // Set non-diagonal elements
    for (int headerIndex = 0; headerIndex < size; ++headerIndex)
    {
        for (int modifierIndex = 0; modifierIndex < size; ++modifierIndex)
        {
            if (headerIndex != modifierIndex)
            {
                (*partititionFunctionMatrix)(headerIndex, modifierIndex) 
                    = -currentEdgeExpWeights[headerIndex][modifierIndex];
            }
        }
    }
    // Set diagonal elements
    for (int modifierIndex = 1; modifierIndex < size; ++modifierIndex)
    {
        double& currentDiagonalElement 
            = (*partititionFunctionMatrix)(modifierIndex, modifierIndex);
        currentDiagonalElement = 0;
        for (int headIndex = 0; headIndex < size; ++headIndex)
        {
            if (headIndex != modifierIndex)
            {
                currentDiagonalElement -= (*partititionFunctionMatrix)(
                    headIndex, modifierIndex);
            }
        }
    }
    // Replace first raw
    for (int modifierIndex = 0; modifierIndex < size; ++modifierIndex)
    {
        (*partititionFunctionMatrix)(0, modifierIndex) = 
            currentRootExpWeights[modifierIndex];
    }
}

M_TREE_OPTIMIZABLE_TEMPLATE
void M_TREE_OPTIMIZABLE_TEMPLATE1::calculateGradPartitionFunction(
    const vector<double>& parameters
    , vector<double>* gradient)
{
    size_t size = this->nodes.size();
    // Calculate gradient for first row
    for (size_t headIndex = 0; headIndex < size; ++headIndex)
    {
        double multiplicator = 
            currentRootExpWeights[headIndex] 
            * inversedPartitionFunctionMartix(headIndex, 0);
        calculateGradLogWeight(headIndex, parameters
            , gradient, -multiplicator);
    }
    // Calculate gradient for NOT first row
    for (size_t headIndex = 1; headIndex < size; ++headIndex)
    {
        for (size_t modifierIndex = 0; modifierIndex < size; ++modifierIndex)
        {
            double matrixValue = inversedPartitionFunctionMartix(
                modifierIndex, headIndex);
            // Calculate for diagonal elements
            if (headIndex == modifierIndex)
            {
                for (size_t sumIndex = 0; sumIndex < size; ++sumIndex)
                {
                    if (sumIndex == headIndex) {
                        continue;
                    } else {
                        double multiplicator = matrixValue 
                            * currentEdgeExpWeights[sumIndex][modifierIndex];
                        calculateGradLogWeight(sumIndex, headIndex 
                            , parameters, gradient, -multiplicator);
                    }
                }
            // Calculate for non-diagonal elements
            } else {
                double multiplicator = matrixValue 
                    * currentEdgeExpWeights[headIndex][modifierIndex];
                calculateGradLogWeight(headIndex, modifierIndex 
                    , parameters, gradient, multiplicator);
            }
        }
    }
}

};

#endif // MATRIX_SYNTAXTREE_OPTIMIZABLE
