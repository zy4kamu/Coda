#include "PredictedMorphologyTreeRecoverer.h"

namespace SyntaxParser
{

PredictedMorphologyTreeRecoverer::PredictedMorphologyTreeRecoverer(
    Tools::Language language)
{
    disambiguator = Disambiguation::IDisambiguator::GetDisambiguator(language);
}

vector<DisambiguatedData> PredictedMorphologyTreeRecoverer::GetMorphology(
    const SyntaxTree& realTree)
{
    size_t size = static_cast<size_t>(realTree.GetSize());
    vector<Token> tokens(size);
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        tokens[nodeIndex] = static_cast<Token>(realTree.GetNodes()[nodeIndex]);
    }
    return disambiguator->Disambiguate(tokens);
}

}

