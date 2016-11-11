/**
 * DictionaryTrieWordFormByFeatures.cpp
 */

#include "DictionaryTrieWordFormByFeatures.h"
#include "Dictionary.h"

/**
 * Constructor of DictionaryTrieWordFormByFeatures
 */
DictionaryTrieWordFormByFeatures::DictionaryTrieWordFormByFeatures(Dictionary* _dic, string dictionaryFile) : DictionaryTrieBinaryFileReader(_dic, dictionaryFile)
{
}

/**
 * Destructor of DictionaryTrieWordFormByFeatures
 */
DictionaryTrieWordFormByFeatures::~DictionaryTrieWordFormByFeatures(void)
{
}

vector<WordForm> DictionaryTrieWordFormByFeatures::getWordFormByFeatures(wstring _word, vector<wstring> features, bool includeLinkedLemmas, bool _beginWithPo)
{
	vector<WordForm> _result = vector<WordForm>();
    DictionaryNode* current = root;
	// _beginWithPo = true    : begin from 1-st character (_word[2])
	// _beginWithPo = false   : begin from 3-st character (_word[0])
	int i_word = _beginWithPo ? 1 : -1;
	int w_len = (int) _word.length();
	wstring _prefix = L"";
	// init dictionaryNodeModels
    set<DictionaryNodeModel*> dictionaryNodeModels = set<DictionaryNodeModel*>();
	while (i_word < w_len)
    {
		// get list of NodeModel of current node
        vector<DictionaryNodeModel*> * _nodeModels = current->getNodeModels();
		// for each NodeModel
        for (int i_model = 0; i_model < (int) _nodeModels->size(); ++i_model) {
            DictionaryNodeModel* _nodeModel = _nodeModels->at(i_model);
			// get modelId of NodeModel
			int _trieModelId = _nodeModel->getModelId();
			// get TrieModel by _trieModelId
			DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
			// continue if TrieModel not found
			if (!_trieModel) {
				continue;
			}
			// get list of TrieModelElement of TrieModel
            vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
			// continue if _trieModelElements is empty
            if (_trieModelElements->empty()) {
				continue;
			}
			// for each TrieModelElement
            for (int i_model_element = 0; i_model_element < (int) _trieModelElements->size(); ++i_model_element) {
                DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i_model_element);
				// continue if PO-attr of TrieModelElement not match with _beginWithPo
				if (_trieModelElement->isBeginWithPo() != _beginWithPo) {
					continue;
				}
				// get suffix of TrieModelElement
                wstring _suffix = *(_trieModelElement->getSuffix());
				// concat prefix and suffix
				wstring _prefix_suffix = _prefix + _suffix;
				// if match with _word
				if (_word.compare(_prefix_suffix) == 0) {
					dictionaryNodeModels.insert(_nodeModel);
					if (includeLinkedLemmas)
					{
						vector<DictionaryNodeModel*> linkFrom = _nodeModel->getLinkFrom();
						for (int i = 0; i < (int) linkFrom.size(); ++i)
						{
							dictionaryNodeModels.insert(linkFrom.at(i));
						}
					}
				}
			}
		}
		// increase i_word
		i_word++;
		// break if all character are processed
        if (i_word >= w_len) {
			break;
		}
		// update prefix
		_prefix = _word.substr(0, i_word + 1);
		// find child node by next character
        DictionaryNode* tmp = current->findChild(_word[i_word]);
		// break if child node not found
        if ( tmp == NULL ) {
            break;
		}
		// set current node to child node
        current = tmp;
	}
	set<DictionaryNodeModel*>::iterator iter;
	for (iter = dictionaryNodeModels.begin(); iter != dictionaryNodeModels.end(); ++iter)
	{
		DictionaryNodeModel* dicNodeModel = *iter;
		//wcout << dicNodeModel->getLemmaId() << endl;
		DictionaryNode* dicNode = dicNodeModel->getNode();
		wstring _prefix = dicNode->getStringFromRoot();
		int _trieModelId = dicNodeModel->getModelId();
		DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
		// continue if TrieModel not found
		if (!_trieModel) {
			continue;
		}
		// get list of TrieModelElement of TrieModel
        vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
		// continue if _trieModelElements is empty
        if (_trieModelElements->empty())
        {
			continue;
		}
		// basic form
		wstring basicForm = L"";
		// get first TrieModelElement
        DictionaryTrieModelElement* _firstTrieModelElement = _trieModelElements->at(0);

		// get link of NodeModel
		DictionaryNodeModel* _linkNode = dicNodeModel->getLink();
		// if NodeModel has a link
		if (_linkNode) {
			// get string from root to link node
			wstring _link_prefix = _linkNode->getStringFromRoot();
			// get link TrieModel
			DictionaryTrieModel *_linkTrieModel = getTrieModelByModelId(_linkNode->getModelId());
			// get first TrieModelElement
			DictionaryTrieModelElement* _firstLinkTrieModelElement = _linkTrieModel->getFirstDictionaryTrieModelElement();
			// get basic form
            basicForm = _link_prefix + *(_firstLinkTrieModelElement->getSuffix());
		} else { // node does not have a link
			// get basic form
            basicForm = _prefix + *(_firstTrieModelElement->getSuffix());
		}

		// for each TrieModelElement
        for (int i_model_element = 0; i_model_element < (int) _trieModelElements->size(); ++i_model_element) {
            DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i_model_element);

			int _basicFeatureListId = _firstTrieModelElement->getFeatureListId();
			int _featureListId = _trieModelElement->getFeatureListId();

			if (featureListMap.count(_featureListId) && featureListMap.count(_basicFeatureListId))
			{
				map<int, int> _featuresSet = map<int, int>();

				vector<int> _basicFeaturesList = featureListMap.at(_basicFeatureListId);
				for (int z = 0; z < (int) _basicFeaturesList.size(); ++z)
				{
					_featuresSet[_basicFeaturesList.at(z)] = 1;
				}

				vector<int> _featuresList = featureListMap.at(_featureListId);
				for (int z = 0; z < (int) _featuresList.size(); ++z)
				{
					_featuresSet[_featuresList.at(z)] = 1;
				}

				bool ok = true;
				for (int z = 0; z < (int) features.size(); ++z)
				{
					if (!featureIdMap.count(features.at(z)))
					{
						//wcout << "ERROR 1 : " << features.at(z) << endl;
						ok = false;
						break;
					}
					int fId = featureIdMap.at(features.at(z));
					if (!_featuresSet.count(fId))
					{
						//wcout << "ERROR 2 : " << fId << endl;
						ok = false;
						break;
					}
				}
				if (ok)
				{
					// get suffix of TrieModelElement
                    wstring _suffix = *(_trieModelElement->getSuffix());
					// concat prefix and suffix
					wstring _wordForm = _prefix + _suffix;
					if (_trieModelElement->isBeginWithPo())
					{
						_wordForm = L"\u043f\u043e" + _word;//here was cyrrilic symbols: по
					}

					// create new GrammInfo
					MorphologicalInfo _morphologicalInfo;
					// set id for GrammInfo
					_morphologicalInfo.lemmaId = dicNodeModel->getLemmaId();
					// set initial form for _morphologicalInfo
					_morphologicalInfo.initial_form = basicForm;
					// set basicFeatureListId for _morphologicalInfo
					_morphologicalInfo.featureListId = _trieModelElement->getFeatureListId();
					// set basicFeatureListId for _morphologicalInfo
					_morphologicalInfo.basicFeatureListId = _firstTrieModelElement->getFeatureListId();
					// frequency
					_morphologicalInfo.frequency = -1;

					WordForm _word_form;
					_word_form.word = _wordForm;
					_word_form.info = _morphologicalInfo;
					_result.push_back(_word_form);
				}
			}
		}
	}
	return _result;
}

