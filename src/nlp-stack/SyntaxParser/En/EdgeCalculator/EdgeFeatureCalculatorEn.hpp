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

#ifndef EDGE_FEATURE_CALCULATOR_EN
#define EDGE_FEATURE_CALCULATOR_EN

#include <memory>
#include <cmath>

#include "BatchTypeIdentifier.h"
#include "NodeFeatureCalculatorEn.h"
#include "SyntaxTree.h"

using std::shared_ptr;

namespace SyntaxParser
{

#define EDGE_FEATURE_CALCULATOR_TEMPLATE template < \
    bool areFeaturesDiscrete \
    , class NodeFCalculator>

#define EDGE_FEATURE_CALCULATOR_TEMPLATE1 EdgeFeatureCalculatorEn< \
    areFeaturesDiscrete, NodeFCalculator>

template <bool areFeaturesDiscrete, class NodeFCalculator>
class EdgeFeatureCalculatorEn
{
public:

    typedef EdgeFeatureCalculatorEn<
        areFeaturesDiscrete
        , NodeFCalculator> EdgeFeatureCalculatorType;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::IntBatchType IntBatch;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::StringBatchType StringBatch;

    void CalculateFeatures(const SyntaxTree& tree
        , vector<vector<StringBatch> >* features);

private:
    NodeFCalculator nodeFeatureCalculator;
};

template<class Int>
wstring Ceil(Int distance)
{
    if (std::abs(distance) < 6) {
        return Tools::ConvertIntToWstring(distance);
    } else if (distance < 0) {
        return L"far-";
    } else {
        return L"far+";
    }
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
void EDGE_FEATURE_CALCULATOR_TEMPLATE1::CalculateFeatures(
    const SyntaxTree& tree
    , vector<vector<StringBatch> >* features)
{
    int size = tree.GetSize();
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    // Calculate features for nodes
    vector<vector<wstring> > nodeFeatures
        = nodeFeatureCalculator.Calculate(tree);
    // Calculate features
    features->resize(size);
    for (int leftIndex = 0; leftIndex < size; ++leftIndex)
    {
        (*features)[leftIndex].resize(size);
        for (int rightIndex = 0; rightIndex < size; ++rightIndex)
        {
            if (leftIndex == rightIndex)
            {
                continue;
            }
            StringBatch& currentFeatures = (*features)[leftIndex][rightIndex];

            wstring distanceAbbr = Ceil(
                nodes[rightIndex].index - nodes[leftIndex].index);

            wstring posLeftMin2 = (leftIndex > 1 ? nodes[leftIndex - 2].label : L"none");
            wstring posLeftMin1 = (leftIndex > 0 ? nodes[leftIndex].label : L"none");
            wstring posLeft = L"!" + nodes[leftIndex].label;
            wstring posLeftPlus1 = (leftIndex + 1 < size ? nodes[leftIndex + 1].label : L"none");
            wstring posLeftPlus2 = (leftIndex + 2 < size ? nodes[leftIndex + 2].label : L"none");


            wstring posRightMin2 = (rightIndex > 1 ? nodes[rightIndex - 2].label : L"none");
            wstring posRightMin1 = (rightIndex > 0 ? nodes[rightIndex].label : L"none");
            wstring posRight = L"!" + nodes[rightIndex].label;
            wstring posRightPlus1 = (rightIndex + 1 < size ? nodes[rightIndex + 1].label : L"none");
            wstring posRightPlus2 = (rightIndex + 2 < size ? nodes[rightIndex + 2].label : L"none");


            currentFeatures.Add(posLeft + L"->");
            currentFeatures.Add(L"->" + posRight);

            currentFeatures.Add(posLeftMin2 + posLeftMin1 + posLeft + posLeftPlus1 + L"->" + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeftMin2 + posLeftMin1 + posLeft + L"->" + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeftMin1 + posLeft + L"->" + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeft + posLeftPlus1 + L"->" + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeft + L"->" + posRight + L"_" + distanceAbbr);

            currentFeatures.Add(posLeft + L"->" + posRightMin2 + posRightMin1 + posRight + posRightPlus1 + L"_" + distanceAbbr);
            currentFeatures.Add(posLeft + L"->" + posRightMin2 + posRightMin1 + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeft + L"->" + posRightMin1 + posRight + L"_" + distanceAbbr);
            currentFeatures.Add(posLeft + L"->" + posRight + posRightPlus1 + L"_" + distanceAbbr);


            // Add features
            wstring prefix = nodes[leftIndex].label + L"_" + nodes[rightIndex].label;
            currentFeatures.Add(L"L" + nodes[leftIndex].label);
            currentFeatures.Add(L"R" + nodes[rightIndex].label);
            // Add left node features
            const vector<wstring>& leftFeatures = nodeFeatures[leftIndex];
            for (size_t it = 0; it < leftFeatures.size(); ++it)
            {
                currentFeatures.Add(
                    prefix + L"L" + leftFeatures[it]);
            }
            // Add right node features
            const vector<wstring>& rightFeatures = nodeFeatures[rightIndex];
            for (size_t it = 0; it < rightFeatures.size(); ++it)
            {
                currentFeatures.Add(prefix + L"R" + rightFeatures[it]);
            }

            currentFeatures.Add(nodes[leftIndex].lemma + L"->" + nodes[rightIndex].label + L"_" + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].label + L"->" + nodes[rightIndex].lemma + L"_" + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].content + L"->" + nodes[rightIndex].label + L"_" + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].label + L"->" + nodes[rightIndex].content + L"_" + distanceAbbr);

            currentFeatures.Add(nodes[leftIndex].lemma + L"->" + nodes[rightIndex].label);
            currentFeatures.Add(nodes[leftIndex].label + L"->" + nodes[rightIndex].lemma);
            currentFeatures.Add(nodes[leftIndex].content + L"->" + nodes[rightIndex].label);
            currentFeatures.Add(nodes[leftIndex].label + L"->" + nodes[rightIndex].content);
        }
    }
}

}

#endif // EDGE_FEATURE_CALCULATOR_EN

