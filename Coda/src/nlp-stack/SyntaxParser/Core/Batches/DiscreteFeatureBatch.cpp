#include "DiscreteFeatureBatch.hpp"

namespace SyntaxParser
{

typedef DiscreteFeatureBatch<int> IntBatch;
typedef DiscreteFeatureBatch<wstring> StringBatch;

IntBatch Translate(
    const StringBatch& batch,
    shared_ptr<Alphabet> alphabet, bool addingToAlphabet)
{
    size_t size = batch.GetSize();
    vector<int> indexes;
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

    return IntBatch(std::move(indexes));
}

StringBatch Translate(
    const IntBatch& batch,
    shared_ptr<Alphabet> alphabet)
{
    vector<wstring> features;

    size_t size = batch.GetSize();
    features.resize(size);
    for (size_t index = 0; index < size; ++index)
    {
        features[index] = alphabet->LookUpObject(batch.GetFeature(index));
    }

    return StringBatch(std::move(features));
}

}

