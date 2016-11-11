#include "SimpleEdgeCalculator.h"
#include "Tools.h"
#include "FileManager.h"
#include <cmath>

SimpleEdgeCalculator::SimpleEdgeCalculator(void)
{
    Read("C:\\Users\\s.muzychka\\Documents\\Visual Studio 2010\\Projects\\MyTrials\\Release\\Token_Frequency.txt", tokenFrequency);
}


SimpleEdgeCalculator::~SimpleEdgeCalculator(void)
{
}

wstring SimpleEdgeCalculator::GetNodePrefix(const SyntaxNode& node)
{
    /*vector<wstring> splitted;
    Split(node.token, L"|", splitted);
    wstring token = ToLower(splitted[0]);
    if (tokenFrequency.find(token) != tokenFrequency.end())
    {
        return token;
    }*/
    return node.features.size() > 0 ? node.features[0] : L"empty";
}

wstring SimpleEdgeCalculator::GetPrefix(const SyntaxNode& left, const SyntaxNode& right)
{
    return GetNodePrefix(left) + L"#" + GetNodePrefix(right);
}

vector<wstring> SimpleEdgeCalculator::operator () (
    const SyntaxNode& left, const SyntaxNode& right, const SyntaxTree& tree)
{
    vector<wstring> feat;
    /*wstring prefix = GetPrefix(left, right);

    int dist = right.index - left.index;
    if (dist > 8) dist = 100;
    else if (dist < -8) dist = -100;
    wstring distance = std::to_wstring(static_cast<long double>(dist));
    feat.push_back(prefix + L"@distance@" + distance);

    wstring fullLeftTag = L"";
    for (size_t it = 0; it < left.features.size(); ++it)
    {
        feat.push_back(prefix + L"@Left@" + left.features[it] + L"@distance@" + distance);
        feat.push_back(prefix + L"@Left@" + left.features[it]);
        fullLeftTag += left.features[it];
    }
    wstring fullRightTag = L"";
    for (size_t it = 0; it < right.features.size(); ++it)
    {
        fullRightTag += right.features[it];
        feat.push_back(prefix + L"@Left@" + right.features[it] + L"@distance@" + distance);
        feat.push_back(prefix + L"@Left@" + right.features[it]);
    }
    feat.push_back(prefix + L"@Left@" + fullLeftTag);
    feat.push_back(prefix + L"@Right@" + fullRightTag);

    vector<wstring> splitted;
    Split(left.token, L"|", splitted);
    wstring leftToken = ToLower(splitted[0]);
    if (tokenFrequency.find(leftToken) != tokenFrequency.end())
    {
        feat.push_back(prefix + L"@Left@" + leftToken + L"@distance@" + distance);
    }

    Split(right.token, L"|", splitted);
    wstring rightToken = ToLower(splitted[0]);
    if (tokenFrequency.find(rightToken) != tokenFrequency.end())
    {
        feat.push_back(prefix + L"@Right@" + rightToken + L"@distance@" + distance);
    }*/
    // Prefix;
    wstring prefix = GetPrefix(left, right);
    feat.push_back(prefix);
    // Distance
    int distance = right.index - left.index;
    feat.push_back(prefix + L"_distance_" + std::to_wstring(static_cast<long double>(distance)));
    // Conjunctions
    int conjunctions = 2;
    int leftIndex = left.index;
    int rightIndex = right.index;
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    int size = tree.GetSize();
    for (int conj = max(leftIndex - conjunctions, 0); conj <= min(leftIndex + conjunctions, size - 1); ++conj)
    {
        //if (nodes[conj].features.size() > 0 && 
        //     (nodes[conj].features[0] == L"CONJ" || nodes[conj].features[0] == L"PART" || nodes[conj].features[0] == L"PR"))
        if (tokenFrequency.find(nodes[conj].token) != tokenFrequency.end())
        {
            feat.push_back(prefix + L"L#" + Tools::ToLower(nodes[conj].token) +
                L"#" + std::to_wstring(static_cast<long double>(conj - leftIndex)));
        }
        for (size_t it = 1; it < nodes[conj].features.size(); ++it)
        {
            feat.push_back(
                prefix + L"L#" + nodes[conj].features[it] + 
                L"#" + std::to_wstring(static_cast<long double>(conj - leftIndex)));
        }
    }
    for (int conj = max(rightIndex - conjunctions, 0); conj <= min(rightIndex + conjunctions, size - 1); ++conj)
    {
        //if (nodes[conj].features.size() > 0 && 
        //    (nodes[conj].features[0] == L"CONJ" || nodes[conj].features[0] == L"PART" || nodes[conj].features[0] == L"PR"))
        if (tokenFrequency.find(nodes[conj].token) != tokenFrequency.end())
        {
            feat.push_back(prefix + L"R#" + Tools::ToLower(nodes[conj].token) +
                L"#" + std::to_wstring(static_cast<long double>(conj - rightIndex)));
        }
        for (size_t it = 1; it < nodes[conj].features.size(); ++it)
        {
            feat.push_back(prefix + L"R#" + nodes[conj].features[it] + 
                L"#" + std::to_wstring(static_cast<long double>(conj - rightIndex)));
        }
    }
    return feat;
}
