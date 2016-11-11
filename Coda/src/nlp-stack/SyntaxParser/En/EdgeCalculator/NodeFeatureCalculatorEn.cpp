#include "NodeFeatureCalculatorEn.h"
#include <map>

namespace SyntaxParser
{

using std::unordered_map;

NodeFeatureCalculatorEn::NodeFeatureCalculatorEn() :
    exceptionTokens(new std::unordered_set<wstring>)
{
    unordered_map<wstring, int> tokenFrequency;
    FileManager::Read(string(SYNTAX_CONFIG_FOLDER) + "EN/FrequentTokens.txt", &tokenFrequency, L' ');
    for (auto iterator = tokenFrequency.begin()
        ; iterator != tokenFrequency.end()
        ; ++iterator)
    {
        exceptionTokens->insert(iterator->first);
    }
}

vector<vector<wstring> > NodeFeatureCalculatorEn::Calculate(
    const SyntaxTree& tree)
{
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    vector<vector<wstring> > features = calculateOneTokenFeatures(nodes);
    applyConjunctions(&features, 2);
    return features;
}

vector<vector<wstring> > NodeFeatureCalculatorEn::calculateOneTokenFeatures(
    const vector<SyntaxNode>& nodes)
{
    vector<vector<wstring> > features(nodes.size());
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        vector<wstring>& currentFeatures = features[nodeIndex];
        // Add label
        currentFeatures.push_back(nodes[nodeIndex].label);
        // Add exception tokens (that are in FrequentTokens.txt file)
        if (exceptionTokens->find(nodes[nodeIndex].content) != exceptionTokens->end())
        {
            currentFeatures.push_back(nodes[nodeIndex].content);
        }
    }
    return features;
}

void NodeFeatureCalculatorEn::applyConjunctions(
    vector<vector<wstring> >* features
    , int conjunctions)
{
    for (int nodeIndex = 0; nodeIndex < int(features->size()); ++nodeIndex)
    {
        vector<wstring>& currentFeatures = (*features)[nodeIndex];
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
            // Get distance
            wstring distance = Tools::ToWstring(neighbourIndex - nodeIndex);
            // Add conjunction feature
            for (size_t it = 0; it < neighbourFeatures.size(); ++it)
            {
                if (Tools::ContainsSubstring(neighbourFeatures[it], L"conj_"))
                {
                    continue;
                }
                wstring conjunctionFeature =
                    L"conj_"
                    + neighbourFeatures[it]
                    + L"#"
                    + distance;
                currentFeatures.push_back(conjunctionFeature);
            }
        }
    }
}

}
