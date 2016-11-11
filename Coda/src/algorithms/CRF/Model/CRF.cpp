/** 
 * @file	CRF.cpp
 * @brief	source file of the CRF-model
 */

#include <limits>

#include "CRF.h"
#include "VectorFactorsExtractor.h"
#include "ViterbiPercentageNode.h"

using namespace LinearCRF;

/********************** CRF MODEL UPDATE DATA **********************/

CRFModelUpdatedData::CRFModelUpdatedData(const CRFModelUpdatedData& data)
    : cliqueToAddedFeatures(data.cliqueToAddedFeatures)
    , addedInitialWeights(data.addedInitialWeights)
    , addedFinalWeights(data.addedFinalWeights)
    , addedDefaultWeights(data.addedDefaultWeights)
{
}

CRFModelUpdatedData::CRFModelUpdatedData(CRFModelUpdatedData&& data)
    : cliqueToAddedFeatures(std::move(data.cliqueToAddedFeatures))
    , addedInitialWeights(std::move(data.addedInitialWeights))
    , addedFinalWeights(std::move(data.addedFinalWeights))
    , addedDefaultWeights(std::move(data.addedDefaultWeights))
{
}

/********************** CONSTRUCTORS *******************************/

CRF::CRF(void) 
{
}

CRF::~CRF(void)
{
}

CRF::CRF(const string& file, bool isBinary)
{
	if (isBinary)
	{
		CRFBinaryReader parser(file);
		parser.Read(*this);
	}
	else
	{
		CRFTextReader parser(file);
		parser.Read(*this);
	}
}

/********************** COMMON *******************************/

void CRF::SetInputAlphabet(shared_ptr<Alphabet> _inputAlphabet)
{
	inputAlphabet = _inputAlphabet;
}

void CRF::SetOutputAlphabet(shared_ptr<Alphabet> _outputAlphabet)
{
	outputAlphabet = _outputAlphabet;
}

void CRF::SetWeightAlphabet(shared_ptr<Alphabet> _weightAlphabet)
{
	parameters.SetWeightAlphabet(_weightAlphabet);
}

void CRF::SetStates(shared_ptr<vector<State>> data)
{
	states = data;
	for (size_t i = 0; i < states->size(); ++i)
	{
		name2state.insert(std::pair<wstring, int>(
			states->at(i).GetName(), 
			states->at(i).GetIndex()));
	}
}

void CRF::SetWeights(shared_ptr<vector<unordered_map<int, double> > > weights)
{
	parameters.SetWeights(weights);
}

const Factors& CRF::GetParameters() const
{
	return parameters;
}

int CRF::GetNumCliques() const
{
	return parameters.GetWeightAlphabet().Size();
}

void CRF::SetDefaultWeights(shared_ptr<vector<double>> data)
{
	parameters.SetDefaultWeights(data);
}

void CRF::SetInitialWeights(shared_ptr<vector<double>> data)
{
	parameters.SetInitialWeights(data);
    for (size_t stateIndex = 0; stateIndex < states->size(); ++stateIndex)
	{
		if (parameters.GetInitialWeights()[stateIndex] > IMPOSSIBLE_WEIGHT)
        {
			initialStates.push_back(states->at(stateIndex));
        }
	}
}

void CRF::SetFinalWeights(shared_ptr<vector<double>> data)
{
	parameters.SetFinalWeights(data);
}

const Alphabet& CRF::GetInputAlphabet() const
{
	return *inputAlphabet;
}

const Alphabet& CRF::GetOutputAlphabet() const
{
	return *outputAlphabet;
}

const Alphabet& CRF::GetWeightsAlphabet() const
{
	return parameters.GetWeightAlphabet();
}

shared_ptr<const Alphabet> CRF::GetInputAlphabetPtr() const
{
	return inputAlphabet;
}

shared_ptr<const Alphabet> CRF::GetOutputAlphabetPtr() const
{
	return outputAlphabet;
}

shared_ptr<const Alphabet> CRF::GetWeightsAlphabetPtr() const
{
	return parameters.GetWeightAlphabetPtr();
}

shared_ptr<Alphabet> CRF::GetInputAlphabetPtr()
{
    return inputAlphabet;
}

