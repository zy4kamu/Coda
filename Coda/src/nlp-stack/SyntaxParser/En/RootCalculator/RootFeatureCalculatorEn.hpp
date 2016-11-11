/**
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

#ifndef ROOT_FEATURE_CALCULATOR_EN
#define ROOT_FEATURE_CALCULATOR_EN

#include "BatchTypeIdentifier.h"
#include "SyntaxNode.h"
#include "SyntaxTree.h"

namespace SyntaxParser
{

#define ROOT_FEATURE_CALCULATOR_TEMPLATE template < \
    bool areFeaturesDiscrete>

#define ROOT_FEATURE_CALCULATOR_TEMPLATE1 RootFeatureCalculatorEn< \
    areFeaturesDiscrete>

ROOT_FEATURE_CALCULATOR_TEMPLATE
class RootFeatureCalculatorEn
{
public:
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::IntBatchType IntBatch;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::StringBatchType StringBatch;

    StringBatch Calculate(
        const SyntaxNode& node
        , const SyntaxTree& tree);

    vector<StringBatch> Calculate(const SyntaxTree& tree);
};

ROOT_FEATURE_CALCULATOR_TEMPLATE
typename ROOT_FEATURE_CALCULATOR_TEMPLATE1::StringBatch
ROOT_FEATURE_CALCULATOR_TEMPLATE1::Calculate(
    const SyntaxNode& node, const SyntaxTree& tree)
{
    return StringBatch(vector<wstring> { L"Root" + node.label });
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

#endif // ROOT_FEATURE_CALCULATOR_EN
