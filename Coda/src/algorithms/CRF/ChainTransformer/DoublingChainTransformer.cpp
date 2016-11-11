#include "DoublingChainTransformer.h"

namespace LinearCRF
{
    DoublingChainTransformer::DoublingChainTransformer()
    {
    }

    DoublingChainTransformer::~DoublingChainTransformer()
    {
    }

    Chain DoublingChainTransformer::ForwardTransform(Chain& chain) const
    {
        int oldLength = chain.GetSize();
        // Create tokens
        const vector<wstring>& oldTokens = chain.GetTokens();
        vector<wstring> tokens(2 * oldLength);
        for (size_t nodeIndex = 0; nodeIndex < oldTokens.size(); ++nodeIndex)
        {
            tokens[2 * nodeIndex + 1] = L"copy_" + oldTokens[nodeIndex];
            tokens[2 * nodeIndex] = std::move(oldTokens[nodeIndex]);
        }
        // Create labels
        const vector<wstring>& oldLabels = chain.GetLabels();
        vector<wstring> labels(2 * oldLength);
        for (size_t nodeIndex = 0; nodeIndex < oldLabels.size(); ++nodeIndex)
        {
            labels[2 * nodeIndex] = oldLabels[nodeIndex];
            labels[2 * nodeIndex + 1] = L"copy_" + oldLabels[nodeIndex];
        }
        // Create features
        const vector<vector<wstring> >& oldFeatures = chain.GetFeatures();
        vector<vector<wstring> > features(2 * oldLength);
        for (size_t nodeIndex = 0; nodeIndex < oldLabels.size(); ++nodeIndex)
        {
            features[2 * nodeIndex] = std::move(oldFeatures[nodeIndex]);
        }
        // Return chain
        Chain transformed(
            std::move(tokens), std::move(features)
            , std::move(labels), vector<vector<wstring> >());
        return transformed;
    }

    vector<int> DoublingChainTransformer::BackwardTransform(
        const vector<int>& transformedPredictedStates) const
    {
        vector<int> predictedStates(transformedPredictedStates.size() / 2);
        for (size_t stateIndex = 0; stateIndex < predictedStates.size(); ++stateIndex)
        {
            predictedStates[stateIndex] = transformedPredictedStates[2 * stateIndex];
        }
        return predictedStates;
    }

    vector<double> DoublingChainTransformer::BackwardTransform(
        const vector<double>& transformedPredictedStates) const
    {
        vector<double> predictedStates(transformedPredictedStates.size() / 2);
        for (size_t stateIndex = 0; stateIndex < predictedStates.size(); ++stateIndex)
        {
            predictedStates[stateIndex] = transformedPredictedStates[2 * stateIndex];
        }
        return predictedStates;
    }
};
