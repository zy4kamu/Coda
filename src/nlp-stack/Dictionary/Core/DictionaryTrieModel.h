/**
 * DictionaryTrieModel.h
 * Class DictionaryTrieModel
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
#ifndef _DICTIONARYTRIEMODEL_H_
#define _DICTIONARYTRIEMODEL_H_

#include "DictionaryTrieModelElement.h"

/**
 * class DictionaryTrieModel :
 */
class DictionaryTrieModel
{
public :
	DictionaryTrieModel(void);
	~DictionaryTrieModel(void);
	/**
	 * @brief add a DictionaryTrieModelElement to model
	 */
	void addDictionaryTrieModelElement(DictionaryTrieModelElement* element) { trieModelElements.push_back(element); }
	/**
	 * @brief get number of DictionaryTrieModelElement of model
	 */
	int getNumberOfDictionaryTrieModelElement(void) { return trieModelElements.size(); }
	/**
	 * @brief a DictionaryTrieModel is valid if not empty
	 */
	bool isValid() { return (trieModelElements.size() > 0); }
	/**
	 * @brief get list of DictionaryTrieModelElement
	 */
    vector<DictionaryTrieModelElement*> * getDictionaryTrieModelElements(void) { return &trieModelElements; }
	/**
	 * @brief get attributes list of first DictionaryTrieModelElement
	 */
	DictionaryTrieModelElement* getFirstDictionaryTrieModelElement(void);
private:
	// list of DictionaryTrieModelElement
	vector<DictionaryTrieModelElement*> trieModelElements;
};

#endif /* _DICTIONARYTRIEMODEL_H_ */
