/**
 * NGramTrie.h
 * Class NGramTrie
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
#ifndef _NGRAMTRIE_H_
#define _NGRAMTRIE_H_

#include "NGramNode.h"

using namespace std;

class Dictionary;

/**
 * class NGramTrie :
 */
class NGramTrie
{
public:
	NGramTrie(Dictionary* _dic);
	~NGramTrie(void);

	/**
	 * @brief update a vector of morphological information
	 */
	void updateMorphologicalInfo(vector<MorphologicalInfo> _morphologicalInfoList);
	/**
	 * @brief get N-Gram from vector of morphological information
	 */
	int getNGrammCount(vector<MorphologicalInfo> _morphologicalInfoList);
	/**
	 * @brief get morphological information of sentence
	 */
	vector<MorphologicalInfo> sentenceToMorphologicalInfo(wstring _sentence);
	/**
	 * @brief get N-Gram value of a vector of morphological information
	 */
	int morphologicalNGramValue(vector<MorphologicalInfo> _morphologicalInfoList, bool extra = true);
	/**
	 * @brief test a sentence
	 */
	void testSentence(wstring _sentence);
	/**
	 * @brief get N constant of N-Gram
	 */
	int getN(void) { return N;}
	/**
	 * @brief get parent object - dictionary
	 */
	Dictionary* getDictionary(void) { return dic;}
protected:
	NGramNode* root;
	Dictionary* dic;
	int N;
	int numberOfNodes;
};
#endif /* _NGRAMTRIE_H_ */
