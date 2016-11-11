/**
 * SuffixModelTrie.h
 * Class SuffixModelTrie
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
#ifndef _SUFFIXMODELTRIE_H_
#define _SUFFIXMODELTRIE_H_

#include "SuffixModelNode.h"

class Dictionary;

using namespace std;

/**
 * class SuffixModelTrie for searching featureListId for suffix of a non-dictionary word
 * SuffixModelTrie is built from DictionaryTrie
 */
class SuffixModelTrie
{
public:
	SuffixModelTrie(Dictionary* _dic);
	~SuffixModelTrie(void);
	
	/**
	 * @brief update featureID
	 */
	void updateFeatureId(int _basicFeatureListId, int _featureListId);
	/**
	 * @brief update frequency of feature
	 */
	int getFeatureFrequency(int _basicFeatureListId, int _featureListId);
	/**
	 * @brief update suffix by list of morphological information
	 */
	void updateSuffix(wstring _suffix, vector<MorphologicalInfo> minfos);
	/**
	 * @brief update suffix by featureId
	 */
	void updateSuffix(wstring _suffix, int _basicFeatureListId, int _featureListid); 
	/**
	 * @brief get morphological prediction of word by suffix
	 */
	vector<MorphologicalInfo> getMorphologicalPredictionBySuffix(wstring _word);
	/**
	 * @brief get number of nodes in suffix model trie
	 */
	int getNumberOfNode(void) { return numberOfNodes;}
	/**
	 * @brief get parent object - dictionary
	 */
	Dictionary* getDictionary(void) { return dic;}
protected:
	SuffixModelNode* root;
	int numberOfNodes;
	map<int, int> featureIdFrequency;
	Dictionary* dic;
};

#endif /* _SUFFIXMODELTRIE_H_ */
