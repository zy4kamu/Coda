#include "DictionaryCreator.h"
#include "Tools.h"

extern "C"
{

namespace DictionaryOps
{

void CreateDictionary(const char * i_language)
{
    Tools::Language language = Tools::StringToLanguage(i_language);
    DictionaryCreator::getDictionaryCreator().getDictionary(language);
}

vector<Morphology> getGramInfoReturnValue;
vector<wstring> synthesizeTokenFromLemmaReturnValue;

size_t getGramInfo(const wchar_t* i_token, const char* i_language)
{
   getGramInfoReturnValue.clear();
   Tools::Language language = Tools::StringToLanguage(i_language);
   auto dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
   getGramInfoReturnValue = dic->getMorphologicalInformation(i_token);
   return getGramInfoReturnValue.size();
}

size_t synthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures,
                                size_t i_numberOfFeatures, const char* i_language)
{
    std::vector<wstring> features;
    for(int i = 0; i < i_numberOfFeatures; i++)
    {
        features.emplace_back(i_grammarFeatures[i]);
    }
    Tools::Language language = Tools::StringToLanguage(i_language);
    auto dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
    synthesizeTokenFromLemmaReturnValue = dic->synthesizeWordform(i_lemma, features);
    return synthesizeTokenFromLemmaReturnValue.size();
}

const wchar_t* requestGetGramInfoReturnValue(size_t i_index)
{
    //return currentTokens[index].content.c_str();
}

const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index)
{
   // return currentTokens[index].punctuation.size();
}

}

}
