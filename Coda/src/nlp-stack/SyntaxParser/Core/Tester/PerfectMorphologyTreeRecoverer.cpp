#include "PerfectMorphologyTreeRecoverer.h"

namespace SyntaxParser
{

PerfectMorphologyTreeRecoverer::PerfectMorphologyTreeRecoverer(
    Tools::Language language)
{
}

vector<DisambiguatedData> PerfectMorphologyTreeRecoverer::GetMorphology(
    const SyntaxTree& realTree)
{
    size_t size = static_cast<size_t>(realTree.GetSize());
    vector<DisambiguatedData> data(size);
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        data[nodeIndex] = static_cast<DisambiguatedData>(realTree.GetNodes()[nodeIndex]);
    }
    return data;
}

}
