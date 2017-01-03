/**
 * SuffixModelNode.cpp
 */

#include "SuffixModelNode.h"

SuffixModelNode::SuffixModelNode(wchar_t _character)
{
	character = _character;
	featureFrequencyMap = map<int, int>();
	childrenNodes = vector<SuffixModelNode*>();
}

SuffixModelNode::~SuffixModelNode(void)
{
	/*
	for (int i = 0; i < (int) childrenNodes.size(); ++i)
	{
		SuffixModelNode* childNode = childrenNodes.at(i);
		if (childNode)
		{
			delete childNode;
		}
	}
	*/
	childrenNodes.clear();
	featureFrequencyMap.clear();
}

void SuffixModelNode::updateFeature(int _basicFeatureListId, int _featureListId)
{
	int _featureId = 1000 * _basicFeatureListId + _featureListId;
	if (featureFrequencyMap.count(_featureId) == 0)
	{
		featureFrequencyMap.insert(pair<int, int>(_featureId, 1));
	}
	else
	{
		featureFrequencyMap.at(_featureId)++;
	}
}

SuffixModelNode* SuffixModelNode::findChildNode(wchar_t _character)
{
	for (int i = 0; i < (int) childrenNodes.size(); ++i)
	{
		if (childrenNodes.at(i)->getCharacter() == _character)
		{
			return childrenNodes.at(i);
		}
	}
	return NULL;
}

