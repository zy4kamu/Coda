#include "ContinuousFeatureBatch.hpp"

namespace SyntaxParser
{

typedef ContinuousFeatureBatch<int> IntBatch;
typedef ContinuousFeatureBatch<wstring> StringBatch;

IntBatch Translate(const StringBatch& batch,
    shared_ptr<Alphabet> alphabet, bool addingToAlphabet)
{
    vector<int> indexes;
    unordered_map<int, double> weights;

    size_t size = batch.GetSize();
    indexes.reserve(size);
    for (size_t index = 0; index < size; ++index)
    {
        if (addingToAlphabet) {
            indexes.push_back(alphabet->LookUpIndex_Adding(
                batch.GetFeature(index)));
        } else {
            int toAdd = alphabet->LookUpIndex_NoAdding(
                batch.GetFeature(index));
            if (toAdd != -1)
            {
                indexes.push_back(toAdd);
            }
        }
    }

    const unordered_map<wstring, double>& stringWeights = batch.GetWeights();
    for (auto iter = stringWeights.begin(); iter != stringWeights.end(); ++iter)
    {
        if (addingToAlphabet) {
            weights[alphabet->LookUpIndex_Adding(iter->first)] = iter->second;
        } else {
            int toAdd = alphabet->LookUpIndex_NoAdding(iter->first);
            if (toAdd != -1)
            {
                weights[toAdd] = iter->second;
            }
        }
    }

    return IntBatch(std::move(indexes), std::move(weights));
}

StringBatch Translate(const IntBatch& batch,
    shared_ptr<Alphabet> alphabet)
{
    vector<wstring> features;
    unordered_map<wstring, double> weights;

    size_t size = batch.GetSize();
    features.resize(size);
    for (size_t index = 0; index < size; ++index)
    {
        features[index] = alphabet->LookUpObject(batch.GetFeature(index));
    }

    const unordered_map<int, double>& intWeights = batch.GetWeights();
    for (auto iter = intWeights.begin(); iter != intWeights.end(); ++iter)
    {
        weights[alphabet->LookUpObject(iter->first)] = iter->second;
    }

    return StringBatch(std::move(features), std::move(weights));
}

}
