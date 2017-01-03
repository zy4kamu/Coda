/**
 * DictionaryTrieAccent.h
 * Class DictionaryTrieAccent
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
#ifndef _DICTIONARYTRIEACCENT_H_
#define _DICTIONARYTRIEACCENT_H_

#include "DictionaryTrieWordFormByFeatures.h"

class Dictionary;

/**
 * Class DictionaryTrieAccent :
 * Find word form with stress
 */
class DictionaryTrieAccent : public DictionaryTrieWordFormByFeatures
{
public:
    DictionaryTrieAccent(Dictionary* _dic, string dictionaryFile);
	~DictionaryTrieAccent(void);

	/**
	 * @brief read all stress model from file
	 */
	void readAllModelFromFile(string filePath);
	/**
	 * @brief add stress + morphological analyze
	 */
	vector<WordInfo> getWordInfo(wstring word);
	/**
	 * @brief add stress + morphological analyze
	 */
	vector<WordInfo> getWordInfo(wstring word, bool _beginWithPo);
	/**
	 * @brief add a vector<char> to buffer
	 */
    void writeToBuffer(vector<unsigned char> charVector);
	/**
	 * @brief save stress information to binary file
	 * @date 2013-07-04
	 */
	void saveStressInFormationToBinaryFile(string filePath);
	/**
	 * @brief load stress information from binary file
	 * @date 2013-07-04
	 */
	void loadStressInFormationFromBinaryFile(string filePath);

protected:
	// stressPosList[lemmaId] = {pos_1, pos_2, ...pos_k}
	vector<vector<int> > stressPosList;
	// 2013-07-03 stressModelId[lemmaId] = stress_model_id
	vector<int> stressModelId;
	// 2013-07-03 stressModel[stress_model_id] = {pos_1, pos_2, ...pos_k}
	vector<vector<int> > stressModel;
	// number of stress models
	int numberOfStressModels;
	// buffer
    unsigned char *buffer;
	// size of buffer
	int bufferSize;

	// 2013-06-22 map<RusCorporaFeature, featureId>
	map<wstring, int> RusCorporaFeatureId;
	// 2013-06-22 map<featureId, RusCorporaFeature>
	map<int, wstring> IdRusCorporaFeature;
};

#endif /* _DICTIONARYTRIEACCENT_H_ */
