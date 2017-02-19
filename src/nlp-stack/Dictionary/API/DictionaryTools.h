/**
 * DictionaryTools.h
 * Class DictionaryTools
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
#ifndef _DICTIONARYTOOLS_H_
#define _DICTIONARYTOOLS_H_

#include "DictionaryDataTypes.h"

class Dictionary;

union DoubleChar
{
    double doubleValue;
    unsigned char charArray[8];
};

class DictionaryTools
{
public:
    DictionaryTools(Dictionary* _dic);
    DictionaryTools();
    ~DictionaryTools(void);

    /**
     * @brief convert a string to lower-case
     */
    wchar_t charToLowerCase(wchar_t ch);
	/**
	 * @brief convert a string to lower-case
	 */
    wstring toLowerCase(const wstring & _string);
	/**
	 * @brief delete all spaces in string
	 */
    wstring deleteSpaces(const wstring & _string);
	/**
	 * @brief refine string
	 */
    wstring refineString(const wstring & _string);
	/**
	 * @brief refine string
	 */
    wstring refineStringByAlphabet(const wstring & _string, const wstring & _alphabet);
	/**
	 * @brief split a string by delimiter
	 */
    vector<wstring> dictionarySplit(const wstring & str, const wstring & delimiter, bool splitByDelimiter = true);
	/**
	 * @brief get all modification of a word, trying to fix one misspell
	 */
    vector<wstring> getModifications(const wstring & word);
	/**
	 * @brief get all \u0435/\u0451 variant of a word//here was cyrrilic symbols: е,ё
	 */
    vector<wstring> getAllEeVariant(const wstring & word);
	/**
	 * @brief get all word variant
	 */
    vector<wstring> getAllWordVariant(const wstring & word, bool ignoreE, bool hyphenSplit);
	/**
	 * @brief get parent object - dictionary
	 */
	Dictionary* getDictionary(void) { return dic;}
	/**
	 * @brief check if a string begins with "\u043f\u043e"//here was cyrrilic symbols: по
	 */
    bool stringBeginsWithPo(const wstring & _str);
	/**
	 * @brief init alpha
	 */
	void initAlpha(void);
    /**
     * @brief initialize char map
     */
    void initCharMap_old(void);
    /**
     * @brief initialize char map
     */
    void initCharMap(void);
    /**
	 * @brief init alphabet
	 */
    unsigned char wcharToChar(wchar_t wchar);
	/**
	 * @brief convert char to wchar_t
	 */
    wchar_t charToWchar(unsigned char _char);
	/**
	 * @brief convert int to vector<char>(1)
	 */
    vector<unsigned char> intToCharVector1(int _number);
	/**
	 * @brief convert int to vector<char>(2)
	 */
    vector<unsigned char> intToCharVector2(int _number);
	/**
	 * @brief convert int to vector<char>(3)
	 */
    vector<unsigned char> intToCharVector3(int _number);
	/**
	 * @brief convert int to vector<char>(4)
	 */
    vector<unsigned char> intToCharVector4(int _number);
	/**
	 * @brief convert wstring to vector<char>
	 */
    vector<unsigned char> wstringToCharVector(const wstring & _str);
    /**
     * @brief convert double to vector<char>(8)
     */
    vector<unsigned char> doubleToCharVector8(double _number);
    /**
     * @brief convert vector<char>(8) to double
     */
    double charVector8ToDouble(vector<unsigned char> _charVector);
    // for Linux
	void initLinuxCharMap();
    string wchar2char(wchar_t wc);
    string wstring2string(const wstring & _wstr);
    //wstring char2wchar(char c);
    //wstring string2wstring(string _str);
    bool isSuffix(const wstring & s1, const wstring & s2);

private:
	Dictionary* dic;
	// alpha for modifications, only russian alphabet
	vector<wstring> alpha;
	// alphabet for converting wchar_t to char
    map<wchar_t, unsigned char> alphabet;
	// rAlphabet for converting char to wchar_t
    map<unsigned char, wchar_t> rAlphabet;

    // for Linux
    map<wchar_t, string> alphabetLinux;
    //map<char, wstring> linuxAlphabet;
};
#endif /* DictionaryTools */
