#include "DoublingPossibleStateFinder.h"
#include "FileReader.h"

namespace LinearCRF
{

DoublingPossibleStateFinder::DoublingPossibleStateFinder()
{
}

DoublingPossibleStateFinder::DoublingPossibleStateFinder(
    const string& tagsFile
    , const string& exceptionalTokensFile)
{
    if (exceptionalTokensFile.size() > 0)
    {
        FileManager::Read(exceptionalTokensFile, &fullTagsForExceptionTokens, L'\t', L',');
    }
    FileManager::Read(tagsFile, &allTags);
}

void DoublingPossibleStateFinder::FindPossibleStates(Chain* chain)
{
    size_t length = chain->GetSize();
    vector<vector<wstring> > possibleStates(length);
    // Iterate over even nodes
    for (size_t nodeIndex = 0; nodeIndex < length; nodeIndex += 2)
    {
        const vector<wstring>& currentFeaturesIndexes = chain->GetFeatures()[nodeIndex];
        vector<wstring>& currentPossibleStates = possibleStates[nodeIndex];
        set<wstring> currentPossibleStatesSet;
        // Iterate over features
        for (size_t featureIndex = 0; featureIndex < currentFeaturesIndexes.size()
            ; ++featureIndex)
        {
            // Get string representation of feature
            const wstring& currentFeature = 
                currentFeaturesIndexes[featureIndex];
            // Evaluate exceptional tokens
            if (evaluateExceptionalTokens(currentFeature 
                , &currentPossibleStates))
            {
                break;
            }
            // Evaluate full tags nodes
            evaluateEvenNode(currentFeature
                , &currentPossibleStatesSet);
        }
        // Adds possible states for nodes without possible states
        if (currentPossibleStatesSet.size() == 0
            && currentPossibleStates.size() == 0)
        {
            evaluateUnrealizedNode(
                &currentPossibleStates);
        } else if (currentPossibleStatesSet.size() > 0) {
            for (auto iter = currentPossibleStatesSet.begin()
                ; iter != currentPossibleStatesSet.end(); ++iter)
            {
                currentPossibleStates.push_back(*iter);
            }
        }
        // Evaluate odd nodes
        evaluateOddNode(
            currentPossibleStates
            , &possibleStates[nodeIndex + 1]);
    }
    chain->SetPossibleStates(std::move(possibleStates));
}

bool DoublingPossibleStateFinder::evaluateExceptionalTokens(
    const wstring& currentFeature
    , vector<wstring>* currentPossibleStates) const
{
    if (currentFeature.size() < 4)
    {
        return false;
    }
    wstring exceptionalPretendent(
                currentFeature.begin() + 2, currentFeature.end() - 2);
    if (fullTagsForExceptionTokens.find(exceptionalPretendent) 
        != fullTagsForExceptionTokens.end())
    {
        *currentPossibleStates
            = fullTagsForExceptionTokens.find(exceptionalPretendent)->second;
        return true;
    }
    return false;
}

void DoublingPossibleStateFinder::evaluateEvenNode(
    const wstring& currentFeature
    , set<wstring>* currentPossibleStatesSet) const
{
    if (allTags.find(currentFeature) != allTags.end())
    {
        currentPossibleStatesSet->insert(currentFeature);
    }
}

void DoublingPossibleStateFinder::evaluateUnrealizedNode(
    vector<wstring>* currentPossibleStates) const
{
    for (auto defaultSetIterator = allTags.begin(); 
        defaultSetIterator != allTags.end(); 
        ++defaultSetIterator)
    {
        currentPossibleStates->push_back(*defaultSetIterator);
    }
}

void DoublingPossibleStateFinder::evaluateOddNode(
    const vector<wstring>& neighbourStates
    , vector<wstring>* statesToCalculate) const
{
    for (size_t stateIndex = 0; stateIndex < neighbourStates.size()
        ; ++stateIndex)
    {
        statesToCalculate->push_back(L"copy_" + neighbourStates[stateIndex]);
    }
}

void DoublingPossibleStateFinder::ConnectWithModel(
    const CRF& crf)
{
    allTags = unordered_set<wstring>(
        crf.GetOutputAlphabet().GetData().begin()
        , crf.GetOutputAlphabet().GetData().end());
}

}
