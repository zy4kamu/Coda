/**
 * DictionaryNodeModel.cpp
 */

#include "DictionaryNodeModel.h"
#include "DictionaryNode.h"

/**
 * Constructor of DictionaryNodeModel
 */
DictionaryNodeModel::DictionaryNodeModel(int _modelId, int _lemmaId)
{
	setModelId(_modelId);
	setLemmaId(_lemmaId);
	linkNode = NULL;
	linkFrom = vector<DictionaryNodeModel*>();
	node = NULL;
}

/**
 * Destructor of DictionaryNodeModel
 */
DictionaryNodeModel::~DictionaryNodeModel(void)
{
	linkNode = NULL;
	linkFrom.clear();
	node = NULL;
}

/**
 * get string from root to this DictionaryNodeModel's node
 */
wstring DictionaryNodeModel::getStringFromRoot(void)
{
	if (getNode() == NULL) {
		return L"";
	}
	return getNode()->getStringFromRoot();
}

