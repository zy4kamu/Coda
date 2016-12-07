/** 
 * @file	CRF.h
 * @brief	header file for CRF-model
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


#ifndef _CRF
#define _CRF

#include <map>
#include <memory>
#include <fstream> 
#include <cmath>
#include <iostream>
#include <tuple>
#include <functional>
#include "State.h"
#include "Factors.h"
#include "ViterbiNode.h"
#include "Factors.h"
#include "Chain.h"
#include "CRFTextReader.h"
#include "CRFBinaryReader.h"
#include "VectorFactorsExtractor.h"
#include "ViterbiNBestNode.h"

using std::string;
using std::map;
using std::tuple;

namespace LinearCRF
{

static const double EPS = 0.000001;

typedef map<int, set<int> > CLIQUE_TO_ADDED_FEATURE;

struct CRFModelUpdatedData
{
    unordered_map<int, unordered_set<int> > cliqueToAddedFeatures;
    unordered_set<int> addedInitialWeights;
    unordered_set<int> addedFinalWeights;
    unordered_set<int> addedDefaultWeights;

    CRFModelUpdatedData() {}

    CRFModelUpdatedData(const CRFModelUpdatedData& data);

    CRFModelUpdatedData(CRFModelUpdatedData&& data);
};

/** 
 * @class	CRF
 * @brief	class describing CRF-model
 */
class CRF
{
public:
    /**
     * @brief	Constructor of CRF
     */
    CRF(void);

    /**
     * @brief	Destructor of CRF
     */
    ~CRF(void);

    /**
     * @brief	Constructor of CRF that reads model from file
     * @param[in]	file - file containing CRF-model
     * @param[in]	isBinary - determines if file containing CRF-model is binary
     */
    CRF(const string& file, bool isBinary);

    /**
     * @brief	Sets input alphabet of the CRF-model
     * @param[in]	_inputAlphabet - input alphabet
     */
    void SetInputAlphabet(shared_ptr<Alphabet> _inputAlphabet);

    /**
     * @brief	Sets output alphabet of the CRF-model
     * @param[in]	_inputAlphabet - output alphabet
     */
    void SetOutputAlphabet(shared_ptr<Alphabet> _outputAlphabet);

    /**
     * @brief	Sets weight alphabet of the CRF-model
     * @param[in]	_weightAlphabet - weight alphabet
     */
    void SetWeightAlphabet(shared_ptr<Alphabet> _weightAlphabet);

    /**
     * @brief	Sets defaults weights of the CRF-model
     * @param[in]	data - default weights
     */
    void SetDefaultWeights(shared_ptr<vector<double> > data);

    /**
     * @brief	Sets initial weights of the CRF-model
     * @param[in]	data - initial weights
     */
    void SetInitialWeights(shared_ptr<vector<double> > data);

    /**
     * @brief	Sets final weights of the CRF-model
     * @param[in]	data - final weights
     */
    void SetFinalWeights(shared_ptr<vector<double> > data);

    /**
     * @brief	Sets states of the CRF-model
     * @param[in]	states - states of the CRF-model
     */
    void SetStates(shared_ptr<vector<State> > data);

    /**
     * @brief	Sets weights of the cliques of the CRF-model
     * @param[in]	weights - parameters of the cliques
     */
    void SetWeights(shared_ptr<vector<unordered_map<int, double> > > weights);
    
    /**
     * @brief	Returns number of cliques in the CRF-model
     */
    int GetNumCliques() const;

    /**
     * @brief	Returns number of states in the CRF-model
     */
    int GetNumStates() const;

    /**
     * @brief	Returns input alphabet of the CRF-model
     */
    const Alphabet& GetInputAlphabet() const;

    /**
     * @brief	Returns output alphabet of the CRF-model
     */
    const Alphabet& GetOutputAlphabet() const;

    /**
     * @brief	Returns weight alphabet of the CRF-model
     */
    const Alphabet& GetWeightsAlphabet() const;

    /**
     * @brief	Returns input alphabet of the CRF-model
     */
    shared_ptr<const Alphabet> GetInputAlphabetPtr() const;

