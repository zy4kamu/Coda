/**
 * DictionaryTrieIterator.cpp
 */

#include "DictionaryTrieIterator.h"
#include <stack>

/**
 * Constructor of DictionaryTrieIterator
 */
DictionaryTrieIterator::DictionaryTrieIterator(Dictionary* _dic, string dictionaryFile) : DictionaryTrieBinaryFileReader(_dic, dictionaryFile)
{
	setLemmaOnly(true);
	result = vector<wstring>();
}

/**
 * Destructor of DictionaryTrieIterator
 */
DictionaryTrieIterator::~DictionaryTrieIterator(void) 
{
	dic = NULL;
}

/**
 * Iterate
 */
void DictionaryTrieIterator::iterate(void) 
{
	result = vector<wstring>();
	stack<DictionaryNode*> nodeStack = stack<DictionaryNode*>();
	vector<DictionaryNode*> nodeChildren = root->getChildren();
	for (int i = (int) nodeChildren.size() - 1; i >= 0; --i)
	{
		nodeStack.push(nodeChildren.at(i));
	}

	while (!nodeStack.empty())
	{
		DictionaryNode* current = nodeStack.top();
		nodeStack.pop();

		// process a DictionaryTrieNode
		wstring _prefix = current->getStringFromRoot();
		//wcout << "---------------" << endl << _prefix << endl;

        vector<DictionaryNodeModel*> * _nodeModels = current->getNodeModels();
        for (int i_model = 0; i_model < (int) _nodeModels->size(); ++i_model)
		{
            DictionaryNodeModel* _nodeModel = _nodeModels->at(i_model);
			// get modelId of NodeModel
			int _trieModelId = _nodeModel->getModelId();

			// get TrieModel by _trieModelId
			DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
			// continue if TrieModel not found
			if (!_trieModel) 
			{
				continue;
			}
			// get list of TrieModelElement of TrieModel
            vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
			// continue if _trieModelElements is empty
            if (_trieModelElements->empty())
			{
				continue;
			}

            int _size = lemmaOnly ? 1 : (int) _trieModelElements->size();

			for (int i_model_element = 0 ; i_model_element < _size; ++i_model_element) 
			{
                DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i_model_element);
				// get suffix of TrieModelElement
                wstring _suffix = *(_trieModelElement->getSuffix());
				// concat prefix and suffix
				wstring _word = _prefix + _suffix;
				if (_trieModelElement->isBeginWithPo()) 
				{
					_word += L"\u043f\u043e";//here was cyrrilic symbols: по
				}
				//wcout << _word << endl;
				result.push_back(_word);
			}
		}

		vector<DictionaryNode*> nodeChildren = current->getChildren();
		for (int i = (int) nodeChildren.size() - 1; i >= 0; --i)
		{
			nodeStack.push(nodeChildren.at(i));
		}
	}
}