shared_ptr<Alphabet> CRF::GetOutputAlphabetPtr()
{
    return outputAlphabet;
}

shared_ptr<Alphabet> CRF::GetWeightsAlphabetPtr()
{
    return parameters.GetWeightAlphabetPtr();
}

void CRF::Truncate(double truncationLevel)
{
    parameters.Truncate(truncationLevel);
}

 /************************* FIND ONE BEST SEQUENCE ********************/

void CRF::FindBestSequence(
    const vector<vector<wstring> >& features
    , const vector<vector<wstring> >& possibleStates
    , vector<wstring>* bestSequence
    , vector<double>* bestSequenceWeights) const
{
	// Translate features into digital format;
	vector<vector<int> > intFeatures = 
        translateFeaturesIntoDigitalForm(features);
    // ranslate possible states into digital format;
    vector<vector<int> > intPossibleStates = 
        translatePossibleStatesIntoDigitalForm(possibleStates);
	// Calculate Viterbi lattice;
	vector<vector<ViterbiNode> > viterbiLattice;
    calculateViterbiLattice(
        intFeatures, intPossibleStates, viterbiLattice);
	// Find best end element;
    int bestEndIndex = findBestEndInViterbiLattice(viterbiLattice);
	// Iterate backward algorithm and get best sequence;
    getPathFromViterbiLattice(
        viterbiLattice, bestEndIndex
        , bestSequence, bestSequenceWeights);
}

void CRF::FindBestSequence_Digital(
    const vector<vector<int> >& features
    , const vector<vector<int> >& possibleStates
    , vector<int>* bestSequence
    , vector<double>* bestSequenceWeights) const
{
    // Calculate Viterbi lattice;
	vector<vector<ViterbiNode> > viterbiLattice;
    calculateViterbiLattice(features, possibleStates, viterbiLattice);
    // Find best end element;
    int bestEndIndex = findBestEndInViterbiLattice(viterbiLattice);
    // Iterate backward algorithm and get best sequence;
    getPathFromViterbiLattice(
        viterbiLattice, bestEndIndex
        , bestSequence, bestSequenceWeights);
}

void CRF::getPathFromViterbiLattice(
    const vector<vector<ViterbiNode> >& viterbiLattice
    , int bestEndIndex
    , vector<wstring>* bestPath
    , vector<double>* bestPathWeights) const
{
	int latticeLength = viterbiLattice.size();
    bestPath->clear();
    bestPathWeights->clear();
    bestPath->resize(latticeLength);
    bestPathWeights->resize(latticeLength);
	const ViterbiNode* bestNodeIterator = 
        &viterbiLattice[latticeLength - 1][bestEndIndex];
	for (int layerIndex = 0; layerIndex < latticeLength; ++layerIndex)
	{
		(*bestPath)[latticeLength - layerIndex - 1] 
            = states->at(bestNodeIterator->stateIndex).GetName();
        (*bestPathWeights)[latticeLength - layerIndex - 1]
            = bestNodeIterator->weight;
		bestNodeIterator = bestNodeIterator->bestPredecessor;
	}
}

void CRF::getPathFromViterbiLattice(
    const vector<vector<ViterbiNode> >& viterbiLattice
    , int bestEndIndex
    , vector<int>* bestPath
    , vector<double>* bestPathWeights) const
{
	int latticeLength = viterbiLattice.size();
    bestPath->clear();
    bestPathWeights->clear();
    bestPath->resize(latticeLength);
    bestPathWeights->resize(latticeLength);
	const ViterbiNode* bestNodeIterator = 
        &viterbiLattice[latticeLength - 1][bestEndIndex];
	for (int layerIndex = 0; layerIndex < latticeLength; ++layerIndex)
	{
		(*bestPath)[latticeLength - layerIndex - 1] 
            = states->at(bestNodeIterator->stateIndex).GetIndex();
        (*bestPathWeights)[latticeLength - layerIndex - 1]
            = bestNodeIterator->weight;
		bestNodeIterator = bestNodeIterator->bestPredecessor;
	}
}

