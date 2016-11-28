#include "ChuLiuEdmondDependencySearcher.h"

#include <algorithm>

const double MINUSE_INFINITY = -1000000000;
const double SMALL_MINUSE_INFINITY = -10000;
const int DEFAULT_PARENT_INDEX = -1;
const double EPS = 1e-10;

using namespace SyntaxParser;

void ChuLiuEdmondDependencySearcher::FindDependencies(
        SyntaxTree* tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights)
{
    size_t size = rootWeights.size();
    double bestWeight = MINUSE_INFINITY;
    vector<int> bestTreeHeads;
    for (size_t rootIndex = 0; rootIndex < size; ++rootIndex)
    {
        initialize(rootWeights, edgeWeights, rootIndex);
        process();
        vector<int> treeHeads = constructTree(size);
        double weight = calculateWeight(
            treeHeads, edgeWeights, rootWeights);
        if (weight > bestWeight)
        {
            bestTreeHeads = treeHeads;
            bestWeight = weight;
        }
        setTreeDependencies(treeHeads, tree);
    }
    setTreeDependencies(bestTreeHeads, tree);
}

void ChuLiuEdmondDependencySearcher::initialize(
    const vector<double>& rootWeights
    , const vector<vector<double> >& edgeWeights
    , size_t rootIndex)
{
    size_t size = rootWeights.size();
    initializeCurrentNodes(size);
    intializeWeights(rootWeights, edgeWeights, rootIndex);
    initializeHeads();
}

void ChuLiuEdmondDependencySearcher::initializeCurrentNodes(
    size_t size)
{
    currentNodes.clear();
    for (size_t nodeIndex = 0; nodeIndex < size + 1; ++nodeIndex)
    {
        currentNodes.insert(nodeIndex);
    }
}

void ChuLiuEdmondDependencySearcher::intializeWeights(
    const vector<double>& rootWeights
    , const vector<vector<double> >& edgeWeights
    , size_t rootIndex)
{
    weights.clear();
    size_t size = rootWeights.size();
    weights.resize(size + 1, vector<double>(size + 1, MINUSE_INFINITY));
    for (size_t modifierIndex = 1; modifierIndex < size + 1; ++modifierIndex)
    {
        if (rootIndex + 1 == modifierIndex) {
            weights[modifierIndex][0] = rootWeights[modifierIndex - 1];
        } else {
            weights[modifierIndex][0] = SMALL_MINUSE_INFINITY;
        }
        for (size_t headIndex = 1; headIndex < size + 1; ++headIndex)
        {
            weights[modifierIndex][headIndex] = edgeWeights[headIndex - 1][modifierIndex - 1];
        }
        weights[modifierIndex][modifierIndex] = MINUSE_INFINITY;
    }
}

void ChuLiuEdmondDependencySearcher::initializeHeads()
{
    heads.clear();
    size_t size = currentNodes.size() - 1;
    heads.resize(size + 1, DEFAULT_PARENT_INDEX);
    for (size_t modifierIndex = 1; modifierIndex < size + 1; ++modifierIndex)
    {
        double bestWeight = MINUSE_INFINITY;
        for (size_t headIndex = 0; headIndex < size + 1; ++headIndex)
        {
            double pretendentWeight = weights[modifierIndex][headIndex];
            if (pretendentWeight > bestWeight)
            {
                bestWeight = pretendentWeight;
                heads[modifierIndex] = static_cast<int>(headIndex);
            }
        }
    }
}

bool ChuLiuEdmondDependencySearcher::findCycle(vector<int>* cycleVector)
{
    set<int> notLoopedNodes;
    set<int> cycle;
    for (auto nodeIterator = currentNodes.begin()
        ; nodeIterator != currentNodes.end()
        ; ++nodeIterator)
    {
        if (notLoopedNodes.find(*nodeIterator) != notLoopedNodes.end())
        {
            continue;
        } else {
            cycle.clear();
            cycle.insert(*nodeIterator);
            int currentNode = *nodeIterator;
            while ((currentNode = heads[currentNode]) != DEFAULT_PARENT_INDEX)
            {
                if (cycle.find(currentNode) != cycle.end())
                {
                    cycleVector->resize(1, currentNode);
                    int cycleNode = currentNode;
                    while ((cycleNode = heads[cycleNode]) != currentNode)
                    {
                        cycleVector->push_back(cycleNode);
                    }
                    return true;
                } else if (notLoopedNodes.find(currentNode) != notLoopedNodes.end()) {
                    break;
                } else {
                    cycle.insert(currentNode);
                }
            }
            notLoopedNodes.insert(cycle.begin(), cycle.end());
        }
    }
    return false;
}

