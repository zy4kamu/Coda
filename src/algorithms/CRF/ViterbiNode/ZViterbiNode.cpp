#include "ZViterbiNode.h"
#include <cmath>

namespace LinearCRF
{

ZViterbiNode::ZViterbiNode()
{
}

void ZViterbiNode::Initialize(int i_layer, int i_stateIndex)
{
    AbstractViterbiNode::Initialize(i_layer, i_stateIndex);
    partitionFunction = 0;
}

void ZViterbiNode::Initialize(int i_layer, int i_stateIndex,
    double initialLogWeight)
{
    AbstractViterbiNode::Initialize(i_layer, i_stateIndex);
    partitionFunction = std::exp(initialLogWeight);
}

void ZViterbiNode::AddTransition(
    AbstractViterbiNode* fromNode,
    double logTransitionWeight)
{
    partitionFunction +=
        static_cast<const ZViterbiNode*>(fromNode)->partitionFunction
        * std::exp(logTransitionWeight);
}

double ZViterbiNode::GetWeight()
{
    return std::log(partitionFunction);
}

ZViterbiNode::~ZViterbiNode(void)
{
}

}
