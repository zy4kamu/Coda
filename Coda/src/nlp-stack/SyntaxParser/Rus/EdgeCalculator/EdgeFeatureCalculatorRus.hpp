/**
 * @file	EdgeFeatureCalculator.h
 * @brief	header file for class calculating features between two nodes in syntax tree
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


#ifndef EDGE_FEATURE_CALCULATOR
#define EDGE_FEATURE_CALCULATOR


#include <memory>
#include <cmath>

#include "BatchTypeIdentifier.h"
#include "NodeFeatureCalculatorRus.h"
#include "SchemaCalculatorRus.h"
#include "SyntaxTree.h"

using std::shared_ptr;

namespace SyntaxParser
{

#define EDGE_FEATURE_CALCULATOR_TEMPLATE template < \
    bool areFeaturesDiscrete \
    , class NodeFCalculator \
    , bool USE_UNPRESENTED_EDGES>

#define EDGE_FEATURE_CALCULATOR_TEMPLATE1 EdgeFeatureCalculatorRus< \
    areFeaturesDiscrete, NodeFCalculator, USE_UNPRESENTED_EDGES>

/**
* @class	EdgeFeatureCalculatorRus
* @brief	Calculates features between two nodes in syntax tree
*/
template <bool areFeaturesDiscrete, class NodeFCalculator
    , bool USE_UNPRESENTED_EDGES = true>
class EdgeFeatureCalculatorRus
{
public:

    typedef EdgeFeatureCalculatorRus<
        areFeaturesDiscrete
        , NodeFCalculator
        , USE_UNPRESENTED_EDGES> EdgeFeatureCalculatorType;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::IntBatchType IntBatch;
    typedef typename BatchTypeIdentifier<areFeaturesDiscrete>::StringBatchType StringBatch;

    /**
     * @brief	Constructor
     */
    EdgeFeatureCalculatorRus();

    /**
     * @brief	Destructor
     */
    virtual ~EdgeFeatureCalculatorRus(void) {};

    /**
     * @brief	Calculates features for each node in the full-connected graph of syntax nodes
     * @param[in]	tree - syntax tree to process
     * @param[out]	features - features to calculate
     */
    void CalculateFeatures(const SyntaxTree& tree
        , vector<vector<StringBatch> >* features);

private:
    NodeFCalculator nodeFeatureCalculator; /**< calculates features for each node in the sentence (they are concatenated for each edges)*/
    unordered_map<wstring, unordered_map<wstring, int> > connections;/**< statistics on parent -> son statistics*/

    vector<wstring> getFullTags(const SyntaxTree& tree);

    void getTokensAndPunctuation(const SyntaxTree& tree
        , vector<wstring>* tokens, vector<wstring>* punctuation);

    wstring getDistanceSchema(
        int leftIndex, int rightIndex
        , const vector<wstring>& tokens
        , const vector<wstring>& fullTags
        , const vector<wstring>& punctuation);

    wstring getPartialSchema(
        int leftIndex, int rightIndex
        , const vector<SyntaxNode>& syntaxNodes
        , const vector<wstring>& punctuation);

    bool setUnbelivableTransitions(
        int leftIndex, int rightIndex
        , const vector<SyntaxNode>& syntaxNodes
        , vector<vector<StringBatch> >* features);

    int Ceil(int z);
};

