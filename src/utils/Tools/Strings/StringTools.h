/**
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

#ifndef STRING_TOOLS
#define STRING_TOOLS

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

#include "Locale/RussianLocale.h"

using std::string;
using std::wstring;
using std::vector;

static const int NOT_FOUND = -1;

namespace Tools
{

extern const std::wstring SpaceSymbols;
extern const std::wstring SpaceAndCaretSymbols;
extern const std::wstring Digits;
extern const std::wstring Punctuators;

template <class StringType> struct StringOptions
{
    typedef std::string String;
    typedef char Char;
    static const char tab = '\t';
    static const char space = ' ';
    static const char winr = '\r';
    static const char newLine = '\n';
};

template<> struct StringOptions<std::wstring>
{
    typedef std::wstring String;
    typedef wchar_t Char;
    static const wchar_t tab = L'\t';
    static const wchar_t space = L' ';
    static const wchar_t winr = L'\r';
    static const wchar_t newLine = L'\n';
};

/************** COUNT ***********************/

template <class String>
int Count(const String& data, typename StringOptions<String>::Char character)
{
    int sum = 0;
    for (size_t index = 0; index < data.size(); ++index)
    {
        if (data[index] == character)
        {
            ++sum;
        }
    }
    return sum;
}

/************** REMOVE **********************/

template <class String>
String Remove(const String& data, typename StringOptions<String>::Char character)
{
    String toRemove = data;
    toRemove.erase(
        std::remove(toRemove.begin(), toRemove.end(), character)
        , toRemove.end());
    return toRemove;
}

/************* CONCATENATE ********************/

template<class Container, class Separator> struct Concatenator;

template <class Container, int N> struct
Concatenator<Container, typename StringOptions<typename Container::value_type>::Char[N]>
{
    static typename Container::value_type Concatenate(const Container& container
        , const typename StringOptions<typename Container::value_type>::Char separator [N])
    {
        typename Container::value_type concatenated;
        typename Container::value_type separatorString(separator);
        typename Container::const_iterator iter = container.begin();
        while (iter != container.end())
        {
            concatenated += *iter;
            ++iter;
            if (iter != container.end())
            {
                concatenated += separatorString;
            }
        }
        return concatenated;
    }
};

template <class Container> struct
Concatenator<Container, typename Container::value_type>
{
    static typename Container::value_type Concatenate(
        const Container& container
        , const typename Container::value_type& separator)
    {
        typename Container::value_type concatenated;
        typename Container::const_iterator iter = container.begin();
        while (iter != container.end())
        {
            concatenated += *iter;
            ++iter;
            if (iter != container.end())
            {
                concatenated += separator;
            }
        }
        return concatenated;
    }
};

template <class Container, class Separator>
typename Container::value_type Concatenate(
    const Container& container
    , const Separator& separator)
{
    return Concatenator<Container, Separator>::Concatenate(
        container, separator);
}

/******************* CONTAINS ***************************/

bool ContainsSubstring(const string& str1, const string& str2);

bool ContainsSubstring(const wstring& str1, const wstring& str2);

/******************* FIND SUBSTRING ***************************/

int FindSubstring( const string& str, const string& substr );

int FindSubstring( const wstring& str, const wstring& substr );

/***************** TRIM ********************************/
wstring Trim_Left(const wstring& s);

string Trim_Left(const string& s);

wstring Trim_Right(const wstring& s);

string Trim_Right(const string& s);

string Trim(string s);

wstring Trim(wstring s);

/***************** REPLACE ****************************/
/**
* @brief	Replaces the first occurence of the first string by the second
* @param[out]	str - string that should be changed
* @param[in]	from - the first string
* @param[in]	to - the second string
*/
template <class String>
bool ReplaceFirst(String* str, const String& from, const String& to)
{
    size_t start_pos = str->find(from);
    if(start_pos == String::npos) {
        return false;
    } else {
        str->replace(start_pos, from.length(), to);
        return true;
    }
}

/**
* @brief	Replaces the all occurence of the first string by the second
* @param[out]	str - string that should be changed
* @param[in]	from - the first string
* @param[in]	to - the second string
*/
wstring Replace(const wstring& str, const wstring& from, const wstring& to);

/**
* @brief	Replaces the all occurence of the first string by the second
* @param[in]	str - string that should be changed
* @param[in]	from - the first string
* @param[in]	to - the second string
*/
string Replace(const string& str, const string& from, const string& to);

/******************** TO LOWER **********************************/
/**
 * @brief	Function for convering string to lowercase
 * @param[in]	wstr - given string
 */
wstring ToLower(const wstring& wstr);

/**
 * @brief	Function for convering string to lowercase
 * @param[in]	str - given string
 */
string ToLower(const string& str);