vector<vector<int> > CRF::translateFeaturesIntoDigitalForm(
        const vector<vector<wstring>>& features) const
{
    vector<vector<int> > intFeatures(features.size());
	for(size_t tokenIndex = 0; tokenIndex < features.size(); ++tokenIndex)
	{
		for (size_t featureIndex = 0; featureIndex < features[tokenIndex].size();
            ++featureIndex)
		{
            int featureToAdd = 
                inputAlphabet->LookUpIndex_NoAdding(features[tokenIndex][featureIndex]);
            if (featureToAdd != -1)
            {
                intFeatures[tokenIndex].push_back( 
                    featureToAdd);
            }
		}
	}
    return intFeatures;
}

vector<vector<int> > CRF::translatePossibleStatesIntoDigitalForm(
        const vector<vector<wstring>>& possibleStates) const
{
    vector<vector<int> > intFeatures(possibleStates.size());
	for(size_t tokenIndex = 0; tokenIndex < possibleStates.size(); ++tokenIndex)
	{
		intFeatures[tokenIndex].reserve(possibleStates[tokenIndex].size());
		for (size_t featureIndex = 0; featureIndex < possibleStates[tokenIndex].size();
            ++featureIndex)
		{
            int alphabetIndex = outputAlphabet->LookUpIndex_NoAdding(
                possibleStates[tokenIndex][featureIndex]);
            if (alphabetIndex != -1)
            {
                intFeatures[tokenIndex].push_back(alphabetIndex); 
            }
		}
	}
    return intFeatures;
}


