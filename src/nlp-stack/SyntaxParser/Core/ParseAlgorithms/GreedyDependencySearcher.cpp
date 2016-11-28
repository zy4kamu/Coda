#include "GreedyDependencySearcher.h"

using namespace SyntaxParser;

bool IsVerbPresent(const SyntaxTree& tree)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    for (size_t it = 0; it < nodes.size(); ++it)
    {
        if (nodes[it].splittedLabel.size() > 0 
            && nodes[it].splittedLabel[0] == L"V")
        {
            return true;
        }
    }
    return false;
}

void GreedyDependencySearcher::FindDependencies(
        SyntaxTree* tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights)
{
    int size = tree->GetSize();
    int bestRoot = -1;
    double bestWeight = -1e+5;
    vector<vector<int> > dependencies;
    // bool isVerbPresent = IsVerbPresent(*tree);

    // Iterate over roots
    for (int rootIndex = 0; rootIndex < size; ++rootIndex)
    {
        // Find best tree with this node
        double weight = findBestConnections(
            *tree, edgeWeights
            , rootWeights, rootIndex
            , &dependencies);
        // Update best root pretendent
        if (weight > bestWeight)
        {
            bestWeight = weight;
            bestRoot = rootIndex;
        }
    }
    findBestConnections(
        *tree, edgeWeights
        , rootWeights, bestRoot
        , &dependencies);
    // Set connections to the tree
    tree->SetEdges(dependencies);
    tree->SetRoot(bestRoot);
}

double GreedyDependencySearcher::findBestConnections(
        const SyntaxTree& tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights
        ,int rootIndex
        , vector<vector<int> >* dependencies)
{
    int size = tree.GetSize();
    double sumWeight = rootWeights[rootIndex];
   
    dependencies->clear();
    dependencies->resize(size);
    
    set<int> addedNodes;
    addedNodes.insert(rootIndex);
    
    set<int> notAddedNodes;
    for (int it = 0; it < size; ++it)
    {
        if (it != rootIndex)
        {
            notAddedNodes.insert(it);
        }
    }

    for (int it = 0; it < size - 1; ++it)
    {
        int bestLeft = -1;
        int bestRight = -1;
        double bestEdge = -1e+5;
        for (auto leftIter = addedNodes.begin(); leftIter != addedNodes.end(); ++leftIter)
        {
            for (auto rightIter = notAddedNodes.begin(); rightIter != notAddedNodes.end(); ++rightIter)
            {
                double edge = edgeWeights[*leftIter][*rightIter];
                if (edge > bestEdge)
                {
                    bestEdge = edge;
                    bestLeft = *leftIter;
                    bestRight = *rightIter;
                }
            }
        }
        (*dependencies)[bestLeft].push_back(bestRight);
        addedNodes.insert(bestRight);
        notAddedNodes.erase(bestRight);
        sumWeight += bestEdge;
    }

    postProcess(
        tree, edgeWeights
        , rootWeights, rootIndex
        , dependencies, &sumWeight);
    return sumWeight;
}

void getSubTree(int index
    , const vector<vector<int> >& dependencies
    , set<int>* subTree)
{
    for (size_t it = 0; it < dependencies[index].size(); ++it)
    {
        int toAddIndex = dependencies[index][it];
        subTree->insert(toAddIndex);
        getSubTree(toAddIndex, dependencies, subTree);
    }
}

int getParentIdex(int index
    , const vector<vector<int> >& dependencies)
{
    for (size_t parentIndex = 0; parentIndex < dependencies.size(); ++parentIndex)
    {
        for (size_t sonIndex = 0; sonIndex < dependencies[parentIndex].size(); ++sonIndex)
        {
            if (dependencies[parentIndex][sonIndex] == index)
            {
                return parentIndex;
            }
        }
    }
    throw ("Couldn't take parent index");
}

void GreedyDependencySearcher::postProcess(
    const SyntaxTree& tree
    , vector<vector<double> >& edgeWeights
    , vector<double>& rootWeights
    , int rootIndex
    , vector<vector<int> >* dependencies
    , double* sumWeight)
{
    int size = tree.GetSize();
    bool canBeRefined = true;
    
    while (canBeRefined)
    {
        canBeRefined = false;
        for (int index = 0; index < size; ++index)
        {
            if (index == rootIndex)
            {
                continue;
            }
            // Get parent edge weight
            int parentIndex = getParentIdex(index, *dependencies);
            if (parentIndex == -1) continue;
            double parentEdgeWeight = edgeWeights[parentIndex][index];
            // Find pretendent
            set<int> subTree;
            getSubTree(index, *dependencies, &subTree);
            int bestPretendentIndex = -1;
            double bestPretendentWeight = -1e+10;
            for (int pretendentIndex = 0; pretendentIndex < size; ++pretendentIndex)
            {
                if (subTree.find(pretendentIndex) != subTree.end() ||
                    pretendentIndex == index)
                {
                    continue;
                }
                double pretendentWeight = edgeWeights[pretendentIndex][index];
                if (pretendentWeight > bestPretendentWeight)
                {
                    bestPretendentWeight = pretendentWeight;
                    bestPretendentIndex = pretendentIndex;
                }
            }
            // Change parent
            if (bestPretendentWeight > parentEdgeWeight)
            {
                canBeRefined = true;
                // Drop Parent -> index;
                for (auto iter = (*dependencies)[parentIndex].begin()
                    ; iter != (*dependencies)[parentIndex].end()
                    ; ++iter)
                {
                    if (*iter == index)
                    {
                        (*dependencies)[parentIndex].erase(iter);
                        break;
                    }
                }
                // Set Pretendent -> Index;
                (*dependencies)[bestPretendentIndex].push_back(index);
                // Change sumWeight;
                *sumWeight -= parentEdgeWeight;
                *sumWeight += bestPretendentWeight;
                break;
            }
        }
    }
}
