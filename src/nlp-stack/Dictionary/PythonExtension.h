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
void createDictionary(const char*  i_language);
size_t getGramInfo(const wchar_t* i_token, const char * i_language);
size_t synthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures, const char* i_language);
size_t getParadigmForLemma(const wchar_t* i_lemma, const char* i_language);


/* GETTING RESULTS FOR PYTHON */
const MorphologyWrapper* requestGetGramInfoReturnValue(size_t i_index);
const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index);
const size_t requestGetParadigmForLemmaSize(size_t i_index);
const MorphologyWrapper* requestGetParadigmForLemmaReturnValueMorphology(size_t i_paradigm_index, size_t i_element_index);
const wchar_t* requestGetParadigmForLemmaReturnValueWordform(size_t i_paradigm_index, size_t i_element_index);

/* CLEAR MEMORY */
void cleanGetGramInfoReturnValue();
void cleanSynthesizeTokenFromLemmaReturnValue();
void cleanGetParadigmForLemmaReturnValue();
}

}
