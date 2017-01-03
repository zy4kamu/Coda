/**
 * DictionaryTrieModelElement.h
 * Class DictionaryTrieModelElement
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
#ifndef _DICTIONARYTRIEMODELELEMENT_H_
#define _DICTIONARYTRIEMODELELEMENT_H_

#include "DictionaryDataTypes.h"

/**
 * Class DictionaryTrieModelElement :
 */
class DictionaryTrieModelElement
{
public :
	DictionaryTrieModelElement(void);
	~DictionaryTrieModelElement(void);
	/**
	 * @brief get suffix of model element
	 */
    wstring * getSuffix() { return &suffix; }
	/**
	 * @brief set suffix to model element
	 */
    void setSuffix(const wstring & _suffix) { suffix = _suffix; }
	/**
	 * @brief get featureListId
	 */
	int getFeatureListId(void) { return featureListId;} 
	/**
	 * @brief set featureListId
	 */
	void setFeatureListId(int _featureListId) { featureListId = _featureListId;} 
	/**
	 * @brief true if word of model element begins with PO
	 */
	bool isBeginWithPo(void) { return beginWithPo; }
	/**
	 * @brief word of model element begins with PO
	 */
	void setBeginWithPo(bool _beginWithPo) { beginWithPo = _beginWithPo; }
	/**
	 * @brief a DictionaryTrieModelElement is valid if suffix is not empty or have 1 or more attributes
	 */
	bool isValid(void) { return (suffix.length() > 0 || featureListId >= 0); }
private:
	// suffix of DictionaryTrieModelElement
	wstring suffix;
	// attributes of DictionaryTrieModelElement
	int featureListId;
	// true if word begins with prefix PO
	bool beginWithPo;
};

#endif /* _DICTIONARYTRIEMODELELEMENT_H_ */ 
