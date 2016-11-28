/** 
 * @file	ChainCollectionLearner.h
 * @brief	header file of the CRF chain model learner
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


#ifndef CHAIN_COLLECTION_LEARNER
#define CHAIN_COLLECTION_LEARNER

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <omp.h>
#include <tuple>

#include "IChainTransformer.h"
#include "ChainOptimizable.h"
#include "IPossibleStateFinder.h"
#include "IRegularization.h"
#include "VectorFactorsExtractor.h"
#include "Factors.h"
#include "LBFGS.h"
#include "CRF.h"
#include "State.h"
#include "CRFBinaryWriter.h"
#include "CRFBinaryReader.h"
#include "ChainCollectionTester.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::unordered_map;
using std::unordered_set;
using std::tuple;
using std::wcout;
using std::wcin;

#define CHAIN_COLLECTION_LEARNER_TEMPLATE template < \
    class Optimizer_ \
    , class Tester_ \
    , ConsideringOfFeaturesInCliques typeOfFeaturesConsideration>

#define CHAIN_COLLECTION_LEARNER_TEMPLATE1 ChainCollectionLearner< \
    Optimizer_, Tester_, typeOfFeaturesConsideration>

namespace LinearCRF
{

const string problemChainsFile = "IndexesOfProblemSentences.txt";
static int MAX_NUMBER_IN_TRAINING_SET = 10000000;

/** 
 * @class	ChainCollectionLearner
 * @brief	CRF chain model learner
 */
