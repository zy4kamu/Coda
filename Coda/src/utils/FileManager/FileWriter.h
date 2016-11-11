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

#ifndef WRITER
#define WRITER

#include "FileStreamIdentifier.h"
#include "ReadConverter.h"
#include "Tools.h"

#include <fstream>
#include <string>

using std::ofstream;
using std::wofstream;
using std::string;
using std::wstring;

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

namespace FileManager
{

template<class Container>
void WriteUnary(const string& file, const Container& container)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto iter = container.begin(); iter != container.end(); ++iter)
    {
        writer << *iter << std::endl;
    }
    writer.close();
}

template<class Container>
void WriteUnaryUnary(const string& file
    , const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto iter = container.begin(); iter != container.end(); ++iter)
    {
        auto lastIter = iter->end();
        --lastIter;
        for (auto secondIter = iter->begin(); secondIter != iter->end(); ++secondIter)
        {
            writer << *secondIter; 
            if (secondIter != lastIter) 
            {
                writer << separator;
            }
        }
        writer << std::endl;
    }
    writer.close();
}

template<class Container>
void WriteUnaryCoupling(const string& file
    , const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto firstIter = container.begin()
            ; firstIter != container.end(); ++firstIter)
    {
        auto secondIter = firstIter->begin();
        while (secondIter != firstIter->end())
        {
            writer << secondIter->first << secondSeparator 
                    << secondIter->second; 
            ++secondIter;
            if (secondIter != firstIter->end())
            {
                writer << firstSeparator;
            }
        }
        writer << std::endl;
    }
    writer.close();
}

template<class Container>
void WriteCoupling(const string& file
    , const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto iter = container.begin(); iter != container.end(); ++iter)
    {
        writer << iter->first << separator << iter->second << std::endl;
    }
    writer.close();
}

template<class Container>
void WriteCouplingUnary(const string& file
    , const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto firstIter = container.begin()
        ; firstIter != container.end(); ++firstIter)
    {
        writer << firstIter->first << firstSeparator;
        auto lastIterator = firstIter->second.end();
        --lastIterator;
        for (auto secondIter = firstIter->second.begin()
            ; secondIter != firstIter->second.end(); ++secondIter)
        {
            writer << *secondIter;
            if (secondIter != lastIterator)
            {
                writer << secondSeparator;
            }
        }
        writer << std::endl;
    }
    writer.close();
}

template<class Container>
void WriteCouplingCoupling(const string& file
    , const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char thirdSeparator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    for (auto firstIter = container.begin()
        ; firstIter != container.end(); ++firstIter)
    {
        writer << firstIter->first << firstSeparator;
        auto secondIter = firstIter->second.begin();
        while (secondIter != firstIter->second.end())
        {
            writer << secondIter->first << thirdSeparator
                   << secondIter->second;
            ++secondIter;
            if (secondIter != firstIter->second.end())
            {
                writer << secondSeparator;
            }
        }
        writer << std::endl;
    }
    writer.close();
}

template<class Container, int extendedContainerType>
struct Writer;

template<class Container>
struct Writer<Container, Tools::EXTENDED_UNARY>
{
    static void Write(const string& file, const Container& container)
    {
        WriteUnary(file, container);
    }
};

template<class Container>
struct Writer<Container, Tools::EXTENDED_UNARY_UNARY>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Write(const string& file, const Container& container)
    {
        WriteUnaryUnary(file, container
            , StreamOptions::defaultFirstSeparator);
    }

    static void Write(const string& file, const Container& container
        , typename StreamOptions::Char separator)
    {
        WriteUnaryUnary(file, container, separator);
    }
};

template<class Container>
struct Writer<Container, Tools::EXTENDED_UNARY_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Write(const string& file, const Container& container)
    {
        WriteUnaryCoupling(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator);
    }

    static void Write(const string& file, const Container& container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator)
    {
        WriteUnaryCoupling(file, container
            , firstSeparator, secondSeparator);
    }
};

template<class Container>
struct Writer<Container, Tools::EXTENDED_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Write(const string& file, const Container& container)
    {
        WriteCoupling(file, container
            , StreamOptions::defaultFirstSeparator);
    }

    static void Write(const string& file, const Container& container
        , typename StreamOptions::Char separator)
    {
        WriteCoupling(file, container, separator);
    }
};

template<class Container>
struct Writer<Container, Tools::EXTENDED_COUPLING_UNARY>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Write(const string& file, const Container& container)
    {
        WriteCouplingUnary(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator);
    }

    static void Write(const string& file, const Container& container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator)
    {
        WriteCouplingUnary(file, container
            , firstSeparator, secondSeparator);
    }
};

template<class Container>
struct Writer<Container, Tools::EXTENDED_COUPLING_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Write(const string& file, const Container& container)
    {
        WriteCouplingCoupling(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator
            , StreamOptions::defaultThirdSeparator);
    }

    static void Write(const string& file, const Container& container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator
        , typename StreamOptions::Char thirdSeparator)
    {
        WriteCouplingCoupling(file, container
            , firstSeparator, secondSeparator, thirdSeparator);
    }
};

