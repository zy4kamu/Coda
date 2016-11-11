/** 
 * @file	NodeFeatureCalculator.h
 * @brief	header file for class calculating features used for edge calculator
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


#ifndef NODE_FEATURE_CALCULATOR
#define NODE_FEATURE_CALCULATOR

#include <set>
#include <memory>

#include "SyntaxTree.h"

using std::set;
using std::shared_ptr;

namespace SyntaxParser
{

/** 
 * @class	NodeFeatureCalculator
 * @brief	Calculates features for each node that combines features for edges
 */
class NodeFeatureCalculatorRus
{
public:
    /**
     * @brief	Constructor
     */
    NodeFeatureCalculatorRus();

    /**
     * @brief	Destructor
     */
    ~NodeFeatureCalculatorRus(void);

    /**
     * @brief	For each node in the tree calculates features
	 * @param[out]	features - features to calculate
     */
    vector<vector<wstring> > Calculate(const SyntaxTree& tree);
private:
    /**
     * @brief	For each node in the tree calculates indexes of the tokens containing punctuation
	 * @param[int]	nodes - nodes to process
     * @param[out]	indexes - indexes of punctuation nodes
     */
    vector<int> calculatePunctuationSplittings(
        const vector<SyntaxNode>& nodes);

    /**
     * @brief	Calculates features for each node in the sequence
	 * @param[int]	nodes - nodes to process
     * @param[out]	features - features for each node
     */
    vector<vector<wstring> > calculateOneTokenFeatures(
        const vector<SyntaxNode>& nodes);
    
    /**
     * @brief	Adds neighbours features to the node
	 * @param[int]	punctuationSplitters - indexes of tokens containing punctuation
     * @param[out]	features - features to update
     * @param[int]	conjunctions - number of neighbours to use while updating
     */
    void applyConjunctions(
        const vector<int>& punctuationSplitters
        , vector<vector<wstring> >* features
        , int conjunctions);
    
    /**
     * @brief	Adds features reltated to tokens in one part (two tokens are in one part if there is no punctuation between them)
	 * @param[int]	punctuationSplitters - indexes of tokens containing punctuation
     * @param[out]	features - features to update
     * @param[int]	conjunctions - number of neighbours to use while updating
     */
    void calculateOnePartFeatures(
        const vector<SyntaxNode>& nodes
        , const vector<int>& punctuationSplitters
        , vector<vector<wstring> >* features);

    shared_ptr<set<wstring> > exceptionTokens; /**< most frequent tokens used as features*/
};

};

#endif // NODE_FEATURE_CALCULATOR