    /**
     * @brief	Returns output alphabet of the CRF-model
     */
    shared_ptr<const Alphabet> GetOutputAlphabetPtr() const;

    /**
     * @brief	Returns weight alphabet of the CRF-model
     */
    shared_ptr<const Alphabet> GetWeightsAlphabetPtr() const;

    /**
     * @brief	Returns input alphabet of the CRF-model
     */
    shared_ptr<Alphabet> GetInputAlphabetPtr();

    /**
     * @brief	Returns output alphabet of the CRF-model
     */
    shared_ptr<Alphabet> GetOutputAlphabetPtr();

    /**
     * @brief	Returns weight alphabet of the CRF-model
     */
    shared_ptr<Alphabet> GetWeightsAlphabetPtr();

    /**
     * @brief	Returns parameters of the CRF-model (weights of the cliques, weights of initial and final states of the model)
     */
    const Factors& GetParameters() const;

    /**
     * @brief	Returns initial weights of the CRF-model
     */
    const vector<double>& GetInitialWeights() const;

    /**
     * @brief	Returns final weights of the CRF-model
     */
    const vector<double>& GetFinalWeights() const;

    /**
     * @brief	Returns states of the CRF-model
     */
    const vector<State>& GetStates() const;

    /**
     * @brief	Runs Viterbi algorithm and gets the best hidden sequence for input data
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	bestSequence - best prediction of the hidden sequence
     */
    void FindBestSequence(
        const vector<vector<wstring> >& features
        , const vector<vector<int> >& possibleStates
        , vector<wstring>& bestSequence) const;

    /**
     * @brief	Runs Viterbi algorithm and gets the best hidden sequence for input data
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	bestSequence - best prediction of the hidden sequence
     * @param[out]	bestSequenceWeights - weights of the best sequence
     */
    void FindBestSequence(
        const vector<vector<wstring> >& features
        , const vector<vector<wstring> >& possibleStates
        , vector<wstring>* bestSequence
        , vector<double>* bestSequenceWeights) const;

    /**
     * @brief	Runs Viterbi algorithm and gets the best hidden sequence for input data (version of function where all parameters are passed as indexes)
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	bestSequence - best prediction of the hidden sequence
     * @param[out]	bestSequenceWeights - weights of the best sequence
     */
    void FindBestSequence_Digital(
        const vector<vector<int> >& features
        , const vector<vector<int> >& possibleStates
        , vector<int>* bestSequence
        , vector<double>* bestSequenceWeights) const;

    /**
     * @brief	Runs Viterbi algorithm and gets the best hidden sequence for input data
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[in]	numberOfHypothesis - number of hypothesis
     * @param[out]	predictions - best prediction of the hidden sequence
     * @param[out]	predictionWeights - weights of best predictions
     */
    void FindNBestSequences(
        const vector<vector<wstring> >& features
        , const vector<vector<wstring> >& possibleStates
        , vector<vector<wstring> >* predictions
        , vector<vector<double> >* predictionWeights
        , double* partitionFunction
        , int numberOfHypothesis) const;

    /**
     * @brief	Runs Viterbi algorithm and gets the best hidden sequence for input data
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[in]	numberOfHypothesis - number of hypothesis
     * @param[out]	predictions - best prediction of the hidden sequence
     * @param[out]	predictionWeights - weights of best predictions
     */
    void FindNBestSequences_Digital(
        const vector<vector<int> >& features
        , const vector<vector<int> >& possibleStates
        , vector<vector<int> >* predictions
        , vector<vector<double> >* predictionWeights
        , double* partitionFunction
        , int numberOfHypothesis) const;

    void FindPercentageBestSequences(
        const vector<vector<wstring> >& features
        , const vector<vector<wstring> >& possibleStates
        , vector<vector<wstring> >* predictions
        , vector<vector<double> >* predictionWeights
        , double* partitionFunction
        , double percentage
        , size_t maxNumberOfPaths) const;

    void FindPercentageBestSequences_Digital(
        const vector<vector<int> >& features
        , const vector<vector<int> >& possibleStates
        , vector<vector<int> >* predictions
        , vector<vector<double> >* predictionWeights
        , double* partitionFunction
        , double percentage
        , size_t maxNumberOfPaths) const;

