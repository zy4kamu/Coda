#include "Word.h"
#include "tagsetConstants.h"

using std::wstring;
using std::vector;
using std::unordered_map;
using std::unordered_set;

namespace Phoenix
{

Word::Word(const wstring& _lemma, const vector<wstring>& _tags, const wstring& _word)
{
	word = _word;
	lemma = _lemma;
	tags = SynTagRusTagInterpreter::GetInstance().ProcessTags(_tags);

	unordered_set<wstring> SynTagRusProblemWordforms;
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0423");//here was cyrrilic symbols: БУДУ
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0415\u041c");//here was cyrrilic symbols: БУДЕМ
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0415\u0428\u042c");//here was cyrrilic symbols: БУДЕШЬ
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0415\u0422\u0415");//here was cyrrilic symbols: БУДЕТЕ
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0415\u0422");//here was cyrrilic symbols: БУДЕТ
	SynTagRusProblemWordforms.insert(L"\u0411\u0423\u0414\u0423\u0422");//here was cyrrilic symbols: БУДУТ

	if(SynTagRusProblemWordforms.find(word) != SynTagRusProblemWordforms.end())
	{
		tags[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_FUTURE;
	}
}

wstring Word::GetLemma() const
{
	return lemma;
}

bool Word::DoesHaveFeature(const wstring& feature) const
{
	if(tags.find(feature) != tags.end())
	{
		return true;
	}
	return false;
}

wstring Word::GetFeature(const wstring& feature) const
{
	if(tags.find(feature) != tags.end())
	{
		return tags.at(feature);
	}
	else
	{
		throw ENoFeature();
	}
}

vector<wstring> Word::GetAvailableFeatures() const
{
	vector<wstring> toReturn;
	for(auto feat_p = tags.begin(); feat_p != tags.end(); feat_p++)
	{
		toReturn.push_back(feat_p->first);
	}

	return toReturn;
}

unordered_map<wstring, wstring> Word::GetAllFeatures() const
{
	return tags;
}

unordered_set<wstring> Word::GetAllFeaturesSet() const
{
	unordered_set<wstring> toReturn;
	
	for(auto tags_p = tags.begin(); tags_p != tags.end(); tags_p++)
	{
		toReturn.insert(tags_p->second);
	}

	return toReturn;
}

} //namespace Phoenix