/******************** TO UPPER **********************************/
/**
 * @brief	Function for convering string to uppercase
 * @param[in]	wstr - given string
 */
wstring ToUpper(const wstring& wstr);

/********************* STARTS WITH *******************************/

bool StartsWith(const string& str1, const string& str2);

bool StartsWith(const wstring& str1, const wstring& str2);

/********************* ENDS WITH *******************************/

bool EndsWith(const string& str1, const string& str2);

bool EndsWith(const wstring& str1, const wstring& str2);

/********************* STRING TRANSFORMATIONS *******************/

/**
* @brief	Removes duplicated spaces from string
* @param[in]	str - input string
*/
string ReplaceDuplicatedSpaces(const string& str);

/**
* @brief	Removes duplicated spaces from wstring
* @param[in]	str - input string
*/
wstring ReplaceDuplicatedSpaces(const wstring& str);

/**
* @brief	Removes punctuation from the right side of the string and returns punctuation
* @param[out]	str - input (and output) string
* @param[out]	punctuation - punctuation
*/
void RemovePunctuation(wstring& str, vector<wchar_t>& punctuation);

/**
* @brief	Removes punctuation from the right side of the string and returns punctuation
* @param[out]	str - input (and output) string
* @param[out]	punctuation - punctuation
*/
void RemovePunctuation(wstring& str, wstring& punctuation);

/**
* @brief	Removes all punctuation from sentence
* @param[in]	str - input string
* @param[out]	string without punctuation
*/
wstring RemoveAllPunctuation(const wstring& str);

void ParseWstring(vector<wstring>& parsed
    , const wstring input, const wstring delimiter
    , bool addEmptyStrings );

wstring RemoveSurrounding(
    const wstring source
    , const wstring surrounding );

/********************** PUNCTUATION TO STRING *********************/

/**
* @brief	Returns the name of the punctuation (For example, ',' -> 'COMMA', '.' -> 'DOT')
* @param[in]	punctuation - input punctuation
*/
wstring PunctuationToString(const wstring& punctuation);

/**
* @brief	Returns the name of the punctuation (For example, ',' -> 'COMMA', '.' -> 'DOT')
* @param[in]	punctuation - input punctuation
*/
wstring PunctuationToString(wchar_t punctuation);


/********************** STRING PREDICATES *************************/

/**
 * @brief	Function for detection in string letters in UpperCase
 * @param[in]	const wstring str - given string
 * @return	Return TRUE if there is uppercase letter in string, FALSE otherwise
 */
bool HasUpperCaseLetter(const wstring& str);

/**
 * @brief	Function for detection in string letters in LowerCase
 * @param[in]	str - given string
 * @return	Return TRUE if there is lowercase letter in string, FALSE otherwise
 */
bool HasLowerCaseLetter(const wstring& str);

/**
 * @brief	Function for detection in string punctuation symbols
 * @param[in]	str - given string
 * @return	Return TRUE if there is punctuation symbols in string, FALSE otherwise
 */
bool HasPunctLetter(const wstring& str);

/**
 * @brief	Function for detection in string digital symbols
 * @param[in]	str - given string
 * @return	Return TRUE if there is digital symbols in string, FALSE otherwise
 */
bool HasDigitLetter(const wstring& str);

/**
 * @brief	Function for detection in string alphabetic characters
 * @param[in]	str - given string
 * @return	Return TRUE if there is alphabetic characters in string, FALSE otherwise
 */
bool HasAlphaLetter(const wstring& str);

/**
* @brief	Verifies if the string is a number
* @param[in]	str - input string
*/
bool IsNumber(const wstring& str);

/**
* @brief	Verifies if char is punctuation
* @param[in]	ch - input char
*/
bool IsPunctuation(wchar_t ch);

/**
@brief Verifies if there are lain symbols in the wide string
@param data string to process
*/
bool ContainsLatin(const wstring& data);

/**
* @brief	Verifies if input char is Upper case
* @param[in]	ch - input character
*/
bool IsUpper(wchar_t ch);

/**************** COMPARISON *************************/

template<typename T>
bool CompareNoCase( const T& first, const T& second )
{
    RussianLocale locale;

    if( first.length() != second.length() ) {
        return false;
    }
    for( unsigned int i = 0; i < first.length(); i++ ) {
        if( tolower( first[i] ) != tolower( second[i] ) ) {
            return false;
        }
    }
    return true;
}

/**************** CONSONANT AND VOWELS *****************/


/**
* @brief	Verifies if input character is russian consonant
* @param[in]	letter - input character
*/
bool IsRussianConsonant(wchar_t letter);

/**
* @brief	Verifies if input character is russian vowel
* @param[in]	letter - input character
*/
bool IsRussianVowel(wchar_t letter);

};

#endif // STRING_TOOLS