CHAIN_COLLECTION_LEARNER_TEMPLATE
class ChainCollectionLearner 
    : public Optimization::IOptimizable
    , public Optimization::ICallable
{
public:
    typedef ChainOptimizable<typeOfFeaturesConsideration> OptimizableChain;
    typedef Optimizer_ Optimizer;
    typedef Tester_ Tester;

    ChainCollectionLearner() {}

    /**
	 * @brief	Constructor. Reads file filled in special <token features label> format
	 * @param[in]	trainingSetFile - file for training
     * @param[in]	testSetFile - file for testing intermediate results
     * @param[in]	ChainTransformer - transforms each chain
     * @param[in]	possibleStateFinder - finds possible states of chains (only using features)
     * @param[in]	regularizationFactor - factor used for smoothing optimizable function
	 */
    ChainCollectionLearner(
        const string& trainingSetFile
        , const string& testSetFile
        , const string& outputFile
        , shared_ptr<IChainTransformer> ChainTransformer 
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , shared_ptr<IRegularization> regularizator);


    /**
	 * @brief	Prints chain in the form that enables to check correctness of pipe and possible state finder
     *          (token, label, possible states)
	 * @param[in]	file - file where to print results
	 */
    void PrintForChecking(const string& file);

    /**
	 * @brief	Prints chain in the form that enables to check correctness of pipe and possible state finder
     *          (token, label, possible states)
	 * @param[in]	file - file where to print results
     * @param[in]	sentenceIndex - index of sentence in training set
	 */
    void PrintOneSentenceForChecking(const string& file, int sentenceIndex);

    /**
	 * @brief	Destructor
	 */
    ~ChainCollectionLearner(void);

    /**
	 * @brief	Calculates log probability of the chain
	 * @param[in]	point - parameters for calculations
	 */
    double CalculateValue(const vector<double>& point);

    /**
	 * @brief	Calculates gradient of log probability of the chain
	 * @param[in]	point - parameters for calculations
     * @param[out]	gradient - gradient of log probability of the chain
	 */
    void CalculateGradient(
        const vector<double>& point, vector<double>* gradient);

    /**
	 * @brief	Finds optimal parameters for CRF
	 * @param[in]	numberOfIterations - max number of gradient descent steps
     * @param[in]	periodOfTesting - period of iterations when preliminary tester starts evaluation
     * @param[in]	periodOfPriniting - period of iterations when the model is saved to auxiliary file
	 */
    virtual CRF Learn(int numberOfIterations, int periodOfTesting
        , int periodOfPriniting);

     /**
     * @brief	Evaluates auxiliary results and decides if it's time to stop learning
     * @param[in]	currentIteration - iteration of gradient calculations
     * @param[in]	_parameters - current parameters of the model
     * @param[in]	dimension - size of parameters
     */
    virtual bool Call(int currentIteration
        , const double* _parameters
        , int dimension);

    /**
    * @brief	Gets vector factors extractor. For example, can be used to get special regularization in relearning
    */
    shared_ptr<VectorFactorsExtractor> GetVectorFactorsExtractor();


    string outputFile;
    shared_ptr<IChainTransformer> chainTransformer; /**< ChainTransformer transforming chains from file into some prefered format*/
    shared_ptr<IPossibleStateFinder> possibleStateFinder; /**< Enables to find possible states of chains (only using features)*/
    shared_ptr<IRegularization> regularizator; /**< Factor used for smoothing optimizable function*/
    shared_ptr<Alphabet> weightAlphabet; /**< cliques of CRF*/
    shared_ptr<Alphabet> inputAlphabet; /**< features for CRF*/
    shared_ptr<Alphabet> outputAlphabet; /**< hidden labels of CRF*/
    shared_ptr<TransitionConverter> transitionConverter;  /**< converts tranisition (input index to output index) to index of weightAlphabet*/
    shared_ptr<VectorFactorsExtractor> vectorFactorsExtractor; /**< enables to find CRF parameters from vector*/
    vector<OptimizableChain> chains; /**< data for learning*/
    int periodOfTesting; /**< period of iterations when preliminary tester starts evaluation*/
    int periodOfPrinting; /**< period of iterations when the model is saved to auxiliary file*/
    CRF outputModel; /**< model that should be learned and printed by the algorithm*/
    Tester tester; /**< evaluates intermediate results of the algorithm*/
    
    bool areValueAndGradientCalculated /**< gradient and value are calculated together at once. This variable enables not to recalculate them in alternating call of CalculateValue and CalculateGradient*/;
    double value;
    vector<double> gradient;

    /**
	 * @brief	Gets chains optimizables from file
	 * @param[in]	file - file to read
	 */
    void readFile(const string& file);

    /**
	 * @brief	Finds possible features for each clique
	 */
    unordered_map<int, unordered_set<int> > findCliquePossibleFeatures();

    /**
	 * @brief	Initializes vectorFactorsExtractor
	 */
    void initializeVectorFactorsExtractor();

    /**
	 * @brief	Prints some information after chains are downloaded
	 */
    void printResultsOfInitialization();

    /**
     * @brief	Calculates value and gradient; Calculate value and gradient calls this function
     * @param[in]	point - parameters to optimize
     */
    void calculateGradientAndValue(const vector<double>& point);

    /**
     * @brief	Finds states that are used in output CRF model
     * @param[in]	crf - crf model
     */
    shared_ptr<vector<State> > getStates(CRF* crf);

    /**
     * @brief	Finds cliques data that is used in output CRF model
     * @param[in]	factorsVector - parameters of the model
     */
    shared_ptr<vector<unordered_map<int, double> > > getCliquesData(
        const vector<double>& factorsVector);

    /**
     * @brief	After the training file is read we full alphabets and states in outputModel
     */
    void initializeOutputModel();

    /**
     * @brief	Reads data for tester
     */
    void initializeTester(const string& testSetFile);

    /**
     * @brief	Fills crf-model with calculated content
     * @param[in]	factorsVector - parameters of the model
     */
    void fillOutputModel(const vector<double>& factorsVector);

    /**
     * @brief	Verifies if possible states are correct in chain
     * @param[in]	chain - chain to process
     */
    bool checkChain(const Chain& chain);

    /**
     * @brief	Constructs transitionConverter by CRF-model
     * @param[in]	i_crf - CRF-model
     */
    void fillTransitionConverter(const CRF& i_crf);
};

