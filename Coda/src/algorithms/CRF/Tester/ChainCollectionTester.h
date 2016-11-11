/**
 * @class	ChainCollectionTester
 * @brief	Tests CRF classifier
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


#ifndef CHAIN_COLLECTION_TESTER
#define CHAIN_COLLECTION_TESTER

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <iostream>

#include "IChainTransformer.h"
#include "IPossibleStateFinder.h"
#include "CRF.h"
#include "Chain.h"

using std::string;
using std::wstring;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::tuple;

namespace LinearCRF
{

struct EncodedCRFData
{
    vector<vector<int> > intFeatures;
    vector<vector<int> > possibleStates;
    vector<int> intLabels;
};

struct DecodedCRFData
{
    vector<wstring> tokens;
    vector<wstring> labels;
};

struct TestedResultsData
{
    int numberOfCorrect;
    int totalNumber;
    int numberOfFullyRightChains;
    int numberOfAllChains;

    TestedResultsData(
        int numberOfCorrect, int totalNumber
        , int numberOfFullyRightChains
        , int numberOfAllChains);

    void Print();
};

/**
 * @class	ChainCollectionTester
 * @brief	Tests CRF classifier
 */
class ChainCollectionTester
{
public:
    /**
     * @brief	Default constructor
     */
    ChainCollectionTester();

    /**
     * @brief	Constructor. Reads file filled in special <token features label> format
     * @param[in]	chainTransformer - transforms chain before applying crf
     * @param[in]	possibleStateFinder - searches for possible state in chain
     * @param[in]	crf - the model
     */
    ChainCollectionTester(
        shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , CRF* crf);

    /**
     * @brief	Performs results on test set
     */
    virtual TestedResultsData Test();

    /**
     * @brief	Sets classifier for testing
     * @param[in]	testSetFile - file with data to read
     */
    void Initialize(const string& testSetFile);

    /**
     * @brief	Calculates errors and prints results for each sentence in test data
     * @param[in]	outputFile - file where to print results
     */
    virtual TestedResultsData TestToFile(const string& outputFile);

    /**
     * @brief	Prints results to folder
     * @param[in]	outputFolder - folder where to print results
     */
    virtual TestedResultsData TestToFolder(const string& outputFolder);
protected:
    CRF* crf_; /**< classifier*/
    shared_ptr<IChainTransformer> chainTransformer_; /**< ChainTransformer transforming chains from file into some prefered format*/
    shared_ptr<IPossibleStateFinder> possibleStateFinder_; /**< finds possible states in the chain*/
    shared_ptr<Alphabet> inputAlphabet_; /**< Converts features to their indexes*/
    shared_ptr<Alphabet> outputAlphabet_; /**< Converts labels to their indexes*/
    vector<EncodedCRFData> encodedData_; /**< Data for testing in integer representation (after chainConverter appliance)*/
    vector<DecodedCRFData> decodedData_; /**< Data for testing in string representation (before chainConverter appliance)*/

    /**
     * @brief	Calculates indexes of features using chain
     * @param[in]	inputAlphabet - features to their indexes
     * @param[in]	chain - chain containing names of features
     */
    vector<vector<int> > getFeatureIndexes(
        const Chain& chain);

    vector<vector<int> > getPossibleStates(
        const Chain& chain);

    /**
     * @brief	Calculates indexes of features using chain
     * @param[in]	outputAlphabet - features to their indexes
     * @param[in]	chain - chain containing names of labels
     * @param[out]	labels - labels to get
     * @param[out]	labelIndexes - label indexes to get
     */
    vector<int> getLabelIndexes(const Chain& chain);
};

};

#endif // CHAIN_COLLECTION_TESTER
