#include "DefaultPossibleStateFinder.h"

namespace LinearCRF
{

DefaultPossibleStateFinder::DefaultPossibleStateFinder()
{
}

DefaultPossibleStateFinder::DefaultPossibleStateFinder(
    const string& tagsFile)
{
    FileManager::Read(tagsFile, &allTags);
}

void DefaultPossibleStateFinder::FindPossibleStates(
    Chain* chain)
{
    size_t size = chain->GetSize();
    vector<vector<wstring> > possibleStates;
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        const vector<wstring>& currentFeatures
            = chain->GetFeatures()[nodeIndex];
        possibleStates.emplace_back(vector<wstring>());
        vector<wstring>& currentPossibleStates = possibleStates.back();
        for (size_t featureIndex = 0; featureIndex < currentFeatures.size()
            ; ++featureIndex)
        {
            if (allTags.find(currentFeatures[featureIndex]) != allTags.end())
            {
                currentPossibleStates.push_back(currentFeatures[featureIndex]);
            }
        }
    }
    chain->SetPossibleStates(
        std::move(possibleStates));
}

void DefaultPossibleStateFinder::ConnectWithModel(
    const CRF& crf)
{
    allTags = unordered_set<wstring>(
        crf.GetOutputAlphabet().GetData().begin()
        , crf.GetOutputAlphabet().GetData().end());
}

}

