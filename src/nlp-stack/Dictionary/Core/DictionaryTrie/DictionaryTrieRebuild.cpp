/**
 * DictionaryTrieRebuild.cpp
 */

#include "DictionaryTrieRebuild.h"
#include "Dictionary.h"

/**
 * Constructor of DictionaryTrieRebuild
 */
DictionaryTrieRebuild::DictionaryTrieRebuild(Dictionary* _dic, string dictionaryFile) : DictionaryTrieAccentUpdate(_dic, dictionaryFile)
{
}

/**
 * Destructor of DictionaryTrieRebuild
 */
DictionaryTrieRebuild::~DictionaryTrieRebuild(void)
{
}

void DictionaryTrieRebuild::rebuildDictionary(void)
{
	// map <featureIdSet, featureListId>
	map<set<int>, int> featureList_id_map = map<set<int>, int>();
	// map <featureListId, featureIdSet>
	map<int, set<int> > id_featureList_map = map<int, set<int> >();
	// map <oldFeatureListId, newFeatureListId>
	map<int, int> featureListIdMap = map<int, int>();
	// rebuild index
	int newFeatureListId = 0;
	// check each featureList
	for (map<int, vector<int> >::iterator i_flm = featureListMap.begin(); i_flm != featureListMap.end(); ++i_flm)
	{
		int oldFeatureListId = i_flm->first;
		vector<int> featureIdVector = i_flm->second;
		/*
		wcout << featureListId;
		for (int i = 0; i < featureIdVector.size(); ++i)
		{
			wcout << " - " << dic->getDictionaryTrie()->idToShortId(featureIdVector.at(i));
		}
		wcout << endl;
		*/
		// make set of features from vector
		set<int> featureIdSet = set<int>();
		for (int i = 0; i < (int) featureIdVector.size(); ++i)
		{
			featureIdSet.insert(featureIdVector.at(i));
		}
		// new set
		if (!featureList_id_map.count(featureIdSet))
		{
			// new featureListId
			newFeatureListId++;
			// saving featureIdSet & newFeatureListId
			featureList_id_map[featureIdSet] = newFeatureListId;
			id_featureList_map[newFeatureListId] = featureIdSet;
			// mapping
			featureListIdMap[oldFeatureListId] = newFeatureListId;
			//wcout << oldFeatureListId << " -> " << newFeatureListId << endl;
		}
		else // old set
		{
			int _newFeatureListId = featureList_id_map.at(featureIdSet);
			// mapping
			featureListIdMap[oldFeatureListId] = _newFeatureListId;
			//wcout << oldFeatureListId << " -> " << _newFeatureListId << endl;
		}
	}

	wcout << "UPDATE begins" << endl;

	// update featureListMap
	featureListMap = map<int, vector<int> >();
	for (map<int, set<int> >::iterator iter = id_featureList_map.begin(); iter != id_featureList_map.end(); ++iter)
	{
		int featureListId = iter->first;
		set<int> featureIdSet = iter->second;
		vector<int> featureList = vector<int>();
		for (set<int>::iterator it = featureIdSet.begin(); it != featureIdSet.end(); ++it)
		{
			featureList.push_back(*it);
		}
		featureListMap[featureListId] = featureList;
	}

	// update trieModelMap
	for (map<int, DictionaryTrieModel*>::iterator iter = trieModelMap.begin(); iter != trieModelMap.end(); ++iter)
	{
		DictionaryTrieModel* dictionaryTrieModel = iter->second;
        vector<DictionaryTrieModelElement*> * dictionaryTrieModelElements = dictionaryTrieModel->getDictionaryTrieModelElements();
        for (int i = 0; i < (int) dictionaryTrieModelElements->size(); ++i)
		{
            DictionaryTrieModelElement* dictionaryTrieModelElement = dictionaryTrieModelElements->at(i);
			int oldFeatureListId = dictionaryTrieModelElement->getFeatureListId();
			if (featureListIdMap.count(oldFeatureListId))
			{
				int newFeatureListId = featureListIdMap.at(oldFeatureListId);
				dictionaryTrieModelElement->setFeatureListId(newFeatureListId);
			}
			else
			{
				wcout << "###### UPDATE trieModelMap : Error : " << oldFeatureListId << " -> " << newFeatureListId << endl;
			}
		}
	}
}

