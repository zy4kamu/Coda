#include "FullMorphologyChainTransformer.h"
#include "Tools.h"

namespace Disambiguation
{

FullMorphologyChainTransformer::FullMorphologyChainTransformer(void)
{
}


FullMorphologyChainTransformer::~FullMorphologyChainTransformer(void)
{
}

Chain FullMorphologyChainTransformer::ForwardTransform(Chain& chain) const
{
    int oldLength = chain.GetSize();
    if (oldLength == 0)
    {
        return chain;
    }
    // Create tokens
    const vector<wstring>& oldTokens = chain.GetTokens();
    vector<wstring> tokens(2 * oldLength - 1);
    for (size_t nodeIndex = 0; nodeIndex + 1 < oldTokens.size(); ++nodeIndex)
    {
        tokens[2 * nodeIndex + 1] = L"copy_" + oldTokens[nodeIndex];
        tokens[2 * nodeIndex] = std::move(oldTokens[nodeIndex]);
    }
    // Create labels
    const vector<wstring>& oldLabels = chain.GetLabels();
    vector<wstring> labels(2 * oldLength - 1);
    for (size_t nodeIndex = 0; nodeIndex + 1 < oldLabels.size(); ++nodeIndex)
    {
        const wstring& fullLabel = oldLabels[nodeIndex];
        if (fullLabel.size() > 0)
        {
            vector<wstring> splitted;
            splitted = Tools::Split(fullLabel, L"@");
            labels[2 * nodeIndex] = fullLabel;
            labels[2 * nodeIndex + 1] = L"POS_" + splitted[0];
        }
    }
    labels[labels.size() - 1] = oldLabels.back();
    // Create features
    const vector<vector<wstring> >& oldFeatures = chain.GetFeatures();
    vector<vector<wstring> > features(2 * oldLength - 1);
    for (size_t nodeIndex = 0; nodeIndex + 1 < oldLabels.size(); ++nodeIndex)
    {
        features[2 * nodeIndex] = oldFeatures[nodeIndex];
        features[2 * nodeIndex + 1] = std::move(oldFeatures[nodeIndex]);
    }
    features[features.size() - 1] = oldFeatures.back();
    // Return chain
    Chain transformed(
        std::move(tokens)
        , std::move(features)
        , std::move(labels)
        , vector<vector<wstring> >());
    return transformed;
}

vector<int> FullMorphologyChainTransformer::BackwardTransform(
        const vector<int>& transformedPredictedStates) const
{
    vector<int> predictedStates((transformedPredictedStates.size() + 1) / 2);
    for (size_t stateIndex = 0; stateIndex < predictedStates.size(); ++stateIndex)
    {
        predictedStates[stateIndex] = transformedPredictedStates[2 * stateIndex];
    }
    return predictedStates;
}

vector<double> FullMorphologyChainTransformer::BackwardTransform(
        const vector<double>& transformedPredictedStates) const
{
    vector<double> predictedStates((transformedPredictedStates.size() + 1) / 2);
    for (size_t stateIndex = 0; stateIndex < predictedStates.size(); ++stateIndex)
    {
        predictedStates[stateIndex] = transformedPredictedStates[2 * stateIndex];
    }
    return predictedStates;
}

}
