/**
 * DictionaryNode.cpp
 */

#include "DictionaryNode.h"

/**
 * Contructor of DictionaryNodeModel
 */
DictionaryNode::DictionaryNode(void) 
{
	parentNode = NULL;
	character = L' ';
	nodeModels = vector<DictionaryNodeModel*>();
}

/**
 * Destructor of DictionaryNode
 */
DictionaryNode::~DictionaryNode(void) 
{
	/*
	for (int i = 0; i < (int) childrenNodes.size(); ++i)
	{
		DictionaryNode* childNode = childrenNodes.at(i);
		if (childNode)
		{
			delete childNode;
		}
	}
	*/
	childrenNodes.clear();
	/*
	for (int i = 0; i < (int) nodeModels.size(); ++i)
	{
		DictionaryNodeModel* nodeModel = nodeModels.at(i);
		if (nodeModel)
		{
			delete nodeModel;
		}
	}
	*/
	nodeModels.clear();
	parentNode = NULL;
}

/**
 * Find chile node by character
 */
DictionaryNode* DictionaryNode::findChild(wchar_t _character)
{
	// for each childNode
    for (int i = 0; i < (int) childrenNodes.size(); i++ ) {
        DictionaryNode* tmp = childrenNodes.at(i);
		// if this child node have _character then return
        if ( tmp->getCharacter() == _character ) {
            return tmp;
        }
    }
	// if not found return NULL
    return NULL;
}

/**
 * get string from root to this node
 */
wstring DictionaryNode::getStringFromRoot(void)
{
	// current node is this node
	DictionaryNode* current = this;
	wstring _str(L"");
	while (true) {
		// stop if root is reached
		if (current == NULL) 
		{
			break;
		}
		wchar_t _ch = current->getCharacter();
		if (_ch == L' ')
		{
			break;
		}
		_str.push_back(_ch);
		// jump to parent node
		current = current->getParentNode();
	}
	// initialize an empty string
	wstring result(L"");
	int _len = _str.length();
	for (int i = _len - 1; i >= 0; --i) {
		result.push_back(_str.back());
		_str.pop_back();
	}
	return result;
}

