#include "NodeFeatureCalculatorRus.h"
#include <map>

using namespace SyntaxParser;
using std::unordered_map;

NodeFeatureCalculatorRus::NodeFeatureCalculatorRus() :
    exceptionTokens(new set<wstring>)
{
    unordered_map<wstring, int> tokenFrequency;
    FileManager::Read(string(SYNTAX_CONFIG_FOLDER) + "RU/FrequentTokens.txt", &tokenFrequency, L',');
    for (auto iterator = tokenFrequency.begin()
        ; iterator != tokenFrequency.end()
        ; ++iterator)
    {
        exceptionTokens->insert(iterator->first);
    }
}


NodeFeatureCalculatorRus::~NodeFeatureCalculatorRus(void)
{
}

vector<vector<wstring> > NodeFeatureCalculatorRus::Calculate(
    const SyntaxTree& tree)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    // Calculate punctuation splittings
    vector<int> punctuationSplitters = 
        calculatePunctuationSplittings(nodes);
    // Calculate one token features
    vector<vector<wstring> > features = calculateOneTokenFeatures(nodes);
    // Apply conjunctions
    applyConjunctions(punctuationSplitters, &features, 2);
    // Calculate one part features
    calculateOnePartFeatures(nodes, punctuationSplitters, &features);
    return features;
}

vector<vector<wstring> > NodeFeatureCalculatorRus::calculateOneTokenFeatures(
        const vector<SyntaxNode>& nodes)
{
    vector<vector<wstring> > features(nodes.size());
    // Iterate over nodes
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        // Add features that are already contained in nodes
        const vector<wstring>& feat = nodes[nodeIndex].splittedLabel;
        vector<wstring>& currentFeatures = features[nodeIndex];
        for (size_t it = 0; it < feat.size(); ++it)
        {
            currentFeatures.push_back(feat[it]);
        }
        // Add exception tokens (that are in FrequentTokens.txt file)
        if (exceptionTokens->find(nodes[nodeIndex].content) != exceptionTokens->end())
        {
            currentFeatures.push_back(nodes[nodeIndex].content);
        }
        // Add punctuation as feature
        if (nodes[nodeIndex].punctuation.size() > 0)
        {
            wstring punctuationFeature = L"P";
            for (size_t punctuationIndex = 0
                ; punctuationIndex < nodes[nodeIndex].punctuation.size()
                ; ++punctuationIndex)
            {
                punctuationFeature += nodes[nodeIndex].punctuation[punctuationIndex];
            }
            currentFeatures.push_back(punctuationFeature);
        }
    }
    return features;
}

void NodeFeatureCalculatorRus::calculateOnePartFeatures(
    const vector<SyntaxNode>& nodes 
    , const vector<int>& punctuationSplitters
    , vector<vector<wstring> >* features)
{
    // Iterate over part of sentence between to punctuations
    for (size_t part = 0; part < punctuationSplitters.size() - 1; ++part)
    {
        int left = punctuationSplitters[part];
        int right = punctuationSplitters[part + 1] - 1;
        // Iterate over left node in a part
        for (int nodeIndex = left; nodeIndex <= right; ++nodeIndex)
        {
            // Iterate over right node in a part
            for (int neighbourIndex = left; neighbourIndex <= right; ++neighbourIndex)
            {
                if (neighbourIndex == nodeIndex)
                {
                    continue;
                }
                vector<wstring>& currentFeatures = (*features)[nodeIndex];
                const vector<wstring>& neighbourFeatures = nodes[neighbourIndex].splittedLabel;
                // Adding full neighbour tag
                wstring neighbourFullTag;
                for (size_t it = 0 ; it < neighbourFeatures.size(); ++it)
                {
                    neighbourFullTag += neighbourFeatures[it];
                }
                currentFeatures.push_back(L"T" + neighbourFullTag);
            }
        }
    }
}

vector<int> NodeFeatureCalculatorRus::calculatePunctuationSplittings(
    const vector<SyntaxNode>& nodes)
{
    vector<int> splitters;
    splitters.push_back(0);
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        if (nodes[nodeIndex].punctuation.size() > 0)
        {
            splitters.push_back(nodeIndex + 1);
        }
    }
    if (splitters.back() != static_cast<int>(nodes.size()))
    {
        splitters.push_back(nodes.size());
    }
    return splitters;
}

void NodeFeatureCalculatorRus::applyConjunctions(
    const vector<int>& punctuationSplitters, 
    vector<vector<wstring> >* features, int conjunctions)
{
    for (size_t part = 0; part + 1 < punctuationSplitters.size(); ++part)
    {
        int leftBound = punctuationSplitters[part];
        int rightBound = punctuationSplitters[part + 1] - 1;
        for (int nodeIndex = leftBound; nodeIndex <= rightBound; ++nodeIndex)
        {
            // Get current features
            vector<wstring>& currentFeatures = (*features)[nodeIndex];
            // Iterate over neighbours
            for (int neighbourIndex = nodeIndex - conjunctions; 
                neighbourIndex <= nodeIndex + conjunctions; 
                ++neighbourIndex)
            {
                // Check neighbour for correctness
                if (neighbourIndex < 0 || 
                    neighbourIndex == nodeIndex || 
                    neighbourIndex >= int(features->size()))
                {
                    continue;
                }
                // Get neighbour features
                vector<wstring>& neighbourFeatures = (*features)[neighbourIndex];
                // Get prefix
                wstring partPrefix = 
                    (neighbourIndex < leftBound) ? L"QQP" : 
                    (neighbourIndex > rightBound) ? L"QQN" : L"QQC";
                // Get distance
                wstring distance = Tools::ToWstring(neighbourIndex - nodeIndex);
                // Add conjunction feature
                for (size_t it = 0; it < neighbourFeatures.size(); ++it)
                {
                    if (Tools::ContainsSubstring(neighbourFeatures[it], L"QQ"))
                    {
                        continue;
                    }
                    wstring conjunctionFeature = 
                        partPrefix + L"#" + 
                        neighbourFeatures[it] + L"#" +
                        distance;
                    currentFeatures.push_back(conjunctionFeature);
                }
            }
        }
    }
}
