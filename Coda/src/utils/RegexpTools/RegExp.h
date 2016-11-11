/**
 * @file	RegExp.h
 * @brief	header file for RegExpTools class for processing regular expressions
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



#ifndef _REGEXP_H_
#define _REGEXP_H_

#include <vector>
#include <string>
#include <utility>
#include <boost/regex.hpp>

using std::wstring;
using std::vector;
using std::pair;

struct CTokenWord {
	wstring Token;
	wstring Replacement;
};


const wstring Final = L"####"; /**< Flag that indicates on end of file with regexps*/
const wstring PartOfFileDelimiter = L"##"; /**<Flag that indicates on end of one part of file and begin of another part*/
const wstring RegExpDelimiter = L"#"; /**<Dilimiter of regexp and its meaning*/
const wstring PartOfRegExpVarDelimiter = L"=";/**<Delimiter of name variable and its meaning in file with regexps*/
const wstring RegExpHighLightSign = L"&";/**<HighLighter of variables using in regexp*/
const wstring CommentStringIndicator = L"////"; /**<Flag that indicates comments in file with regexps*/

namespace Tools {
	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const wstring& regex);

	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @param[out]	result - first substring matching to regex
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const wstring& regex, wstring& result);
	
	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @param[out]	result - first substring matching to regex
 	 * @param[out]	substr - vector with namerated substrings in exression
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& substr, wstring& result);

	 /**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @param[out]	result - vector of all substrings matching to regex
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& result);

	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @param[out]	result - vector of all substrings matching to regex
	 * @param[out]	idx - indexes of begining of substring in given string
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const wstring& regex, vector<wstring>& result, vector<int>& idx);

	/**
	 * @brief	Function for matching string to regular expression
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @return	Returns TRUE if string matches regexp, FALSE otherwise
	 */
	extern bool MatchStrOnRegExp(const wstring& str, const wstring& regex);

	/**
	 * @brief	Function for replacing substrings corresponding to format string
	 * @param[in]	str - given string
	 * @param[in]	regex - string defining regular expression
	 * @param[out]	format string
	 * @return	Returns TRUE if string matches regexp, FALSE otherwise
	 */
	extern wstring ReplaceRegExp(const wstring& str, const wstring& regex, const wstring& formatString);

	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const boost::wregex & xRegEx);

	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @param[out]	result - first substring matching to regex
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, wstring& result);
	
	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @param[out]	result - first substring matching to regex
 	 * @param[out]	substr - vector with namerated substrings in exression
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
    extern bool CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& substr, wstring& result);

	 /**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @param[out]	result - vector of all substrings matching to regex
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& result);

	/**
	 * @brief	Function for finding in string substring matching to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @param[out]	result - vector of all substrings matching to regex
	 * @param[out]	idx - indexes of begining of substring in given string
	 * @return	Returns TRUE if there's substring matching regexp, FALSE otherwise
	 */
	extern bool CheckStrOnRegExp(const wstring& str, const boost::wregex& xRegEx, vector<wstring>& result, vector<int>& idx);

	/**
	 * @brief	Function for matching string to regular expression
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @return	Returns TRUE if string matches regexp, FALSE otherwise
	 */
	extern bool MatchStrOnRegExp(const wstring& str, const boost::wregex& xRegEx);

	/**
	 * @brief	Function for replacing substrings corresponding to format string
	 * @param[in]	str - given string
	 * @param[in]	xRegEx - precompiled boost regexp
	 * @param[out]	format string
	 * @return	Returns TRUE if string matches regexp, FALSE otherwise
	 */
    extern wstring ReplaceRegExp(const wstring& str, const boost::wregex& xRegEx, const wstring& formatString, bool with_underscore = false);
};

	/**
	 * @class	RegExpTools
	 * @brief	Class for supporting work with dictionary of regular expressions
	 */
class RegExpTools {
public:
    /**
     * @brief loadRegexpReplacementList is function for loading simple list of regexp <-> replacement
     * @param[in] i_filename is name of file
     */
    void loadRegexpReplacementList(const std::string &i_filename);

    /**
     * @brief applyRegexReplacementList is function that apply regexpReplacementList to string
     * @param[in] i_wstring is string for replacement
     * @return result string
     */
    wstring applyRegexReplacementList(wstring& i_wstring);

	/**
	 * @brief	Function for loading regular expressions dictionary from file
	 * By default name of file storing in variable RegExpDictionaryFileName. Function stores dictionary in private member regExpDictionary
	 */
	void loadRegularExpressions();

	/**
	 * @overload	void loadRegularExpressions(wstring filename)
	 * @param[in]	filename - name of file with regular expressions. By default name of file storing in variable RegExpDictionaryFileName
	 */
	void loadRegularExpressions(wstring filename);

	/**
	 * @brief	Function that returns regular expressions dictionary stored in regExpDictionary
	 * return	Returns value of private member regExpDictionary 
	 */
	void getRegExpDictionary(std::vector<CTokenWord>& regExpDictionary_);

private:
	std::vector<CTokenWord> regExpDictionary; /**< Vector for storing regexps dictionary*/

    vector<pair<boost::wregex,wstring> > m_regexpReplacementList; /**< Vector for storing regexps and its replacements*/

	/**
	 * @brief	Function reads string with variable and adds into vector of variables name and value 
	 */
	void readRegExpVariable(const std::wstring& str, std::vector<CTokenWord>& variables) const;

	/**
	 * @brief	find in string str variables from vector and replace it into values
	 */
	void replaceRegExpVariable(std::wstring& str, const std::vector<CTokenWord>& variables) const;

	/**
	 * @brief	returns found token index, or NotFound
	 */
	int findToken( const vector<CTokenWord>& tokens, const wstring tokenToFind ) const;
};

#endif//REGEXP_H_
