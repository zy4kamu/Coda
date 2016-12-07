/**
 * @class	ChainCollectionTester
 * @brief	Tests CRF classifier for the case when all labels in the sequence are equal
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


#ifndef UNIT_LABEL_CHAIN_COLLECTION_TESTER
#define UNIT_LABEL_CHAIN_COLLECTION_TESTER

#include "ChainCollectionTester.h"

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

namespace LinearCRF
{

/**
* @class	ChainCollectionTester
* @brief	Tests CRF classifier for the case when all labels in the sequence are equal
*/
class UnitLabelChainCollectionTester
    : public ChainCollectionTester
{
public:
    /**
     * @brief	Default constructor
     */
    UnitLabelChainCollectionTester();

    /**
     * @brief	Constructor. Reads file filled in special <token features label> format
     * @param[in]	chainTransformer - transforms chain before applying crf
     * @param[in]	possibleStateFinder - searches for possible state in chain
     * @param[in]	crf - the model
     */
    UnitLabelChainCollectionTester(
        shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , CRF* crf);

    /**
     * @brief	Performs results on test set
     */
    virtual TestedResultsData Test();

    /**
     * @brief	Calculates errors and prints results for each sentence in test data to different types
     * @param[in]	outputFile - file where to print results
     */
    virtual TestedResultsData TestToFolder(const string& outputFolder);
};

};


#endif // UNIT_LABEL_CHAIN_COLLECTION_TESTER