void ChuLiuEdmondDependencySearcher::process()
{
    vector<int> cycle;
    if (findCycle(&cycle))
    {
        int cycleNodeIndex = static_cast<int>(weights.size());
        contract(cycle);
        updateHeads(cycle, cycleNodeIndex);
        process();
        decodeTree(cycle, cycleNodeIndex);
    } else {
        return;
    }
}

void ChuLiuEdmondDependencySearcher::contract(
    const vector<int>& cycle)
{
    contractCurrentNodes(cycle);
    contractEdgesWithModifierInCycle(cycle);
    contractEdgesWithHeadInCycle(cycle);
    currentNodes.insert(static_cast<int>(weights.size() - 1));
}

void ChuLiuEdmondDependencySearcher::contractCurrentNodes(
    const vector<int>& cycle)
{
    for (size_t cycleIndex = 0; cycleIndex < cycle.size(); ++cycleIndex)
    {
        currentNodes.erase(cycle[cycleIndex]);
    }
}

void ChuLiuEdmondDependencySearcher::contractEdgesWithModifierInCycle(
    const vector<int>& cycle)
{
    weights.push_back(vector<double>(weights.size() + 1, MINUSE_INFINITY));
    vector<double>& addedWeights = weights.back();
    for (auto currentNodeIter = currentNodes.begin()
        ; currentNodeIter != currentNodes.end()
        ; ++currentNodeIter)
    {
        for (size_t cycleIndex = 0; cycleIndex < cycle.size(); ++cycleIndex)
        {
            int cycleModifier = cycle[cycleIndex];
            int cycleHead = heads[cycleModifier];
            double cycleEdgeWeight = weights[cycleModifier][cycleHead];
            double pretendentWeight = weights[cycleModifier][*currentNodeIter] 
                    - cycleEdgeWeight;
            if (pretendentWeight > addedWeights[*currentNodeIter])
            {
                addedWeights[*currentNodeIter] = pretendentWeight;
            } 
        }
    }
}

void ChuLiuEdmondDependencySearcher::contractEdgesWithHeadInCycle(
        const vector<int>& cycle)
{
    for (size_t nodeIndex = 0; nodeIndex < weights.size(); ++nodeIndex)
    {
        weights[nodeIndex].push_back(MINUSE_INFINITY);
    }
    for (auto currentNodeIter = currentNodes.begin()
        ; currentNodeIter != currentNodes.end()
        ; ++currentNodeIter)
    {
        int currentNode = *currentNodeIter;
        double& weightToUpdate = weights[currentNode].back();
        for (size_t cycleIndex = 0; cycleIndex < cycle.size(); ++cycleIndex)
        {
            double pretendentWeight = weights[currentNode][cycle[cycleIndex]];
            if (pretendentWeight > weightToUpdate)
            {
                weightToUpdate = pretendentWeight;
            }
        }
    }
}

void ChuLiuEdmondDependencySearcher::decodeTree(
        const vector<int>& cycle, int cycleNodeIndex)
{
    decodeCurrentNodes(cycle, cycleNodeIndex);
    decodeMainNodeInCycle(cycle, cycleNodeIndex);
    decodeHeadsNotInCycle(cycle, cycleNodeIndex);
}

void ChuLiuEdmondDependencySearcher::decodeCurrentNodes(
    const vector<int>& cycle
    , int cycleNodeIndex)
{
    currentNodes.erase(cycleNodeIndex);
    currentNodes.insert(cycle.begin(), cycle.end());
}

