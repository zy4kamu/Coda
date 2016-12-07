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

#ifndef FILE_STREAM_IDENTIFIER
#define FILE_STREAM_IDENTIFIER

#include <iostream>
#include <string>

#include "Tools.h"

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

namespace FileManager
{
// DETERMINE COMMON TYPES BY STREAM

template<class Stream>
struct StringTypeIdentifierByStream
{
    typedef std::string String;
    typedef char Char;
};

template<>
struct StringTypeIdentifierByStream<std::wifstream>
{
    typedef std::wstring String;
    typedef wchar_t Char;
};

template<>
struct StringTypeIdentifierByStream<std::wofstream>
{
    typedef std::wstring String;
    typedef wchar_t Char;
};

template<>
struct StringTypeIdentifierByStream<std::wstringstream>
{
    typedef std::wstring String;
    typedef wchar_t Char;
};

// DETERMINE COMMON TYPES BY STRING

template <class StringType> struct FileStreamOptions
{
    typedef std::ifstream Reader;
    typedef std::ofstream Writer;
    typedef std::string String;
    typedef char Char;
    static const char endLineCharacter = '\n';
    static const char defaultFirstSeparator = '\t';
    static const char defaultSecondSeparator = '~';
    static const char defaultThirdSeparator = '*';
    static const bool isWideString = false;
};

template<> struct FileStreamOptions<std::wstring>
{
    typedef std::wifstream Reader;
    typedef std::wofstream Writer;
    typedef std::wstring String;
    typedef wchar_t Char;
    static const wchar_t endLineCharacter = L'\n';
    static const wchar_t defaultFirstSeparator = L'\t';
    static const wchar_t defaultSecondSeparator = L'~';
    static const wchar_t defaultThirdSeparator = L'*';
    static const bool isWideString = true;
};

template <bool> struct GetFileStreamOptions
{
    typedef FileStreamOptions<std::wstring> Options;
};

template <> struct GetFileStreamOptions<false>
{
    typedef FileStreamOptions<std::string> Options;
};

// DETERMINE STRING TYPE BY CONTAINER

template <class Container, int ExtendedContainerType>
struct _FileStreamIdentifier;

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_UNARY>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::value_type>::isWideString;
};

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_UNARY_UNARY>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::value_type::value_type>::isWideString;
};

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_UNARY_COUPLING>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::value_type::key_type>::isWideString
        || FileStreamOptions<typename Container::value_type::mapped_type>::isWideString;
};

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_COUPLING>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::key_type>::isWideString
        || FileStreamOptions<typename Container::mapped_type>::isWideString;
};

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_COUPLING_UNARY>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::key_type>::isWideString
        || FileStreamOptions<typename Container::mapped_type::value_type>::isWideString;
};

template <class Container>
struct _FileStreamIdentifier<Container, Tools::EXTENDED_COUPLING_COUPLING>
{
    static const bool isWideString 
        = FileStreamOptions<typename Container::key_type>::isWideString
        || FileStreamOptions<typename Container::mapped_type::key_type>::isWideString
        || FileStreamOptions<typename Container::mapped_type::mapped_type>::isWideString;
};

template <class Container>
struct FileStreamIdentifier
{
    static const bool isWideString 
        = _FileStreamIdentifier
        <
            Container,
            Tools::ExtendedTypeIdentifier<Container>::containerType
        >::isWideString;
};

};

#endif // FILE_STREAM_IDENTIFIER
