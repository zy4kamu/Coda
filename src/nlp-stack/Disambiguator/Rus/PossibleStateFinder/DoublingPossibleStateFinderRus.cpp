#include "DoublingPossibleStateFinderRus.h"
#include "FileReader.h"

namespace Disambiguation
{

DoublingPossibleStateFinderRus::DoublingPossibleStateFinderRus(
    const string& exceptionalTokensFile
    , const string& tagsFile)
{
    FileManager::Read(exceptionalTokensFile, &fullTagsForExceptionTokens, L'\t', L',');
    FileManager::Read(tagsFile, &allTags);
}

void DoublingPossibleStateFinderRus::FindPossibleStates(Chain* chain)
{
    size_t length = chain->GetSize();
    vector<vector<wstring> > possibleStates(length);
    // Iterate over even nodes
    for (size_t nodeIndex = 0; nodeIndex < length; nodeIndex += 2)
    {
        const wstring& currentToken = chain->GetTokens()[nodeIndex];
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
            evaluateEvenNode(currentFeature, currentToken
                , &currentPossibleStatesSet);
        }
        // Adds possible states for nodes without possible states
        if (currentPossibleStatesSet.size() == 0
            && currentPossibleStates.size() == 0)
        {
            evaluateUnrealizedNode(nodeIndex,
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

bool DoublingPossibleStateFinderRus::evaluateExceptionalTokens(
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

void DoublingPossibleStateFinderRus::evaluateEvenNode(
    const wstring& currentFeature, const wstring& currentToken
    , set<wstring>* currentPossibleStatesSet) const
{
    if (currentFeature == L"V"
        && Tools::ToLower(currentToken) != L"\u043d\u0435") // !!! THIS IS A FEATURE MADE BECAUSE OF SYNTAGRUS STRANGES !!!//here was cyrrilic symbols: не
    {
        return;
    }
    if (allTags.find(correctFeatureToLabel(currentFeature))
        == allTags.end()) {
        return;
    } else {
        currentPossibleStatesSet->insert(correctFeatureToLabel(currentFeature));
    }
}

void DoublingPossibleStateFinderRus::evaluateUnrealizedNode(
    int indexInChain
    , vector<wstring>* currentPossibleStates) const
{
    for (auto defaultSetIterator = allTags.begin(); 
        defaultSetIterator != allTags.end(); 
        ++defaultSetIterator)
    {
        currentPossibleStates->push_back(*defaultSetIterator);
    }
}

void DoublingPossibleStateFinderRus::evaluateOddNode(
    const vector<wstring>& neighbourStates
    , vector<wstring>* statesToCalculate) const
{
    for (size_t stateIndex = 0; stateIndex < neighbourStates.size()
        ; ++stateIndex)
    {
        statesToCalculate->push_back(L"copy_" + neighbourStates[stateIndex]);
    }
}

wstring DoublingPossibleStateFinderRus::correctFeatureToLabel(
    const wstring& feature) const
{
    wstring converted = Tools::Replace(feature, L"@PEREH", L"");
    converted = Tools::Replace(converted, L"@NEPEREH", L"");
    converted = Tools::Replace(converted, L"@STRAD", L"");
    return converted;
}

}
