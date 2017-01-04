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

extern vector<Morphology> getGramInfoReturnValue;
extern vector<wstring> synthesizeTokenFromLemmaReturnValue;

/* DICTIONARY INTERFACE FUNCTIONS */
void CreateDictionary(const char*  i_language);
size_t getGramInfo(const wchar_t* i_token, const wchar_t * i_language);
size_t synthesizeTokenFromLemma(const wchar_t* i_token, const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures);

/* GETTING RESULTS FROM PYTHON */
const wchar_t* requestGetGramInfoReturnValue(size_t i_index);
const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index);

/* CLEAR MEMORY */
void cleanGetGramInfoReturnValue();
void cleanSynthesizeTokenFromLemmaReturnValue();
}

}
