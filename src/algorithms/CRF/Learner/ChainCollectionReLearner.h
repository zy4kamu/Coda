/**
 * @file	ChainCollectionReLearner.h
 * @brief	header file of the CRF chain model relearner
 * The goal is to make some small corrections and learn on new data with old crf
 * The implementation contructs crf learner and then runs it many times with accordance
 * to optimization strategy.
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


#include "ChainCollectionLearner.h"
#include "ChainCollectionTester.h"
#include "RelearnRegularization.h"


#define CHAIN_COLLECTION_RE_LEARNER_TEMPLATE template < \
    class Learner>

#define CHAIN_COLLECTION_RE_LEARNER_TEMPLATE1 ChainCollectionReLearner< \
    Learner>

namespace LinearCRF
{

/**
 * @class	ChainCollectionReLearner
 * @brief	CRF chain model learner
 */
CHAIN_COLLECTION_RE_LEARNER_TEMPLATE
class ChainCollectionReLearner : public Learner
{

public:
    /**
     * @brief	Constructor. The goal is to make some small corrections and learn on new data with old crf
     * @param[in]	crf - crf model to retrain
     * @param[in]	trainingSetFile - file for training
     * @param[in]	testSetFile - file for testing intermediate results
     * @param[in]	ChainTransformer - transforms each chain
     * @param[in]	possibleStateFinder - finds possible states of chains (only using features)
     * @param[in]	regularizationFactor - factor used for smoothing optimizable function
     */
    ChainCollectionReLearner(
        shared_ptr<CRF> crf
        , const string& trainingSetFile
        , const string& testSetFile
        , const string& outputFile
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , shared_ptr<RelearnRegularization> regularizator
        , int maxNumberOfRelearnIterations = 30);

    /**
     * @brief	Finds optimal parameters for CRF
     * @param[in]	numberOfIterations - max number of gradient descent steps
     * @param[in]	periodOfTesting - period of iterations when preliminary tester starts evaluation
     * @param[in]	periodOfPriniting - period of iterations when the model is saved to auxiliary file
     */
    virtual CRF Learn(int numberOfIterations, int periodOfTesting
        , int periodOfPriniting);

    /**
    * @brief	Evaluates auxiliary results and searches if it's time to stop learning.
    * @param[in]	currentIteration - iteration of gradient calculations
    * @param[in]	_parameters - current parameters of the model
    * @param[in]	dimension - size of parameters
    */
   virtual bool Call(int currentIteration
       , const double* _parameters
       , int dimension);

private:
    string outputFile;
    shared_ptr<CRF> initialCRF;
    vector<double> initialParameters;
    int maxNumberOfRelearnIterations;

    CRFModelUpdatedData correctModel(const string& file);
};

CHAIN_COLLECTION_RE_LEARNER_TEMPLATE
CHAIN_COLLECTION_RE_LEARNER_TEMPLATE1::ChainCollectionReLearner(
    shared_ptr<CRF> crf
    , const string& trainingSetFile
    , const string& testSetFile
    , const string& outputFile
    , shared_ptr<IChainTransformer> _chainTransformer
    , shared_ptr<IPossibleStateFinder> _possibleStateFinder
    , shared_ptr<RelearnRegularization> _regularizator
    , int maxNumberOfIterations)
{
    this->outputFile = outputFile;
    this->chainTransformer = _chainTransformer;
    this->possibleStateFinder = _possibleStateFinder;
    this->regularizator = _regularizator;
    this->transitionConverter = shared_ptr<TransitionConverter>(
        new TransitionConverter);
    this->areValueAndGradientCalculated = false;
    this->maxNumberOfRelearnIterations = maxNumberOfIterations;

    this->initialCRF = crf;
    CRFModelUpdatedData updatedData = correctModel(trainingSetFile);
    this->outputModel = *(this->initialCRF);

    this->weightAlphabet = this->initialCRF->GetWeightsAlphabetPtr();
    this->inputAlphabet = this->initialCRF->GetInputAlphabetPtr();
    this->outputAlphabet = this->initialCRF->GetOutputAlphabetPtr();
    wcout << "ChainCollectionLearner: number of cores: " << omp_get_num_procs() << std::endl;
    this->vectorFactorsExtractor = shared_ptr<VectorFactorsExtractor>(
        new VectorFactorsExtractor(this->outputModel, &initialParameters));
    std::dynamic_pointer_cast<RelearnRegularization>(this->regularizator)->Initialize(
        initialParameters);
    this->fillTransitionConverter(this->outputModel);
    this->readFile(trainingSetFile);
    this->dimension = this->vectorFactorsExtractor->GetDimension();
    this->initializeTester(testSetFile);
    this->printResultsOfInitialization();
}