CHAIN_COLLECTION_LEARNER_TEMPLATE
CHAIN_COLLECTION_LEARNER_TEMPLATE1::ChainCollectionLearner(
    const string& trainingSetFile
    , const string& testSetFile
    , const string& outputFile
    , shared_ptr<IChainTransformer> chainTransformer
    , shared_ptr<IPossibleStateFinder> possibleStateFinder
    , shared_ptr<IRegularization> regularizator)
    : outputFile(outputFile)
    , chainTransformer(chainTransformer)
    , possibleStateFinder(possibleStateFinder)
    , regularizator(regularizator)
    , weightAlphabet(new Alphabet())
    , inputAlphabet(new Alphabet())
    , outputAlphabet(new Alphabet())
    , transitionConverter(new TransitionConverter)
    , areValueAndGradientCalculated(false)
{
    wcout << "ChainCollectionLearner: number of cores: " << omp_get_num_procs() << std::endl;
    vectorFactorsExtractor = std::make_shared<VectorFactorsExtractor>();
    readFile(trainingSetFile);
    initializeVectorFactorsExtractor();
    dimension = vectorFactorsExtractor->GetDimension();
    initializeOutputModel();
    initializeTester(testSetFile);
    printResultsOfInitialization();
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::readFile(const string& file)
{
    wcout << "ChainCollectionLearner: starting reading file..." << std::endl;
    // Create auxiliary iterators, etc.
    int enumerator = 0;
    int badChainsEnumerator = 0;
    wifstream in(file);
	Tools::SetLocale(in);
    Chain chain;
    // Iterate over file
    while(in >> chain)
    {
        if (chain.GetSize() == 0)
        {
            break;
        }
        // Pass chain through ChainTransformer
        Chain transformedChain = chainTransformer->ForwardTransform(chain);
        possibleStateFinder->FindPossibleStates(&transformedChain);
        if (checkChain(transformedChain)) {
            // Transform chain to digital format and insert to the collection
            chains.emplace_back(
                OptimizableChain(
                transitionConverter, vectorFactorsExtractor 
                , weightAlphabet, inputAlphabet
                , outputAlphabet, transformedChain));
        } else {
            ++badChainsEnumerator;
        }
        // Print status of data downloading
        ++enumerator;
        wcout << L"\rHave evaluated "
              << enumerator << L" chains, number of bad chains = "
              << badChainsEnumerator;
        if (enumerator > MAX_NUMBER_IN_TRAINING_SET)
        {
            break;
        }
    }
    in.close();
    wcout << L"\rHave evaluated " << chains.size() << " chains...";
    wcout << L"\nChainCollectionLearner: end reading file...\n";
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
unordered_map<int, unordered_set<int> > CHAIN_COLLECTION_LEARNER_TEMPLATE1::
    findCliquePossibleFeatures()
{
    wcout << L"\nFinding possible features for cliques...\n";
    unordered_map<int, unordered_set<int> > cliquePossibleFeatures;
    for (int iter = 0; iter < weightAlphabet->Size(); ++iter)
    {
        cliquePossibleFeatures[iter] = unordered_set<int>();
    }
    for (size_t chainEnumerator = 0; chainEnumerator < 
        chains.size(); ++chainEnumerator)
    {
        chains[chainEnumerator].UpdateCliquePossibleFeatures(
            &cliquePossibleFeatures);
        wcout << L"\rHave evaluated " << chainEnumerator << L" chains...";
    }
    return cliquePossibleFeatures;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::initializeVectorFactorsExtractor()
{
    unordered_map<int, unordered_set<int> > cliquePossibleFeatures = 
        findCliquePossibleFeatures();
    wcout << "\nInitializing vectors factor extractor...\n";
    vectorFactorsExtractor->Initialize(
        outputAlphabet->Size(), 
        inputAlphabet->Size(), 
        weightAlphabet->Size(), 
        cliquePossibleFeatures);
    wcout << "\nEnd initializing vectors factor extractor...\n";
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::initializeOutputModel()
{
    outputModel.SetInputAlphabet(inputAlphabet);
    outputModel.SetOutputAlphabet(outputAlphabet);
    outputModel.SetWeightAlphabet(weightAlphabet);
    shared_ptr<vector<State> > states = getStates(&outputModel);
    outputModel.SetStates(states);
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::initializeTester(
    const string& testSetFile)
{
    tester = Tester(chainTransformer, possibleStateFinder
        , &outputModel);
    tester.Initialize(testSetFile);
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::printResultsOfInitialization()
{
    wcout << "\nChainCollectionLearner: constructor finished..." << std::endl;
    wcout << "Number of hidden states: " << outputAlphabet->Size() << std::endl;
    wcout << "Number of features: " << inputAlphabet->Size() << std::endl;
    wcout << "Number of cliques: " << weightAlphabet->Size() << std::endl;
    wcout << "Dimension of the model: " << dimension << std::endl;
    wcout << "Input something... " << std::endl;
    wstring s;
    wcin >> s;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
CHAIN_COLLECTION_LEARNER_TEMPLATE1::~ChainCollectionLearner(void)
{
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::PrintForChecking(const string& file)
{
    wofstream out(file);
	Tools::SetLocale(out);
    for (size_t chainIndex = 0; chainIndex < chains.size(); ++chainIndex)
    {
        chains[chainIndex].PrintForChecking(out);
        out << std::endl;
    }
    out.close();
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::PrintOneSentenceForChecking(
    const string& file, int sentenceIndex)
{
    wofstream out(file);
	Tools::SetLocale(out);
    chains[sentenceIndex].PrintForChecking(out);
    out.close();
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
double CHAIN_COLLECTION_LEARNER_TEMPLATE1::CalculateValue(const vector<double>& point)
{
    if (!areValueAndGradientCalculated) 
    {
        calculateGradientAndValue(point);
        areValueAndGradientCalculated = true;
    } else {
        areValueAndGradientCalculated = false;
    }
    return value;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::CalculateGradient(
        const vector<double>& point, vector<double>* gradientToCalculate)
{
    if (!areValueAndGradientCalculated) 
    {
        calculateGradientAndValue(point);
        areValueAndGradientCalculated = true;
    } else {
        areValueAndGradientCalculated = false;
    }
    *gradientToCalculate = gradient;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
shared_ptr<VectorFactorsExtractor> CHAIN_COLLECTION_LEARNER_TEMPLATE1::GetVectorFactorsExtractor()
{
    return vectorFactorsExtractor;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::calculateGradientAndValue(
    const vector<double>& point)
{
    value = 0;
    gradient.resize(point.size());
    for (size_t gradIndex = 0; gradIndex < gradient.size(); ++gradIndex)
    {
        gradient[gradIndex] = 0;
    }
    // Create threads
    vector<double> threadSplittedValues(omp_get_num_procs(), 0);
    vector<vector<double>> threadSplittedGradient(omp_get_num_procs());
    for (int it = 0; it < omp_get_num_procs(); ++it)
    {
        threadSplittedGradient[it].resize(gradient.size());
    }
    // Calculate value
    #pragma omp parallel for
    for (int it = 0; it < int(chains.size()); ++it)
    {
        threadSplittedValues[omp_get_thread_num()] += 
            chains[it].CalculateValueAndGradient(
            point, &threadSplittedGradient[omp_get_thread_num()]);
    }
    // Merge into one value
    for (size_t threadIndex = 0; threadIndex < threadSplittedValues.size()
        ; ++threadIndex)
    {
        value += threadSplittedValues[threadIndex];
        for (size_t gradIndex = 0; gradIndex < gradient.size(); ++gradIndex)
        {
            gradient[gradIndex] += threadSplittedGradient[threadIndex][gradIndex];
        }
    }
    // Calculate normFactor
    regularizator->UpdateValue(point, chains.size(), &value);
    regularizator->UpdateGradient(point, chains.size(), &gradient);
    // std::wcout << L"Value: " << value << std::endl;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
CRF CHAIN_COLLECTION_LEARNER_TEMPLATE1::Learn(
    int numberOfIterations, int _periodOfTesting
    , int _periodOfPriniting)
{
    periodOfTesting = _periodOfTesting;
    periodOfPrinting = _periodOfPriniting;
    clock_t startTime = clock();
    std::wcout << L"Starting learning, number of iterations = "
          << numberOfIterations << std::endl;
    int dimension = vectorFactorsExtractor->GetDimension();
    vector<double> initialPoint(dimension, 0);
    vector<double> parameters(dimension);
    Optimizer optimizer;
    optimizer.Optimize(*this, this, initialPoint, &parameters, numberOfIterations);
    clock_t endTime = clock();
    fillOutputModel(parameters);
    std::wcout << L"Have learned for " <<
        (double)(endTime - startTime) / CLOCKS_PER_SEC << L" seconds" << endl;
    return outputModel;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
shared_ptr<vector<State> > CHAIN_COLLECTION_LEARNER_TEMPLATE1::getStates(CRF* crf)
{
    shared_ptr<vector<State> > states(new vector<State>);
    for (int stateIndex = 0; stateIndex < outputAlphabet->Size(); ++stateIndex)
    {
        // Getting state name
        const wstring& stateName = outputAlphabet->LookUpObject(stateIndex);
        // Getting destinations
        const unordered_map<int, int>& destinationToClique = (*transitionConverter)[stateIndex];
        shared_ptr<vector<int> > destinations(new vector<int>());
        destinations->reserve(destinationToClique.size());
        shared_ptr<vector<vector<int> > > weightIndexes(new vector<vector<int> >);
        weightIndexes->reserve(destinationToClique.size());
        shared_ptr<vector<wstring> > destinationNames(new vector<wstring>);
        destinationNames->reserve(destinationToClique.size());
        for (auto iterator = destinationToClique.begin();
             iterator != destinationToClique.end(); ++iterator)
        {
            destinations->push_back(iterator->first);
            weightIndexes->push_back(vector<int>(1, iterator->second));
            destinationNames->push_back(outputAlphabet->LookUpObject(iterator->first));
        }
        states->emplace_back(State(
            stateName, stateIndex, destinations,
            weightIndexes, destinationNames, crf));
    }
    return states;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
shared_ptr<vector<unordered_map<int, double> > > CHAIN_COLLECTION_LEARNER_TEMPLATE1::getCliquesData(
        const vector<double>& factorsVector)
{
    vector<unordered_map<int, double> > cliquesData;
    for (int cliqueIndex = 0; cliqueIndex < weightAlphabet->Size(); ++cliqueIndex)
    {
        cliquesData.emplace_back(
            vectorFactorsExtractor->GetCliqueWeights(cliqueIndex, factorsVector));
    }
    return std::make_shared<vector<unordered_map<int, double> > >(
        std::move(cliquesData));
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::fillOutputModel(
    const vector<double>& factorsVector)
{
    outputModel.SetInitialWeights(vectorFactorsExtractor->GetInitialWeights(factorsVector));
    outputModel.SetDefaultWeights(vectorFactorsExtractor->GetDefaultWeights(factorsVector));
    outputModel.SetFinalWeights(vectorFactorsExtractor->GetFinalWeights(factorsVector));
    shared_ptr<vector<unordered_map<int, double>> > cliqueWeights = getCliquesData(factorsVector);
    outputModel.SetWeights(cliqueWeights);
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
bool CHAIN_COLLECTION_LEARNER_TEMPLATE1::Call(int currentIteration
    , const double* _parameters, int dimension)
{
    bool shouldPrint = (currentIteration > 0) && (currentIteration % periodOfPrinting == 0);
    bool shouldTest = (currentIteration > 0) && (currentIteration % periodOfTesting == 0);
    if (shouldPrint || shouldTest)
    {
        std::wcout << L"Printing model" << std::endl;
        vector<double> parameters(dimension);
        for (int index = 0; index < dimension; ++index)
        {
            parameters[index] = _parameters[index];
        }
        fillOutputModel(parameters);
        CRFBinaryWriter writer(outputFile);
        writer.Write(outputModel);
    }
    if (shouldTest)
    {
        CRFBinaryReader reader(outputFile);
        reader.Read(outputModel);
        TestedResultsData resultData = tester.TestToFile("REAL_TEST_SET.txt");
        resultData.Print();
    }
    return false;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
bool CHAIN_COLLECTION_LEARNER_TEMPLATE1::checkChain(
    const Chain& chain)
{
    const vector<wstring>& labels = chain.GetLabels();
    const vector<vector<wstring> >& possibleStates = chain.GetPossibleStates();
    for (size_t nodeIndex = 0; nodeIndex < labels.size(); ++nodeIndex)
    {
        bool found = false;
        for (size_t labelIndex = 0; labelIndex < possibleStates[nodeIndex].size()
            ; ++labelIndex)
        {
            if (possibleStates[nodeIndex][labelIndex] == labels[nodeIndex])
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}

CHAIN_COLLECTION_LEARNER_TEMPLATE
void CHAIN_COLLECTION_LEARNER_TEMPLATE1::fillTransitionConverter(
    const CRF& i_crf)
{
    const vector<State>& states = i_crf.GetStates();
    for (size_t sIndex = 0; sIndex < states.size(); ++sIndex)
    {
        // Add state
        int stateIndex = states[sIndex].GetIndex();
        auto stateIterator = transitionConverter->find(stateIndex);
        if (stateIterator == transitionConverter->end())
        {
            (*transitionConverter)[stateIndex] = unordered_map<int, int>();
            stateIterator = transitionConverter->find(stateIndex);
        }
        // Add transitions
        const vector<int>& transitionIndexes = states[sIndex].GetDestinations();
        const vector<vector<int> >& cliqueIndexes = states[sIndex].GetWeightsIndexes();
        for (size_t tIndex = 0; tIndex < transitionIndexes.size(); ++tIndex)
        {
            stateIterator->second[transitionIndexes[tIndex]] = cliqueIndexes[tIndex][0];
        }
    }
}


};

#endif // CHAIN_COLLECTION_LEARNER
