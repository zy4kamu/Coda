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

#ifndef TREE_OPTIMIZABLE_DATA
#define TREE_OPTIMIZABLE_DATA

#include <vector>
#include <memory>

#include "SyntaxTree.h"

using std::vector;
using std::shared_ptr;

namespace SyntaxParser
{

#define TREE_OPTIMIZABLE_DATA_TEMPLATE template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>

#define TREE_OPTIMIZABLE_DATA_TEMPLATE1 TreeOptimizableData< \
    EdgeFeatureCalculator \
    , EdgeWeightCalculator \
    , RootFeatureCalculator \
    , RootWeightCalculator \
    , saveToFile>

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile = false>
class TreeOptimizableData : public SyntaxTree
{
public:
    typedef EdgeFeatureCalculator EdgeFeatureCalculatorType;
    typedef EdgeWeightCalculator EdgeWeightCalculatorType;
    typedef RootFeatureCalculator RootFeatureCalculatorType;
    typedef RootWeightCalculator RootWeightCalculatorType;
    typedef TreeOptimizableData<
        EdgeFeatureCalculator, EdgeWeightCalculator
        , RootFeatureCalculator, RootWeightCalculator, saveToFile> TreeOptimizableDataType;
    typedef typename RootFeatureCalculatorType::IntBatch IntBatch;
    typedef typename RootFeatureCalculatorType::StringBatch StringBatch;

    TreeOptimizableData(
        size_t indexInDataSet
        , const wstring& line
        , shared_ptr<Tools::Alphabet> alphabet
        , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator);

    TreeOptimizableData(TreeOptimizableData&& tree);

    void SetAlphabet(shared_ptr<Tools::Alphabet> newAlphabet);

    template<class TreeOptimizableDataType>
    friend std::ostream& operator << (std::ostream& stream
        , const TreeOptimizableDataType& tree);

    template<class TreeOptimizableDataType>
    friend std::istream& operator >> (std::istream& stream
        , TreeOptimizableDataType& tree);

    void Save();

    void Upload();

    void Clear();

    size_t GetIndexInDataSet() const;

protected:
    size_t indexInDataSet;
    vector<vector<IntBatch> > edgeFeatureIndexes; /**< contains integer representation for features in the edges*/
    vector<IntBatch> rootFeatureIndexes; /**< contains integer representation for features in the root pretendents*/
    shared_ptr<Tools::Alphabet> alphabet; /**< converts string features to their indexes*/

    shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator;
    EdgeWeightCalculator edgeWeightCalculator;
    RootFeatureCalculator rootFeatureCalculator;
    RootWeightCalculator rootWeightCalculator;
private:
    void addEdgeFeaturesToAlphabet(
        const vector<vector<StringBatch> >& edgeFeatures);

    void addRootFeaturesToAlphabet(
        const vector<StringBatch>& rootFeatures);

    void recalculateEdgeFeatureIndexes(
        const vector<vector<StringBatch> >& edgeFeatures);

    void recalculateRootFeatureIndexes(
        const vector<StringBatch>& rootFeatures);

    void recoverEdgeFeaturesByIndexes(
        vector<vector<StringBatch> >* edgeFeatures);

