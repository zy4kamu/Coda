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

#ifndef STRING_CONVERTIONS
#define STRING_CONVERTIONS

#include <locale>
#include <string>
#include <sstream>
#include <cstdlib>
#include <memory>

#include "Locale/RussianLocale.h"

using std::string;
using std::wstring;
using std::stringstream;
using std::wstringstream;
using std::locale;

#ifdef MSVC
    #define CONVERT_TO_WSTRING Tools::ConvertToWstring
    #define CONVERT_TO_UTF8_STRING Tools::ConvertToString
#else
    #define CONVERT_TO_WSTRING Tools::ConvertUTF8StringToWstring
    #define CONVERT_TO_UTF8_STRING Tools::ConvertWstringToUTF8
#endif

namespace Tools
{

void Convert(const char* from, int size, string& to); // Not needed

void Convert(const wchar_t* from, int size, wstring& to); // Not needed

/**
* @brief	Converts char* to string
* @param[in]	from - char* to convert
* @param[in]	size - number of chars in from
* @param[out]	to - the result of convertation
*/
void Convert(const char* from, int size, string& to); // Not needed

/**
* @brief	Converts wchar_t* to wstring
* @param[in]	from - wchar_t* to convert
* @param[in]	size - number of wchar_t's in from
* @param[out]	to - the result of convertation
*/
void Convert(const wchar_t* from, int size, wstring& to); // Not needed

/**
* @brief	Converts wchar_t to wstring
* @param[in]	ch - input character
*/
wstring ConvertWCharToWString(wchar_t ch);

/**
* @brief	Converts the number to string
* @param[in]	number - the number to convert
*/
string ConvertIntToString(int number);

/**
* @brief	Converts the number to string
* @param[in]	number - the number to convert
*/
string ConvertDoubleToString(double number);

/**
* @brief	Converts the number to wstring
* @param[in]	number - the number to convert
* @param[in]	precision - precision
*/
string ConvertDoubleToString(double number, size_t precision);

/**
* @brief	Converts the number to wstring
* @param[in]	number - the number to convert
*/
wstring ConvertIntToWstring(int number);

/**
* @brief	Converts the number to wstring
* @param[in]	number - the number to convert
*/
wstring ConvertDoubleToWstring(double number);

/**
* @brief	Converts the number to wstring
* @param[in]	number - the number to convert
* @param[in]	precision - precision
*/
wstring ConvertDoubleToWstring(double number, size_t precision);

/**
* @brief	Converts wstr to string
* @param[in]	wstr - wstring to convert
* @param[out]	result - returned string
*/
bool ConvertWstringToString(const wstring wstr, string& result , locale loc = std::locale());  // !

/**
* @brief	Converts the string to wstring
* @param[in]	str - string to convert
*/
wstring ConvertStringToWstring(const string& str); // !

/**
* @brief	Converts char* to wstring
* @param[in]	cstr - char* to convert
*/
wstring ConvertCharPtrToWstring(const char* cstr);

/**
@brief Translates int to string
@param number Number to convert
*/
string ToString(int number);

/**
@brief Translates int to wstring
@param number Number to convert
*/
wstring ToWstring(int number);

/**
 * @brief convert string in UTF8 encoding to wstring
 * @param [in] str null-terminated UTF8 string
 * @return if conversion succeeded, converted string; empty string otherwise
 * @remark uses windows function MultiByteToWideChar
 */
wstring ConvertUTF8StringToWstring(const std::string& str, locale loc = std::locale()); // !

/**
 * @brief convert string in UTF8 encoding to wstring
 * @param [in] str input wstring
 * @param [out] result UTF8 string
 * @return true if conversion succeeded; false otherwise
 * @remark uses windows function WideCharToMultiByte
 */
bool ConvertWstringToUTF8(const wstring& wstr, string& result, locale loc = std::locale()); // !

// converts string to string (in current locale encoding)
bool ConvertToString( const wstring wstr, string& result );

// converts string (in current locale encoding) to wstring
wstring ConvertToWstring( const string str );

/**
 * @brief	Function for convering number in string (type of string) view into digital equivalent
 * @param[in]	str - given string
 * @param[out]	number - result of converting
 * @remark	Example: string "123" function will convert to number 123 (int)
 * @return	Return TRUE if converting terminated successfully, FALSE otherwise
 */
bool ConvertToInt(const string& str, int& number);

/**
 * @brief	Function for convering number in string (type of wstring) view into digital equivalent
 * @param[in]	str - given string
 * @param[out]	number - result of converting
 * @remark	Example: string "123" function will convert to number 123 (int)
 * @return	Return TRUE if converting terminated successfully, FALSE otherwise
 */
bool ConvertToInt(const wstring& str, int& number);

/**
 * @brief	Function for convering number in string view into digital equivalent
 * @param[in]	number - given number
 * @param[out]	result - result of converting
 * @remark	Example: number 123 (int) function will convert to string "123" (wstring)
 * @return	Return TRUE if converting terminated successfully, FALSE otherwise
 */
bool IntToWstring(const int number, wstring& result);


};


#endif // STRING_CONVERTIONS
