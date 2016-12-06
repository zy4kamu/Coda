#include "Disambiguator.h"
#include "Tokenizer/PythonExtension.h"

extern "C"
{

namespace Disambiguation
{

/* FUNCTIONS RELATED TO DISAMBIGUATION */

vector<double> currentHypothesisDistribution;
vector<vector<DisambiguatedData> > currentDisambiguated;

void CreateDisambiguator(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
}

void Disambiguate(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
    currentHypothesisDistribution.clear();
    currentDisambiguated.clear();
    currentHypothesisDistribution.push_back(1);
    currentDisambiguated.emplace_back(disambiguator->Disambiguate(
        Tokenization::parsedTokens));
}

void DisambiguateHypothesis(const char* languagePtr, size_t numberOfHypothesis)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
    currentHypothesisDistribution.resize(Tokenization::parsedTokens.size());
    currentDisambiguated = disambiguator->Disambiguate(
        Tokenization::parsedTokens, numberOfHypothesis, &currentHypothesisDistribution);
}

void DisambiguateCoverage(const char* languagePtr, double coverage, size_t maxNumberOfPaths)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
    currentHypothesisDistribution.resize(Tokenization::parsedTokens.size());
    currentDisambiguated = disambiguator->Disambiguate(
        Tokenization::parsedTokens, coverage
        , maxNumberOfPaths, &currentHypothesisDistribution);
}

size_t RequestNumberOfHypothesis()
{
    return currentHypothesisDistribution.size();
}

double RequestHypothesisProbability(size_t hypothesisIndex)
{
    return currentHypothesisDistribution[hypothesisIndex];
}

const wchar_t* RequestLemma(size_t hypothesisIndex, size_t tokenIndex)
{
    return currentDisambiguated[hypothesisIndex][tokenIndex].lemma.c_str();
}

const wchar_t* RequestLabel(size_t hypothesisIndex, size_t tokenIndex)
{
    return currentDisambiguated[hypothesisIndex][tokenIndex].label.c_str();
}

double RequestWeight(size_t hypothesisIndex, size_t tokenIndex)
{
    return currentDisambiguated[hypothesisIndex][tokenIndex].weight;
}

int RequestLemmaId(size_t hypothesisIndex, size_t tokenIndex)
{
    return currentDisambiguated[hypothesisIndex][tokenIndex].lemmaId;
}

/* FUNCTIONS RELATED TO PARSING PYTHON INPUT */

vector<DisambiguatedData> parsedDisambiguated;

void PushParsedDisambiguated(const wchar_t* content, const wchar_t* lemma
    , const wchar_t* label, double weight, int lemmaId)
{
    DisambiguatedData toAdd;
    toAdd.content = content;
    toAdd.lemma = lemma;
    toAdd.label = label;
    toAdd.weight = weight;
    toAdd.lemmaId = lemmaId;
    parsedDisambiguated.emplace_back(std::move(toAdd));
}

void PushParsedDisambiguatedPunctuation(const wchar_t* punctuation)
{
    parsedDisambiguated.back().punctuation.push_back(punctuation);
}

void ResetParsedDisabiguated()
{
    parsedDisambiguated.clear();
}

}

}