    vector<StringBatch> recoverRootFeaturesByIndexes();
};

TREE_OPTIMIZABLE_DATA_TEMPLATE
TREE_OPTIMIZABLE_DATA_TEMPLATE1::TreeOptimizableData(
    size_t indexInDataSet
    , const wstring& line
    , shared_ptr<Tools::Alphabet> alphabet
    , shared_ptr<EdgeFeatureCalculator> edgeFeatureCalculator)
    : SyntaxTree(line)
    , indexInDataSet(indexInDataSet)
    , alphabet(alphabet)
    , edgeFeatureCalculator(edgeFeatureCalculator)
{
    vector<vector<StringBatch> > edgeFeatures;
    vector<StringBatch> rootFeatures;
    // Calculate features in string representation
    edgeFeatureCalculator->CalculateFeatures(
        *this, &edgeFeatures);
    rootFeatures = rootFeatureCalculator.Calculate(
        *this);
    // Update alphabet
    addEdgeFeaturesToAlphabet(edgeFeatures);
    addRootFeaturesToAlphabet(rootFeatures);
    // Calculate features in int representation
    recalculateEdgeFeatureIndexes(edgeFeatures);
    recalculateRootFeatureIndexes(rootFeatures);
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
TREE_OPTIMIZABLE_DATA_TEMPLATE1::TreeOptimizableData(
    TreeOptimizableData&& tree)
    : SyntaxTree(std::move(tree))
    , indexInDataSet(tree.indexInDataSet)
    , edgeFeatureIndexes(std::move(tree.edgeFeatureIndexes))
    , rootFeatureIndexes(std::move(tree.rootFeatureIndexes))
    , alphabet(tree.alphabet)
    , edgeFeatureCalculator(tree.edgeFeatureCalculator)
{
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::addEdgeFeaturesToAlphabet(
    const vector<vector<StringBatch> >& edgeFeatures)
{
    for (size_t it = 0; it < edgeFeatures.size(); ++it)
    {
        for (size_t jt = 0; jt < edgeFeatures[it].size(); ++jt)
        {
            for (size_t kt = 0; kt < edgeFeatures[it][jt].GetSize(); ++kt)
            {
                alphabet->LookUpIndex_Adding(edgeFeatures[it][jt].GetFeature(kt));
            }
        }
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::addRootFeaturesToAlphabet(
    const vector<StringBatch>& rootFeatures)
{
    for (size_t rootIndex = 0; rootIndex < rootFeatures.size()
        ; ++rootIndex)
    {
        for (size_t featureIndex = 0
            ; featureIndex < rootFeatures[rootIndex].GetSize()
            ; ++featureIndex)
        {
            alphabet->LookUpIndex_Adding(
                    rootFeatures[rootIndex].GetFeature(featureIndex));
        }
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::recalculateEdgeFeatureIndexes(
    const vector<vector<StringBatch> >& edgeFeatures)
{
    edgeFeatureIndexes.resize(edgeFeatures.size());
    for (size_t it = 0; it < edgeFeatureIndexes.size(); ++it)
    {
        edgeFeatureIndexes[it].resize(edgeFeatures[it].size());
        for (size_t jt = 0; jt < edgeFeatures[it].size(); ++jt)
        {
            edgeFeatureIndexes[it][jt] = Translate(edgeFeatures[it][jt],
                alphabet, false);
        }
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void  TREE_OPTIMIZABLE_DATA_TEMPLATE1::recalculateRootFeatureIndexes(
    const vector<StringBatch>& rootFeatures)
{
    rootFeatureIndexes.resize(rootFeatures.size());
    for (size_t rootIndex = 0; rootIndex < rootFeatures.size(); ++rootIndex)
    {
        rootFeatureIndexes[rootIndex] = Translate(
            rootFeatures[rootIndex], alphabet, false);
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::SetAlphabet(
    shared_ptr<Tools::Alphabet> newAlphabet)
{
    vector<vector<StringBatch> > edgeFeatures;
    recoverEdgeFeaturesByIndexes(&edgeFeatures);
    vector<StringBatch> rootFeatures =
        recoverRootFeaturesByIndexes();
    alphabet = newAlphabet;
    recalculateEdgeFeatureIndexes(edgeFeatures);
    recalculateRootFeatureIndexes(rootFeatures);
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::recoverEdgeFeaturesByIndexes(
    vector<vector<StringBatch> >* edgeFeatures)
{
    edgeFeatures->resize(edgeFeatureIndexes.size());
    for (size_t leftNodeIndex = 0
        ; leftNodeIndex < edgeFeatures->size()
        ; ++leftNodeIndex)
    {
        (*edgeFeatures)[leftNodeIndex].resize(
            edgeFeatureIndexes[leftNodeIndex].size());
        for (size_t rightNodeIndex = 0
            ; rightNodeIndex < edgeFeatureIndexes[leftNodeIndex].size()
            ; ++rightNodeIndex)
        {
            (*edgeFeatures)[leftNodeIndex][rightNodeIndex] = Translate(
                edgeFeatureIndexes[leftNodeIndex][rightNodeIndex], alphabet);
        }
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
vector<typename TREE_OPTIMIZABLE_DATA_TEMPLATE1::StringBatch>
TREE_OPTIMIZABLE_DATA_TEMPLATE1::recoverRootFeaturesByIndexes()
{
    vector<StringBatch> rootFeatures;
    rootFeatures.resize(rootFeatureIndexes.size());
    for (size_t rootIndex = 0; rootIndex < rootFeatures.size(); ++rootIndex)
    {
        rootFeatures[rootIndex] = Translate(
            rootFeatureIndexes[rootIndex], alphabet);
    }
    return rootFeatures;
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::Clear()
{
    if (saveToFile)
    {
        edgeFeatureIndexes.clear();
        rootFeatureIndexes.clear();
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
size_t TREE_OPTIMIZABLE_DATA_TEMPLATE1::GetIndexInDataSet() const
{
    return indexInDataSet;
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::Save()
{
    if (saveToFile)
    {
        std::ofstream out("/data/del/" + Tools::ConvertIntToString(this->indexInDataSet));
        out << *this;
        out.close();
    }
}

TREE_OPTIMIZABLE_DATA_TEMPLATE
void TREE_OPTIMIZABLE_DATA_TEMPLATE1::Upload()
{
    if (saveToFile)
    {
        std::ifstream in("/data/del/" + Tools::ConvertIntToString(this->indexInDataSet));
        in >> *this;
        in.close();
    }
}

template<class TreeOptimizableDataType>
std::ostream& operator << (std::ostream& stream
    , const TreeOptimizableDataType& tree)
{
    size_t size = tree.edgeFeatureIndexes.size();
    stream << size << std::endl;
    for (size_t leftIndex = 0; leftIndex < size; ++leftIndex)
    {
        for (size_t rightIndex = 0; rightIndex < size; ++rightIndex)
        {
            stream << tree.edgeFeatureIndexes[leftIndex][rightIndex] << std::endl;
        }
    }
    for (size_t index = 0; index < size; ++index)
    {
        stream << tree.rootFeatureIndexes[index] << std::endl;
    }
    return stream;
}

template<class TreeOptimizableDataType>
std::istream& operator >> (std::istream& stream
    , TreeOptimizableDataType& tree)
{
    size_t size;
    stream >> size;
    tree.edgeFeatureIndexes.resize(size);
    for (size_t leftIndex = 0; leftIndex < size; ++leftIndex)
    {
        tree.edgeFeatureIndexes[leftIndex].resize(size);
        for (size_t rightIndex = 0; rightIndex < size; ++rightIndex)
        {
            stream >> tree.edgeFeatureIndexes[leftIndex][rightIndex];
        }
    }
    tree.rootFeatureIndexes.resize(size);
    for (size_t index = 0; index < size; ++index)
    {
        stream >> tree.rootFeatureIndexes[index];
    }
    return stream;
}

}

#endif // TREE_OPTIMIZABLE_DATA
