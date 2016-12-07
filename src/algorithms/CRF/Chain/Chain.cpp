#include "Chain.h"
#include "Tools.h"

namespace LinearCRF
{

Chain::Chain()
{
}

Chain::Chain(const Chain& chain)
    : labels(chain.labels)
    , features(chain.features)
    , tokens(chain.tokens)
    , possibleStates(chain.possibleStates)
{
}

Chain::Chain(const Chain&& chain)
    : labels(std::move(chain.labels))
    , features(std::move(chain.features))
    , tokens(std::move(chain.tokens))
    , possibleStates(std::move(chain.possibleStates))
{
}

Chain::Chain(vector<wstring>& tokens
    , vector<vector<wstring> >& features 
    , vector<wstring>& labels
    , vector<vector<wstring> >& possibleStates)
    : labels(labels)
    , features(features)
    , tokens(tokens)
    , possibleStates(possibleStates)
{
}

Chain::Chain(vector<wstring>&& tokens
    , vector<vector<wstring> >&& features 
    , vector<wstring>&& labels
    , vector<vector<wstring> >&& possibleStates)
    : labels(std::move(labels))
    , features(std::move(features))
    , tokens(std::move(tokens))
    , possibleStates(std::move(possibleStates))
{
}

Chain::Chain(wifstream& stream)
{
    stream >> *this;
}

const vector<wstring>& Chain::GetTokens() const
{
    return tokens;
}

const vector<wstring>& Chain::GetLabels() const
{
    return labels;
}

const vector<vector<wstring> >& Chain::GetFeatures() const
{
    return features;
}

const vector<vector<wstring> >& Chain::GetPossibleStates() const
{
    return possibleStates;
}

void Chain::AddFeature(int nodeIndex, const wstring& feature)
{
    features[nodeIndex].push_back(feature);
}

const size_t Chain::GetSize() const
{
    return features.size();
}

Chain& Chain::operator = (const Chain& chain)
{
    tokens = chain.tokens;
    labels = chain.labels; 
    features = chain.features;
    possibleStates = chain.possibleStates;
    return *this;
}

Chain& Chain::operator = (Chain&& chain)
{
    tokens = std::move(chain.tokens);
    labels = std::move(chain.labels);
    features = std::move(chain.features);
    possibleStates = std::move(chain.possibleStates);
    return *this;
}

Chain::~Chain(void)
{
}

void Chain::SetPossibleStates(
    const vector<vector<wstring> >& _possibleStates)
{
    possibleStates = _possibleStates;
}


void Chain::SetPossibleStates(
    vector<vector<wstring> >&& _possibleStates)
{
    possibleStates = std::move(_possibleStates);
}

wifstream& operator>>(wifstream& stream, Chain& chain)
{
    // Clear
    chain.tokens.clear();
    chain.labels.clear();
    chain.features.clear();
    // Read sentence
    wstring line;
    while(getline(stream, line))
    {
        if (Tools::Trim(line).size() == 0)
        {
            return stream;
        } else {
            vector<wstring> tokenFeaturesLabel = Tools::Split(
                line, L" ");
            
            // Add token
            chain.tokens.push_back(tokenFeaturesLabel[0]);
            // Add label
            chain.labels.push_back(tokenFeaturesLabel[2]);
            // Add features
            chain.features.emplace_back(0);
            chain.features.back() = Tools::Split(tokenFeaturesLabel[1], L",");
        }
    }
    return stream;
}

};