    /**
     * @brief	Calculates partition function on the chain
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	partition function
     */
    double GetPartitionFunction(
        const vector<vector<wstring> >& features
        , const vector<vector<wstring> >& possibleStates) const;

    /**
     * @brief	Calculates partition function on the chain
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	partition function
     */
    double GetPartitionFunction_Digital(
        const vector<vector<int> >& features
        , const vector<vector<int> >& possibleStates) const;

    /**
     * @brief	Removes all features with weight smaller than trancationLevel
     * @param[in]	truncationLevel - Removes all features with weight smaller than trancationLevel
     */
    void Truncate(double truncationLevel);

    /**
     * @brief	Makes chain available for CRF-model (adds states and transitions to alphabet), used in relearning
     * @param[in]	chains - chains that should fit to the model
     * @param[out]	return value contains features that were added by this correction. First is index of clique and second is a set of new features in this clique.
     */
    CRFModelUpdatedData Add(const vector<Chain>& chain);

private:
    shared_ptr<Alphabet> inputAlphabet; /**< contains correspondence between feature names and their indexes*/
    shared_ptr<Alphabet> outputAlphabet; /**< contains correspondence between names of output data and their indexes*/
    shared_ptr<vector<State>> states; /**< contains states of the model*/
    vector<State> initialStates; /**< contains possible initial states of the model*/
    map<wstring, int> name2state; /**< contains correspondence between names of the states and their indexes*/
    Factors parameters; /**< contains main parameters of the CRF-model (weights of the cliques, weights of initial and final states)*/

    /**
     * @brief	Performs Viterbi algorithm used in 'FindBestSequence' function
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[out]	viterbiNodes - represents Viterbi lattice of the input sequence
     */
    template<class TViterbiNode>
    void calculateViterbiLattice(
        const vector<vector<int> >& features, 
        const vector<vector<int> >& possibleStates,
        vector<vector<TViterbiNode> >& viterbiNodes) const;

    /**
     * @brief	Performs Viterbi algorithm used in 'FindBestSequence' function
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
     * @param[in]	numberOfHypothesis - number of hypothesis
     * @param[out]	viterbiNodes - represents Viterbi lattice of the input sequence
     */
    vector<vector<ViterbiNBestNode> > calculateViterbiNBestLattice(
            const vector<vector<int> >& features, 
            const vector<vector<int> >& possibleStates,
            int numberOfHypothesis) const;

    /**
     * @brief	Initializes Viterbi lattice before searching best N paths
     * @param[in]	numStates - number of states
     * @param[in]	latticeLength - length of the lattice
     * @param[out]	viterbiNodes - represents Viterbi lattice of the input sequence
     */
   vector<vector<ViterbiNBestNode> > initializeViterbiNBestLattice(
       int numStates, int latticeLength) const;

   /**
     * @brief	Sets weights for the first layer of the Viterbi lattice 
     * @param[in]	viterbiLattice - lattice to initialize
     * @param[in]	firstLayerPossibleStates - possible states in the first layer
     */
   void initializeFirstLayerOfViterbiLattice(
       vector<vector<ViterbiNBestNode> >* viterbiLattice
       , const vector<int>& firstLayerPossibleStates) const;

   /**
     * @brief	checks for permitted states and permitted transitions in viterbi lattice
     * @param[in]	layerIndex - layer in the lattice
     * @param[in]	possibleStates - possible states
     * @param[in]	viterbiLattice - lattice to initialize
     * @param[out]	 - existsPermittedStates + existPermittedTransitions
     */
   tuple<bool, bool> checkForPossibleTransitions(
       int layerIndex,
       const vector<vector<int> >& possibleStates,
       const vector<vector<ViterbiNBestNode> >& viterbiLattice) const;

