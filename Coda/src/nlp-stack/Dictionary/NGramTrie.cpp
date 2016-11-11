/**
 * NGramTrie.cpp
 */

#include "NGramTrie.h"
#include "Dictionary.h"

NGramTrie::NGramTrie(Dictionary* _dic)
{
	dic = _dic;
	N = 3; // 3-gram
	//root = new NGramNode(0, 0);
	root = NULL;
	numberOfNodes = 1;
}

NGramTrie::~NGramTrie(void)
{
	if (root)
	{
		//delete root;
		root = NULL;
	}
	dic = NULL;
}

void NGramTrie::updateMorphologicalInfo(vector<MorphologicalInfo> _morphologicalInfoList)
{
	NGramNode* currentNode = root;
	for (int i = 0; i < (int) _morphologicalInfoList.size(); ++i)
	{
		MorphologicalInfo _info = _morphologicalInfoList.at(i);
		NGramNode* tempNode = currentNode->findChild(_info.basicFeatureListId, _info.featureListId);
		if (tempNode == NULL)
		{
			tempNode = new NGramNode(_info.basicFeatureListId, _info.featureListId);
			numberOfNodes++;
			currentNode->addChild(tempNode);
		}
		currentNode = tempNode;
		/*
		if (i == (int) _morphologicalInfoList.size() - 1)
		{
			currentNode->increaseNGramCount();
		}
		*/
		// get 1-gram and 2-gram
		currentNode->increaseNGramCount();
	}
}

int NGramTrie::getNGrammCount(vector<MorphologicalInfo> _morphologicalInfoList)
{
	NGramNode* currentNode = root;
	for (int i = 0; i < (int) _morphologicalInfoList.size(); ++i)
	{
		MorphologicalInfo _info = _morphologicalInfoList.at(i);
		NGramNode* tempNode = currentNode->findChild(_info.basicFeatureListId, _info.featureListId);
		if (tempNode == NULL)
		{
			return 0;
		}
		else
		{
			currentNode = tempNode;
		}
		if (i == (int) _morphologicalInfoList.size() - 1)
		{
			return currentNode->getNGramCount();
		}
	}
	return 0;
}

vector<MorphologicalInfo> NGramTrie::sentenceToMorphologicalInfo(wstring _sentence)
{
	vector<wstring> tokens = dic->getDictionaryTools()->dictionarySplit(_sentence, WORD_DELIMITER);
	vector<MorphologicalInfo> result = vector<MorphologicalInfo>();
	for (int i = 0; i < (int) tokens.size(); ++i)
	{
        shared_ptr<vector<MorphologicalInfo> > _morphologicalInfoVector = std::make_shared<vector<MorphologicalInfo> >();
        dic->getDictionaryTrie()->getMorphologicalInfoList(tokens.at(i), _morphologicalInfoVector);
		MorphologicalInfo _morphologicalInfo;
        if ((int) _morphologicalInfoVector->size() > 0)
		{
            _morphologicalInfo = _morphologicalInfoVector->at(0);
		}
		else
		{
			_morphologicalInfo.basicFeatureListId = 0;
			_morphologicalInfo.featureListId = 0;
		}
		result.push_back(_morphologicalInfo);
	}
	return result;
}

int NGramTrie::morphologicalNGramValue(vector<MorphologicalInfo> _morphologicalInfoList, bool extra)
{
	int result = 0;
	if ((int) _morphologicalInfoList.size() == 0)
	{
		return result;
	}
	int _begin = extra ? -N + 1 : 0;
	int _end = extra ? (int) _morphologicalInfoList.size() - 1 : (int) _morphologicalInfoList.size() - N;
	for (int i = _begin; i <= _end; ++i)
	{
		vector<MorphologicalInfo> tmp = vector<MorphologicalInfo>();
		for (int j = 0; j < N; ++j)
		{
			int k = i + j;
			if (k < 0 || k >= (int) _morphologicalInfoList.size())
			{
				MorphologicalInfo _info;
				_info.basicFeatureListId = 0;
				_info.featureListId = 0;
				tmp.push_back(_info);
			}
			else
			{
				tmp.push_back(_morphologicalInfoList.at(k));
			}
		}
		result += getNGrammCount(tmp);
	}
	return result;
}

void NGramTrie::testSentence(wstring _str)
{
	_str = dic->getDictionaryTools()->refineString(_str);
	wcout << "@TEST " << _str << endl;
	vector<wstring> tokens = dic->getDictionaryTools()->dictionarySplit(_str, WORD_DELIMITER);

	vector<MorphologicalInfo> _morphologicalInfoList = sentenceToMorphologicalInfo(_str);
	wcout << "TEST : nGramInfoList.size() = " << _morphologicalInfoList.size() << endl;
	if ((int) _morphologicalInfoList.size() == 0)
	{
		return;
	}
	for (int i = -N + 1; i < (int) _morphologicalInfoList.size(); ++i)
	{
		vector<MorphologicalInfo> tmp = vector<MorphologicalInfo>();
		for (int j = 0; j < N; ++j)
		{
			int k = i + j;
			if (k < 0 || k >= (int) _morphologicalInfoList.size())
			{
				MorphologicalInfo _info;
				_info.basicFeatureListId = 0;
				_info.featureListId = 0;
				tmp.push_back(_info);
			}
			else
			{
				tmp.push_back(_morphologicalInfoList.at(k));
			}
		}

		int _count = getNGrammCount(tmp);
		//wcout << "i = " << i << endl;
		for (int j = 0; j < N; ++j)
		{
			int k = i + j;
			if (k >= 0 && k < (int) tokens.size())
			{
				//wcout << tokens.at(k) << L" ";
				wcout << tokens.at(k) << "(" << tmp.at(j).basicFeatureListId << ", " << tmp.at(j).featureListId << ")" <<" ";
			}
		}
		wcout << "= " << _count << endl;
	}
}

