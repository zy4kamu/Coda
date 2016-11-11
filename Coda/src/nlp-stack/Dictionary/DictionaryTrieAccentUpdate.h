/**
 * DictionaryTrieAccentUpdate.h
 * Class DictionaryTrieAccentUpdate
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
#ifndef _DICTIONARYTRIEACCENTUPDATE_H_
#define _DICTIONARYTRIEACCENTUPDATE_H_

#include "DictionaryTrieAccent.h"

class Dictionary;

struct wordInfoStructure
{
	wstring word;
	set<int> lemmaIdSet;
	int stressPos;
	vector<MorphologicalInfo> minfos;
};

struct wordStressInfoStructure
{
	int lemmaId;
	int pos;
	int stressPos;
};

struct RusCorporaLogElement
{
	int rusCorporaStressPos;
	vector<int> dictionaryStressPos;
};

// 2013-06-21
struct RusCorporaLogElement_Ver1
{
	int rusCorporaStressPos;
	vector<int> dictionaryStressPos;
};

/**
 * Class DictionaryTrieAccentUpdate :
 * Accent analyze
 * Added stressPosList
 */
class DictionaryTrieAccentUpdate : public DictionaryTrieAccent
{
public:
    DictionaryTrieAccentUpdate(Dictionary* _dic, string dictionaryFile);
	~DictionaryTrieAccentUpdate(void);

	/**
	 * @brief get set of lemma ID from vector of morphological information
	 */
	set<int> lemmaIdSet(vector<MorphologicalInfo> minfos);
	/**
	 * @brief get word informations from a sentence
	 */
	vector<wordInfoStructure> getWordInfoStructure(wstring line);
	/**
	 * @brief get word stress information from a sentence
	 */
	vector<wordStressInfoStructure> getWordStressInfoStructure(wstring line);
	/**
	 * @brief add stress to word form
	 */
	vector<WordFormWithStress> getWordFormWithStress(wstring _word, bool _beginWithPo);
	/**
	 * @brief add stress to word form
	 */
	vector<WordFormWithStress> getWordFormWithStress(wstring _word);
	/**
	 * @brief convert word form with stress to wstring
	 */
	wstring convertWordFormWithStressToWstring(WordFormWithStress wfws);
	/**
	 * @brief update stress position
	 */
	void updateStressPos(int lemmaId, int pos, int stressPos);
	/**
	 * @brief update stress information from a sentence
	 */
	void updateStressFromLine(wstring line);
	/**
	 * @brief update stress information from a file
	 */
	void updateStressFromFile(void);
	/**
	 * @brief save all stress model to file
	 */
	void printAllModelToFile(string filePath);

	/**
	 * @brief update stress position
	 * 2013-06-04 : update all variants + write duplications to log
	 */
	void updateStressPos_Ver1(wstring word, int lemmaId, int pos, int stressPos, bool writeToLog = true);
	/**
	 * @brief update stress information from a sentence
	 * 2013-06-04 : update all variants
	 */
	void updateStressFromLine_Ver1(wstring line);
	/**
	 * @brief update stress information from a file
	 * 2013-06-04 : update all variants
	 */
	void updateStressFromFile_Ver1(void);
	/**
	 * @brief save lemma and stress log to file
	 * 2013-06-04 : update all variants
	 */
	void saveLogToFile_Ver1(void);

	/**
	 * @brief test corpus : word
	 * @date 2013-06-14
	 */
	void testRusCorporaWord(wstring word);
	/**
	 * @brief test corpus : line
	 * @date 2013-06-14
	 */
	void testRusCorporaLine(wstring line);
	/**
	 * @brief test corpus : file
	 * @date 2013-06-14
	 */
	void testRusCorporaFile(string filePath);
	/**
	 * @brief test corpus : folder
	 * @date 2013-06-14
	 */
	void testRusCorporaFolder(string path, string fileType);
	/**
	 * @brief write RusCorpora test log to file
	 * @date 2013-06-14
	 */
	void testRusCorporaWriteLogToFile(string filePath);
	
	/**
	 * @brief read RusCorpora feature map
	 * @date 2013-06-22
	 */
	void readRusCorporaFeatureMap(string filePath);

