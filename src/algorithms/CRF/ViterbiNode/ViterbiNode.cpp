#include "ViterbiNode.h"

namespace LinearCRF
{

ViterbiNode::ViterbiNode()
{
}

void ViterbiNode::Initialize(int i_layer, int i_stateIndex)
{
    AbstractViterbiNode::Initialize(i_layer, i_stateIndex);
    weight = MINUSE_INFINITY;
    bestPredecessor = 0;
}

void ViterbiNode::Initialize(int i_layer, int i_stateIndex, double initialLogWeight)
{
    AbstractViterbiNode::Initialize(i_layer, i_stateIndex);
    weight = initialLogWeight;
    bestPredecessor = 0;
}

void ViterbiNode::AddTransition(
    AbstractViterbiNode* fromNode,
    double i_weight)
{
    double pretendentWeight =
        static_cast<const ViterbiNode*>(fromNode)->weight
        + i_weight;
    if (pretendentWeight > weight)
    {
        weight = pretendentWeight;
        bestPredecessor = static_cast<ViterbiNode*>(fromNode);
    }
}

double ViterbiNode::GetWeight()
{
    return weight;
}


ViterbiNode::~ViterbiNode(void)
{
}

}