   /**
     * @brief	evaluates Viterbi node in Viterbi lattice calculation
     * @param[in]	viterbiLattice - lattice to update
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[in]	possibleStates - possible states
     * @param[in]	layerIndex - layer index of the node
     * @param[in]	stateIndex - state index of the node
     * @param[in]	numberOfHypothesis - number of hypothesis
     * @param[in]	existPermittedTransitions - info if there are permitted transition in the current level
     */
   void evaluateTransitionsOfViterbiNode(
       vector<vector<ViterbiNBestNode> >* viterbiLattice,
       const vector<vector<int> >& features,
       const vector<vector<int> >& possibleStates,
       int layerIndex, int stateIndex,
       int numberOfHypothesis,
       bool existPermittedTransitions) const;

   /**
     * @brief	evaluates last layer node by adding final weight and removing urelevant paths
     * @param[in]	node - node to update
     */
   void evaluateLastLayerNode(ViterbiNBestNode* node) const;

   /**
     * @brief	finds index of the best end index
     * @param[in]	viterbiLattice - viterbi lattice
     */
   int findBestEndInViterbiLattice(
       const vector<vector<ViterbiNode> >& viterbiLattice) const;

    /**
     * @brief	Finds N best ends in Viterbi lattice
     * @param[in]	viterbiLattice - Viterbi N best node lattice
     * @param[in]	bestPathItems - best path items
     * @param[in]	numberOfHypothesis - number of hypothesis
     */
    void findNBestEnds(
        const vector<vector<ViterbiNBestNode> >& viterbiLattice, 
        vector<ViterbiPathItem>* bestPathItems,
        int numberOfHypothesis) const;

    /**
     * @brief	Finds weight of the clique connecting two states
     * @param[in]	stateIndex - index of the first state
     * @param[in]	transIndex - index of the second state
     * @param[out]	features - features corresponding the clique
     */
    double calculateTransitionWeight(
            int stateIndex, int transIndex,
            const vector<int>& features) const;

    /**
     * @brief	Translates features to their indexes in input alphabet
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[out]	digital - features in digital form
     */
    vector<vector<int> > translateFeaturesIntoDigitalForm(
        const vector<vector<wstring>>& features) const;

    /**
     * @brief	Finds best path from N best viterbi lattice
     * @param[in]	viterbiLattice - Viterbi N best node lattice
     * @param[in]	lastItem - last node of the viterbi path
     * @param[out]	bestPath - best path
     * @param[out]	bestPathWeights - best weights od the path
     */
    template<class Node, class Path>
    void getPathFromViterbiLattice(
        const vector<vector<Node> >& viterbiLattice
        , Path& lastItem
        , vector<wstring>* bestPath
        , vector<double>* bestPathWeights) const;

    /**
     * @brief	Finds best path from N best viterbi lattice
     * @param[in]	viterbiLattice - Viterbi N best node lattice
     * @param[in]	lastItem - last node of the viterbi path
     * @param[out]	bestPath - best path
     * @param[out]	bestPathWeights - best weights od the path
     */
    template<class Node, class Path>
    void getPathFromViterbiLattice(
        const vector<vector<Node> >& viterbiLattice
        , Path& lastItem
        , vector<int>* bestPath
        , vector<double>* bestPathWeights) const;

    /**
     * @brief	Finds best path from viterbi lattice (returns path in indexes type)
     * @param[in]	viterbiLattice - Viterbi lattice
     * @param[in]	bestIndex - index of last node
     * @param[out]	bestPath - best path
     * @param[out]	bestPathWeights - best weights od the path
     */
    void getPathFromViterbiLattice(
        const vector<vector<ViterbiNode> >& viterbiLattice
        , int bestIndex
        , vector<wstring>* bestPath
        , vector<double>* bestPathWeights) const;

    /**
     * @brief	Finds best path from viterbi lattice (returns path in indexes type)
     * @param[in]	viterbiLattice - Viterbi lattice
     * @param[in]	bestIndex - index of last node
     * @param[out]	bestPath - best path
     * @param[out]	bestPathWeights - best weights od the path
     */
    void getPathFromViterbiLattice(
        const vector<vector<ViterbiNode> >& viterbiLattice
        , int bestEndIndex
        , vector<int>* bestPath
        , vector<double>* bestPathWeights) const;

