/**
 * DictionaryTrieModel.cpp
 */

#include "DictionaryTrieModel.h"

/**
 * Constructor of DictionaryTrieModel
 */
DictionaryTrieModel::DictionaryTrieModel(void)
{
	trieModelElements = vector<DictionaryTrieModelElement*>();
}

/**
 * Destructor of DictionaryTrieModel
 */
DictionaryTrieModel::~DictionaryTrieModel(void)
{
	for (int i = 0; i < (int) trieModelElements.size(); ++i)
	{
		DictionaryTrieModelElement* trieModelElement = trieModelElements.at(i);
		if (trieModelElement)
		{
			delete trieModelElement;
		}
	}
	trieModelElements.clear();
}

/**
 * Get first TrieModelElement
 */
DictionaryTrieModelElement* DictionaryTrieModel::getFirstDictionaryTrieModelElement()
{
	if (trieModelElements.size() == 0)
	{
		return NULL;
	}
	return trieModelElements.at(0);
}

