#include "FullMorphologyPossibleStateFinder.h"
#include "FileReader.h"

namespace Disambiguation
{

FullMorphologyPossibleStateFinder::FullMorphologyPossibleStateFinder(
    const string& exceptionalTokensFile
    , const string& tagsFile)
{
    FileManager::Read(tagsFile, &allTags);
    // initialize allPOS
    for (auto iter = allTags.begin(); iter != allTags.end(); ++iter)
    {
        size_t dogIndex = iter->find(L'@');
        if (dogIndex != wstring::npos) {
            allPOS.insert(L"POS_" + iter->substr(0, dogIndex));
        } else {
            allPOS.insert(L"POS_" + *iter);
        }
    }
    FileManager::Read(exceptionalTokensFile, &fullTagsForExceptionTokens, L'\t', L',');
    // Evaluate POS for exceptional tokens
    for (auto iter = fullTagsForExceptionTokens.begin()
        ; iter != fullTagsForExceptionTokens.end(); ++iter)
    {
        unordered_set<wstring> currentPOS;
        const vector<wstring>& currentFullTags = iter->second;
        for (size_t tagIndex = 0; tagIndex < currentFullTags.size(); ++tagIndex)
        {
            size_t dogIndex = currentFullTags[tagIndex].find(L'@');
            if (dogIndex != wstring::npos) {
                currentPOS.insert(L"POS_" + currentFullTags[tagIndex].substr(
                    0, dogIndex));
            } else {
                currentPOS.insert(L"POS_" + currentFullTags[tagIndex]);
            }
        }
        posTagsForExceptionTokens[iter->first] = vector<wstring>(
            currentPOS.begin(), currentPOS.end());
    }
}

void FullMorphologyPossibleStateFinder::FindPossibleStates(
    LinearCRF::Chain* chain)
{
    size_t length = chain->GetSize();
    vector<vector<wstring> > possibleStates(length);
    // Iterate over nodes
    for (size_t nodeIndex = 0; nodeIndex < length; ++nodeIndex)
    {
        const wstring& currentToken = chain->GetTokens()[nodeIndex];
        const vector<wstring>& currentFeatures = chain->GetFeatures()[nodeIndex];
        vector<wstring>& currentPossibleStates = possibleStates[nodeIndex];
        set<wstring> currentPossibleStatesSet;
        // Iterate over features
        for (size_t featureIndex = 0; featureIndex < currentFeatures.size()
            ; ++featureIndex)
        {
            // Get string representation of feature index
            const wstring& currentFeature
                = currentFeatures[featureIndex];
            // Evaluate exceptional tokens
            if (evaluateExceptionalTokens(nodeIndex, currentFeature, 
                &currentPossibleStates))
            {
                break;
            }
            // Evaluate full tags nodes
            if (nodeIndex % 2 == 0)
            {
                evaluateEvenNode(
                    currentFeature, currentToken
                    , &currentPossibleStatesSet);
            } else {
                evaluateOddNode(
                    currentFeature, currentToken
                    , &currentPossibleStatesSet);
            }
        }
        // Adds possible states for nodes without possible states
        if (currentPossibleStatesSet.size() == 0 &&
            currentPossibleStates.size() == 0)
        {
            evaluateUnrealizedNode(nodeIndex,
                &currentPossibleStates);
        } else if (currentPossibleStatesSet.size() > 0) {
            for (auto iter = currentPossibleStatesSet.begin(); 
                iter != currentPossibleStatesSet.end(); ++iter)
            {
                currentPossibleStates.push_back(*iter);
            }
        }
    }
    chain->SetPossibleStates(std::move(possibleStates));
}

bool FullMorphologyPossibleStateFinder::evaluateExceptionalTokens(
    int indexInChain, const wstring& currentFeature
    , vector<wstring>* currentPossibleStates) const
{
    if (currentFeature.size() < 4)
    {
        return false;
    }
    wstring exceptionalPretendent(
                currentFeature.begin() + 2, currentFeature.end() - 2);
    const unordered_map<wstring, vector<wstring> >& exceptionSet = 
        indexInChain % 2 == 0 ? fullTagsForExceptionTokens : posTagsForExceptionTokens;
    if (exceptionSet.find(exceptionalPretendent) != exceptionSet.end())
    {
        *currentPossibleStates = 
            exceptionSet.find(exceptionalPretendent)->second;
        return true;
    }
    return false;
}

void FullMorphologyPossibleStateFinder::evaluateEvenNode(
    const wstring& currentFeature
    , const wstring& currentToken
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
        currentPossibleStatesSet->insert(
            correctFeatureToLabel(currentFeature));
    }
}

void FullMorphologyPossibleStateFinder::evaluateOddNode(
    const wstring& _currentFeature
    , const wstring& currentToken
    , set<wstring>* currentPossibleStatesSet) const
{
    if (_currentFeature == L"V"
        && Tools::ToLower(currentToken) != L"copy_\u043d\u0435") // !!! THIS IS A FEATURE MADE BECAUSE OF SYNTAGRUS STRANGES !!!//here was cyrrilic symbols: не
    {
        return;
    }
    wstring currentFeature = correctFeatureToLabel(_currentFeature);
    if (allTags.find(currentFeature) == allTags.end())
    {
        return;
    }
    // Find current pos
    int dogIndex = currentFeature.find(L'@');
    wstring currentPOS;
    if (dogIndex >= 0) {
        currentPOS = L"POS_" + wstring(currentFeature.begin(), 
            currentFeature.begin() + dogIndex);
    } else {
        currentPOS = L"POS_" + currentFeature;
    }
    // Add state to collection
    if (allPOS.find(currentPOS) != allPOS.end())
    {
        currentPossibleStatesSet->insert(
            currentPOS);
    }
}

void FullMorphologyPossibleStateFinder::evaluateUnrealizedNode(
    int indexInChain
    , vector<wstring>* currentPossibleStates) const
{
    const unordered_set<wstring>& defaultSet = 
        (indexInChain % 2 == 0) ? allTags : allPOS;
    for (auto defaultSetIterator = defaultSet.begin(); 
        defaultSetIterator != defaultSet.end(); 
        ++defaultSetIterator)
    {
        currentPossibleStates->push_back(*defaultSetIterator);
    }
}

wstring FullMorphologyPossibleStateFinder::correctFeatureToLabel(
    const wstring& feature) const
{
    wstring converted = Tools::Replace(feature, L"@PEREH", L"");
    converted = Tools::Replace(converted, L"@NEPEREH", L"");
    converted = Tools::Replace(converted, L"@STRAD", L"");
    return converted;
}

};
