#include "ViterbiNBestNode.h"

namespace LinearCRF
{

size_t ViterbiNBestNode::MAX_NUMBER_IN_ONE_NODE = 1;

ViterbiNBestNode::ViterbiNBestNode()
{
}

void ViterbiNBestNode::Initialize(int i_layer, int i_stateIndex)
{
    ZViterbiNode::Initialize(i_layer, i_stateIndex);
}

void ViterbiNBestNode::Initialize(int i_layer, int i_stateIndex, double initialLogWeight)
{
    ZViterbiNode::Initialize(i_layer, i_stateIndex, initialLogWeight);
    ViterbiPathItem toInsert(initialLogWeight, this, 0);
    pathItems.insert(toInsert);
}

void ViterbiNBestNode::AddTransition(
    AbstractViterbiNode* fromNode,
    double weight)
{
    ZViterbiNode::AddTransition(fromNode, weight);
    for (const ViterbiPathItem& pretendent : static_cast<ViterbiNBestNode*>(
        fromNode)->pathItems)
    {
        ViterbiPathItem toInsert(
            pretendent.bestWeight + weight, this, &pretendent);
        tryInsert(toInsert);
    }
}

ViterbiNBestNode::~ViterbiNBestNode(void)
{
}

void ViterbiNBestNode::tryInsert(
    const ViterbiPathItem& pretendent)
{
    set<ViterbiPathItem>::const_iterator minElement = pathItems.begin();
    if (pathItems.size() <  MAX_NUMBER_IN_ONE_NODE)
    {
        pathItems.insert(pretendent);
    } else if (*minElement < pretendent) {
        pathItems.erase(minElement);
        pathItems.insert(pretendent);
    }
}

double ViterbiNBestNode::GetWeight()
{
    if (pathItems.size() == 0)
    {
        return MINUSE_INFINITY;
    }
    set<ViterbiPathItem>::iterator last = pathItems.end();
    --last;
     return last->bestWeight;
}

}
