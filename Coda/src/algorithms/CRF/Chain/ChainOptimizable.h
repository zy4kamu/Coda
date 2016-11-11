/** 
 * @file	Chain.h
 * @brief	header file of the class that represents data for CRF-learning (one sentence)
 *          in digital format and enables to calculate value and gradient
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


#ifndef CHAIN_OPTIMIZABLE
#define CHAIN_OPTIMIZABLE

#include <memory>
#include <unordered_map>
#include <fstream>
#include <exception>
#include <set>

#include "Chain.h"
#include "IPossibleStateFinder.h"
#include "VectorFactorsExtractor.h"
#include "IOptimizable.h"
#include "CrossPlatformHeader.h"

typedef unordered_map<int, unordered_map<int, int> > TransitionConverter;
typedef vector<vector<vector<double> > > TransitionWeighter; // layer, leftNodeIndex, rightNodeIndex

using std::shared_ptr;
using std::wofstream;
using std::exception;
using std::set;

#define CHAIN_OPTIMIZABLE_TEMPLATE template < \
    ConsideringOfFeaturesInCliques typeOfFeaturesConsideration>

#define CHAIN_OPTIMIZABLE_TEMPLATE1 ChainOptimizable< \
    typeOfFeaturesConsideration>

namespace LinearCRF
{

enum ConsideringOfFeaturesInCliques
{
    USE_UNPRESENTED_IN_CLIQUES_FEATURES
    , DONT_USE_UNPRESENTED_IN_CLIQUES_FEATURES
};

/** 
 * @class	ChainOptimizable
 * @brief	header file of the class that represents data for CRF-learning (one sentence)
 *          in digital format and enables to calculate value and gradient
 */
CHAIN_OPTIMIZABLE_TEMPLATE
class ChainOptimizable
{
public:
    /**
	 * @brief	Constructor that transfers chain in string representation to digital
     *          and updates alphabets for training
     * @param[in]	transitionConverter - converts tranisition (input 
                    index to output index) to index of weightAlphabet
     * @param[in]	vectorFactorsExtractor - enables to find CRF parameters from vector
	 * @param[in]	weightAlphabet - cliques of CRF
     * @param[in]	inputAlphabet - features for CRF
     * @param[in]	outputAlphabet - hidden labels of CRF
	 */
    ChainOptimizable(
        shared_ptr<TransitionConverter> transitionConverter,
        shared_ptr<VectorFactorsExtractor> vectorFactorsExtractor,
        shared_ptr<Alphabet> weightAlphabet,
        shared_ptr<Alphabet> inputAlphabet,
        shared_ptr<Alphabet> outputAlphabet,
        const Chain& chain);

    /**
	 * @brief	Destructor
	 */
    ~ChainOptimizable(void);

    /**
	 * @brief	Prints chain in the form that enables to check correctness of pipe and possible state finder
     *          (token, label, possible states)
	 * @param[in]	out - output stream
	 */
    wofstream& PrintForChecking(wofstream& out);

    /**
	 * @brief	Adds new possible features for clique
	 * @param[in]	cliquePossibleFeatures - container to update, contains possible features for each clique
	 */
    void UpdateCliquePossibleFeatures(unordered_map<int, unordered_set<int> >* 
        cliquePossibleFeatures);

    /**
	 * @brief	Calculate value and updates gradient under given parameters of the model (returns value)
	 * @param[in]	parameters - parameters of the model
     * @param[out]	gradient - updated gradient
	 */
    double CalculateValueAndGradient(
        const vector<double>& parameters
        , vector<double>* gradient);
private:
    shared_ptr<TransitionConverter> transitionConverter;  /**< converts tranisition (input index to output index) to index of weightAlphabet*/
    shared_ptr<VectorFactorsExtractor> vectorFactorsExtractor; /**< enables to find CRF parameters from vector*/
    shared_ptr<Alphabet> weightAlphabet; /**< cliques of CRF*/
    shared_ptr<Alphabet> inputAlphabet; /**< features for CRF*/
    shared_ptr<Alphabet> outputAlphabet; /**< hidden labels of CRF*/
    
