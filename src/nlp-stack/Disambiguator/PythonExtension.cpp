#include "Disambiguator.h"
#include "Tokenizer/PythonExtension.h"

extern "C"
{

namespace Disambiguation
{

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
    currentHypothesisDistribution.clear();
    currentDisambiguated = disambiguator->Disambiguate(
        Tokenization::parsedTokens, numberOfHypothesis, &currentHypothesisDistribution);
}

void DisambiguateCoverage(const char* languagePtr, double coverage, size_t maxNumberOfPaths)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
    currentHypothesisDistribution.clear();
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

}

}