int CRF::findBestEndInViterbiLattice(
       const vector<vector<ViterbiNode> >& viterbiLattice) const
{
    int numStates = states->size();
	int latticeLength = viterbiLattice.size();
    int bestEndIndex = 0;
	double bestWeight = IMPOSSIBLE_WEIGHT;
	for (int stateIndex = 0; stateIndex < numStates; ++stateIndex)
	{
		if (viterbiLattice[latticeLength - 1][stateIndex].weight > bestWeight)
		{
			bestWeight = viterbiLattice[latticeLength - 1][stateIndex].weight;
			bestEndIndex = stateIndex;
		}
	}
    return bestEndIndex;
}

 double CRF::calculateTransitionWeight(
	 int stateIndex, int transIndex,
     const vector<int>& features) const
 {
	 double retval = 0;
	 int swi;
	 const vector<vector<int>>& weightIndexes = states->at(stateIndex).GetWeightsIndexes();
	 int nwi = weightIndexes.at(transIndex).size();
	 for (int wi = 0; wi < nwi; ++wi)
	 {
		swi = weightIndexes.at(transIndex)[wi];
		retval += parameters.GetDefaultWeights()[swi];
        const unordered_map<int, double>& cliqueData = parameters.GetWeights()[swi];
		for (size_t k = 0; k < features.size(); ++k)
		{
            auto found = cliqueData.find(features[k]);
            if (found != cliqueData.end())
            {
                retval += found->second;
            }
		}
	 }
	 return retval;
 }

 /************************* GET PARTITION FUNCTION *******************/

 double CRF::GetPartitionFunction(
     const vector<vector<wstring> >& features
     , const vector<vector<wstring> >& possibleStates) const
 {
     if (features.empty()) return -1;
     // Translate features into digital format;
     vector<vector<int> > intFeatures =
         translateFeaturesIntoDigitalForm(features);
     // ranslate possible states into digital format;
     vector<vector<int> > intPossibleStates =
         translatePossibleStatesIntoDigitalForm(possibleStates);
     // calculate Viterbi nodes
     GetPartitionFunction_Digital(intFeatures, intPossibleStates);
 }

 double CRF::GetPartitionFunction_Digital(
     const vector<vector<int> >& intFeatures
     , const vector<vector<int> >& intPossibleStates) const
 {
     if (intFeatures.empty()) return - 1;
     // calculate Viterbi nodes
     vector<vector<ZViterbiNode> > viterbiLattice;
     calculateViterbiLattice(
         intFeatures, intPossibleStates, viterbiLattice);
     // Get partition function
     double partitionFunction = 0;
     const vector<ZViterbiNode>& lastLayer = viterbiLattice.back();
     for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
     {
         partitionFunction += lastLayer[nodeIndex].partitionFunction;
     }
     return partitionFunction;
 }


 /************************* FIND N BEST SEQUENCES ********************/

 void CRF::FindNBestSequences(
    const vector<vector<wstring> >& features
    , const vector<vector<wstring> >& possibleStates
    , vector<vector<wstring> >* predictions
    , vector<vector<double> >* predictionWeights
    , double* partitionFunction
    , int numberOfHypothesis) const
{
    if (features.empty()) return;
	// Translate features into digital format;
	vector<vector<int> > intFeatures = 
        translateFeaturesIntoDigitalForm(features);
    // ranslate possible states into digital format;
    vector<vector<int> > intPossibleStates = 
        translatePossibleStatesIntoDigitalForm(possibleStates);
    // calculate Viterbi nodes
    ViterbiNBestNode::MAX_NUMBER_IN_ONE_NODE = numberOfHypothesis;
    vector<vector<ViterbiNBestNode> > viterbiLattice;
    calculateViterbiLattice(
        intFeatures, intPossibleStates, viterbiLattice);
    // Get partition function
    *partitionFunction = 0;
    const vector<ViterbiNBestNode>& lastLayer = viterbiLattice.back();
    for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
    {
        *partitionFunction += lastLayer[nodeIndex].partitionFunction;
    }
    // Find N best ends
    vector<ViterbiPathItem> bestPathItems;
    findNBestEnds(viterbiLattice, &bestPathItems
        , numberOfHypothesis);
    // Fill best paths
    predictions->clear();
    predictions->resize(bestPathItems.size());
    predictionWeights->clear();
    predictionWeights->resize(bestPathItems.size());
    for (size_t pathIndex = 0; pathIndex < predictions->size()
        ; ++pathIndex)
    {
        vector<wstring> currentPath;
        vector<double> currentPathWeights;
        getPathFromViterbiLattice(
            viterbiLattice, bestPathItems[pathIndex]
            , &currentPath, &currentPathWeights);
        (*predictions)[pathIndex] = std::move(currentPath);
        (*predictionWeights)[pathIndex] = std::move(currentPathWeights);
    }
}

 void CRF::FindNBestSequences_Digital(
    const vector<vector<int> >& intFeatures
    , const vector<vector<int> >& intPossibleStates
    , vector<vector<int> >* predictions
    , vector<vector<double> >* predictionWeights
    , double* partitionFunction
    , int numberOfHypothesis) const
{
    if (intFeatures.empty()) return;
    // calculate Viterbi nodes
    ViterbiNBestNode::MAX_NUMBER_IN_ONE_NODE = numberOfHypothesis;
    vector<vector<ViterbiNBestNode> > viterbiLattice;
    calculateViterbiLattice(
        intFeatures, intPossibleStates, viterbiLattice);
    // Find N best ends
    vector<ViterbiPathItem> bestPathItems;
    findNBestEnds(viterbiLattice, &bestPathItems
        , numberOfHypothesis);
    // Get partition function
    *partitionFunction = 0;
    const vector<ViterbiNBestNode>& lastLayer = viterbiLattice.back();
    for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
    {
        *partitionFunction += lastLayer[nodeIndex].partitionFunction;
    }
    // Fill best paths
    predictions->clear();
    predictions->resize(bestPathItems.size());
    predictionWeights->clear();
    predictionWeights->resize(bestPathItems.size());
    for (size_t pathIndex = 0; pathIndex < predictions->size()
        ; ++pathIndex)
    {
        vector<int> currentPath;
        vector<double> currentPathWeights;
        getPathFromViterbiLattice(
            viterbiLattice, bestPathItems[pathIndex]
            , &currentPath, &currentPathWeights);
        (*predictions)[pathIndex] = std::move(currentPath);
        (*predictionWeights)[pathIndex] = std::move(currentPathWeights);
    }
}

