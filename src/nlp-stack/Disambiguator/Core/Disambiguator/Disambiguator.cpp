#include "Disambiguator.h"
#include "ChainCollectionTester.h"

using LinearCRF::EncodedCRFData;
using Tools::Alphabet;
using LinearCRF::Chain;

namespace Disambiguation
{

const int UNSUPPORTED_LEMMA_INDEX = -1;
const wstring NOT_FOUND_LEMMA = L"#LemmaNotFoundError";
const wstring DICT_IS_NULL = L"DICT_IS_NULL";

Disambiguator::Disambiguator(
    const string& crfFile
    , shared_ptr<MorphologicalDictionary> dictionary
    , shared_ptr<IFeatureCalculator> featureCalculator
    , shared_ptr<IChainTransformer> chainTransformer
    , shared_ptr<IPossibleStateFinder> possibleStateFinder
    , Tools::Language language
    , bool isCRFfileBinary)
    : CRFApplier(crfFile, chainTransformer
        , possibleStateFinder, isCRFfileBinary)
    , dictionary(dictionary)
    , featureCalculator(featureCalculator)
    , language(language)
{
}

Disambiguator::Disambiguator(
    shared_ptr<CRF> model
    , shared_ptr<MorphologicalDictionary> dictionary
    , shared_ptr<IFeatureCalculator> featureCalculator
    , shared_ptr<IChainTransformer> chainTransformer
    , shared_ptr<IPossibleStateFinder> possibleStateFinder
    , Tools::Language language)
    : CRFApplier(model, chainTransformer
        , possibleStateFinder)
    , dictionary(dictionary)
    , featureCalculator(featureCalculator)
    , language(language)
{

}

vector<DisambiguatedData> Disambiguator::Disambiguate(
    const vector<Token>& tokens)
{
    vector<PredisambiguatedData> predisambiguated
        = featureCalculator->CalculateFeatures(tokens);
    return disambiguate(predisambiguated);
}

vector<DisambiguatedData> Disambiguator::disambiguate(
    const vector<PredisambiguatedData>& predisambiguated)
{
    // Create chain
    size_t size = predisambiguated.size();
    vector<wstring> words(size);
    vector<vector<wstring> > features(size);
    vector<wstring> labels(size);
    for (size_t chainIndex = 0; chainIndex < size; ++chainIndex)
    {
        words[chainIndex] = predisambiguated[chainIndex].content;
        features[chainIndex] = predisambiguated[chainIndex].features;
    }
    LinearCRF::Chain chain(
        std::move(words)
        , std::move(features)
        , std::move(labels)
        , vector<vector<wstring> >());
    vector<wstring> bestSequence;
    vector<double> bestSequenceWeights;
    this->Apply(chain, &bestSequence, &bestSequenceWeights);
    // Create disambiguated data
    vector<DisambiguatedData> disambiguatedData;
    for (size_t tokenIndex = 0; tokenIndex < size; ++tokenIndex)
    {
        wstring& label = bestSequence[tokenIndex];
        shared_ptr<Morphology> grammInfo = getBestGrammInfo(
            predisambiguated[tokenIndex], label);
        applyPostprocessRules(&label, grammInfo);
        const wstring& lemma
            = dictionary == 0 ? DICT_IS_NULL
            : *(grammInfo->lemma) == NOT_FOUND_LEMMA
            ? Tools::ToLower(predisambiguated[tokenIndex].content) : *(grammInfo->lemma);
        disambiguatedData.emplace_back(
            predisambiguated[tokenIndex].content
            , predisambiguated[tokenIndex].punctuation
            , predisambiguated[tokenIndex].source
            , predisambiguated[tokenIndex].isNextSpace
            , lemma
            , label
            , bestSequenceWeights[tokenIndex]
            , grammInfo->lemma_id);
    }
    return disambiguatedData;
}

vector<vector<DisambiguatedData> > Disambiguator::Disambiguate(
    const vector<Token>& tokens, int numberOfHypothesis
    , vector<double>* hypothesisDistribution)
{
    vector<PredisambiguatedData> predisambiguated
        = featureCalculator->CalculateFeatures(tokens);
    // Create chain
    size_t size = predisambiguated.size();
    vector<wstring> words(size);
    vector<vector<wstring> > features(size);
    vector<wstring> labels(size);
    for (size_t chainIndex = 0; chainIndex < size; ++chainIndex)
    {
        words[chainIndex] = predisambiguated[chainIndex].content;
        features[chainIndex] = predisambiguated[chainIndex].features;
    }
    LinearCRF::Chain chain(
        std::move(words)
        , std::move(features)
        , std::move(labels)
        , vector<vector<wstring> >());

    vector<vector<wstring> > bestSequences;
    vector<vector<double> > bestSequenceWeights;
    this->Apply(chain
        , numberOfHypothesis
        , &bestSequences
        , &bestSequenceWeights
        , hypothesisDistribution);

    vector<vector<DisambiguatedData> > topDisambiguatedSequences;
    for (size_t chainIndex = 0; chainIndex < bestSequences.size()
         ; ++chainIndex)
    {
        vector<DisambiguatedData> disambiguatedData;
        for (size_t tokenIndex = 0; tokenIndex < size; ++tokenIndex)
        {
            wstring& label = bestSequences[chainIndex][tokenIndex];
            shared_ptr<Morphology> grammInfo = getBestGrammInfo(
                predisambiguated[tokenIndex], label);
            applyPostprocessRules(&label, grammInfo);
            const wstring& lemma
                = dictionary == 0 ? DICT_IS_NULL
                : *(grammInfo->lemma) == NOT_FOUND_LEMMA
                ? Tools::ToLower(predisambiguated[tokenIndex].content) : *(grammInfo->lemma);
            disambiguatedData.emplace_back(
                predisambiguated[tokenIndex].content
                , predisambiguated[tokenIndex].punctuation
                , predisambiguated[tokenIndex].source
                , predisambiguated[tokenIndex].isNextSpace
                , lemma
                , label
                , bestSequenceWeights[chainIndex][tokenIndex]
                , grammInfo->lemma_id);
        }
        topDisambiguatedSequences.push_back(std::move(disambiguatedData));
    }
    return topDisambiguatedSequences;
}

vector<vector<DisambiguatedData> > Disambiguator::Disambiguate(
    const vector<Token>& tokens, double percentage, size_t maxNumberOfPaths
    , vector<double>* hypothesisDistribution)
{
    vector<PredisambiguatedData> predisambiguated
        = featureCalculator->CalculateFeatures(tokens);
    // Create chain
    size_t size = predisambiguated.size();
    vector<wstring> words(size);
    vector<vector<wstring> > features(size);
    vector<wstring> labels(size);
    for (size_t chainIndex = 0; chainIndex < size; ++chainIndex)
    {
        words[chainIndex] = predisambiguated[chainIndex].content;
        features[chainIndex] = predisambiguated[chainIndex].features;
    }
    LinearCRF::Chain chain(
        std::move(words)
        , std::move(features)
        , std::move(labels)
        , vector<vector<wstring> >());

    vector<vector<wstring> > bestSequences;
    vector<vector<double> > bestSequenceWeights;
    this->Apply(chain
        , percentage
        , maxNumberOfPaths
        , &bestSequences
        , &bestSequenceWeights
        , hypothesisDistribution);

    vector<vector<DisambiguatedData> > topDisambiguatedSequences;
    for (size_t chainIndex = 0; chainIndex < bestSequences.size()
         ; ++chainIndex)
    {
        vector<DisambiguatedData> disambiguatedData;
        for (size_t tokenIndex = 0; tokenIndex < size; ++tokenIndex)
        {
            wstring& label = bestSequences[chainIndex][tokenIndex];
            shared_ptr<Morphology> grammInfo = getBestGrammInfo(
                predisambiguated[tokenIndex], label);
            applyPostprocessRules(&label, grammInfo);
            const wstring& lemma
                = dictionary == 0 ? DICT_IS_NULL
                : *(grammInfo->lemma) == NOT_FOUND_LEMMA
                ? Tools::ToLower(predisambiguated[tokenIndex].content) : *(grammInfo->lemma);
            disambiguatedData.emplace_back(
                predisambiguated[tokenIndex].content
                , predisambiguated[tokenIndex].punctuation
                , predisambiguated[tokenIndex].source
                , predisambiguated[tokenIndex].isNextSpace
                , lemma
                , label
                , bestSequenceWeights[chainIndex][tokenIndex]
                , grammInfo->lemma_id);
        }
        topDisambiguatedSequences.push_back(std::move(disambiguatedData));
    }
    return topDisambiguatedSequences;
}

double Disambiguator::GetPartitionFunction(
    const vector<Token>& tokens)
{
    vector<PredisambiguatedData> predisambiguated
        = featureCalculator->CalculateFeatures(tokens);
    // Create chain
    size_t size = predisambiguated.size();
    vector<wstring> words(size);
    vector<vector<wstring> > features(size);
    vector<wstring> labels(size);
    for (size_t chainIndex = 0; chainIndex < size; ++chainIndex)
    {
        words[chainIndex] = predisambiguated[chainIndex].content;
        features[chainIndex] = predisambiguated[chainIndex].features;
    }
    LinearCRF::Chain chain(
        std::move(words)
        , std::move(features)
        , std::move(labels)
        , vector<vector<wstring> >());

    return this->GetPartFunction(chain);
}



shared_ptr<Morphology> Disambiguator::getBestGrammInfo(
    const PredisambiguatedData& predisambiguatedData
    , const wstring& label)
{
    const unordered_map<wstring, vector<shared_ptr<Morphology> > >& predisambiguated
        = predisambiguatedData.gramInfo;
    if (predisambiguated.empty())
    {
        shared_ptr<Morphology> info = std::make_shared<Morphology>();
        info->lemma_id = UNSUPPORTED_LEMMA_INDEX;
        info->lemma = std::make_shared<wstring>(NOT_FOUND_LEMMA);
        return info;
    }
    // If tag is present in predisambiguated just return value;
    if (predisambiguated.find(label) != predisambiguated.end())
    {
        return predisambiguated.at(label)[0];
    }
    // Otherwise calculate max number of coincidence of small tags
    vector<wstring> splittedTag = Tools::Split(label, L"@");
    int numGoodPositions = -1;
    wstring bestTag;
    for (auto it = predisambiguated.begin(); it != predisambiguated.end(); ++it)
    {
        vector<wstring> splitted = Tools::Split(
            it->first, L"@");
        set<wstring> curSet;
        for (size_t i = 0; i < splitted.size(); ++i)
        {
            curSet.insert(splitted[i]);
        }
        int sum = 0;
        for (size_t i = 0; i < splittedTag.size(); ++i)
        {
            if (curSet.find(splittedTag[i]) != curSet.end())
            {
                ++sum;
            }
        }
        if (sum > numGoodPositions)
        {
            numGoodPositions = sum;
            bestTag = it->first;
        }
    }
    return predisambiguated.at(bestTag)[0];
}

vector<wstring> Disambiguator::Lemmatize(
    const vector<Token>& tokens)
{
    vector<DisambiguatedData> disambiguated = this->Disambiguate(tokens);
    vector<wstring> lemmas(disambiguated.size());
    for (size_t tokenIndex = 0; tokenIndex < lemmas.size(); ++tokenIndex)
    {
        lemmas[tokenIndex] = std::move(disambiguated[tokenIndex].lemma);
    }
    return lemmas;
}

Disambiguator::~Disambiguator(void)
{
}

const Tools::Alphabet& Disambiguator::OutputAlphabet() const
{
    return crf->GetOutputAlphabet();
}

void Disambiguator::applyPostprocessRules(
    wstring* label
    , shared_ptr<Morphology> gramInfo) const
{
}

Tools::Language Disambiguator::GetLanguage() const
{
    return language;
}

}
