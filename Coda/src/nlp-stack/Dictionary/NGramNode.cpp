/**
 * NGramNode.cpp
 */

#include "NGramNode.h"

NGramNode::NGramNode(void)
{
	children = vector<NGramNode*>();
	NGramCount = 0;
}

NGramNode::NGramNode(int _basicFeatureListId, int _featureListId)
{
	setFeatureListId(_basicFeatureListId, _featureListId);
	children = vector<NGramNode*>();
	NGramCount = 0;
}

NGramNode::~NGramNode(void)
{
	/*
	for (int i = 0; i < children.size(); ++i)
	{
		NGramNode* childNode = children.at(i);
		if (childNode)
		{
			delete childNode;
		}
	}
	*/
	children.clear();
}

NGramNode* NGramNode::findChild(int _basicFeatureListId, int _featureListId)
{
	for (int i = 0; i < (int) children.size(); ++i)
	{
		NGramNode* tmp = children.at(i);
		if (tmp->getBasicFeatureListId() == _basicFeatureListId && tmp->getFeatureListId() == _featureListId)
		{
			return tmp;
		}
	}
	return NULL;
}

