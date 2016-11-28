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

#ifndef READER
#define READER

#include "FileStreamIdentifier.h"
#include "ReadConverter.h"
#include "Tools.h"

#include <fstream>
#include <iostream>
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
void ReadUnary(const string& file, Container* container)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef typename Container::value_type ValueType;
    typedef ReadConverter<String, ValueType> Converter;
    Reader reader(file);
	Tools::SetLocale(reader);
    if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0);
    }
    String line;
    while(getline(reader, line))
    {
        Tools::UnitInserter<Container>::Insert(container
            , Converter::Convert(std::move(line)));
    }
    reader.close();
}

template<class Container>
void ReadUnaryUnary(const string& file
    , Container* container
    ,  typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename Container::value_type::value_type Value;
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef ReadConverter<String, Value> Converter;
    Reader reader(file);
	Tools::SetLocale(reader);
    if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0); 
    }
    String line;
    vector<String> splitted;
    while(getline(reader, line))
    {
        typename Container::value_type value;
        splitted = Tools::Split(line, separator);
        for (size_t iter = 0; iter < splitted.size(); ++iter)
        {
            Tools::UnitInserter<typename Container::value_type>::Insert(&value
            , Converter::Convert(std::move(splitted[iter])));
        }
        Tools::UnitInserter<Container>::Insert(container
            , std::move(value));
    }
    reader.close();
}

template<class Container>
void ReadUnaryCoupling(const string& file
    , Container* container
    ,  typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    ,  typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef typename Container::value_type::mapped_type Value;
    typedef ReadConverter<String, Value> Converter;
    Reader reader(file);
	Tools::SetLocale(reader);
    if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0); 
    }
    String line;
    vector<String> splitted;
    while(getline(reader, line))
    {
        if (line.size() == 0) {
            Tools::UnitInserter<Container>::Insert(container
                , typename Container::value_type());
        } else {
            typename Container::value_type value;
            splitted = Tools::Split(line, firstSeparator);
            for (size_t iter = 0; iter < splitted.size(); ++iter)
            {
                value[splitted[iter].substr(0, splitted[iter].find(secondSeparator))]
                    = Converter::Convert(std::move(
                        splitted[iter].substr(splitted[iter].find(secondSeparator) + 1))
                    );
            }
            Tools::UnitInserter<Container>::Insert(container
                , std::move(value));
        }
    }
    reader.close();
}

template<class Container>
void ReadCoupling(const string& file
    , Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char separator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename Container::key_type Key;
    typedef typename Container::mapped_type Value;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef ReadConverter<String, Key> KeyConverter;
    typedef ReadConverter<String, Value> ValueConverter;
    Reader reader(file);
	Tools::SetLocale(reader);
    if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0); 
    }
    String line;
    while(getline(reader, line))
    {
        size_t tabPosition = line.find(separator);
        try {
            (*container)[KeyConverter::Convert(line.substr(0, tabPosition))]
                = ValueConverter::Convert(line.substr(tabPosition + 1));
        } catch(...) {
            continue;
        }
    }
    reader.close();
}

template<class Container>
void ReadCouplingUnary(const string& file
    , Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename Container::key_type Key;
    typedef typename Container::mapped_type Value;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef typename Container::mapped_type::value_type SubValue;
    typedef ReadConverter<String, Key> KeyConverter;
    typedef ReadConverter<String, SubValue> SubValueConverter;
    Reader reader(file);
	Tools::SetLocale(reader);
	if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0); 
    }
    String line;
    while(getline(reader, line))
    {
        size_t firstSeparatorPosition = line.find(firstSeparator);
        Key firstKey = KeyConverter::Convert(line.substr(0, firstSeparatorPosition));
        Value value;
        vector<String> splittedValue = Tools::Split(
            line.substr(firstSeparatorPosition + 1), secondSeparator);
        for (size_t iter = 0; iter < splittedValue.size(); ++iter)
        {
           Tools::UnitInserter<Value>::Insert(&value, SubValueConverter::Convert(
                String(splittedValue[iter])));
        }
        (*container)[firstKey] = value;
    }
    reader.close();
}

