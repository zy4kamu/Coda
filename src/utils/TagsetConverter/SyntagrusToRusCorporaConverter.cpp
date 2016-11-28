#include "SyntagrusToRusCorporaConverter.h"


using namespace TagsetConverting;

SyntagrusToRusCorporaConverter::SyntagrusToRusCorporaConverter(const string& file)
{
	FileManager::Read(file, &syntagTagToRusCorpTag, L',');
}


SyntagrusToRusCorporaConverter::~SyntagrusToRusCorporaConverter(void)
{
}

wstring SyntagrusToRusCorporaConverter::Convert(const wstring& syntagrusTag) const
{
	vector<wstring> splitted = Tools::Split(
        syntagrusTag, L"@");
	int size = splitted.size();
	if (size == 0) return L"";
	wstring retval = syntagTagToRusCorpTag.at(splitted[0]);
	for (int i = 1; i < size; ++i)
		retval += L" - " + syntagTagToRusCorpTag.at(splitted[i]);
	return retval;
}
