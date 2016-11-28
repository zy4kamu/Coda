/**
 * DictionaryTrieBuild.h
 * Class DictionaryTrieBuild
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
#ifndef _DICTIONARYTRIEBUILD_H_
#define _DICTIONARYTRIEBUILD_H_

#include "DictionaryTrie.h"

/**
 * class DictionaryTrieBuild :
 * read from text files
 */
class DictionaryTrieBuild : public DictionaryTrie
{
public:
    DictionaryTrieBuild(Dictionary* _dic);
	~DictionaryTrieBuild(void);
	
	/**
	 * @brief read models, LPMs and links from text files
	 */
	void readFromTextFiles(string _modelLpmFilePath, string _linkFilePath);
	/**
	 * @brief read models, LPMs from file
	 */
	void readModelsAndLPMsFromFile(string _filePath);
	/**
	 * @brief read links from files
	 */
	void readLinksFromFile(string _filePath);
	/**
	 * @brief process a line to model
	 */
	void addModel(wstring line);
	/**
	 * @brief process a line to LPM
	 */
	void addLPM(wstring line);
	/**
	 * @brief process a line to link
	 */
	void addLink(wstring line);
	/**
	 * @brief process a line to feature
	 */
	void addFeature(wstring line);
	/**
	 * @brief process a line to featureList
	 */
	void addFeatureList(wstring line);
	/**
	 * @brief Process a string _str to TreeModel
	 */
	DictionaryTrieModel* getTrieModelFromString(wstring _str);
	/**
	 * @brief Process a string _str to TreeModelElement
	 */
	DictionaryTrieModelElement* getTrieModelElementFromString(wstring _str);
};

#endif /* _DICTIONARYTRIEBUILD_H_ */
