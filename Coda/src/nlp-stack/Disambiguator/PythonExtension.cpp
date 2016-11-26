#include "Disambiguator.h"
#include "Tokenizer/PythonExtension.h"

extern "C"
{

namespace Disambiguation
{

vector<DisambiguatedData> currentDisambiguated;

void CreateDisambiguator(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
}

void Disambiguate(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(language);
    currentDisambiguated = disambiguator->Disambiguate(
        Tokenization::parsedTokens);
}

const wchar_t* RequestLemma(size_t index)
{
    return currentDisambiguated[index].lemma.c_str();
}

const wchar_t* RequestLabel(size_t index)
{
    return currentDisambiguated[index].label.c_str();
}

double RequestWeight(size_t index)
{
    return currentDisambiguated[index].weight;
}

int RequestLemmaId(size_t index)
{
    return currentDisambiguated[index].lemmaId;
}


}

}