    vector<int> cliqueIndexes; /**< indexes of the cliques for the current chain*/
    vector<int> labelIndexes; /**< indexes of the labels for the current chain*/
    vector<int> labelIndexesInPossibleStates; /**< contains index in possibleStates for each label in the chain*/
    vector<vector<int> > featureIndexes; /**< indexes of the features for the current chain*/
    vector<vector<int> > possibleStates; /**< indexes of the possible states calculate using features*/

    vector<vector<double> > logForwardWeights; /**< forward weights on chain*/
    vector<vector<double> > logBackwardWeights; /**< backward weights on chain*/

    TransitionWeighter transitionWeights; /**< contains weights of the transition between two nodes in chain*/
    TransitionWeighter logTransitionWeights; /**< contains log weights of the transition between two nodes in chain*/
    vector<double> initialWeights; /**< contains initial weights*/
    vector<double> logInitialWeights; /**< contains log initial weights*/
    vector<double> finalWeights; /**< contains final weights*/
    vector<double> logFinalWeights; /**< contains log final weights*/

    double logPartitionFunction; /**< partitition function*/

    /**
     * @brief	Calculates forward/backward weights and partition function
     *          (they both are used for calculating value and gradient)
     * @param[in]	factorsVector - vector of CRF parameters
     * @param[out]	true if there were some calculations during this call
     */
    bool makePreparationsBeforeValueAndGradientCalculating(
        const vector<double>& factorsVector);

    /**
     * @brief	Calculates partition function
     */
    void calculatePartitionFunction();

    /**
	 * @brief	Constructs labelIndexesInPossibleStates
	 */
    void prepareLabelIndexesInPossibleStates();

    /**
	 * @brief	Given possible states initializes forwardWeights and backwardWeights
	 */
    void initializeForwardAndBackwardWeights();

    /**
	 * @brief	Calculate forward weights for calculating value and gradient
	 */
    void calculateForwardWeights();

    /**
	 * @brief	Calculate backward weights for calculating value and gradient
	 */
    void calculateBackwardWeights();

    /**
	 * @brief	Updates gradient for initial weights
     * @param[in]	gradient - gradient which we update
	 */
    void calculateGradientForInitialWeights(
        vector<double>* gradient);

    /**
	 * @brief	Updates gradient for final weights
     * @param[in]	gradient - gradient which we update
	 */
    void calculateGradientForFinalWeights(
        vector<double>* gradient);

    /**
	 * @brief	Updates gradient for probability without partition function
     * @param[in]	gradient - gradient which we update
	 */
    void calculateGradientForNumeratorOfProbability(
        vector<double>* gradient);

    /**
	 * @brief	Updates gradient for partitition function
     * @param[in]	gradient - gradient which we update
	 */
    void calculateGradientForLogPartitionFunction(
        vector<double>* gradient);

    /**
	 * @brief	Calculates weight of transition between two nodes in the chain
     * @param[in]	chainNodeIndex - index of the node in chain
     * @param[in]	fromLabelIndex - index of left node in outputAlphabet
     * @param[in]	toLabelIndex - index of right node in outputAlphabet
     * @param[in]	factorsVector - vector of CRF parameters
	 */
    double calculateWeightTransition(int chainNodeIndex, 
        int fromLabelIndex, int toLabelIndex,
        const vector<double>& factorsVector);

    /**
     * @brief	Calculates weights of transition between each two nodes in the chain
     * @param[in]	factorsVector - vector of CRF parameters
     */
    void calculateAllWeights(
            const vector<double>& factorsVector);

    /**
	 * @brief	An auxiliary function. Calculates gradient using only function get value.
     *          Never use it in production (only for debugging)
     * @param[in]	chainNodeIndex - index of the node in chain
	 */
    vector<double> calculateHardGradient(
        const vector<double>& factorsVector);