	/**
	 * @brief read StressAndLemmaLog.txt
	 * for function readRusCorporaFeatureMap(string filePath)
	 * @date 2013-06-22
	 */
	void readAccentLogFile(string filePath);

	/**
	 * @brief init feature score
	 * @date 2013-07-01
	 */
	void initFeatureScore();

	/**
	 * @brief get feature score
	 * @date 2013-07-01
	 */
	double getFeatureScore(int featureId);

	/**
	 * @brief compare 2 lists of features
	 * @date 2013-07-01
	 */
	double featureListMatchScore(vector<int> featureIdList_1, vector<int> featureIdList_2);

	/**
	 * @brief analyze RusCorpora
	 * update to Dictionary
	 * @date 2013-07-01
	 */
	void analyzeRusCorporaVer2_word_updateAccent_2013_07_01(wstring word, int stressPos, vector<int> featureIdList);

	/**
	 * @brief analyze file contain list of words from Accent.txt that have more than 1 stress variant
	 * E:\Projects\2012_12_24_Russian_Dialog_System\___Data\2013_06_14_TestAccentRusCorpora\StressAndLemmaLog.txt
	 * Save these words to map
	 * Then analyze RusCorpora to find variants for them
	 *
	 * analyze update accent
	 *
	 * @date 2013-06-22
	 */
	void analyzeRusCorporaVer2_word_updateAccent(wstring word, int stressPos, vector<int> featureIdList);

	/**
	 * @brief analyze file contain list of words from Accent.txt that have more than 1 stress variant
	 * E:\Projects\2012_12_24_Russian_Dialog_System\___Data\2013_06_14_TestAccentRusCorpora\StressAndLemmaLog.txt
	 * Save these words to map
	 * Then analyze RusCorpora to find variants for them
	 *
	 * analyze word
	 *
	 * @date 2013-06-22
	 */
	void analyzeRusCorporaVer2_word(wstring word);

	/**
	 * @brief analyze file contain list of words from Accent.txt that have more than 1 stress variant
	 * E:\Projects\2012_12_24_Russian_Dialog_System\___Data\2013_06_14_TestAccentRusCorpora\StressAndLemmaLog.txt
	 * Save these words to map
	 * Then analyze RusCorpora to find variants for them
	 *
	 * analyze line
	 *
	 * @date 2013-06-22
	 */
	void analyzeRusCorporaVer2_line(wstring line);

	/**
	 * @brief analyze file contain list of words from Accent.txt that have more than 1 stress variant
	 * E:\Projects\2012_12_24_Russian_Dialog_System\___Data\2013_06_14_TestAccentRusCorpora\StressAndLemmaLog.txt
	 * Save these words to map
	 * Then analyze RusCorpora to find variants for them
	 *
	 * analyze file
	 *
	 * @date 2013-06-22
	 */
	void analyzeRusCorporaVer2_file(string filePath);

	/**
	 * @brief analyze file contain list of words from Accent.txt that have more than 1 stress variant
	 * E:\Projects\2012_12_24_Russian_Dialog_System\___Data\2013_06_14_TestAccentRusCorpora\StressAndLemmaLog.txt
	 * Save these words to map
	 * Then analyze RusCorpora to find variants for them
	 *
	 * analyze folder
	 *
	 * @date 2013-06-22
	 */
	void analyzeRusCorporaVer2_folder(string path, string fileType);

protected:
	// log file
	wofstream flog;
	// count duplications
	int dupCount;

	// duplications

	// map<word, set<stressPos>> stressPosLog
	map<wstring, set<int> > stressPosLog;
	// map<word, set<lemmaId>> stressPosLog
	map<wstring, set<int> > lemmaIdLog;
	// TEST RUSCORPORA
	map<wstring, RusCorporaLogElement> rusCorporaLog;
	// 2013-06-22 map<word, line> from StressAndLemmaLog.txt
	map<wstring, wstring> _stressAndLemmaLog;
	// 2013-07-01 map<featureId, score> for evaluting importance of featureId -> for compare 2 lists of features
	map<int, double> featureScore;

	int analyzeRusCorporaVer2LogCount;
};

#endif /* _DICTIONARYTRIEACCENTUPDATE_H_ */
