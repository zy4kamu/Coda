/** 
 * @file	ITokenizer.h
 * @brief	header file for interfece of tokenizer
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


#ifndef DICTIONARY_FACADE
#define DICTIONARY_FACADE

#include <vector>
#include <string>
#include <memory>

#include "DictionaryInterface.h"
#include "MorphologicalDictionary.h"
#include "Tools.h"

using std::vector;
using std::wstring;
using std::string;
using std::shared_ptr;

namespace DictionaryOps
{

/** 
 * @class	DictionaryFacade
 * @brief	Easy to use dictionary
 */
class DictionaryFacade
{
public:   
	/**
     * @brief constructor
	 */
     DictionaryFacade(Tools::Language i_language);

	/**
     * @brief non-virtual destructor. Class is not supposed to be inherited
	 */
     ~DictionaryFacade() { if(m_dicInterface) delete m_dicInterface;}

    /**
     * @brief	Returns morphologicalInformation for given token
     * @param[in]	i_wordForm - input token
     * @param[out]	info - morphological analysis for given token
     */
    vector<Morphology> getMorphologicalInformation(const wstring& i_wordForm) const;

    /**
     * @brief	Synthesizes correct word form given lemma and morphological features
     * @param[in]	i_lemma - lemma/normal form
     * @param[out]	i_morphology - grammatical features
     * @param[out]	tokens - possible word forms
     */
    vector<wstring> synthesizeWordform(const wstring& i_lemma, vector<wstring>& i_morphology) const;

    /**
     * @brief	Returns language code
     * @param[out]	language - language code
     */
    Tools::Language getLanguage() const;

private:
    DictionaryInterface* m_dicInterface;
    shared_ptr<MorphologicalDictionary> m_morphologicalDictionary;
    Tools::Language m_language;
};

}


#endif // DICTIONARY_FACADE
