/**
 * TestMorphologicalAnalyze.h
 * Class TestMorphologicalAnalyze
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
#ifndef _TESTMORPHOLOGICALANALYZE_H_
#define _TESTMORPHOLOGICALANALYZE_H_

#include "DictionaryDataTypes.h"

class Dictionary;

using namespace std;

class TestMorphologicalAnalyze
{
public:
    TestMorphologicalAnalyze(Dictionary* _dic);
	~TestMorphologicalAnalyze(void);

	/**
	 * @brief Compare two vector of wstring
	 */
	int compareTwoVectorOfWstring(vector<wstring> w1, vector<wstring> w2);
	/**
	 * @brief Test one sentence
	 */
	void testSentence(wstring _sentence);
	/**
	 * @brief Test one file
	 */
	void testFile(string file_path);
	/**
	 * @brief Test one folder
	 */
	void testFolder(string _path, string _file_type);
	/**
	 * @brief Test function
	 */
	void test(void);
protected:
	int totalWords;
	int totalWordsMatch;
	wstring testLog;

	Dictionary* dic;
};
#endif /* _TESTMORPHOLOGICALANALYZE_H_ */
