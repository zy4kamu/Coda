#include <string>
#include <vector>
#include "DictionaryInterface.h"
#include "MorphologicalDictionary.h"
#include "Tools.h"

using std::wstring;
using std::vector;

extern "C"
{
namespace DictionaryOps
{
/* VARIABLES TO STORE TEMPORARY RESULTS */
typedef struct
{
  wchar_t lemma[80];
  int nFeatures;
  wchar_t **features;
} MorphologyWrapper;

extern vector<Morphology> getGramInfoReturnValue;
extern vector<wstring> synthesizeTokenFromLemmaReturnValue;
extern vector<vector<Morphology>> getParadigmForLemmaReturnValue;
extern MorphologyWrapper* io_val;

/* DICTIONARY INTERFACE FUNCTIONS */
void CreateDictionary(const char*  i_language);
size_t GetGramInfo(const wchar_t* i_token, const char * i_language);
size_t SynthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures, const char* i_language);
size_t GetParadigmForLemma(const wchar_t* i_lemma, const char* i_language);


/* GETTING RESULTS FOR PYTHON */
const MorphologyWrapper* RequestGetGramInfoReturnValue(size_t i_index);
const wchar_t* RequestSynthesizeTokenFromLemmaReturnValue(size_t i_index);
const size_t RequestGetParadigmForLemmaSize(size_t i_index);
const MorphologyWrapper* RequestGetParadigmForLemmaReturnValueMorphology(size_t i_paradigm_index, size_t i_element_index);
const wchar_t* RequestGetParadigmForLemmaReturnValueWordform(size_t i_paradigm_index, size_t i_element_index);

/* CLEAR MEMORY */
void CleanGetGramInfoReturnValue();
void CleanSynthesizeTokenFromLemmaReturnValue();
void CleanGetParadigmForLemmaReturnValue();
}

}
