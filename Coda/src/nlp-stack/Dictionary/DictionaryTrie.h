/**
 * DictionaryTrie.h
 * Class DictionaryTrie
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


//#pragma warning( disable: 4018 )
#ifndef _DICTIONARYTRIE_H_
#define _DICTIONARYTRIE_H_

#include "DictionaryNodeModel.h"
#include "DictionaryTrieModel.h"
#include "DictionaryNode.h"
#include <memory>

class Dictionary;

struct MorphologyRule
{
    wstring word_suffix;
    wstring lemma_suffix;
    int word_feature_list_id;
    int lemma_feature_list_id;
};

struct MorphologyRuleSet
{
    vector<MorphologyRule> morphologyRules;
    int min_lemma_length; // min length of lemma (Is -> I is not correct)
};

/**
 * class DictionaryTrie :
 */
class DictionaryTrie
{
public:
    static int morphology_count;
    static int getMorphologyCount() { return morphology_count;}

    DictionaryTrie(Dictionary* _dic);
	~DictionaryTrie(void);
	
	/**
	 * @brief get NodeModel by lemmaId
	 */
	DictionaryNodeModel* getNodeModelByLemmaId(int _lemmaId);
	/**
	 * @brief map _modelIndex to _trieModel
	 */
	void mapModelIndexToTrieModel(int _modelIndex, DictionaryTrieModel* _trieModel);
	/**
	 * @brief add a LPM Model to trie
	 */
    void addLPMModel(const wstring & _lpm, int _modelIndex, int _lemmaIndex);
	/**
	 * @brief add a link by lemmaIds
	 */
	void addLinkByLemmaIds(int fromLemmaId, int toLemmaId);
	/**
	 * @brief get DictionaryTrieModel by modelId
	 */
	DictionaryTrieModel* getTrieModelByModelId(int _modelId);
	/**
	 * @brief Get list of MorphologicalInfo by a given word, depends on that it begins with PO or not
	 */
    void getMorphologicalInfoList(const wstring & _word, bool _beginWithPo, shared_ptr<vector<MorphologicalInfo> > result);
    /**
     * @brief Get list of MorphologicalInfo by a given word (not depends on that it begins with PO or not)
     */
    void getMorphologicalInfoList(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
    /**
     * @brief Get list of MorphologicalInfo by a given lemmaId
     */
    void getMorphologicalInfoListOfLemmaId(int lemmaId, shared_ptr<vector<MorphologicalInfo> > result);
    /**
     * @brief convert featureListId to features
     */
    vector<vector<wstring> > convertFeatureListIdToFeaturePairs(int featureListId);
    /**
     * @brief convert MorphologicalInfo to GrammInfo
     */
    void convertMorphologicalInfoToGrammInfo(MorphologicalInfo * minfo, shared_ptr<GrammInfo> ginfo);
    /**
	 * @brief get parent object - dictionary
	 */
	Dictionary* getDictionary(void) { return dic;}
	/**
	 * @brief get morphological predictions for neologisms
	 */
    void getMorphologicalPrediction(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
	/**
	 * @brief get root node
	 */
	DictionaryNode* getRoot(void) { return root;}
	/**
	 * @brief create feature-ID map
	 */
    void createFeatureIdMap(const string & _filePath);
	
	// 2013-07-09 edit dictionary
	map<int, vector<int> > getFeatureListMap() {return featureListMap;}
	// 2014-03-05 print morphological info
    map<int, wstring> * getFeatureMap() {return &featureMap;}
    map<int, wstring> * getIdFeatureMap() { return &idFeatureMap;}

	wstring idToShortId(int id) { return idFeatureMap.count(id) ? idFeatureMap.at(id) : L"?";}

	void setMainWordFormFromLinkedLemmas(bool _mainWordFormFromLinkedLemmas)
	{
		mainWordFormFromLinkedLemmas = _mainWordFormFromLinkedLemmas;
	}
    // 2015-04-20 English Morphology : rules
    void readRulesFromTextFile(const string& filePath);
    void enableRules(bool _useRules);
    bool ruleIsEnable() { return useRules;}
    void getMorphologicalInfoListByRules(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
protected:
	// Dictionary
	Dictionary* dic;
	// root of Trie
	DictionaryNode* root;
	// map <ModelIndex, TrieModel>
	map<int, DictionaryTrieModel*> trieModelMap;
	// map <lemmaId, NodeModel>, for links adding
	map<int, DictionaryNodeModel*> nodeModelMap;
	// map <featureListId, vector<featureId>>
	map<int, vector<int> > featureListMap;
	// map <featureId, feature>
	map<int, wstring> featureMap;
	// number of nodes
	int numberOfNodes;
	// number of links
	int numberOfLinks;
	// number of DictionaryNodeModel
	int numberOfNodeModels;
	// map<short-feature, Id>
	map<wstring, int> featureIdMap;
	// map<Id, short-feature>
	map<int, wstring> idFeatureMap;

	// 2014-03-17 get main word form from linked lemmas
	bool mainWordFormFromLinkedLemmas;

    // 2015-04-21 English Morphology rules (Morphy)
    bool useRules;
    MorphologyRuleSet ruleSet;
};

#endif /* _DICTIONARYTRIE_H_ */