CHAIN_COLLECTION_RE_LEARNER_TEMPLATE
CRF CHAIN_COLLECTION_RE_LEARNER_TEMPLATE1::Learn(
    int numberOfIterations, int _periodOfTesting
    , int _periodOfPriniting)
{
    this->periodOfTesting = _periodOfTesting;
    this->periodOfPrinting = _periodOfPriniting;
    bool shouldTerminate = false;

    int iterationIndex = 0;
    while(!shouldTerminate)
    {
        // Learn
        int dimension = this->vectorFactorsExtractor->GetDimension();
        vector<double> parameters(dimension);
        typename Learner::Optimizer optimizer;
        optimizer.Optimize(*this, this, initialParameters, &parameters, numberOfIterations, false);
        this->fillOutputModel(parameters);
        // Print model to file
        CRFBinaryWriter writer(outputFile);
        writer.Write(this->outputModel);
        // Test
        CRFBinaryReader reader(outputFile);
        reader.Read(this->outputModel);
        TestedResultsData resultData = this->tester.Test();
        std::wcout
            << L"Results after this iteration: "
            << resultData.numberOfCorrect
            << L" is correct of "
            << resultData.totalNumber
            << L". Accuracy = "
            << static_cast<double>(resultData.numberOfCorrect)
               / static_cast<double>(resultData.totalNumber)
            << L" ... " << std::endl << std::endl;
        shouldTerminate = (resultData.numberOfCorrect == resultData.totalNumber)
            || (iterationIndex >= maxNumberOfRelearnIterations);
        // Decrease regularization factor
        if (!shouldTerminate)
        {
            ++iterationIndex;
            shared_ptr<RelearnRegularization> regularizer
                = std::dynamic_pointer_cast<RelearnRegularization>(this->regularizator);
            regularizer->SetRegularizationFactor(2.0 * regularizer->GetRegularizationFactor());
            std::wcout << iterationIndex
                       << L": Increasing regularization factor to "
                       << regularizer->GetRegularizationFactor()
                       << L" ..." << std::endl << std::endl;

        } else {
            std::wcout << L"Terminating..." << std::endl;
        }
    }
    return this->outputModel;
}

CHAIN_COLLECTION_RE_LEARNER_TEMPLATE
CRFModelUpdatedData CHAIN_COLLECTION_RE_LEARNER_TEMPLATE1::correctModel(
    const string& file)
{
    wifstream in(file);
	Tools::SetLocale(in);
    vector<Chain> chains;
    Chain chain;
    // Iterate over file
    while(in >> chain)
    {
        // Pass chain through ChainTransformer
        Chain transformedChain = this->chainTransformer->ForwardTransform(chain);
        this->possibleStateFinder->FindPossibleStates(&transformedChain);
        chains.push_back(std::move(transformedChain));
    }
    in.close();
    return initialCRF->Add(chains);
}

CHAIN_COLLECTION_RE_LEARNER_TEMPLATE
bool CHAIN_COLLECTION_RE_LEARNER_TEMPLATE1::Call(int currentIteration
    , const double* _parameters, int dimension)
{
    bool shouldPrint = (currentIteration > 0) && (currentIteration % this->periodOfPrinting == 0);
    bool shouldTest = (currentIteration > 0) && (currentIteration % this->periodOfTesting == 0);
    if (shouldPrint || shouldTest)
    {
        vector<double> parameters(dimension);
        for (int index = 0; index < dimension; ++index)
        {
            parameters[index] = _parameters[index];
        }
        this->fillOutputModel(parameters);
        CRFBinaryWriter writer(outputFile);
        writer.Write(this->outputModel);
    }
    if (shouldTest)
    {
        CRFBinaryReader reader(outputFile);
        reader.Read(this->outputModel);
        TestedResultsData resultData = this->tester.Test();
        if (resultData.numberOfCorrect == resultData.totalNumber) {
            return true;
        } else {
            std::wcout
                << resultData.numberOfCorrect
                << L" is correct of "
                << resultData.totalNumber
                << L". Accuracy = "
                << static_cast<double>(resultData.numberOfCorrect)
                   / static_cast<double>(resultData.totalNumber)
                << L" ... " << std::endl;
        }
    }
    return false;
}

}
