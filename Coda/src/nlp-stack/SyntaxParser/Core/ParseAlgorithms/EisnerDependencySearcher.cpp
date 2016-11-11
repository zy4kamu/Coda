#include "EisnerDependencySearcher.h"
#include "FileWriter.h"

using namespace SyntaxParser;

const double MINUSE_INFINITY = -1000000000;
const int DEFAULT_SPLITTING_INDEX = -1;

void EisnerDependencySearcher::FindDependencies(
        SyntaxTree* tree
        , vector<vector<double> >& _edgeWeights
        , vector<double>& _rootWeights)
{
    std::lock_guard<std::recursive_mutex> locker(m_lock);
    rootWeights = _rootWeights;
    edgeWeights = _edgeWeights;
    clear();
    initializeSpans(rootWeights.size());
    process();
    size_t rootIndex = findRoot();
    vector<int> heads = decodeTree(rootIndex);
    setTreeDependencies(heads, tree);
}

void EisnerDependencySearcher::clear()
{
    fullSpan.clear();
    partialSpan.clear();
    fullSplitting.clear();
    partialSplitting.clear();

}

void EisnerDependencySearcher::initializeSpans(
    size_t size)
{
    fullSpan.resize(size, vector<double>(
        size, 0));
    partialSpan.resize(size, vector<double>(
        size, 0));
    fullSplitting.resize(size, vector<int>(
        size, DEFAULT_SPLITTING_INDEX));
    partialSplitting.resize(size, vector<int>(
        size, DEFAULT_SPLITTING_INDEX));
}

void EisnerDependencySearcher::process()
{
    size_t size = rootWeights.size();
    for (size_t length = 1; length < size; ++length)
    {
        for (size_t leftNode = 0; leftNode < size; ++leftNode)
        {
            size_t rightNode = leftNode + length;
            if (rightNode >= size)
            {
                continue;
            }
            evaluateLeftPartialSpan(leftNode, rightNode);
            evaluateRightPartialSpan(leftNode, rightNode);
            evaluateLeftFullSpan(leftNode, rightNode);
            evaluateRightFullSpan(leftNode, rightNode);
        }
    }
}

void EisnerDependencySearcher::evaluateLeftFullSpan(
        size_t leftNode, size_t rightNode)
{
    fullSpan[leftNode][rightNode] = MINUSE_INFINITY;
    fullSplitting[leftNode][rightNode] = DEFAULT_SPLITTING_INDEX;
    for (size_t splitIndex = leftNode + 1; splitIndex <= rightNode
        ; ++splitIndex)
    {
        double pretendentWeight = partialSpan[leftNode][splitIndex]
            + fullSpan[splitIndex][rightNode];
        if (pretendentWeight > fullSpan[leftNode][rightNode])
        {
            fullSpan[leftNode][rightNode] = pretendentWeight;
            fullSplitting[leftNode][rightNode] = splitIndex;
        }
    }
}

void EisnerDependencySearcher::evaluateRightFullSpan(
    size_t leftNode, size_t rightNode)
{
    fullSpan[rightNode][leftNode] = MINUSE_INFINITY;
    fullSplitting[rightNode][leftNode] = DEFAULT_SPLITTING_INDEX;
    for (size_t splitIndex = leftNode; splitIndex < rightNode
        ; ++splitIndex)
    {
        double pretendentWeight = partialSpan[rightNode][splitIndex]
            + fullSpan[splitIndex][leftNode];
        if (pretendentWeight > fullSpan[rightNode][leftNode])
        {
            fullSpan[rightNode][leftNode] = pretendentWeight;
            fullSplitting[rightNode][leftNode] = splitIndex;
        }
    }
}

void EisnerDependencySearcher::evaluateLeftPartialSpan(
    size_t leftNode, size_t rightNode)
{
    partialSpan[leftNode][rightNode] = MINUSE_INFINITY;
    partialSplitting[leftNode][rightNode] = DEFAULT_SPLITTING_INDEX;
    for (size_t splitIndex = leftNode; splitIndex < rightNode
        ; ++splitIndex)
    {
        double pretendentWeight = 
            fullSpan[leftNode][splitIndex]
            + fullSpan[rightNode][splitIndex + 1]
            + edgeWeights[leftNode][rightNode];
        if (pretendentWeight > partialSpan[leftNode][rightNode])
        {
            partialSpan[leftNode][rightNode] = pretendentWeight;
            partialSplitting[leftNode][rightNode] = splitIndex;
        }
    }
}

