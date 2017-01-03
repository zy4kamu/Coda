/**
 * Dictionary.h
 * Class Dictionary
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
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include "DictionaryInterface.h"
#include "DictionaryTools.h"
#include "DictionaryTrieRebuild.h"
#include "NGramTrieBinaryFileReader.h"
//#include "NGramTrieBinaryFileCreator.h"
#include "SuffixModelTrieBinaryFileReader.h"
#include "TestMorphologicalAnalyze.h"

using namespace std;

/**
 * @class Dictionary
 * @brief Main class of project, contain tools, dictionary, n-gram database and suffix tree.
 */
class Dictionary  : public DictionaryInterface
{
public:
	/**
	 * @brief Constructor of dictionary
	 */
    Dictionary(const string & dictionaryFile = BIN_DIC_FILE_PATH_RUSSIAN, int _dicLevel = 0);
	/**
	 * @brief Destructor of dictionary
	 */
	~Dictionary(void);
    /**
     * @brief get grammatical information of word by level
     */
    void getGrammInfo(const wstring & word, shared_ptr<vector<GrammInfo> > result, int level = 0);
    /**
	 * @brief get word form by basic form and features
	 */
    vector<WordForm> getWordFormByFeatures(const wstring & _word, vector<wstring> features, bool includeLinkedLemmas = true)
	{ 
		return this->getDictionaryTrie()->getWordFormByFeatures(_word, features, includeLinkedLemmas);
	}
	/**
	 * @brief get morphological information of word
	 */
    void getMorphologicalInfo(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result, int level = 0);
    /**
	 * @brief get morphological information of word after trying to fix ONE misspell
	 */
    void getPrediction(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
    /**
	 * @brief get morphological information of word not considering differ \u0435/\u0451//here was cyrrilic symbols: е,ё
	 */
    void getMorphologicalInfoIgnoreE(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
    /**
	 * @brief get morphological information of each part of word-combination
	 */
    void getMorphologicalInfoHyphenSplit(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result);
    /**
	 * @brief get morphological information of each word of sentence
	 */
    vector<MorphologicalAnalyze> getMorphologicalAnalyze(const wstring & _sentence);
	/**
	 * @brief get morphological information of each word of sentence with n-gram
	 */
    NGramMorphologicalAnalyze getNGramMorphologicalAnalyze(const wstring & sentence);
	/**
	 * @brief set ignore \u0435/\u0451 option//here was cyrrilic symbols: е,ё
	 */
	void setIgnoreE(bool _ignoreE) { ignoreE = _ignoreE; }
	/**
	 * @brief get ignore \u0435/\u0451 option//here was cyrrilic symbols: е,ё
	 */
	bool getIgnoreE(void) { return ignoreE; }
	/**
	 * @brief set hyphen split option
	 */
	void setHyphenSplit(bool _hyphenSplit) { hyphenSplit = _hyphenSplit; }
	/**
	 * @brief get hyphen split option
	 */
	bool getHyphenSplit(void) { return hyphenSplit; }
	/**
	 * @brief get dictionary trie
	 */
    DictionaryTrieRebuild* getDictionaryTrie(void) { return trie;}
	/**
	 * @brief get dictionary tools
	 */
	DictionaryTools* getDictionaryTools(void) { return tools;}
	/**
	 * @brief get suffix model trie
	 */
	SuffixModelTrieBinaryFileReader* getSuffixModelTrie(void) { return suffixModelTrie;}
	/**
	 * @brief get N-Gram trie
	 */
	NGramTrieBinaryFileReader* getNGramTrie(void) { return nGram;}
	/**
	 * @brief get N-Gram trie version 2
	 */
	//NGramTrieBinaryFileCreator* getNGramTrieBinaryFileCreator(void) { return nGramVer2;}
	/**
	 * @brief get test of morphological analyze
	 */
	TestMorphologicalAnalyze* getTestMorphologicalAnalyze(void) { return testMorphologicalAnalyze; }
	/**
	 * @brief get morphological information of word
	 * @date 2013-06-25
	 * @param [in] word : word that need morphological analyze
	 * @param [in] level : define level of finding non-dictionary word
	 * @return a vector of struct MorphologicalInfo
	 * @TODO : level
	 */
    vector<WordInfo> getWordInfo(const wstring & word);

    string getPathToData() { return pathToData;}
    void setPathToData(const string& _pathToData) { pathToData = _pathToData;}

    void setMainWordFormFromLinkedLemmas(bool _mainWordFormFromLinkedLemmas)
    {
        mainWordFormFromLinkedLemmas = _mainWordFormFromLinkedLemmas;
        trie->setMainWordFormFromLinkedLemmas(_mainWordFormFromLinkedLemmas);
    }

    bool useLinkedLemmas()
    {
        return mainWordFormFromLinkedLemmas;
    }

    /**
     * @brief Get list of MorphologicalInfo by a given lemmaId
     */
    void getMorphologicalInfoOfLemmaId(int lemmaId, shared_ptr<vector<MorphologicalInfo> > result);
    /**
     * @brief Get list of GrammInfo by a given lemmaId
     */
    void getGrammInfoOfLemmaId(int lemmaId, shared_ptr<vector<GrammInfo> > result);
    /**
     * @brief get lemma of lemmaId
     * @param [in] lemmaId : lemmaId that need morphological analyze
     * @return a vector of struct GrammInfo
     */
    wstring getLemmaOfLemmaId(int lemmaId);

    void setDictionaryLevel(int _dicLevel) { dicLevel = _dicLevel;}
    void enableRules(bool _useRules)
    {
        trie->enableRules(_useRules);
    }

protected:
    DictionaryTrieRebuild* trie;
    DictionaryTools* tools;
	SuffixModelTrieBinaryFileReader* suffixModelTrie;
	NGramTrieBinaryFileReader* nGram;
	//NGramTrieBinaryFileCreator* nGramVer2;
	TestMorphologicalAnalyze* testMorphologicalAnalyze;

	bool ignoreE; // if ignoreE = true \u0435/\u0451 will not be different//here was cyrrilic symbols: е,ё
	bool hyphenSplit; // if hyphenSplit = true 

	// 2013-07-03 level
	// dicLevel  >= 0 : morphological analyze + stress information
	// dicLevel  >= 1 : suffix trie
	// dicLevel  >= 2 : n-gram
	int dicLevel;
	
	// 2013-07-17 path to data folder
	string pathToData;

	// 2014-03-17 get main word form from linked lemmas
	bool mainWordFormFromLinkedLemmas;
};
#endif /* _DICTIONARY_H_ */
