/**
 * DictionaryTrieWordFormByFeatures.h
 * Class DictionaryTrieWordFormByFeatures
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
#ifndef _DICTIONARYTRIEWORDFORMBYFEATURES_H_
#define _DICTIONARYTRIEWORDFORMBYFEATURES_H_

#include "DictionaryTrieBinaryFileReader.h"

/**
 * Class DictionaryTrieWordFormByFeatures :
 * added SuffixModelTrie
 * find word form by word and features
 */
class DictionaryTrieWordFormByFeatures : public DictionaryTrieBinaryFileReader
{
public:
    DictionaryTrieWordFormByFeatures(Dictionary* _dic, string dictionaryFile);
	~DictionaryTrieWordFormByFeatures(void);

	/**
	 * @brief get word form by features
	 */
	vector<WordForm> getWordFormByFeatures(wstring _word, vector<wstring> features, bool includeLinkedLemmas, bool _beginWithPo);
	/**
	 * @brief get word form by features
	 */
	vector<WordForm> getWordFormByFeatures(wstring _word, vector<wstring> features, bool includeLinkedLemmas = true);
	/**
	 * @brief convert featureListId to feature in form wstring
	 */
	vector<wstring> convertFeatureListIdToFeatures(int featureListId);
	/**
	 * @brief convert morphological information to feature in form wstring
	 */
	vector<wstring> convertMorphologicalInfoToFeatureIds(MorphologicalInfo minfo);
};

#endif /* _DICTIONARYTRIEWORDFORMBYFEATURES_H_ */