    /**
     * @brief	Translates states to their indexes in output alphabet
     * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
     * @param[out]	digital - features in digital form
     */
    vector<vector<int> > translatePossibleStatesIntoDigitalForm(
        const vector<vector<wstring>>& features) const;

    template<class Node, class Path>
    void getAllEnds(
        const vector<vector<Node> >& viterbiLattice,
        vector<Path>* bestPathItems) const;

/********************** DEPRECATED *********************************/

/**
 * @brief	Performs Viterbi algorithm used in 'FindBestSequence' function
 * @param[in]	features - input data, features[i] contains features for the i-th hidden state of the CRF-model
 * @param[in]	possibleStates - possible states of the hidden states, possibleStates[i] contains possible states for the i-th hidden states of the CRF-model
 * @param[out]	viterbiNodes - represents Viterbi lattice of the input sequence
 */
void calculateViterbiLattice_MALLET_FUNCTION_DEPRECATED(
    const vector<vector<int> >& features,
    const vector<vector<int> >& possibleStates,
    vector<vector<ViterbiNode> >& viterbiNodes) const;

};

template<class TViterbiNode>
void CRF::calculateViterbiLattice(
    const vector<vector<int> >& features,
    const vector<vector<int> >& possibleStates,
    vector<vector<TViterbiNode> >& viterbiNodes) const
{
    size_t numStates = states->size();
    size_t latticeLength = features.size();
    // Create ViterbiNodes with initializing first layer;
    viterbiNodes.resize(latticeLength, vector<TViterbiNode>(numStates));
    for (size_t stateIndex = 0; stateIndex < numStates; ++stateIndex)
    {
        viterbiNodes[0][stateIndex].Initialize(0, stateIndex,
            parameters.GetInitialWeights()[stateIndex]);
    }
    for (size_t layerIndex = 1; layerIndex < latticeLength; ++layerIndex)
    {
        for (size_t stateIndex = 0; stateIndex < numStates; ++stateIndex)
        {
            viterbiNodes[layerIndex][stateIndex].Initialize(
                layerIndex, stateIndex);
        }
    }
    // Main loop
    for (size_t layer = 0; layer + 1 < latticeLength; ++layer)
    {
        // Check that there are permissible states and transitions;
        bool existsPermittedStates = false;
        bool existPermittedTransitions = false;
        for (size_t currentState = 0; currentState < numStates; ++currentState)
        {
            if ((abs(viterbiNodes[layer][currentState].GetWeight() - MINUSE_INFINITY) > EPS)
                && Tools::Contains(possibleStates[layer], static_cast<int>(currentState)))
            {
                existsPermittedStates = true;
                State& s = states->at(currentState);
                // Check for permissible transitions;
                for (int nextState = 0; nextState < s.GetNumDestinations(); ++nextState)
                {
                    if (Tools::Contains(possibleStates[layer + 1], (int)s.GetDestinations()[nextState]))
                    {
                        existPermittedTransitions = true;
                    }
                }
            }
        }
        //Calculate viterbiNodes;
        bool enabledToFindTransition = false;
        if (existPermittedTransitions || !existsPermittedStates)
        {
            for (size_t currentState = 0; currentState < numStates; ++currentState)
            {
                if (abs(viterbiNodes[layer][currentState].GetWeight() - MINUSE_INFINITY) < EPS)
                {
                    continue;
                }
                if (existPermittedTransitions && !Tools::Contains(possibleStates[layer], int(currentState)))
                {
                    continue;
                }
                State& s = states->at(currentState);
                for (int nextState = 0; nextState < s.GetNumDestinations(); ++nextState)
                {
                    State& destination = states->at((unsigned)s.GetDestinations()[nextState]);
                    if (!Tools::Contains(possibleStates[layer + 1], destination.GetIndex()))
                    {
                        continue;
                    }
                    enabledToFindTransition = true;
                    double weight = calculateTransitionWeight(
                        currentState, nextState, features[layer]);
                    if (layer == latticeLength - 2)
                    {
                        weight += parameters.GetFinalWeights()[destination.GetIndex()];
                    }
                    viterbiNodes[layer + 1][destination.GetIndex()].AddTransition(
                        &viterbiNodes[layer][currentState], weight);
                }
            }
        }
        if (!enabledToFindTransition) // if there is no any transition we allow all transitions
        {
            // Calculate best predecessor
            double defaultWeight = MINUSE_INFINITY;
            int defaultState = -1;
            for(size_t stateIndex = 0; stateIndex < numStates; ++stateIndex)
            {
                if (viterbiNodes[layer][stateIndex].GetWeight() > defaultWeight)
                {
                    defaultWeight = viterbiNodes[layer][stateIndex].GetWeight();
                    defaultState = stateIndex;
                }
            }
            // Case when there are possible states in next level
            if (possibleStates[layer + 1].size() > 0)
            {
                for (size_t stateIndex = 0; stateIndex < possibleStates[layer + 1].size()
                    ; ++stateIndex)
                {
                    double transitionWeight = (layer == latticeLength - 2) ?
                        parameters.GetFinalWeights()[possibleStates[layer + 1][stateIndex]]
                        : 0;
                    viterbiNodes[layer + 1][possibleStates[layer + 1][stateIndex]].AddTransition(
                        &viterbiNodes[layer][defaultState], transitionWeight);
                }
            } else { // Case when there are no possible states in next level
                for (size_t stateIndex = 0; stateIndex < numStates
                    ; ++stateIndex)
                {
                    double transitionWeight = (layer == latticeLength - 2) ?
                        parameters.GetFinalWeights()[stateIndex]
                        : 0;
                    viterbiNodes[layer + 1][stateIndex].AddTransition(
                        &viterbiNodes[layer][defaultState], transitionWeight);
                }
            }
        }
    }
}