template <class Container>
void Write(const string& file, const Container& container)
{
    Writer
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Write(file, container);
}

template <class Container>
void Write(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator)
{
    Writer
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Write(file, container, firstSeparator);
}

template <class Container>
void Write(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    Writer
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Write(file, container, firstSeparator
            , secondSeparator);
}

template <class Container>
void Write(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char thirdSeparator)
{
    Writer
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Write(file, container, firstSeparator
            , secondSeparator, thirdSeparator);
}

/************* WRITE SORTED MAP ******************/

template <class Container>
void WriteSortedMap(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename Container::key_type Key;
    typedef typename Container::mapped_type Value;
    // Get keys and values
    vector<Key> keys;
    vector<Value> values;
    for (auto iter = std::begin(container); iter != std::end(container);
        ++iter)
    {
        keys.push_back(iter->first);
        values.push_back(iter->second);
    }
    // Sort
    Tools::Sort(values, keys, false);
    // Create writer
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    // Print
    for (size_t index = 0; index < keys.size(); ++index)
    {
        writer << keys[index] << separator << values[index] << std::endl;
    }
    writer.close();
}

template <class Container>
void WriteSortedMap(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator
    , typename Container::mapped_type lowerBound)
{
    typedef typename Container::key_type Key;
    typedef typename Container::mapped_type Value;
    // Get keys and values
    vector<Key> keys;
    vector<Value> values;
    for (auto iter = std::begin(container); iter != std::end(container);
        ++iter)
    {
        if (iter->second >= lowerBound)
        {
            keys.push_back(iter->first);
            values.push_back(iter->second);
        }
    }
    // Sort
    Tools::Sort(values, keys, false);
    // Create writer
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    // Print
    for (size_t index = 0; index < keys.size(); ++index)
    {
        writer << keys[index] << separator << values[index] << std::endl;
    }
    writer.close();
}


/************* WRITE SORTED MAP MAP ******************/

template <class Container>
void WriteSortedTable(const string& file, const Container& container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename Container::key_type FirstKey;
    typedef typename Container::mapped_type::key_type SecondKey;
    typedef typename Container::mapped_type::mapped_type Value;
    // Sort rows in appropriate order
    vector<FirstKey> firstKeys;
    vector<Value> firstSums;
    unordered_map<SecondKey, Value> secondSumsMap;
    for (auto firstIter = container.begin(); firstIter != container.end(); ++firstIter)
    {
        firstKeys.push_back(firstIter->first);
        firstSums.push_back(0);
        for (auto secondIter = firstIter->second.begin(); secondIter != firstIter->second.end()
             ; ++secondIter)
        {
            firstSums.back() += secondIter->second;
            Tools::AddToMap(&secondSumsMap, secondIter->first, secondIter->second);
        }
    }
    Tools::Sort(firstSums, firstKeys, false);
    // Sort columns in appropriate order
    vector<SecondKey> secondKeys;
    vector<Value> secondSums;
    for (auto iter = secondSumsMap.begin(); iter != secondSumsMap.end(); ++iter)
    {
        secondKeys.push_back(iter->first);
        secondSums.push_back(iter->second);
    }
    Tools::Sort(secondSums, secondKeys, false);
    // Create writer
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Writer Writer;
    Writer writer(file);
	Tools::SetLocale(writer);
    // Write cap
    for (size_t secondIndex = 0; secondIndex < secondKeys.size(); ++secondIndex)
    {
        writer << separator << secondKeys[secondIndex];
    }
    writer << std::endl;
    // Write body
    for (size_t firstIndex = 0; firstIndex < firstKeys.size(); ++firstIndex)
    {
        writer << firstKeys[firstIndex];
        const typename Container::mapped_type& secondMap = container.at(firstKeys[firstIndex]);
        for (size_t secondIndex = 0; secondIndex < secondKeys.size(); ++secondIndex)
        {
            auto found = secondMap.find(secondKeys[secondIndex]);
            if (found != secondMap.end()) {
                writer << separator << found->second;
            } else {
                writer << separator << 0;
            }
        }
        writer << std::endl;
    }
    writer.close();
}

/*********************** WRITE VISUALISABLE ****************************/
template<class T>
void WriteVisualizable(const string& file, const vector<T>& data)
{
    wofstream out(file, std::ofstream::app);
	Tools::SetLocale(out);
    for (size_t index = 0; index < data.size(); ++index)
    {
        out << L"," << index;
    }
    out << L"\n";
    for (size_t index = 0; index < data.size(); ++index)
    {
        out << L"," << data[index];
    }
    out << L"\n";
    out.close();
}

template<class T>
void WriteVisualizable(const string& file, const vector<vector<T> >& data)
{
    wofstream out(file, std::ofstream::app);
	Tools::SetLocale(out);
    for (size_t index = 0; index < data.size(); ++index)
    {
        out << L"," << index;
    }
    out << L"\n";
    for (size_t index = 0; index < data.size(); ++index)
    {
        out << index;
        for (size_t index1 = 0; index1 < data[index].size(); ++index1)
        {
            out << L"," << data[index][index1];
        }
        out << L"\n";
    }
    out.close();
}

}

#endif // WRITER