vector<WordForm> DictionaryTrieWordFormByFeatures::getWordFormByFeatures(wstring _word, vector<wstring> features, bool includeLinkedLemmas)
{
	vector<WordForm> result = getWordFormByFeatures(_word, features, includeLinkedLemmas, false);
	if (this->getDictionary()->getDictionaryTools()->stringBeginsWithPo(_word))
	{
		vector<WordForm> result_1 = getWordFormByFeatures(_word, features, includeLinkedLemmas, true);
		result.insert(result.end(), result_1.begin(), result_1.end());
	}
	return result;
}

vector<wstring> DictionaryTrieWordFormByFeatures::convertFeatureListIdToFeatures(int featureListId)
{
	vector<wstring> _features = vector<wstring>();
	if (featureListMap.count(featureListId) > 0)
	{
		vector<int> _featuresList = featureListMap.at(featureListId);
		for (int i = 0; i < (int) _featuresList.size(); ++i)
		{
			int _featureId = _featuresList.at(i);
			if (featureMap.count(_featureId) > 0)
			{
				_features.push_back(idFeatureMap.at(_featureId));
			}
		}
	}
	return _features;
}

vector<wstring> DictionaryTrieWordFormByFeatures::convertMorphologicalInfoToFeatureIds(MorphologicalInfo minfo)
{
	vector<wstring> result = vector<wstring>();
	if (minfo.basicFeatureListId != -1)
	{
		result = convertFeatureListIdToFeatures(minfo.basicFeatureListId);
	}
	if (featureListMap.count(minfo.featureListId) > 0)
	{
		vector<wstring> _features = convertFeatureListIdToFeatures(minfo.featureListId);
		result.insert(result.end(), _features.begin(), _features.end());
	}
	return result;
}

