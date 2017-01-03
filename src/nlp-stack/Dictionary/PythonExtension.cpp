#include "DictionaryCreator.h"
#include "Tools.h"

extern "C"
{

namespace DictionaryOps
{

void CreateDictionary(const char* i_language)
{
    Tools::Language language = Tools::StringToLanguage(i_language);
    DictionaryCreator::getDictionaryCreator().getDictionary(language);
}

vector<GrammInfo> getGramInfoReturnValue;
vector<wstring> synthesizeTokenFromLemmaReturnValue;

size_t getGramInfo(const wchar_t* i_token)
{
    return 0;
}

size_t synthesizeTokenFromLemma(const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures)
{
    return 0;
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