void DictionaryTrieRebuild::exportDictionaryToTextFile(string text_file_path)
{
    std::wofstream fo(text_file_path.c_str());

#ifdef MSVC
    fo.imbue(std::locale(fo.getloc(), new std::codecvt_utf8<wchar_t>));
#else
    fo.imbue(std::locale("en_US.UTF-8"));
#endif

    // Nodes

    fo << std::to_wstring(numberOfNodes) << endl;
    queue<DictionaryNode*> nodeQueue = queue<DictionaryNode*>();
    nodeQueue.push(root);
    int currentNodeId = 0;
    fo << root->getCharacter() << endl;
    fo << -1 << endl;
    while (!nodeQueue.empty())
    {
        DictionaryNode* currentNode = nodeQueue.front();
        nodeQueue.pop();
        vector<DictionaryNode*> childrenNodes = currentNode->getChildren();
        for (int i = 0; i < (int) childrenNodes.size(); ++i)
        {
            DictionaryNode* childNode = childrenNodes.at(i);
            nodeQueue.push(childNode);

            fo << childNode->getCharacter() << endl;
            fo << std::to_wstring(currentNodeId) << endl;
        }
        currentNodeId++;
    }
    wcout << "Nodes ok" << endl;

    // NodeModels

    fo << std::to_wstring((int) nodeModelMap.size()) << endl;

    nodeQueue.empty();
    nodeQueue.push(root);
    currentNodeId = 0;
    int nodeModelCount = 0;

    vector<pair<int, int> > links = vector<pair<int, int> >();

    while (!nodeQueue.empty())
    {
        DictionaryNode* currentNode = nodeQueue.front();
        nodeQueue.pop();

        vector<DictionaryNodeModel*> * _nodeModels = currentNode->getNodeModels();
        for (int i = 0; i < (int) _nodeModels->size(); ++i)
        {
            DictionaryNodeModel* _nodeModel = _nodeModels->at(i);

            fo << std::to_wstring((int)_nodeModel->getModelId()) << endl;
            fo << std::to_wstring((int)_nodeModel->getLemmaId()) << endl;
            fo << std::to_wstring((int)currentNodeId) << endl;

            DictionaryNodeModel* _nodeModelFrom = _nodeModel->getLink();
            if (_nodeModelFrom != NULL)
            {
                links.push_back(pair<int, int>(_nodeModelFrom->getLemmaId(), _nodeModel->getLemmaId()));
            }

            nodeModelCount++;
        }

        vector<DictionaryNode*> childrenNodes = currentNode->getChildren();
        for (int i = 0; i < (int) childrenNodes.size(); ++i)
        {
            DictionaryNode* childNode = childrenNodes.at(i);
            nodeQueue.push(childNode);
        }
        currentNodeId++;
    }

    wcout << "Node models ok" << endl;

    // Links

    fo << std::to_wstring((int)links.size()) << endl;
    for (vector<pair<int, int> >::iterator iter = links.begin(); iter != links.end(); ++iter)
    {
        pair<int, int> ppp = *iter;
        fo << std::to_wstring((int) ppp.first) << endl << std::to_wstring((int) ppp.second) << endl;
    }

    wcout << "Links ok" << endl;

    // TrieModels

    fo << std::to_wstring((int) trieModelMap.size()) << endl;
    for (int ind = 1; ind <= (int) trieModelMap.size(); ++ind)
    {
        DictionaryTrieModel* _trieModel = trieModelMap.at(ind);

        vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
        fo << std::to_wstring((int) _trieModelElements->size()) << endl;
        for (vector<DictionaryTrieModelElement*>::iterator iter = _trieModelElements->begin(); iter != _trieModelElements->end(); ++iter)
        {
            DictionaryTrieModelElement* _trieModelElement = *iter;
            fo << _trieModelElement->getSuffix() << endl;

            int _beginWithPo = _trieModelElement->isBeginWithPo() ? 1 : 0;
            fo << _beginWithPo << endl;

            fo << std::to_wstring((int) _trieModelElement->getFeatureListId()) << endl;
        }
    }

    wcout << "Trie models ok" << endl;

    // featureListMap

    fo << std::to_wstring((int) featureListMap.size()) << endl;
    for (int iFeatureList = 1; iFeatureList <= (int) featureListMap.size(); ++iFeatureList)
    {
        vector<int> featureList = featureListMap.at(iFeatureList);
        fo << std::to_wstring((int) featureList.size()) << endl;
        for (int i = 0; i < (int) featureList.size(); ++i)
        {
            fo << std::to_wstring((int) featureList.at(i)) << endl;
        }
    }

    wcout << "feature list map ok" << endl;

    // feature & description
    wcout << "idFeatureMap.size() = " << idFeatureMap.size() << endl;
    wcout << "featureMap.size() = " << featureMap.size() << endl;
    fo << std::to_wstring((int) featureMap.size()) << endl;
    for (int i = 1; i <= (int) featureMap.size(); ++i)
    {
        fo << idFeatureMap.at(i) << endl;
        fo << featureMap.at(i) << endl;
    }

    wcout << "features & description ok" << endl;

    fo.close();
}