    /**
    * @brief	Calculates log(sum(weights[i] * exp(logs(i)))
    * @param[in]	logs - logarifms to process
    * @param[in]	weights - weights of logs
    */
    double calculateLogSumExp(
        const vector<double>& logs
        , const vector<double>& weights);
};

class WrongPossibleStatesException : public exception
{
public:
    const char* what() const NOEXCEPT
    {
        return "There was not found node in the list of possible states !\n";
    }
};

CHAIN_OPTIMIZABLE_TEMPLATE
double CHAIN_OPTIMIZABLE_TEMPLATE1::calculateLogSumExp(
    const vector<double>& logs, const vector<double>& weights)
{
    if (logs.empty())
    {
        return MINUSE_INFINITY;
    }
    double max = MINUSE_INFINITY;
    for (size_t iter = 0; iter < logs.size(); ++iter)
    {
        if (logs[iter] > max)
        {
            max = logs[iter];
        }
    }
    double expPart = 0;
    for (size_t iter = 0; iter < logs.size(); ++iter)
    {
        expPart += std::exp(logs[iter] - max) * weights[iter];
    }
    return max + std::log(expPart);
}

CHAIN_OPTIMIZABLE_TEMPLATE
CHAIN_OPTIMIZABLE_TEMPLATE1::ChainOptimizable(
    shared_ptr<TransitionConverter> transitionConverter,
    shared_ptr<VectorFactorsExtractor> vectorFactorsExtractor,
    shared_ptr<Alphabet> weightAlphabet,
    shared_ptr<Alphabet> inputAlphabet,
    shared_ptr<Alphabet> outputAlphabet,
    const Chain& chain) : 
    transitionConverter(transitionConverter),
    vectorFactorsExtractor(vectorFactorsExtractor),
    weightAlphabet(weightAlphabet), 
    inputAlphabet(inputAlphabet), 
    outputAlphabet(outputAlphabet)
{
    size_t length = chain.GetSize();
    // Set labels
    const vector<wstring>& labels = chain.GetLabels();
    labelIndexes.resize(length);
    for (size_t labelIndex = 0; labelIndex < length; ++labelIndex)
    {
        labelIndexes[labelIndex] = 
            outputAlphabet->LookUpIndex_Adding(labels[labelIndex]);
    }
    // Set features
    const vector<vector<wstring> >& features = chain.GetFeatures();
    featureIndexes.resize(length);
    for (size_t tokenIndex = 0; tokenIndex < length; ++tokenIndex)
    {
        vector<int>& currentFeatures = featureIndexes[tokenIndex];
        currentFeatures.resize(features[tokenIndex].size());
        for (size_t featureIndex = 0; featureIndex < currentFeatures.size();
            ++featureIndex)
        {
            currentFeatures[featureIndex] = inputAlphabet->LookUpIndex_Adding(
                features[tokenIndex][featureIndex]);
        }
    }
    // Set cliques
    cliqueIndexes.resize(length - 1);
    for (size_t transitionIndex = 0; transitionIndex < length - 1;
        ++transitionIndex)
    {
        wstring transition = labels[transitionIndex] + L"->" + // Attention!!!!
            labels[transitionIndex + 1];
        cliqueIndexes[transitionIndex] = weightAlphabet->LookUpIndex_Adding(transition);
        // Add to transition converter
        int inputIndex = labelIndexes[transitionIndex];
        int outputIndex = labelIndexes[transitionIndex + 1];
        if (transitionConverter->find(inputIndex) != transitionConverter->end())
        {
            (*transitionConverter)[inputIndex][outputIndex] = cliqueIndexes[transitionIndex];
        }
        else
        {
            unordered_map<int, int> toAdd;
            toAdd[outputIndex] = cliqueIndexes[transitionIndex];
            (*transitionConverter)[inputIndex] = toAdd;
        }
    }
    // Set possible states
    const vector<vector<wstring> >& possibleStatesString 
        = chain.GetPossibleStates();
    possibleStates.resize(possibleStatesString.size());
    for (size_t stateIndex = 0; stateIndex < possibleStates.size()
        ; ++stateIndex)
    {
        for (size_t featIndex = 0; featIndex < possibleStatesString[stateIndex].size()
            ; ++featIndex)
        {
            int featureIndex = outputAlphabet->LookUpIndex_Adding(
                possibleStatesString[stateIndex][featIndex]);
            if (featureIndex >= 0) 
            {
                possibleStates[stateIndex].push_back(featureIndex);
            }
        }
    }
    initializeForwardAndBackwardWeights();
    prepareLabelIndexesInPossibleStates();
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::prepareLabelIndexesInPossibleStates()
{
    int length = labelIndexes.size();
    labelIndexesInPossibleStates.resize(length, -1);
    for (int nodeIndex = 0; nodeIndex < length; ++nodeIndex)
    {
        int currentLabel = labelIndexes[nodeIndex];
        const vector<int>& currentPossibleStates = possibleStates[nodeIndex];
        for (size_t possibleIndex = 0; possibleIndex < currentPossibleStates.size(); 
            ++possibleIndex)
        {
            if (currentPossibleStates[possibleIndex] == currentLabel)
            {
                labelIndexesInPossibleStates[nodeIndex] = possibleIndex;
                break;
            }
        }
        if (labelIndexesInPossibleStates[nodeIndex] == -1)
        {
            throw WrongPossibleStatesException();
        }
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
CHAIN_OPTIMIZABLE_TEMPLATE1::~ChainOptimizable(void)
{
}

CHAIN_OPTIMIZABLE_TEMPLATE
wofstream& CHAIN_OPTIMIZABLE_TEMPLATE1::PrintForChecking(wofstream& out)
{
    // Print features, possible states, label 
    int length = labelIndexes.size();
    for (int nodeIndex = 0; nodeIndex < length; ++nodeIndex)
    {
        // Print label
        out << outputAlphabet->LookUpObject(
                labelIndexes[nodeIndex]);
        // Print possible states
        out << L"\t";
        const vector<int>& currentPossibleStates = possibleStates[nodeIndex];
        if (currentPossibleStates.size() > 0)
        {
            out << outputAlphabet->LookUpObject(currentPossibleStates[0]); 
        }
        for (size_t possibleStateIndex = 1; 
            possibleStateIndex < currentPossibleStates.size(); 
            ++possibleStateIndex)
        {
            out << L"," << outputAlphabet->LookUpObject(
                currentPossibleStates[possibleStateIndex]);
        }
        // Print features
        out << L"\t\t\t\t";
        const vector<int>& currentFeatureIndexes = featureIndexes[nodeIndex];
        if (currentFeatureIndexes.size() > 0)
        {
            out << inputAlphabet->LookUpObject(currentFeatureIndexes[0]); 
        }
        for (size_t featIndex = 1; featIndex < currentFeatureIndexes.size();
            ++featIndex)
        {
            out << L"," << inputAlphabet->LookUpObject(
                currentFeatureIndexes[featIndex]);
        }
        out << std::endl;
    }
    return out;
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::initializeForwardAndBackwardWeights()
{
    int chainLength = labelIndexes.size();
    logForwardWeights.resize(chainLength);
    logBackwardWeights.resize(chainLength);
    for (int nodeIndex = 0; nodeIndex < chainLength; ++nodeIndex)
    {
        logForwardWeights[nodeIndex].resize(possibleStates[nodeIndex].size());
        logBackwardWeights[nodeIndex].resize(possibleStates[nodeIndex].size());
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculatePartitionFunction()
{
    const vector<int>& lastPossibleStates = possibleStates.back();
    const vector<double>& lastLayerWeights = logForwardWeights.back();
    // Find max among lastLayerWeights
    double maxLastLayerWeight = MINUSE_INFINITY;
    for (size_t nodeIndex = 0; nodeIndex < lastLayerWeights.size(); ++nodeIndex)
    {
        if (lastLayerWeights[nodeIndex] > maxLastLayerWeight)
        {
            maxLastLayerWeight = lastLayerWeights[nodeIndex];
        }
    }
    // Calculate sum
    double exponentPart = 0;
    for (size_t nodeIndex = 0; nodeIndex < lastPossibleStates.size(); ++nodeIndex)
    {
        exponentPart += std::exp(lastLayerWeights[nodeIndex] - maxLastLayerWeight) 
            * finalWeights[nodeIndex];
    }
    // Calculate partition function
    logPartitionFunction = maxLastLayerWeight + std::log(exponentPart);
}

CHAIN_OPTIMIZABLE_TEMPLATE
double CHAIN_OPTIMIZABLE_TEMPLATE1::CalculateValueAndGradient(
    const vector<double>& factorsVector
    , vector<double>* gradient)
{
    initializeForwardAndBackwardWeights();
    calculateAllWeights(factorsVector);
    calculateForwardWeights();
    calculateBackwardWeights();
    calculatePartitionFunction();
    // value = initialweight + finalweight - log(partitionFunction)
    double value = 
        vectorFactorsExtractor->GetInitialWeight(labelIndexes[0], factorsVector) + 
        vectorFactorsExtractor->GetFinalWeight(labelIndexes.back(), factorsVector) - 
        logPartitionFunction;
    // Add weights of transitions
    for (size_t nodeIndex = 0; nodeIndex < cliqueIndexes.size(); ++nodeIndex)
    {
        value += logTransitionWeights[nodeIndex]
                [labelIndexesInPossibleStates[nodeIndex]]
                [labelIndexesInPossibleStates[nodeIndex + 1]];
    }
    // Calculate gradient
    calculateGradientForNumeratorOfProbability(gradient);
    calculateGradientForLogPartitionFunction(gradient);
    calculateGradientForInitialWeights(gradient);
    calculateGradientForFinalWeights(gradient);
    // Clear parameters (for memory economy)
    transitionWeights.clear();
    logTransitionWeights.clear();
    initialWeights.clear();
    logInitialWeights.clear();
    finalWeights.clear();
    logFinalWeights.clear();
    logForwardWeights.clear();
    logBackwardWeights.clear();

    return -value;
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateGradientForNumeratorOfProbability(
        vector<double>* gradient)
{
    for (size_t nodeIndex = 0; nodeIndex < cliqueIndexes.size(); ++nodeIndex)
    {
        // Evaluate features
        const vector<int>& currentFeatures = featureIndexes[nodeIndex];
        for (size_t iter = 0; iter < currentFeatures.size(); ++iter)
        {
            if (vectorFactorsExtractor->ContainsFeature(
                cliqueIndexes[nodeIndex], currentFeatures[iter]))
            {
                int gradientIndex = vectorFactorsExtractor->GetFeatureWeightIndex(
                        cliqueIndexes[nodeIndex], currentFeatures[iter]);
                --(*gradient)[gradientIndex];
            }
        }
        // Evaluate default weights
        int gradientIndex = vectorFactorsExtractor->GetDefaultWeightIndex(
                    cliqueIndexes[nodeIndex]);
        --(*gradient)[gradientIndex];
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateGradientForLogPartitionFunction(
        vector<double>* gradient)
{
    for (size_t nodeIndex = 0; nodeIndex < cliqueIndexes.size(); ++nodeIndex)
    {
        const vector<int>& currentPossibleStates = possibleStates[nodeIndex];
        const vector<int>& nextPossibleStates = possibleStates[nodeIndex + 1];
        // Iterate over current node
        for (size_t currentIndex = 0; currentIndex < currentPossibleStates.size(); 
            ++currentIndex)
        {
            int currentNode = currentPossibleStates[currentIndex];
            const vector<int>& currentFeatures = featureIndexes[nodeIndex];
            if (transitionConverter->find(currentNode) == transitionConverter->end())
            {
                continue;
            }
            unordered_map<int, int>& currentTransitionMap = transitionConverter->at(currentNode);
            // Iterate over next node
            for (size_t nextIndex = 0; nextIndex < nextPossibleStates.size(); 
                ++nextIndex)
            {
                int nextNode = nextPossibleStates[nextIndex];
                if (currentTransitionMap.find(nextNode) == currentTransitionMap.end())
                {
                    continue;
                }
                int currentClique = currentTransitionMap[nextNode];
                double logCurrentProbability = 
                    logForwardWeights[nodeIndex][currentIndex] 
                    + logBackwardWeights[nodeIndex + 1][nextIndex]
                    + logTransitionWeights[nodeIndex][currentIndex][nextIndex]
                    - logPartitionFunction;
                double currentProbability = std::exp(logCurrentProbability);
                // Update gradient for features
                for (size_t featureIter = 0; featureIter < currentFeatures.size(); 
                    ++featureIter)
                {
                    if (vectorFactorsExtractor->ContainsFeature(
                        currentClique, currentFeatures[featureIter]))
                    {
                        int gradientIndex = vectorFactorsExtractor->GetFeatureWeightIndex(
                            currentClique, currentFeatures[featureIter]);
                        (*gradient)[gradientIndex] += currentProbability;
                    }
                }
                // Update gradient for default weights
                int gradientIndex = vectorFactorsExtractor->GetDefaultWeightIndex(
                    currentClique);
                (*gradient)[gradientIndex] += currentProbability;
            }
        }
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateGradientForInitialWeights(
        vector<double>* gradient)
{
    int gradientIndex = vectorFactorsExtractor->GetInitialWeightIndex(
        labelIndexes[0]);
    --(*gradient)[gradientIndex];
    const vector<int>& initialPossibleStates = possibleStates[0];
    for (size_t iter = 0; iter < initialPossibleStates.size(); ++iter)
    {
        double logBackwardWeight = logBackwardWeights[0][iter];
        double logForwardWeight = logForwardWeights[0][iter];
        gradientIndex = vectorFactorsExtractor->GetInitialWeightIndex(
            initialPossibleStates[iter]);
        double logToAdd = logBackwardWeight + logForwardWeight - logPartitionFunction;
        (*gradient)[gradientIndex] += std::exp(logToAdd);
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateGradientForFinalWeights(
        vector<double>* gradient)
{
    int gradientIndex = vectorFactorsExtractor->GetFinalWeightIndex(
        labelIndexes.back());
    --(*gradient)[gradientIndex];
    const vector<int>& finalPossibleStates = possibleStates.back();
    for (size_t iter = 0; iter < finalPossibleStates.size(); ++iter)
    {
        double logForwardWeight = logForwardWeights.back()[iter];
        double logBackwardWeight = logBackwardWeights.back()[iter];
        gradientIndex = vectorFactorsExtractor->GetFinalWeightIndex(
            finalPossibleStates[iter]);
        double logToAdd = logForwardWeight + logBackwardWeight 
            - logPartitionFunction;
        (*gradient)[gradientIndex] += std::exp(logToAdd);
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateForwardWeights()
{
    int chainLength = labelIndexes.size();
    // Set initial weights;
    logForwardWeights[0] = logInitialWeights;
    // Calculate forward weights
    for (int nodeIndex = 1; nodeIndex < chainLength; ++nodeIndex)
    {
        // Clear current forward weights
        for (size_t iter = 0; iter < logForwardWeights[nodeIndex].size(); ++iter)
        {
            logForwardWeights[nodeIndex][iter] = MINUSE_INFINITY;
        }
        // Get previous and current possible states
        const vector<int>& currentPossibleStates = possibleStates[nodeIndex];
        const vector<int>& previousPossibleStates = possibleStates[nodeIndex - 1];
        // Iterate over current nodes
        for (size_t currentIndex = 0; currentIndex < currentPossibleStates.size(); ++currentIndex)
        {
            int currentStateIndex = currentPossibleStates[currentIndex];
            vector<double> transitionWeightsToAdd;
            vector<double> logForwardWeightsToAdd;
            // Iterate over previous nodes and construct transitionWeightsToAdd
            // and logForwardWeightsToAdd
            for (size_t previousIndex = 0; previousIndex < previousPossibleStates.size();
                ++previousIndex)
            {
                int previousStateIndex = previousPossibleStates[previousIndex];
                if (transitionConverter->find(previousStateIndex) != transitionConverter->end())
                {
                    const unordered_map<int, int>& currentCliquesMap = 
                        transitionConverter->at(previousStateIndex);
                    if (currentCliquesMap.find(currentStateIndex) != currentCliquesMap.end())
                    {
                        transitionWeightsToAdd.push_back(
                            transitionWeights[nodeIndex - 1][previousIndex][currentIndex]);
                        logForwardWeightsToAdd.push_back(
                            logForwardWeights[nodeIndex - 1][previousIndex]);
                    }
                }
            }
            logForwardWeights[nodeIndex][currentIndex] = calculateLogSumExp(
                logForwardWeightsToAdd, transitionWeightsToAdd);
        }
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateBackwardWeights()
{
    int chainLength = labelIndexes.size();
    // Set final weights;
    logBackwardWeights[logBackwardWeights.size() - 1] = logFinalWeights;
    // Calculate backward weights
    for (int nodeIndex = chainLength - 2; nodeIndex >= 0; --nodeIndex)
    {
        // Clear current forward weights
        for (size_t iter = 0; iter < logBackwardWeights[nodeIndex].size(); ++iter)
        {
            logBackwardWeights[nodeIndex][iter] = MINUSE_INFINITY;
        }
        // Get previous and current possible states
        const vector<int>& currentPossibleStates = possibleStates[nodeIndex];
        const vector<int>& previousPossibleStates = possibleStates[nodeIndex + 1];
        // Iterate over current nodes
        for (size_t currentIndex = 0; currentIndex < currentPossibleStates.size(); ++currentIndex)
        {
            int currentStateIndex = currentPossibleStates[currentIndex];
            vector<double> transitionWeightsToAdd;
            vector<double> logBackwardWeightsToAdd;
            // Iterate over previous nodes
            for (size_t previousIndex = 0; previousIndex < previousPossibleStates.size();
                ++previousIndex)
            {
                int previousStateIndex = previousPossibleStates[previousIndex];
                if (transitionConverter->find(currentStateIndex) != transitionConverter->end())
                {
                    const unordered_map<int, int>& currentCliquesMap = 
                        transitionConverter->at(currentStateIndex);
                    if (currentCliquesMap.find(previousStateIndex) != currentCliquesMap.end())
                    {
                        transitionWeightsToAdd.push_back(
                            transitionWeights[nodeIndex][currentIndex][previousIndex]);
                        logBackwardWeightsToAdd.push_back(
                            logBackwardWeights[nodeIndex + 1][previousIndex]);
                    }
                }
            }
            logBackwardWeights[nodeIndex][currentIndex] = calculateLogSumExp(
                logBackwardWeightsToAdd, transitionWeightsToAdd);
        }
    }
}

CHAIN_OPTIMIZABLE_TEMPLATE
double CHAIN_OPTIMIZABLE_TEMPLATE1::calculateWeightTransition(int chainNodeIndex, 
        int fromLabelIndex, int toLabelIndex,
        const vector<double>& factorsVector)
{
    int cliqueIndex = transitionConverter->at(fromLabelIndex)[toLabelIndex];
    const vector<int>& features = featureIndexes[chainNodeIndex];
    double weight = vectorFactorsExtractor->GetDefaultWeight(
        cliqueIndex, factorsVector);
    for (size_t featureIndex = 0; featureIndex < features.size(); ++featureIndex)
    {
        if (vectorFactorsExtractor->ContainsFeature(cliqueIndex, features[featureIndex]))
        {
            weight += vectorFactorsExtractor->GetFeatureWeight(
                cliqueIndex, features[featureIndex], factorsVector);
        }
    }
    return weight;
}

CHAIN_OPTIMIZABLE_TEMPLATE
vector<double> CHAIN_OPTIMIZABLE_TEMPLATE1::calculateHardGradient(
        const vector<double>& factorsVector)
{
    // DEPRECATED
    return vector<double>();
//    bool prevAreForwardBackwardWeightsCalculated = areForwardBackwardWeightsCalculated;
//    const double STEP = 0.0001;
//    vector<double> grad(factorsVector.size());
//    areForwardBackwardWeightsCalculated = false;
//    double value = CalculateValue(factorsVector);
//    for (size_t i = 0; i < grad.size(); i++)
//    {
//        vector<double> movedPoint = factorsVector;
//        movedPoint[i] += STEP;
//        areForwardBackwardWeightsCalculated = false;
//        double movedValue = CalculateValue(movedPoint);
//        grad[i] = (movedValue - value) / STEP;
//    }
//    areForwardBackwardWeightsCalculated = prevAreForwardBackwardWeightsCalculated;
//    return grad;
}

CHAIN_OPTIMIZABLE_TEMPLATE
void CHAIN_OPTIMIZABLE_TEMPLATE1::calculateAllWeights(
        const vector<double>& factorsVector)
{
    // Calculate initial weights
    size_t initialWeightsSize = possibleStates[0].size();
    initialWeights.resize(initialWeightsSize);
    logInitialWeights.resize(initialWeightsSize);
    for (size_t iter = 0; iter < initialWeightsSize; ++iter)
    {
        logInitialWeights[iter] = vectorFactorsExtractor->GetInitialWeight(
                    possibleStates[0][iter], factorsVector);
        initialWeights[iter] = std::exp(logInitialWeights[iter]);
    }
    // Calculate final weights
    size_t finalWeightsSize = possibleStates.back().size();
    finalWeights.resize(finalWeightsSize);
    logFinalWeights.resize(finalWeightsSize);
    for (size_t iter = 0; iter < finalWeightsSize; ++iter)
    {
        logFinalWeights[iter] = vectorFactorsExtractor->GetFinalWeight(
                    possibleStates.back()[iter], factorsVector);
        finalWeights[iter] = std::exp(logFinalWeights[iter]);
    }
    // Calculate transitions weights
    transitionWeights.resize(cliqueIndexes.size());
    logTransitionWeights.resize(cliqueIndexes.size());
    for (size_t nodeIndex = 0; nodeIndex < cliqueIndexes.size(); ++nodeIndex)
    {
        const vector<int>& leftPossibleStates = possibleStates[nodeIndex];
        const vector<int>& rightPossibleStates = possibleStates[nodeIndex + 1];
        transitionWeights[nodeIndex].resize(leftPossibleStates.size());
        logTransitionWeights[nodeIndex].resize(leftPossibleStates.size());
        for (size_t leftNodeIndex = 0; leftNodeIndex < leftPossibleStates.size(); ++leftNodeIndex)
        {
            transitionWeights[nodeIndex][leftNodeIndex].resize(rightPossibleStates.size());
            logTransitionWeights[nodeIndex][leftNodeIndex].resize(rightPossibleStates.size());
            int leftState = leftPossibleStates[leftNodeIndex];
            if (transitionConverter->find(leftState) == transitionConverter->end())
            {
                continue;
            }
            for (size_t rightNodeIndex = 0; rightNodeIndex < rightPossibleStates.size();
                ++rightNodeIndex)
            {
                int rightState = rightPossibleStates[rightNodeIndex];
                const unordered_map<int, int>& currentCliquesMap =
                    transitionConverter->at(leftState);
                if (currentCliquesMap.find(rightState) == currentCliquesMap.end())
                {
                    continue;
                }
                double currentWeight = calculateWeightTransition(
                            nodeIndex, leftState,
                            rightState, factorsVector);
                logTransitionWeights[nodeIndex][leftNodeIndex][rightNodeIndex] =
                    currentWeight;
                transitionWeights[nodeIndex][leftNodeIndex][rightNodeIndex] =
                    std::exp(currentWeight);
            }
        }
    }
}


};

#endif // CHAIN OPTIMIZABLE

