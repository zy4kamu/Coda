#include <string>
#include <vector>
#include <memory>
#include <map>
#include "OpenCorporaToSynagrusConverter.h"

using std::vector;
using std::wstring;
using std::shared_ptr;
using std::map;

extern "C"
{

namespace TagsetConverting
{

/* MODULE SCOPE VARIABLES */
extern vector<wstring> currentVariants;
extern shared_ptr<OpenCorporaToSynagrusConverter> converter;
extern map<wstring, wstring> ocMap;

/* FUNCTIONS RELATED TO CONVERSION */
void createConverter();
size_t convert(const wchar_t ** i_markers, size_t i_numberOfFeatures);
void removeConverter();

/* GETTING RESULTS FOR PYTHON */
const wchar_t* requestConvertReturnValue(size_t i_index);

/* CLEAR MEMORY */
void clearConvertReturnValue();
}

}
