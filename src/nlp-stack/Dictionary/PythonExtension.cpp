#include "PythonExtension.h"
#include "DictionaryCreator.h"
#include "Tools.h"

extern "C"
{

namespace DictionaryOps
{

void createDictionary(const char * i_language)
{
    Tools::Language language = Tools::StringToLanguage(i_language);
    DictionaryCreator::getDictionaryCreator().getDictionary(language);
}

vector<Morphology> getGramInfoReturnValue;
vector<wstring> synthesizeTokenFromLemmaReturnValue;
MorphologyWrapper* io_val = 0;

size_t getGramInfo(const wchar_t* i_token, const char * i_language)
{
   getGramInfoReturnValue.clear();
   Tools::Language language = Tools::StringToLanguage(i_language);
   shared_ptr<DictionaryFacade> dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
   getGramInfoReturnValue = dic->getMorphologicalInformation(L"Россия");
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

void clearLastReturnValue_()
{
    int nFeatures = io_val->nFeatures;
    for(int i = 0; i < nFeatures; i++)
    {
        delete[] io_val->features[i];
    }
    delete[] io_val->features;
    delete io_val;
    io_val = 0;
}

const MorphologyWrapper* requestGetGramInfoReturnValue(size_t i_index)
{
    if(io_val)
    {
        clearLastReturnValue_();
    }
    io_val = new MorphologyWrapper;
    io_val->nFeatures = getGramInfoReturnValue[i_index].features.size();
    wcscpy(io_val->lemma, getGramInfoReturnValue[i_index].lemma->c_str());
    io_val->features = new wchar_t* [io_val->nFeatures];
    for(int i = 0; i < io_val->nFeatures; i++)
    {
        io_val->features[i] = new wchar_t[16];
        wcscpy(io_val->features[i], getGramInfoReturnValue[i_index].features[i]->c_str());
    }
    return io_val;
}

const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index)
{
    return synthesizeTokenFromLemmaReturnValue[i_index].c_str();
}

void cleanGetGramInfoReturnValue()
{
    if(io_val)
    {
        clearLastReturnValue_();
    }
    io_val = 0;
    getGramInfoReturnValue.clear();
}

void cleanSynthesizeTokenFromLemmaReturnValue()
{
    synthesizeTokenFromLemmaReturnValue.clear();
}

}

}
