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
vector<vector<Morphology>> getParadigmForLemmaReturnValue;
MorphologyWrapper* io_val = 0;

size_t getGramInfo(const wchar_t* i_token, const char * i_language)
{
   getGramInfoReturnValue.clear();
   Tools::Language language = Tools::StringToLanguage(i_language);
   shared_ptr<DictionaryFacade> dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
   getGramInfoReturnValue = dic->getMorphologicalInformation(i_token);
   return getGramInfoReturnValue.size();
}

size_t synthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures,
                                size_t i_numberOfFeatures, const char* i_language)
{
    std::vector<wstring> features;
    for(size_t i = 0; i < i_numberOfFeatures; i++)
    {
        features.emplace_back(i_grammarFeatures[i]);
    }
    Tools::Language language = Tools::StringToLanguage(i_language);
    auto dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
    synthesizeTokenFromLemmaReturnValue = dic->synthesizeWordform(i_lemma, features);
    return synthesizeTokenFromLemmaReturnValue.size();
}

size_t getParadigmForLemma(const wchar_t* i_lemma, const char* i_language)
{
     std::vector<wstring> features;
     Tools::Language language = Tools::StringToLanguage(i_language);
     // currently implemented only for Russian
     if(language != Tools::RU) {
         return 0;
     }
     auto dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
     getParadigmForLemmaReturnValue = dic->getParadigmForLemma(i_lemma);
     return getParadigmForLemmaReturnValue.size();
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

MorphologyWrapper* wrapCurrentMorphologyValue(Morphology& morphology_value)
{
    if(io_val)
    {
        clearLastReturnValue_();
    }
    io_val = new MorphologyWrapper;
    io_val->nFeatures = morphology_value.features.size();
    wcscpy(io_val->lemma,  morphology_value.lemma->c_str());
    io_val->features = new wchar_t* [io_val->nFeatures];
    for(int i = 0; i < io_val->nFeatures; i++)
    {
        io_val->features[i] = new wchar_t[morphology_value.features[i]->length()+1];
        wcscpy(io_val->features[i],  morphology_value.features[i]->c_str());
    }
    return io_val;
}

const MorphologyWrapper* requestGetGramInfoReturnValue(size_t i_index)
{
    return wrapCurrentMorphologyValue(getGramInfoReturnValue[i_index]);
}

const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index)
{
    return synthesizeTokenFromLemmaReturnValue[i_index].c_str();
}

const size_t requestGetParadigmForLemmaSize(size_t i_index)
{
    return getParadigmForLemmaReturnValue[i_index].size();
}

const MorphologyWrapper* requestGetParadigmForLemmaReturnValueMorphology(size_t i_paradigm_index, size_t i_element_index)
{
    Morphology& current = getParadigmForLemmaReturnValue[i_paradigm_index][i_element_index];

    return wrapCurrentMorphologyValue(current);
}

const wchar_t* requestGetParadigmForLemmaReturnValueWordform(size_t i_paradigm_index, size_t i_element_index)
{
    Morphology& current = getParadigmForLemmaReturnValue[i_paradigm_index][i_element_index];

    return current.word->c_str();
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

void cleanGetParadigmForLemmaReturnValue()
{
    if(io_val)
    {
        clearLastReturnValue_();
    }
    io_val = 0;
    getParadigmForLemmaReturnValue.clear();
}


}

}
