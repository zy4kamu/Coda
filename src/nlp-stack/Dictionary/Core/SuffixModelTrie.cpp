/**
 * SuffixModelTrie.cpp
 */

#include "SuffixModelTrie.h"

SuffixModelTrie::SuffixModelTrie(Dictionary* _dic)
{
	//root = new SuffixModelNode(L' ');
	root = NULL;
	numberOfNodes = 1;
	dic = _dic;
	featureIdFrequency = map<int, int>();
}

SuffixModelTrie::~SuffixModelTrie(void)
{
	if (root)
	{
		//delete root;
		root = NULL;
	}
	featureIdFrequency.clear();
	dic = NULL;
}

void SuffixModelTrie::updateFeatureId(int _basicFeatureListId, int _featureListId)
{
	int _featureId = _basicFeatureListId * 1000 + _featureListId;
	if (!featureIdFrequency.count(_featureId))
	{
		featureIdFrequency.insert(pair<int, int>(_featureId, 1));
	}
	else
	{
		featureIdFrequency.at(_featureId)++;
	}
}

int SuffixModelTrie::getFeatureFrequency(int _basicFeatureListId, int _featureListId)
{
	int _featureId = _basicFeatureListId * 1000 + _featureListId;
	if (!featureIdFrequency.count(_featureId))
	{
		return 0;
	}
	else
	{
		return featureIdFrequency.at(_featureId);
	}
}

void SuffixModelTrie::updateSuffix(wstring _suffix, vector<MorphologicalInfo> minfos)
{
	SuffixModelNode* currentNode = root;
	for (int i = (int) _suffix.length() - 1; i >= 0; --i)
	{
		SuffixModelNode* tmpNode = currentNode->findChildNode(_suffix.at(i));
		if (tmpNode == NULL)
		{
			tmpNode = new SuffixModelNode(_suffix.at(i));
			numberOfNodes++;
			currentNode->addChildNode(tmpNode);
		}
		for (int j = 0; j < (int) minfos.size(); ++j)
		{
			tmpNode->updateFeature(minfos.at(j).basicFeatureListId, minfos.at(j).featureListId);
			updateFeatureId(minfos.at(j).basicFeatureListId, minfos.at(j).featureListId);
		}
		currentNode = tmpNode;
	}
}

/**
 * add a suffix with _basicFeatureListId and _featureListid to trie
 */
void SuffixModelTrie::updateSuffix(wstring _suffix, int _basicFeatureListId, int _featureListId)
{
	SuffixModelNode* currentNode = root;
	for (int i = (int) _suffix.length() - 1; i >= 0; --i)
	{
		SuffixModelNode* tmpNode = currentNode->findChildNode(_suffix.at(i));
		if (tmpNode == NULL)
		{
			tmpNode = new SuffixModelNode(_suffix.at(i));
			numberOfNodes++;
			currentNode->addChildNode(tmpNode);
		}
		tmpNode->updateFeature(_basicFeatureListId, _featureListId);
		updateFeatureId(_basicFeatureListId, _featureListId);
		currentNode = tmpNode;
	}
}

/**
 * predict MorphologicalInfo by suffix
 */
vector<MorphologicalInfo> SuffixModelTrie::getMorphologicalPredictionBySuffix(wstring _word)
{
	vector<MorphologicalInfo> result = vector<MorphologicalInfo>();
	SuffixModelNode* currentNode = root;
	int suffixLength = 0;
	for (int i = (int) _word.length() - 1; i >= 0; --i)
	{
		SuffixModelNode* tmpNode = currentNode->findChildNode(_word.at(i));
		if (tmpNode == NULL)
		{
			break;
		}
		currentNode = tmpNode;
		suffixLength++;
        //wcout << _word.at(i) << " : " << currentNode->getFeatureFrequencyMap().size() << endl;
	}
	if (suffixLength == 0)
	{
		return result;
	}
    //wcout << "Suffix length = " << suffixLength << endl;
	map<int, int> _featureFrequencyMap = currentNode->getFeatureFrequencyMap();
    //wcout << "_featureFrequencyMap's size = " << _featureFrequencyMap.size() << endl;
	map<int, int>::iterator iter;
    //@TODO : \u043f\u0435\u0440\u0440\u0441\u0441\u043e\u043d//here was cyrrilic symbols: перрссон
	for (iter = _featureFrequencyMap.begin(); iter != _featureFrequencyMap.end(); ++iter)
	{
		int _featureId = iter->first;
		int _frequency = iter->second;
		int _basicFeatureListId = _featureId / 1000;
		int _featureListId = _featureId % 1000;
		wstring _initial_form = suffixLength < (int) _word.length() ? L"-" + _word.substr(_word.length() - suffixLength) : _word;
		MorphologicalInfo _morphologicalInfo;
		_morphologicalInfo.basicFeatureListId = _basicFeatureListId;
		_morphologicalInfo.featureListId = _featureListId;
		_morphologicalInfo.frequency = _frequency;
		_morphologicalInfo.initial_form = _initial_form;
		_morphologicalInfo.lemmaId = 0;
		_morphologicalInfo.suffix_length = suffixLength;
		result.push_back(_morphologicalInfo);
	}
	return result;
}