void CRF::findNBestEnds(
    const vector<vector<ViterbiNBestNode> >& viterbiLattice,
    vector<ViterbiPathItem>* bestPathItems,
    int numberOfHypothesis) const
{
    int numStates = states->size();
    vector<set<ViterbiPathItem>::reverse_iterator> pathEnumerators(numStates);
    for (int stateIndex = 0; stateIndex < numStates; ++stateIndex)
    {
        pathEnumerators[stateIndex] = 
            viterbiLattice.back()[stateIndex].pathItems.rbegin();
    }
    for (int pretendentIndex = 0; pretendentIndex < numberOfHypothesis; 
        ++pretendentIndex)
    {
        const ViterbiPathItem* bestNode = NULL;
        for (int stateIndex = 0; stateIndex < numStates; ++stateIndex)
        {
            const ViterbiNBestNode& currentNode = viterbiLattice.back()[stateIndex];
            if (pathEnumerators[stateIndex] == currentNode.pathItems.rend())
            {
                continue;
            } else if (bestNode == NULL
                || *bestNode < *pathEnumerators[stateIndex])
            {
                bestNode = &(*pathEnumerators[stateIndex]);
            }
        }
        if (bestNode == NULL)
        {
            break;
        } else {
            bestPathItems->push_back(*bestNode);
            ++pathEnumerators[bestNode->currentNode->stateIndex];
        }
    }
}

/******************* FIND PERCENTAGE SEQUENCES *********************/

void CRF::FindPercentageBestSequences(
   const vector<vector<wstring> >& features
   , const vector<vector<wstring> >& possibleStates
   , vector<vector<wstring> >* predictions
   , vector<vector<double> >* predictionWeights
   , double* partitionFunction
   , double percentage
   , size_t maxNumberOfPaths) const
{
   if (features.empty()) return;
   // Translate features into digital format;
   vector<vector<int> > intFeatures =
       translateFeaturesIntoDigitalForm(features);
   // ranslate possible states into digital format;
   vector<vector<int> > intPossibleStates =
       translatePossibleStatesIntoDigitalForm(possibleStates);
   // calculate Viterbi nodes
   ViterbiPercentageNode::VITERBI_PERCENTAGE_VALUE = percentage;
   ViterbiPercentageNode::MAX_NUMBER_OF_PATHS = maxNumberOfPaths;
   vector<vector<ViterbiPercentageNode> > viterbiLattice;
   calculateViterbiLattice(
       intFeatures, intPossibleStates, viterbiLattice);
   // Get partition function
   *partitionFunction = 0;
   const vector<ViterbiPercentageNode>& lastLayer = viterbiLattice.back();
   for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
   {
       *partitionFunction += lastLayer[nodeIndex].partitionFunction;
   }
   // Find N best ends
   vector<ViterbiPercentagePathItem> bestPathItems;
   getAllEnds(viterbiLattice, &bestPathItems);
   double cutThreshold = *partitionFunction * percentage;
   double sum = 0;
   size_t index = 0;
   size_t top = std::min(bestPathItems.size(), maxNumberOfPaths);
   for (; index < top; ++index)
   {
       sum += bestPathItems[index].expWeight;
       if (sum > cutThreshold)
       {
           break;
       }
   }
   bestPathItems.resize(index + 1);
   // Fill best paths
   predictions->clear();
   predictions->resize(bestPathItems.size());
   predictionWeights->clear();
   predictionWeights->resize(bestPathItems.size());
   for (size_t pathIndex = 0; pathIndex < predictions->size()
       ; ++pathIndex)
   {
       vector<wstring> currentPath;
       vector<double> currentPathWeights;
       getPathFromViterbiLattice(
           viterbiLattice, bestPathItems[pathIndex]
           , &currentPath, &currentPathWeights);
       (*predictions)[pathIndex] = std::move(currentPath);
       (*predictionWeights)[pathIndex] = std::move(currentPathWeights);
   }
}