void ChuLiuEdmondDependencySearcher::decodeMainNodeInCycle(
    const vector<int>& cycle
    , int cycleNodeIndex)
{
    int cycleHead = heads[cycleNodeIndex];
    double headWeight = weights[cycleNodeIndex][cycleHead];
    for (size_t indexInCycle = 0; indexInCycle < cycle.size(); ++indexInCycle)
    {
        int cycleModifier = cycle[indexInCycle];
        double pretendentWeight = weights[cycleModifier][cycleHead]
            - weights[cycleModifier][heads[cycleModifier]];
        if (std::abs(pretendentWeight - headWeight) < EPS)
        {
            heads[cycleModifier] = cycleHead;
            break;
        }
    }
}

void ChuLiuEdmondDependencySearcher::decodeHeadsNotInCycle(
    const vector<int>& cycle
    , int cycleNodeIndex)
{
        currentNodes.erase(cycleNodeIndex);
    for (auto currentNodeIter = currentNodes.begin()
        ; currentNodeIter != currentNodes.end()
        ; ++currentNodeIter)
    {
        int currentNode = *currentNodeIter;
        if (heads[currentNode] != cycleNodeIndex)
        {
            continue;
        }
        double weight = weights[currentNode][cycleNodeIndex];
        for (size_t indexInCycle = 0; indexInCycle < cycle.size(); ++indexInCycle)
        {
            if (std::abs(weights[currentNode][cycle[indexInCycle]] - weight) < EPS)
            {
                heads[currentNode] = cycle[indexInCycle];
                break;
            }
        }
    }
}

void ChuLiuEdmondDependencySearcher::updateHeads(
    const vector<int>& cycle, int cycleNodeIndex)
{
    // encode nodes with head in cycle
    heads.push_back(DEFAULT_PARENT_INDEX);
    for (auto currentNodeIter = currentNodes.begin()
        ; currentNodeIter != currentNodes.end()
        ; ++currentNodeIter)
    {
        int currentNode = *currentNodeIter;
        if (std::find(cycle.begin(), cycle.end(), heads[currentNode]) 
            != cycle.end())
        {
            heads[currentNode] = cycleNodeIndex;
        }
    }
    // find head for cycle
    int& headToUpdate = heads.back();
    double bestHeadWeight = MINUSE_INFINITY;
    for (auto currentNodeIter = currentNodes.begin()
        ; currentNodeIter != currentNodes.end()
        ; ++currentNodeIter)
    {
        double pretendentWeight = weights[cycleNodeIndex][*currentNodeIter];
        if (pretendentWeight > bestHeadWeight)
        {
            headToUpdate = *currentNodeIter;
            bestHeadWeight = pretendentWeight;
        }
    }
}

vector<int> ChuLiuEdmondDependencySearcher::constructTree(
    size_t numberOfNodes)
{
    vector<int> constructedTree(
        heads.begin() + 1
        , heads.begin() + numberOfNodes + 1);
    for (size_t nodeIndex = 0; nodeIndex < constructedTree.size()
        ; ++nodeIndex)
    {
        --constructedTree[nodeIndex];
    }
    return constructedTree;
}

void ChuLiuEdmondDependencySearcher::setTreeDependencies(
        const vector<int>& treeHeads
        , SyntaxTree* tree)
{
    size_t size = treeHeads.size();
    int root;
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

double ChuLiuEdmondDependencySearcher::calculateWeight(
    const vector<int>& treeHeads
    , vector<vector<double> >& edgeWeights
    , vector<double>& rootWeights)
{
    double weight = 0;
    for (size_t index = 0; index < treeHeads.size(); ++index)
    {
        if (treeHeads[index] >= 0) {
            weight += edgeWeights[treeHeads[index]][index];
        } else {
            weight += rootWeights[index];
        }
    }
    return weight;
}

void ChuLiuEdmondDependencySearcher::visualize() const
{
    wofstream out(SYNTAX_OUTPUT_TREE_PATH);
    out << L"digraph {" << std::endl;
    for (int node : this->currentNodes)
    {
        out << node << L" -> " << heads[node] << L";" << std::endl;
    }
    out << L"}" << std::endl;
    out.close();

    string drawTreeScript = SYNTAX_DRAW_TREE_SCRIPT;
    drawTreeScript += " ";
    drawTreeScript += GRAPHVIZ_PATH;
    drawTreeScript += " ";
    drawTreeScript += SYNTAX_OUTPUT_TREE_PATH;
    system(drawTreeScript.c_str());
}
