#include "AbstractViterbiNode.h"

namespace LinearCRF
{

AbstractViterbiNode::AbstractViterbiNode()
{
}

void AbstractViterbiNode::Initialize(
    int i_layer, int i_stateIndex)
{
    layer = i_layer;
    stateIndex = i_stateIndex;
}

}
