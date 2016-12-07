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

#ifndef LOCALE_SETTINGS
#define LOCALE_SETTINGS

#include <fstream>
#include <locale>
#include <string>
#ifdef MSVC
#include <codecvt>
#endif // MSVC


#include "Language.h"

namespace Tools
{

/************ STREAM CHAR IDENTIFIER **************/

template<class Stream>
struct StreamCharIdentifier;

template<>
struct StreamCharIdentifier<std::ifstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::fstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::ofstream> { typedef char Char; };

template<>
struct StreamCharIdentifier<std::wifstream> { typedef wchar_t Char; };

template<>
struct StreamCharIdentifier<std::wofstream> { typedef wchar_t Char; };

template<>
struct StreamCharIdentifier<std::wfstream> { typedef wchar_t Char; };

/*********** SET STREAM LOCALE FUNCTION ******************/

/**
* @brief	Makes thousands separator equal 0
*/
template<class Char>
struct no_separator : std::numpunct<Char>
{
protected:
    virtual std::string do_grouping() const
    {
        return "\000";
    }
};

template<class Stream>
void SetLocale(Stream& stream, Tools::Language language = Tools::Language::RU)
{
    typedef typename StreamCharIdentifier<Stream>::Char Char;
#ifdef MSVC
    stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<Char>));
#else
    const std::string& localeString = Tools::LanguageToLocaleString(language);
    stream.imbue(
        std::locale(
            std::locale(localeString.c_str()),
            new no_separator<Char>));
#endif
}

/*********** SET CONSOLE STREAM FUNCTION ******************/

void PrepareConsole(Tools::Language language = Tools::Language::RU);

}

#endif // LOCALE_SETTINGS