template<class Node, class Path>
void CRF::getAllEnds(
    const vector<vector<Node> >& viterbiLattice,
    vector<Path>* bestPathItems) const
{
    const vector<Node>& lastLayer = viterbiLattice.back();
    for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
    {
        std::copy(
            lastLayer[nodeIndex].pathItems.begin(),
            lastLayer[nodeIndex].pathItems.end(),
            std::back_inserter(*bestPathItems));
    }
    std::sort(bestPathItems->begin(), bestPathItems->end(), std::greater<Path>());
}

template<class Node, class Path>
void CRF::getPathFromViterbiLattice(
    const vector<vector<Node> >& viterbiLattice
    , Path& lastItem
    , vector<wstring>* bestPath
    , vector<double>* bestPathWeights) const
{
    size_t latticeLength = viterbiLattice.size();
    bestPath->clear();
    bestPathWeights->clear();
    bestPath->resize(latticeLength);
    bestPathWeights->resize(latticeLength);
    const Path* currentPathItem =  &lastItem;
    for (size_t layerIndex = 0; layerIndex < latticeLength; ++layerIndex)
    {
        (*bestPath)[latticeLength - layerIndex - 1]
            = states->at(currentPathItem->currentNode->stateIndex).GetName();
        (*bestPathWeights)[latticeLength - layerIndex - 1]
            = currentPathItem->bestWeight;
        currentPathItem = currentPathItem->previousItem;
    }
}

template<class Node, class Path>
void CRF::getPathFromViterbiLattice(
    const vector<vector<Node> >& viterbiLattice
    , Path& lastItem
    , vector<int>* bestPath
    , vector<double>* bestPathWeights) const
{
    size_t latticeLength = viterbiLattice.size();
    bestPath->clear();
    bestPathWeights->clear();
    bestPath->resize(latticeLength);
    bestPathWeights->resize(latticeLength);
    const Path* currentPathItem =  &lastItem;
    for (size_t layerIndex = 0; layerIndex < latticeLength; ++layerIndex)
    {
        (*bestPath)[latticeLength - layerIndex - 1]
            = states->at(currentPathItem->currentNode->stateIndex).GetIndex();
        (*bestPathWeights)[latticeLength - layerIndex - 1]
            = currentPathItem->bestWeight;
        currentPathItem = currentPathItem->previousItem;
    }
}

};

#endif // _CRF