void EisnerDependencySearcher::evaluateRightPartialSpan(
    size_t leftNode, size_t rightNode)
{
    partialSpan[rightNode][leftNode] = MINUSE_INFINITY;
    partialSplitting[rightNode][leftNode] = DEFAULT_SPLITTING_INDEX;
    for (size_t splitIndex = leftNode; splitIndex < rightNode
        ; ++splitIndex)
    {
        double pretendentWeight = 
            fullSpan[leftNode][splitIndex]
            + fullSpan[rightNode][splitIndex + 1]
            + edgeWeights[rightNode][leftNode];
        if (pretendentWeight > partialSpan[rightNode][leftNode])
        {
            partialSpan[rightNode][leftNode] = pretendentWeight;
            partialSplitting[rightNode][leftNode] = splitIndex;
        }
    }
}

size_t EisnerDependencySearcher::findRoot()
{
    double maxWeight = MINUSE_INFINITY;
    size_t bestRoot = -1;
    for (size_t nodeIndex = 0; nodeIndex < rootWeights.size(); ++nodeIndex)
    {
        double pretendentWeight =
            rootWeights[nodeIndex]
            + fullSpan[nodeIndex][0]
            + fullSpan[nodeIndex][rootWeights.size() - 1];
        if (pretendentWeight > maxWeight)
        {
            maxWeight = pretendentWeight;
            bestRoot = nodeIndex;
        }
    }
    return bestRoot;
}

vector<int> EisnerDependencySearcher::decodeTree(size_t rootIndex)
{
    vector<int> leftHeads(rootWeights.size(), -1);
    decodeTree(0, rootIndex, true, false, &leftHeads);

    vector<int> rightHeads(rootWeights.size(), -1);
    decodeTree(rootIndex, rootWeights.size() - 1, true, true, &rightHeads);

    vector<int> heads(leftHeads.begin(), leftHeads.begin() + rootIndex + 1);
    heads.insert(heads.end(), rightHeads.begin() + rootIndex + 1, rightHeads.end());

    return heads;
}

void EisnerDependencySearcher::decodeTree(
    size_t leftIndex, size_t rightIndex
    , bool fullOrPartial, bool leftOrRight
    , vector<int>* heads)
{
    if (leftIndex == rightIndex)
    {
        return;
    }
    if (fullOrPartial)
    {
        if (leftOrRight)
        {
            int splitIndex = fullSplitting[leftIndex][rightIndex];
            decodeTree(leftIndex, splitIndex, false, true, heads);
            decodeTree(splitIndex, rightIndex, true, true, heads);
        } else {
            int splitIndex = fullSplitting[rightIndex][leftIndex];
            decodeTree(splitIndex, rightIndex, false, false, heads);
            decodeTree(leftIndex, splitIndex, true, false, heads);
        }
    } else {
        if (leftOrRight)
        {
            (*heads)[rightIndex] = leftIndex;
            int splitIndex = partialSplitting[leftIndex][rightIndex];
            decodeTree(leftIndex, splitIndex, true, true, heads);
            decodeTree(splitIndex + 1, rightIndex, true, false, heads);
        } else {
            (*heads)[leftIndex] = rightIndex;
            int splitIndex = partialSplitting[rightIndex][leftIndex];
            decodeTree(leftIndex, splitIndex, true, true, heads);
            decodeTree(splitIndex + 1, rightIndex, true, false, heads);
        }
    }
}

void EisnerDependencySearcher::setTreeDependencies(
        const vector<int>& treeHeads
        , SyntaxTree* tree)
{
    size_t size = treeHeads.size();
    int root = -1;
    vector<vector<int> > dependencies(size);
    for (size_t modifier = 0; modifier < size; ++modifier)
    {
        int head = treeHeads[modifier];
        if (head < 0)
        {
            root = modifier;
        } else {
            dependencies[head].push_back(modifier);
        }
    }
    tree->SetRoot(root);
    tree->SetEdges(dependencies);
}
