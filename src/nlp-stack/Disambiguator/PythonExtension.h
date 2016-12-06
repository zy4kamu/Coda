#include "Disambiguator.h"

extern "C"
{

namespace Disambiguation
{

/* FUNCTIONS RELATED TO DISAMBIGUATION */

void CreateDisambiguator(const char* languagePtr);
void Disambiguate(const char* languagePtr);
void DisambiguateHypothesis(const char* languagePtr, size_t numberOfHypothesis);
void DisambiguateCoverage(const char* languagePtr, double coverage, size_t maxNumberOfPaths);
size_t RequestNumberOfHypothesis();
double RequestHypothesisProbability(size_t hypothesisIndex);
const wchar_t* RequestLemma(size_t hypothesisIndex, size_t tokenIndex);
const wchar_t* RequestLabel(size_t hypothesisIndex, size_t tokenIndex);
double RequestWeight(size_t hypothesisIndex, size_t tokenIndex);
int RequestLemmaId(size_t hypothesisIndex, size_t tokenIndex);

/* FUNCTIONS RELATED TO PARSING PYTHON INPUT */

extern vector<DisambiguatedData> parsedDisambiguated;

void PushParsedDisambiguated(const wchar_t* content
    , const wchar_t* lemma, const wchar_t* label
    , double weight, int lemmaId);
void PushParsedDisambiguatedPunctuation(const wchar_t* punctuation);
void ResetParsedDisabiguated();

}

}
