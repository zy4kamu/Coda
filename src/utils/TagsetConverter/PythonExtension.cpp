#include "PythonExtension.h"
#include "MorphologicalDictionary.h"

extern "C"
{

namespace TagsetConverting
{

vector<wstring> currentVariants;
std::shared_ptr<OpenCorporaToSynagrusConverter> converter;
map<wstring, wstring> ocMap;

void createConverter()
{
    converter = std::make_shared<OpenCorporaToSynagrusConverter>(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Translation of OpenCorpora properties to Synatgrus.txt");
    ocMap = make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_RUSSIAN)->getShortToLongMap();
}

void removeConverter()
{
    converter.reset();
}

size_t convert(const wchar_t ** i_markers, size_t i_numberOfFeatures)
{
    wstring tag;
    for(size_t i = 0; i < i_numberOfFeatures; i++)
    {
        if(ocMap.find(i_markers[i]) != ocMap.end())
        {
            tag += ocMap[i_markers[i]];
            tag += L" - ";
        }
    }
    tag = tag.erase(tag.size() - 3);
    currentVariants = converter->PartialConvert(tag);
    return currentVariants.size();
}

const wchar_t* requestConvertReturnValue(size_t i_index)
{
    return currentVariants[i_index].c_str();
}

void clearConvertReturnValue()
{
    currentVariants.clear();
}

}

}
