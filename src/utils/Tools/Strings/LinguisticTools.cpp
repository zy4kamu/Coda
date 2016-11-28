#include "LinguisticTools.h"

#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::unordered_set;
using std::wstring;
using std::vector;

LinguisticTools::LinguisticTools(void)
{
	punctuation.insert(L',');
	punctuation.insert(L'.');
	punctuation.insert(L'-');
	punctuation.insert(L'!');
	punctuation.insert(L'?');
	punctuation.insert(L'\"');
}

vector<wstring> LinguisticTools::tokenize(const wstring& input)
{	
	vector<wstring> raw = split(input, L" ");
	vector<wstring> toReturn;

	for(auto p_token = raw.begin(); p_token != raw.end(); p_token++)
	{
		wstring raw_token = *(p_token);
		if(punctuation.find(raw_token[raw_token.length() - 1]) != punctuation.end())
		{
			toReturn.push_back(raw_token.substr(0, raw_token.size() - 1));
			if(raw_token.length() > 1)
			{
				toReturn.push_back(raw_token.substr(raw_token.size() - 1, 1));
			}
		}
		else
		{
			toReturn.push_back(raw_token);
		}
	}

	return toReturn;
}

vector<wstring> LinguisticTools::split(wstring line, wstring delim)
{
	vector<wstring> res(0);
	size_t current;
	size_t next = -1;
	do
	{
	  current = next + 1;
	  next = line.find_first_of( delim, current );
	  res.push_back(line.substr( current, next - current ));
	}
	while (next != wstring::npos);
	return res;
}

LinguisticTools::~LinguisticTools(void)
{
}
