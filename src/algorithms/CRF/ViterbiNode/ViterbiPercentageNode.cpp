#include "ViterbiPercentageNode.h"
#include <cmath>

namespace LinearCRF
{

double ViterbiPercentageNode::VITERBI_PERCENTAGE_VALUE = 0.99;
size_t ViterbiPercentageNode::MAX_NUMBER_OF_PATHS = 30;

ViterbiPercentagePathItem::ViterbiPercentagePathItem()
{
}

ViterbiPercentagePathItem::ViterbiPercentagePathItem(
    double bestWeight,
    ViterbiPercentageNode* currentNode,
    ViterbiPercentagePathPointer previousItem)
    : bestWeight(bestWeight)
    , expWeight(std::exp(bestWeight))
    , currentNode(currentNode)
    , previousItem(previousItem)
{
}

bool operator < (
    const ViterbiPercentagePathItem& first,
    const ViterbiPercentagePathItem& second)
{
    return first.bestWeight < second.bestWeight;
}

bool operator > (
    const ViterbiPercentagePathItem& first,
    const ViterbiPercentagePathItem& second)
{
    return first.bestWeight > second.bestWeight;
}

ViterbiPercentageNode::ViterbiPercentageNode()
{
}

void ViterbiPercentageNode::Initialize(int i_layer, int i_stateIndex)
{
    ZViterbiNode::Initialize(i_layer, i_stateIndex);
    currentSum = 0;
}

void ViterbiPercentageNode::Initialize(int i_layer, int i_stateIndex, double initialLogWeight)
{
    ZViterbiNode::Initialize(i_layer, i_stateIndex, initialLogWeight);
    ViterbiPercentagePathItem toInsert(initialLogWeight, this, 0);
    pathItems.insert(toInsert);
    currentSum = this->partitionFunction;
}

void ViterbiPercentageNode::AddTransition(
    AbstractViterbiNode* fromNode,
    double weight)
{
    ZViterbiNode::AddTransition(fromNode, weight);
    for (const ViterbiPercentagePathItem& pretendent : static_cast<ViterbiPercentageNode*>(
        fromNode)->pathItems)
    {
        ViterbiPercentagePathItem toInsert(
            pretendent.bestWeight + weight, this, &pretendent);
        pathItems.insert(toInsert);
        currentSum += toInsert.expWeight;
    }
    // Cut
    bool shouldCut = pathItems.size() > 0;
    double cutThreshold = VITERBI_PERCENTAGE_VALUE * this->partitionFunction;
    while (shouldCut)
    {
        auto minElement = pathItems.begin();
        double substractedSum = currentSum - minElement->expWeight;
        if (substractedSum > cutThreshold
            || pathItems.size() > MAX_NUMBER_OF_PATHS)
        {
            currentSum = substractedSum;
            pathItems.erase(minElement);
        } else {
            shouldCut = false;
        }
    }
}

ViterbiPercentageNode::~ViterbiPercentageNode(void)
{
}

double ViterbiPercentageNode::GetWeight()
{
    if (pathItems.size() == 0)
    {
        return MINUSE_INFINITY;
    }
    set<ViterbiPercentagePathItem>::iterator last = pathItems.end();
    --last;
     return last->bestWeight;
}

}

