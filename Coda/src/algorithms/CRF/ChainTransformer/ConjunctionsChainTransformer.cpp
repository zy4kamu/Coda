#include "ConjunctionsChainTransformer.h"

using namespace LinearCRF;

ConjunctionsChainTransformer::ConjunctionsChainTransformer(
    const vector<int>& conjunctions) :
    conjunctions(conjunctions)
{
}


ConjunctionsChainTransformer::~ConjunctionsChainTransformer(void)
{
}

Chain ConjunctionsChainTransformer::ForwardTransform(Chain& chain) const
{
    // Save old features (now we will change them)
    vector<vector<wstring> > oldFeatures = chain.GetFeatures();
    // Iterate over nodes
    for (size_t nodeIndex = 0; nodeIndex < chain.GetSize(); ++nodeIndex)
    {
     // Iterate over conjunctions
     for (size_t conjIndex = 0; conjIndex < conjunctions.size(); ++conjIndex)
     {
         // Taking neighbour
         int neighbourIndex = nodeIndex + conjunctions[conjIndex];
         if (neighbourIndex >= 0 && size_t(neighbourIndex) < chain.GetSize())
         {
             // Adding neighbour features
             const vector<wstring>& neighbourFeatures = oldFeatures[neighbourIndex];
             for (size_t index = 0; index < neighbourFeatures.size(); ++index)
             {
                 wstring featureToAdd = neighbourFeatures[index] + L"#" +
                     std::to_wstring(static_cast<long long>(conjunctions[conjIndex]));
                 chain.AddFeature(nodeIndex, featureToAdd);
             }
         }
     }
    }
    Chain copy = std::move(chain);
    return copy;
}

vector<vector<wstring> > ConjunctionsChainTransformer::ForwardTransform(
    vector<vector<wstring> >&& oldFeatures) const
{
    vector<vector<wstring> > features = oldFeatures;
    for (size_t nodeIndex = 0; nodeIndex < oldFeatures.size(); ++nodeIndex)
    {
        // Iterate over conjunctions
        for (size_t conjIndex = 0; conjIndex < conjunctions.size(); ++conjIndex)
        {
            // Taking neighbour
            int neighbourIndex = nodeIndex + conjunctions[conjIndex];
            if (neighbourIndex >= 0 && size_t(neighbourIndex) < oldFeatures.size())
            {
                // Adding neighbour features
                const vector<wstring>& neighbourFeatures = oldFeatures[neighbourIndex];
                for (size_t index = 0; index < neighbourFeatures.size(); ++index)
                {
                    wstring featureToAdd = neighbourFeatures[index] + L"#" +
                        std::to_wstring(static_cast<long long>(conjunctions[conjIndex]));
                    features[nodeIndex].push_back(featureToAdd);
                }
            }
        }
    }
    return features;
}

vector<int> ConjunctionsChainTransformer::BackwardTransform(
    const vector<int>& transformedPredictedStates) const
{
 return transformedPredictedStates;
}

vector<double> ConjunctionsChainTransformer::BackwardTransform(
    const vector<double>& transformedPredictedStates) const
{
 return transformedPredictedStates;
}
