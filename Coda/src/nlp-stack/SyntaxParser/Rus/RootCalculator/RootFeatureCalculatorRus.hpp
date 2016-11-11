/** 
 * @file	RootFeatureCaclulator.h
 * @brief	header file for root features calculator
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


#ifndef ROOT_FEATURE_CALCULATOR
#define ROOT_FEATURE_CALCULATOR

#include "BatchTypeIdentifier.h"
#include "SyntaxNode.h"
#include "SyntaxTree.h"

namespace SyntaxParser
{

#define ROOT_FEATURE_CALCULATOR_TEMPLATE template < \
    bool areFeaturesDiscrete>

#define ROOT_FEATURE_CALCULATOR_TEMPLATE1 RootFeatureCalculatorRus< \
    areFeaturesDiscrete>

/** 
 * @class	RootFeatureCalculatorRus
 * @brief	Calculates root features one separate nodes in syntax tree
 */
ROOT_FEATURE_CALCULATOR_TEMPLATE
class RootFeatureCalculatorRus
{
public:
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::IntBatchType IntBatch;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::StringBatchType StringBatch;

    /**
     * @brief	Constructor
     */
    RootFeatureCalculatorRus() {};

    /**
     * @brief	Destructor
     */
    ~RootFeatureCalculatorRus() {};

    /**
     * @brief	Calculates features for node
     * @param[in]	node - node to process
     * @param[in]	tree - syntax tree to process
	 * @param[out]	features - features to calculate
     */
    StringBatch Calculate(
        const SyntaxNode& node
        , const SyntaxTree& tree);

    /**
     * @brief	Calculates features for node
     * @param[in]	tree - syntax tree to process
	 * @param[out]	features - features to calculate
     */
    vector<StringBatch> Calculate(const SyntaxTree& tree);
};

ROOT_FEATURE_CALCULATOR_TEMPLATE
typename ROOT_FEATURE_CALCULATOR_TEMPLATE1::StringBatch
ROOT_FEATURE_CALCULATOR_TEMPLATE1::Calculate(
    const SyntaxNode& node, const SyntaxTree& tree)
{
    vector<wstring> features = node.splittedLabel;
    wstring full = L"Root";
    for (size_t it = 0; it < features.size(); ++it)
    {
        full += features[it] + L"@";
    }
    for(size_t it = 0; it < features.size(); ++it)
    {
        features[it] = L"Root" + features[it];
    }
    features.push_back(full);
    return StringBatch(std::move(features));
}

ROOT_FEATURE_CALCULATOR_TEMPLATE
vector<typename ROOT_FEATURE_CALCULATOR_TEMPLATE1::StringBatch>
ROOT_FEATURE_CALCULATOR_TEMPLATE1::Calculate(
    const SyntaxTree& tree)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    vector<StringBatch> features;
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        features.emplace_back(Calculate(nodes[nodeIndex], tree));
    }
    return features;
}

}

#endif // ROOT_FEATURE_CALCULATOR
