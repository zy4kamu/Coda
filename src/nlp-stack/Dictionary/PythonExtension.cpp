#include "PythonExtension.h"
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
vector<vector<Morphology>> getParadigmForLemmaReturnValue;
MorphologyWrapper* io_val = 0;

size_t GetGramInfo(const wchar_t* i_token, const char * i_language)
{
   getGramInfoReturnValue.clear();
   Tools::Language language = Tools::StringToLanguage(i_language);
   shared_ptr<DictionaryFacade> dic = DictionaryCreator::getDictionaryCreator().getDictionary(language);
   getGramInfoReturnValue = dic->getMorphologicalInformation(i_token);
   return getGramInfoReturnValue.size();
}

size_t SynthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures,
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

size_t GetParadigmForLemma(const wchar_t* i_lemma, const char* i_language)
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


void ClearLastReturnValue_()
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

MorphologyWrapper* WrapCurrentMorphologyValue(Morphology& morphology_value)
{
    if(io_val)
    {
        ClearLastReturnValue_();
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

const MorphologyWrapper* RequestGetGramInfoReturnValue(size_t i_index)
{
    return WrapCurrentMorphologyValue(getGramInfoReturnValue[i_index]);
}

const wchar_t* RequestSynthesizeTokenFromLemmaReturnValue(size_t i_index)
{
    return synthesizeTokenFromLemmaReturnValue[i_index].c_str();
}

const size_t RequestGetParadigmForLemmaSize(size_t i_index)
{
    return getParadigmForLemmaReturnValue[i_index].size();
}

const MorphologyWrapper* RequestGetParadigmForLemmaReturnValueMorphology(size_t i_paradigm_index, size_t i_element_index)
{
    Morphology& current = getParadigmForLemmaReturnValue[i_paradigm_index][i_element_index];

    return WrapCurrentMorphologyValue(current);
}

const wchar_t* RequestGetParadigmForLemmaReturnValueWordform(size_t i_paradigm_index, size_t i_element_index)
{
    Morphology& current = getParadigmForLemmaReturnValue[i_paradigm_index][i_element_index];

    return current.word->c_str();
}

void CleanGetGramInfoReturnValue()
{
    if(io_val)
    {
        ClearLastReturnValue_();
    }
    io_val = 0;
    getGramInfoReturnValue.clear();
}

void CleanSynthesizeTokenFromLemmaReturnValue()
{
    synthesizeTokenFromLemmaReturnValue.clear();
}

void CleanGetParadigmForLemmaReturnValue()
{
    if(io_val)
    {
        ClearLastReturnValue_();
    }
    io_val = 0;
    getParadigmForLemmaReturnValue.clear();
}


}

}