template<class Container>
void ReadCouplingCoupling(const string& file
    , Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char thirdSeparator)
{
    typedef typename Container::key_type FirstKey;
    typedef typename Container::mapped_type::key_type SecondKey;
    typedef typename Container::mapped_type FirstValue;
    typedef typename Container::mapped_type::mapped_type SecondValue;
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;
    typedef typename StreamOptions::Reader Reader;
    typedef typename StreamOptions::String String;
    typedef ReadConverter<String, FirstKey> FirstKeyConverter;
    typedef ReadConverter<String, SecondKey> SecondKeyConverter;
    typedef ReadConverter<String, SecondValue> SecondValueConverter;
    Reader reader(file);
	Tools::SetLocale(reader);
    if (!reader)
    {
        std::wcout << L"Couldn't open file " 
                   << Tools::ConvertStringToWstring(file) << std::endl; 
        exit(0); 
    }
    String line;
    while(getline(reader, line))
    {
        size_t firstSeparatorPosition = line.find(firstSeparator);
        FirstKey firstKey = FirstKeyConverter::Convert(line.substr(0, firstSeparatorPosition));
        FirstValue firstValue;
        vector<String> splittedFirstValue = Tools::Split(
            line.substr(firstSeparatorPosition + 1)
            , secondSeparator);
        for (size_t iter = 0; iter < splittedFirstValue.size(); ++iter)
        {
            String& currentSecondItem = splittedFirstValue[iter];
            size_t secondSeparatorPosition = currentSecondItem.find(thirdSeparator);
            firstValue[SecondKeyConverter::Convert(
                currentSecondItem.substr(0, secondSeparatorPosition))]
                = SecondValueConverter::Convert(currentSecondItem.substr(secondSeparatorPosition + 1));
        }
        (*container)[firstKey] = firstValue;
    }
    reader.close();
}

template<class Container, int extendedContainerType>
struct Reader;

template<class Container>
struct Reader<Container, Tools::EXTENDED_UNARY>
{
    static void Read(const string& file, Container* container)
    {
        ReadUnary(file, container);
    }
};

template<class Container>
struct Reader<Container, Tools::EXTENDED_UNARY_UNARY>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Read(const string& file, Container* container)
    {
        ReadUnaryUnary(file, container
            , StreamOptions::defaultFirstSeparator);
    }

    static void Read(const string& file, Container* container
        , typename StreamOptions::Char separator)
    {
        ReadUnaryUnary(file, container, separator);
    }
};

template<class Container>
struct Reader<Container, Tools::EXTENDED_UNARY_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Read(const string& file, Container* container)
    {
        ReadUnaryCoupling(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator);
    }

    static void Read(const string& file, Container* container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator)
    {
        ReadUnaryCoupling(file, container
            , firstSeparator, secondSeparator);
    }
};

template<class Container>
struct Reader<Container, Tools::EXTENDED_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Read(const string& file, Container* container)
    {
        ReadCoupling(file, container
            , StreamOptions::defaultFirstSeparator);
    }

    static void Read(const string& file, Container* container
        , typename StreamOptions::Char separator)
    {
        ReadCoupling(file, container, separator);
    }
};

template<class Container>
struct Reader<Container, Tools::EXTENDED_COUPLING_UNARY>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Read(const string& file, Container* container)
    {
        ReadCouplingUnary(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator);
    }

    static void Read(const string& file, Container* container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator)
    {
        ReadCouplingUnary(file, container
            , firstSeparator, secondSeparator);
    }
};

template<class Container>
struct Reader<Container, Tools::EXTENDED_COUPLING_COUPLING>
{
    typedef typename GetFileStreamOptions
    <
        FileStreamIdentifier<Container>::isWideString
    >::Options StreamOptions;

    static void Read(const string& file, Container* container)
    {
        ReadCouplingCoupling(file, container
            , StreamOptions::defaultFirstSeparator
            , StreamOptions::defaultSecondSeparator
            , StreamOptions::defaultThirdSeparator);
    }

    static void Read(const string& file, Container* container
        , typename StreamOptions::Char firstSeparator
        , typename StreamOptions::Char secondSeparator
        , typename StreamOptions::Char thirdSeparator)
    {
        ReadCouplingCoupling(file, container
            , firstSeparator, secondSeparator, thirdSeparator);
    }
};

template <class Container>
void Read(const string& file, Container* container)
{
    Reader
    <
        Container,
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Read(file, container);
}

template <class Container>
void Read(const string& file, Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator)
{
    Reader
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Read(file, container, firstSeparator);
}

template <class Container>
void Read(const string& file, Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator)
{
    Reader
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Read(file, container, firstSeparator
            , secondSeparator);
}

template <class Container>
void Read(const string& file, Container* container
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char firstSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char secondSeparator
    , typename GetFileStreamOptions<FileStreamIdentifier<Container>::isWideString>::Options::Char thirdSeparator)
{
    Reader
    <
        Container, 
        Tools::ExtendedTypeIdentifier<Container>::containerType
    >::Read(file, container, firstSeparator
            , secondSeparator, thirdSeparator);
}


///////////////// INSERT IT IN THE COMMON SCHEME /////////////////////
void ReadWithThreshold(const string& file
    , unordered_map<wstring, unordered_map<wstring, int> >& data
    , int countThreshold);

};

#endif // READER