void CRF::FindPercentageBestSequences_Digital(
    const vector<vector<int> >& intFeatures
    , const vector<vector<int> >& intPossibleStates
    , vector<vector<int> >* predictions
    , vector<vector<double> >* predictionWeights
    , double* partitionFunction
    , double percentage
    , size_t maxNumberOfPaths) const
{
    if (intFeatures.empty()) return;
    // calculate Viterbi nodes
    ViterbiPercentageNode::VITERBI_PERCENTAGE_VALUE = percentage;
    ViterbiPercentageNode::MAX_NUMBER_OF_PATHS = maxNumberOfPaths;
    vector<vector<ViterbiPercentageNode> > viterbiLattice;
    calculateViterbiLattice(
        intFeatures, intPossibleStates, viterbiLattice);
    // Get partition function
    *partitionFunction = 0;
    const vector<ViterbiPercentageNode>& lastLayer = viterbiLattice.back();
    for (size_t nodeIndex = 0; nodeIndex < lastLayer.size(); ++nodeIndex)
    {
        *partitionFunction += lastLayer[nodeIndex].partitionFunction;
    }
    // Find N best ends
    vector<ViterbiPercentagePathItem> bestPathItems;
    getAllEnds(viterbiLattice, &bestPathItems);
    double cutThreshold = *partitionFunction * percentage;
    double sum = 0;
    size_t index = 0;
    size_t top = std::min(bestPathItems.size(), maxNumberOfPaths);
    for (; index < top; ++index)
    {
        sum += bestPathItems[index].expWeight;
        if (sum > cutThreshold)
        {
            break;
        }
    }
    size_t resultNumber = std::min(index + 1, maxNumberOfPaths);
    resultNumber = std::min(resultNumber, top);
    bestPathItems.resize(resultNumber);
    // Fill best paths
    predictions->clear();
    predictions->resize(resultNumber);
    predictionWeights->clear();
    predictionWeights->resize(resultNumber);
    for (size_t pathIndex = 0; pathIndex < resultNumber
        ; ++pathIndex)
    {
        vector<int> currentPath;
        vector<double> currentPathWeights;
        getPathFromViterbiLattice(
            viterbiLattice, bestPathItems[pathIndex]
            , &currentPath, &currentPathWeights);
        (*predictions)[pathIndex] = std::move(currentPath);
        (*predictionWeights)[pathIndex] = std::move(currentPathWeights);
    }
}

