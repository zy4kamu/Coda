/**
 * DictionaryInterface.h
 *
 * Interface
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


#ifndef _DICTIONARY_INTERFACE_H_
#define _DICTIONARY_INTERFACE_H_

#include "DictionaryDataTypes.h"
#include "DictionaryTrieRebuild.h"

class Dictionary;

class DictionaryInterface {
public:   
	/**
	 * @brief constructor of interface
	 */
    static DictionaryInterface* CreateDictionary(const string & dictionaryFile = "RussianMorphology.bin", int _dicLevel = 0);
    virtual ~DictionaryInterface() {}

    /**
     * @brief get morphological information of word
     * @param [in] word : word that need morphological analyze
     * @param [in] level : define level of finding non-dictionary word
     * @return a vector of struct MorphologicalInfo
     */
    virtual void getMorphologicalInfo(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result, int level = 0) = 0;

    /**
     * @brief get grammartical information of word
     * @param [in] word : word that need morphological analyze
     * @param [in] level : define level of finding non-dictionary word
     * @return a vector of struct MorphologicalInfo
     */
    virtual void getGrammInfo(const wstring & word, shared_ptr<vector<GrammInfo> > result, int level = 0) = 0;

    /**
     * @brief get morphological information of lemmaId
     * @param [in] lemmaId : lemmaId that need morphological analyze
     * @return a vector of struct MorphologicalInfo
     */
    virtual void getMorphologicalInfoOfLemmaId(int lemmaId, shared_ptr<vector<MorphologicalInfo> > result) = 0;

    /**
     * @brief get grammartical information of lemmaId
     * @param [in] lemmaId : lemmaId that need morphological analyze
     * @return a vector of struct GrammInfo
     */
    virtual void getGrammInfoOfLemmaId(int lemmaId, shared_ptr<vector<GrammInfo> > result) = 0;

    /**
     * @brief get lemma of lemmaId
     * @param [in] lemmaId : lemmaId that need morphological analyze
     * @return a vector of struct GrammInfo
     */
    virtual wstring getLemmaOfLemmaId(int lemmaId) = 0;

    /**
	 * @brief get word form by basic form and features
	 * @param _word : basic form of word to be found
	 * @param features : features of word to be found
	 */
    virtual vector<WordForm> getWordFormByFeatures(const wstring & _word, vector<wstring> features, bool includeLinkedLemmas = true) = 0;
	
	/**
	 * @brief get information of word
	 * @date 2013-06-25
	 * @param [in] word : word that need stress adding morphological analyze
	 * @param [in] level : define level of finding non-dictionary word
	 * @return a vector of struct WordInfo
	 */
    virtual vector<WordInfo> getWordInfo(const wstring & word) = 0;
	
	/**
	 * @brief set main word form from linked lemmas
	 * @date 2014-03-17
	 */
	virtual void setMainWordFormFromLinkedLemmas(bool _mainWordFormFromLinkedLemmas) = 0;

    /**
     * @brief returns the whole paradigm of the given lemma
     * @param [in] lemma : word in initial form
     * @return list of possible wordforms
     */
    virtual void getParadigmsForLemma(const wstring lemma, vector<vector<GrammInfo>>& result) = 0;

    virtual DictionaryTrieRebuild* getDictionaryTrie(void) = 0;
    virtual void setDictionaryLevel(int _dicLevel) = 0;
    virtual void enableRules(bool _useRules) = 0;
};

#endif /* _DICTIONARY_INTERFACE_H_ */