EDGE_FEATURE_CALCULATOR_TEMPLATE
EDGE_FEATURE_CALCULATOR_TEMPLATE1::EdgeFeatureCalculatorRus()
{
    if (USE_UNPRESENTED_EDGES)
    {
        FileManager::Read(
            string(SYNTAX_CONFIG_FOLDER) + "RU/EdgeCalculatorConfig.txt"
            , &connections);
    }
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
int EDGE_FEATURE_CALCULATOR_TEMPLATE1::Ceil(int z)
{
    if (std::abs(z) < 5) return z;
    if (std::abs(z) < 10) return z > 0 ? 5 : -5;
    if (std::abs(z) < 15) return z > 0 ? 10 : -10;
    if (std::abs(z) < 20) return z > 0 ? 15 : -15;
    return 20;
}

template<class Int>
wstring MyCeil(Int distance)
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
void EDGE_FEATURE_CALCULATOR_TEMPLATE1::getTokensAndPunctuation(
    const SyntaxTree& tree, vector<wstring>* tokens, vector<wstring>* punctuation)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    size_t size = nodes.size();
    tokens->resize(size);
    punctuation->resize(size);
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        // Add token
        (*tokens)[nodeIndex] = nodes[nodeIndex].content;
        // Add punctuation
        for (size_t punctuationIndex = 0
            ; punctuationIndex < nodes[nodeIndex].punctuation.size()
            ; ++punctuationIndex)
        {
            (*punctuation)[nodeIndex] += nodes[nodeIndex].punctuation[punctuationIndex];
        }
    }
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
vector<wstring> EDGE_FEATURE_CALCULATOR_TEMPLATE1::getFullTags(
    const SyntaxTree& tree)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    vector<wstring> fullTags(nodes.size());
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        const vector<wstring>& parts = nodes[nodeIndex].splittedLabel;
        for (size_t featureIndex = 0; featureIndex < parts.size(); ++featureIndex)
        {
            fullTags[nodeIndex] += parts[featureIndex] + L"@";
        }
    }
    return fullTags;
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
wstring EDGE_FEATURE_CALCULATOR_TEMPLATE1::getDistanceSchema(
        int leftIndex, int rightIndex
        , const vector<wstring>& tokens
        , const vector<wstring>& fullTags
        , const vector<wstring>& punctuation)
{
    wstring schema = L"schema_";
    if (leftIndex < rightIndex)
    {
        schema += L"L_";
    } else {
        schema += L"R_";
    }
    for (int currentIndex = std::max(leftIndex, 0)
         ; currentIndex <= std::min(rightIndex, int(tokens.size()) - 1)
         ; ++currentIndex)
    {
        wstring posToAdd = (
            fullTags[currentIndex] == L"PR" ||
            fullTags[currentIndex] == L"CONJ" ||
            fullTags[currentIndex] == L"PART") ? tokens[currentIndex] : fullTags[currentIndex];
        schema += posToAdd + punctuation[currentIndex];
    }
    return schema;
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
wstring EDGE_FEATURE_CALCULATOR_TEMPLATE1::getPartialSchema(
        int leftIndex, int rightIndex
        , const vector<SyntaxNode>& syntaxNodes
        , const vector<wstring>& punctuation)
{
    wstring schema = L"part_schema_";
    if (leftIndex < rightIndex)
    {
        schema += L"L_";
    } else {
        schema += L"R_";
    }
    for (int currentIndex = std::max(leftIndex, 0)
         ; currentIndex <= std::min(rightIndex, int(syntaxNodes.size()) - 1)
         ; ++currentIndex)
    {
        wstring posToAdd = (
            syntaxNodes[currentIndex].splittedLabel[0] == L"PR" ||
            syntaxNodes[currentIndex].splittedLabel[0] == L"CONJ" ||
            syntaxNodes[currentIndex].splittedLabel[0] == L"PART") ? syntaxNodes[currentIndex].content : syntaxNodes[currentIndex].splittedLabel[0];
        schema += posToAdd + punctuation[currentIndex];
    }
    return schema;
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
bool EDGE_FEATURE_CALCULATOR_TEMPLATE1::setUnbelivableTransitions(
    int leftIndex, int rightIndex
    , const vector<SyntaxNode>& syntaxNodes
    , vector<vector<StringBatch> >* features)
{
//    const wstring& leftLabel = syntaxNodes[leftIndex].label;
    const wstring& leftPOS = syntaxNodes[leftIndex].splittedLabel[0];
//    const wstring& rightLabel = syntaxNodes[rightIndex].label;
//    const wstring& rightPOS = syntaxNodes[rightIndex].splittedLabel[0];
    // PR -> only to right side
    StringBatch& currentFeatures = (*features)[leftIndex][rightIndex];
    if (rightIndex < leftIndex && leftPOS == L"PR")
    {
        currentFeatures.Add(wstring(Tools::Alphabet::UNBELIEVABLE_ELEMENT));
        return true;
    }
//    // S@МУЖ -> А@МУЖ
//    wstring leftGender =
//        Tools::ContainsSubstring(leftLabel, L"МУЖ") ? L"МУЖ" :
//        Tools::ContainsSubstring(leftLabel, L"ЖЕН") ? L"ЖЕН" :
//        Tools::ContainsSubstring(leftLabel, L"СР") ? L"СР" : L"";
//    wstring rightGender =
//        Tools::ContainsSubstring(rightLabel, L"МУЖ") ? L"МУЖ" :
//        Tools::ContainsSubstring(rightLabel, L"ЖЕН") ? L"ЖЕН" :
//        Tools::ContainsSubstring(rightLabel, L"СР") ? L"СР" : L"";
//    if (leftPOS == L"S"
//        && (rightPOS == L"A" || Tools::ContainsSubstring(rightLabel, L"@ПРИЧ"))
//        && rightGender.size() > 0 && leftGender.size() > 0
//        && leftGender != rightGender) {
//        currentFeatures.push_back(Tools::Alphabet::UNBELIEVABLE_ELEMENT);
//        return true;
//    }
    return false;
}

EDGE_FEATURE_CALCULATOR_TEMPLATE
void EDGE_FEATURE_CALCULATOR_TEMPLATE1::CalculateFeatures(
    const SyntaxTree& tree, vector<vector<StringBatch> >* features)
{
    int size = tree.GetSize();
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    SchemaCalculatorRus schemaCalculator(&nodes);
    vector<wstring> fullTags = getFullTags(tree);
    vector<wstring> tokens;
    vector<wstring> punctuation;
    getTokensAndPunctuation(tree, &tokens, &punctuation);
    // Calculate features for nodes
    vector<vector<wstring> > nodeFeatures
        = nodeFeatureCalculator.Calculate(tree);
    // Calculate features
    features->resize(size);
    // Iterate over left node
    for (int leftIndex = 0; leftIndex < size; ++leftIndex)
    {
        (*features)[leftIndex].resize(size);
        auto firstFound = connections.find(nodes[leftIndex].label);
        if (!USE_UNPRESENTED_EDGES && firstFound == connections.end())
        {
            continue;
        }
        const wstring& leftPOS = nodes[leftIndex].splittedLabel[0];
        // Iterate over right node
        for (int rightIndex = 0; rightIndex < size; ++rightIndex)
        {
            if (!USE_UNPRESENTED_EDGES)
            {
                auto secondFound = firstFound->second.find(nodes[rightIndex].label);
                if (secondFound == firstFound->second.end())
                {
                    continue;
                }
            }
            if (leftIndex == rightIndex)
            {
                continue;
            }
            if (setUnbelivableTransitions(leftIndex, rightIndex, nodes, features)) // WORKAROUND
            {
                continue;
            }
            StringBatch& currentFeatures = (*features)[leftIndex][rightIndex];
            currentFeatures.Add(fullTags[leftIndex] + L"_" + fullTags[rightIndex]);
            currentFeatures.Add(tokens[leftIndex] + L"_" + nodes[rightIndex].splittedLabel[0]);
            currentFeatures.Add(nodes[leftIndex].splittedLabel[0] + L"_" + tokens[rightIndex]);
            currentFeatures.Add(nodes[leftIndex].lemma + L"_" + nodes[rightIndex].splittedLabel[0]);
            currentFeatures.Add(nodes[leftIndex].splittedLabel[0] + L"_" + nodes[rightIndex].lemma);

            wstring distanceAbbr = MyCeil(
                nodes[rightIndex].index - nodes[leftIndex].index);
            currentFeatures.Add(fullTags[leftIndex] + L"_" + fullTags[rightIndex] + distanceAbbr);
            currentFeatures.Add(tokens[leftIndex] + L"_" + nodes[rightIndex].splittedLabel[0]  + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].splittedLabel[0] + L"_" + tokens[rightIndex]  + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].lemma + L"_" + nodes[rightIndex].splittedLabel[0]  + distanceAbbr);
            currentFeatures.Add(nodes[leftIndex].splittedLabel[0] + L"_" + nodes[rightIndex].lemma  + distanceAbbr);

//            currentFeatures.Add(L"syntax_actants", actantsCalculator->calculate( // SYNTAX ACTANTS
//                nodes[leftIndex], nodes[rightIndex]) / 1000000.0);

            vector<wstring> schemas = schemaCalculator.CalculateSchema(leftIndex, rightIndex);
            for (wstring& feature : schemas)
            {
                currentFeatures.Add(std::move(feature));
            }

            const wstring& rightPOS = nodes[rightIndex].splittedLabel[0];
            wstring prefix = leftPOS + L"_" + rightPOS;
            // Get left part
            int leftPart = 0;
            for (int punctuationIndex = 0
                ; punctuationIndex < leftIndex
                ; ++punctuationIndex)
            {
                if (nodes[punctuationIndex].punctuation.size() > 0)
                {
                    ++leftPart;
                }
            }
            // Get right part
            int rightPart = 0;
            for (int it = 0; it < rightIndex; ++it)
            {
                if (nodes[it].punctuation.size() > 0)
                {
                    ++rightPart;
                }
            }
            // Part difference
            wstring partDifference = Tools::ToWstring(rightPart - leftPart);
            // Add features
            currentFeatures.Add(L"L" + leftPOS);
            currentFeatures.Add(L"R" + rightPOS);
            currentFeatures.Add(L"B" + prefix);
            wstring distance = Tools::ToWstring(
                nodes[rightIndex].index - nodes[leftIndex].index);
            currentFeatures.Add(prefix + L"_" + distance);
            currentFeatures.Add(prefix + L"_" + partDifference);
            currentFeatures.Add(prefix + L"_" + distance + L"_" + partDifference);
            // Add left node features
            const vector<wstring>& leftFeatures = nodeFeatures[leftIndex];
            for (size_t it = 0; it < leftFeatures.size(); ++it)
            {
                currentFeatures.Add(
                    prefix + L"L" + leftFeatures[it] +
                    ((rightPart == leftPart) ? L"O" : L"NO"));
            }
            // Add right node features
            const vector<wstring>& rightFeatures = nodeFeatures[rightIndex];
            for (size_t it = 0; it < rightFeatures.size(); ++it)
            {
                currentFeatures.Add(prefix + L"R" + rightFeatures[it] +
                    ((rightPart == leftPart) ? L"O" : L"NO"));
            }
        }
    }
}

}

#endif // EDGE_FEATURE_CALCULATOR