/******************* AUXILIARY *********************/

 int CRF::GetNumStates() const
 {
	 return states->size();
 }

 const vector<double>& CRF::GetInitialWeights() const
 {
	 return parameters.GetInitialWeights();
 }

 const vector<double>& CRF::GetFinalWeights() const
 {
	 return parameters.GetFinalWeights();
 }

 const vector<State>& CRF::GetStates() const
 {
	 return *states;
 }

 /****************** FOR RELEARNING *******************/

 CRFModelUpdatedData CRF::Add(const vector<Chain>& chains)
 {
     CRFModelUpdatedData updateData;
     shared_ptr<Alphabet> weightAlphabet = parameters.GetWeightAlphabetPtr();
     shared_ptr<vector<double> > defaultWeights = parameters.GetDefaultWeightsPtr();
     shared_ptr<vector<double> > initialWeights = parameters.GetInitialWeightsPtr();
     shared_ptr<vector<double> > finalWeights = parameters.GetFinalWeightsPtr();
     shared_ptr<vector<unordered_map<int, double> > > weights = parameters.GetWeightsPtr();
     int addedCliquesEnumerator = 0;
     // Find minimal default weight
     double minDefaultWeight = std::numeric_limits<double>::infinity();
     for (size_t defIndex = 0; defIndex < defaultWeights->size(); ++defIndex)
     {
         if (minDefaultWeight > (*defaultWeights)[defIndex])
         {
             minDefaultWeight = (*defaultWeights)[defIndex];
         }
     }
     for (size_t chainIndex = 0; chainIndex < chains.size(); ++chainIndex)
     {
         const Chain& chain = chains[chainIndex];
         const vector<vector<wstring> >& features = chain.GetFeatures();
         const vector<wstring>& labels = chain.GetLabels();

         // Input alphabet
         for (size_t nodeIndex = 0; nodeIndex < features.size(); ++nodeIndex)
         {
             for (size_t featIndex = 0; featIndex < features[nodeIndex].size(); ++featIndex)
             {
                 int prevSize = inputAlphabet->Size();
                 inputAlphabet->LookUpIndex_Adding(features[nodeIndex][featIndex]);
                 if (inputAlphabet->Size() != prevSize)
                 {
                     std::wcout << L"Added to input alphabet: "
                        << features[nodeIndex][featIndex] << std::endl;
                 }
             }
         }
         // Output alphabet
         for (size_t nodeIndex = 0; nodeIndex < labels.size(); ++nodeIndex)
         {
             int prevSize = outputAlphabet->Size();
             int currentStateIndex = outputAlphabet->LookUpIndex_Adding(labels[nodeIndex]);
             if (outputAlphabet->Size() != prevSize)
             {
                 std::wcout << L"Added to output alphabet: "
                    << labels[nodeIndex] << std::endl;
                 states->emplace_back(State(this));
                 State& currentState = states->back();
                 currentState.index = currentStateIndex;
                 currentState.name = labels[nodeIndex];
                 name2state[currentState.name] = currentState.index;
                 if (nodeIndex == 0) // Attention !!!
                 {
                    initialStates.push_back(currentState);
                 }
                 initialWeights->push_back(0); // Attention !!!
                 finalWeights->push_back(0); // Attention !!!
                 updateData.addedInitialWeights.insert(currentStateIndex);
                 updateData.addedFinalWeights.insert(currentStateIndex);
             }
         }
         // Weight alphabet
         for (size_t nodeIndex = 0; nodeIndex + 1 < labels.size(); ++nodeIndex)
         {
             int prevSize = weightAlphabet->Size();
             wstring toAdd = labels[nodeIndex] + L"->" + labels[nodeIndex + 1];
             int transitionIndex = weightAlphabet->LookUpIndex_Adding(toAdd);
             if (weightAlphabet->Size() != prevSize)
             {
                 std::wcout << addedCliquesEnumerator << L": Added to weight alphabet: "
                    << toAdd << std::endl;
                 defaultWeights->push_back(minDefaultWeight); // !!! Attention !!! Default weights for new cliques are set to minimal default weights
                 updateData.addedDefaultWeights.insert(transitionIndex);
                 ++addedCliquesEnumerator;
                 State& currentState = (*states)[outputAlphabet->LookUpIndex_Adding(labels[nodeIndex])];
                 currentState.destinationNames->push_back(labels[nodeIndex + 1]);
                 currentState.destinations->push_back(
                    outputAlphabet->LookUpIndex_Adding(labels[nodeIndex + 1]));
                 currentState.weightsIndexes->push_back(vector<int>(1, transitionIndex));
                 weights->emplace_back();
             }
         }
         // Update transition maps
         unordered_map<int, unordered_set<int> >& cliqueToAddedFeatures = updateData.cliqueToAddedFeatures;
         for (size_t nodeIndex = 0; nodeIndex + 1 < labels.size(); ++nodeIndex)
         {
             wstring toAdd = labels[nodeIndex] + L"->" + labels[nodeIndex + 1];
             int transitionIndex = weightAlphabet->LookUpIndex_Adding(toAdd);
             unordered_map<int, double>& currentTransitionWeights = (*weights)[transitionIndex];
             for (size_t featIndex = 0; featIndex < features[nodeIndex].size(); ++featIndex)
             {
                 int featureIndex = inputAlphabet->LookUpIndex_Adding(features[nodeIndex][featIndex]);
                 if (currentTransitionWeights.find(featureIndex) == currentTransitionWeights.end())
                 {
                     currentTransitionWeights[featureIndex] = 0;
                     if (cliqueToAddedFeatures.find(transitionIndex) == cliqueToAddedFeatures.end()) {
                         unordered_set<int> toAddSet;
                         toAddSet.insert(featureIndex);
                         cliqueToAddedFeatures[transitionIndex] = std::move(toAddSet);
                     } else {
                         cliqueToAddedFeatures[transitionIndex].insert(featureIndex);
                     }

                 }
             }
         }
     }
     return updateData;
 }

 /****************** DEPRECATED ***********************/

 void CRF::FindBestSequence(
    const vector<vector<wstring>>& features,
    const vector<vector<int>>& possibleStates,
    vector<wstring>& bestSequence) const
 {
     std::wcout << L"CRF::FindBestSequence usage of deprecated function" << std::endl;
     int numStates = states->size();
     int latticeLength = features.size() + 1;
     // Translate features into digital format;
     vector<vector<int> > intFeatures =
         translateFeaturesIntoDigitalForm(features);
     // Calculate Viterbi lattice;
     vector<vector<ViterbiNode>> viterbiNodes;
     calculateViterbiLattice_MALLET_FUNCTION_DEPRECATED(
         intFeatures, possibleStates, viterbiNodes);
     // Find best end element;
     int bestEndIndex = 0;
     double bestWeight = IMPOSSIBLE_WEIGHT;
     for (int i = 0; i < numStates; ++i)
     {
         if (viterbiNodes[latticeLength - 1][i].weight > bestWeight)
         {
             bestWeight = viterbiNodes[latticeLength - 1][i].weight;
             bestEndIndex = i;
         }
     }
     // Iterate backward algorithm and get best sequence;
     ViterbiNode* it = &viterbiNodes[latticeLength - 1][bestEndIndex];
     bestSequence.resize(latticeLength);
     for (int i = 0; i < latticeLength; ++i)
     {
         bestSequence[latticeLength - i - 1] = states->at(it->stateIndex).GetName();
         it = it->bestPredecessor;
     }
 }

 void CRF::calculateViterbiLattice_MALLET_FUNCTION_DEPRECATED(
             const vector<vector<int>>& features,
             const vector<vector<int>>& possibleStates,
             vector<vector<ViterbiNode>>& viterbiNodes) const
 {
     int numStates = states->size();
     int latticeLength = features.size() + 1;
     // Create ViterbiNodes;
     viterbiNodes.reserve(latticeLength);
     for (int i = 0; i < latticeLength; ++i)
     {
         viterbiNodes.emplace_back(vector<ViterbiNode>());
         for (int j = 0; j < numStates; ++j)
         {
             ViterbiNode toAdd;
             toAdd.Initialize(i, j, IMPOSSIBLE_WEIGHT);
             viterbiNodes[i].emplace_back(toAdd);
         }
     }
     // First layer;
     for (int i = 0; i < numStates; ++i)
         viterbiNodes[0][i].weight = parameters.GetInitialWeights()[i];
     for (int ip = 0; ip < latticeLength - 1; ++ip)
     {
         // Check that there are permissible states and transitions;
         bool existsPermittedStates = false;
         bool existPermittedTransitions = false;
         for (int i = 0; i < numStates; ++i)
         {
             if ((abs(viterbiNodes[ip][i].weight - IMPOSSIBLE_WEIGHT) > EPS) &&
                 Tools::Contains(possibleStates[ip], i))
             {
                 existsPermittedStates = true;
                 State& s = states->at(i);
                 // Check for permissible transitions;
                 for (int j = 0; j < s.GetNumDestinations(); ++j)
                 {
                     if (Tools::Contains(possibleStates[ip + 1], (int)s.GetDestinations()[j]))
                         existPermittedTransitions = true;
                 }
             }
         }
         //Calculate viterbiNodes;
         for (int i = 0; i < numStates; ++i)
         {
             if (abs(viterbiNodes[ip][i].weight - IMPOSSIBLE_WEIGHT) < EPS)
                 continue;
             if (existsPermittedStates && !Tools::Contains(possibleStates[ip], i))
                 continue;
             State& s = states->at(i);
             for (int j = 0; j < s.GetNumDestinations(); ++j)
             {
                 State& destination = states->at((unsigned)s.GetDestinations()[j]);
                 if (existPermittedTransitions &&
                     !Tools::Contains(possibleStates[ip + 1], destination.GetIndex()))
                     continue;
                 //int viterbiDestinationIndex = (ip + 1) * numStates + destination.index;
                 double weight = viterbiNodes[ip][i].weight +
                     calculateTransitionWeight(i, j, features[ip]);
                 if (ip == latticeLength - 2)
                     weight += parameters.GetFinalWeights()[destination.GetIndex()];
                 if (weight > viterbiNodes[ip + 1][destination.GetIndex()].weight)
                 {
                     viterbiNodes[ip + 1][destination.GetIndex()].weight = weight;
                     viterbiNodes[ip + 1][destination.GetIndex()].bestPredecessor = &viterbiNodes[ip][i];
                 }
             }
         }
     }
 }
	
