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

#ifndef SPLITTER
#define SPLITTER

#include <string>
#include <vector>

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

namespace Tools
{

/**************** CONTAINER *************/
template <class Value> struct IsContainer
{
    static const bool value = true;
};

template <> struct IsContainer<const std::string>
{
    static const bool value = false;
};

template <> struct IsContainer<const std::wstring>
{
    static const bool value = false;
};

template <> struct IsContainer<const char*>
{
    static const bool value = false;
};

template <int N> 
struct IsContainer<const char[N]>
{
    static const bool value = false;
};

template <> struct IsContainer<const wchar_t*>
{
    static const bool value = false;
};

template <int N> 
struct IsContainer<const wchar_t[N]>
{
    static const bool value = false;
};

template <> struct IsContainer<const char>
{
    static const bool value = false;
};

template <> struct IsContainer<const wchar_t>
{
    static const bool value = false;
};

/**************** CHAR TRAITS ***********/

template <class Value> struct CharTraits
{
    static const bool isChar = false;
    typedef Value StringType;

    static size_t GetSize(const Value& value) 
    {
        return value.size();
    }
};

template <size_t N> struct CharTraits<const char[N]>
{
    static const bool isChar = false;
    typedef std::string StringType;

    static size_t GetSize(const char (&value)[N]) 
    {
        return N - 1;
    }
};

template <size_t N> struct CharTraits<const wchar_t[N]>
{
    static const bool isChar = false;
    typedef std::wstring StringType;

    static size_t GetSize(const wchar_t (&value)[N]) 
    {
        return N - 1;
    }
};

template<> struct CharTraits<const char>
{
    static const bool isChar = true;
    typedef char StringType;

    static size_t GetSize(char value) 
    {
        return 1;
    }
};

template<> struct CharTraits<const wchar_t>
{
    static const bool isChar = true;
    typedef wchar_t StringType;

    static size_t GetSize(wchar_t value) 
    {
        return 1;
    }
};

/**************** SPLITTER CLASS ***********/

template <class String, class Container, bool isContainer>
struct Splitter;

template <class String, class Container>
struct Splitter<String, Container, true>
{
    static vector<String> Split(
        const String& line, const Container& delimeters
        , bool dropEmptyString)
    {
        vector<String> splitted;
        String line_copy = line;
        while (line_copy.size() > 0)
        {
            size_t minimalPosition = -1;
            typename Container::const_iterator currentIter;
            for (auto containerIter = delimeters.begin()
                ; containerIter != delimeters.end()
                ; ++containerIter)
            {
                size_t foundPosition = line_copy.find(*containerIter);
                if (foundPosition != String::npos
                    && foundPosition < minimalPosition)
                {
                    minimalPosition = foundPosition;
                    currentIter = containerIter;
                }
            }
            if (minimalPosition == size_t(-1)) {
                splitted.push_back(std::move(line_copy));
                return splitted;
            } else {
                if (minimalPosition != 0) {
                    splitted.push_back(line_copy.substr(0, minimalPosition));
                } else if (!dropEmptyString) {
                    splitted.push_back(String());
                }
                line_copy = line_copy.substr(
                    minimalPosition 
                    + CharTraits<const typename Container::value_type>::GetSize(*currentIter));
            } 
        }
        return splitted;
    }
};

template <class String, class Container>
struct Splitter<String, Container, false>
{
    static vector<String> Split(
        const String& line, const Container& delimeter
        , bool dropEmptyString)
    {
        vector<String> splitted;
        String line_copy = line;
        while (line_copy.size() > 0)
        {
            size_t minimalPosition = line_copy.find(delimeter);
            if (minimalPosition == size_t(-1)) {
                splitted.push_back(std::move(line_copy));
                return splitted;
            } else {
                if (minimalPosition != 0) {
                    splitted.push_back(line_copy.substr(0, minimalPosition));
                } else if (!dropEmptyString) {
                    splitted.push_back(String());
                }
                line_copy = line_copy.substr(
                    minimalPosition 
                    + CharTraits<const Container>::GetSize(delimeter));
            } 
        }
        return splitted;
    }
};

/**************** SPLITTER FUNCTION ***********/

template <class String, class Container>
vector<String> Split(
    const String& line, const Container& delimeters
    , bool dropEmptyString = false)
{
    return Splitter<String, Container, IsContainer<const Container>::value>::Split(
        line, delimeters, dropEmptyString);
}

template <class String, class Delimeter>
vector<String> Split(
    const String& line
    , const Delimeter& firstDelimeter
    , const Delimeter& secondDelimeter
    , bool dropEmptyString = false)
{
    typedef typename CharTraits<const Delimeter>::StringType StringConvertionType;
    std::vector<StringConvertionType> delimeters;
    delimeters.push_back(StringConvertionType(firstDelimeter));
    delimeters.push_back(StringConvertionType(secondDelimeter));
    return Splitter<String, std::vector<StringConvertionType>, true>::Split(
        line, delimeters, dropEmptyString);
}

template <class String, class Delimeter>
vector<String> Split(
    const String& line
    , const Delimeter& firstDelimeter
    , const Delimeter& secondDelimeter
    , const Delimeter& thirdDelimeter
    , bool dropEmptyString = false)
{
    typedef typename CharTraits<const Delimeter>::StringType StringConvertionType;
    std::vector<StringConvertionType> delimeters;
    delimeters.push_back(StringConvertionType(firstDelimeter));
    delimeters.push_back(StringConvertionType(secondDelimeter));
    delimeters.push_back(StringConvertionType(thirdDelimeter));
    return Splitter<String, std::vector<StringConvertionType>, true>::Split(
        line, delimeters, dropEmptyString);
}

template <class String, class Delimeter>
vector<String> Split(
    const String& line
    , const Delimeter& firstDelimeter
    , const Delimeter& secondDelimeter
    , const Delimeter& thirdDelimeter
    , const Delimeter& forthDelimeter
    , bool dropEmptyString = false)
{
    typedef typename CharTraits<const Delimeter>::StringType StringConvertionType;
    std::vector<StringConvertionType> delimeters;
    delimeters.push_back(StringConvertionType(firstDelimeter));
    delimeters.push_back(StringConvertionType(secondDelimeter));
    delimeters.push_back(StringConvertionType(thirdDelimeter));
    delimeters.push_back(StringConvertionType(forthDelimeter));
    return Splitter<String, std::vector<StringConvertionType>, true>::Split(
        line, delimeters, dropEmptyString);
}

template <class String, class Delimeter>
vector<String> Split(
    const String& line
    , const Delimeter& firstDelimeter
    , const Delimeter& secondDelimeter
    , const Delimeter& thirdDelimeter
    , const Delimeter& forthDelimeter
    , const Delimeter& fifthDelimeter
    , bool dropEmptyString = false)
{
    typedef typename CharTraits<const Delimeter>::StringType StringConvertionType;
    std::vector<StringConvertionType> delimeters;
    delimeters.push_back(StringConvertionType(firstDelimeter));
    delimeters.push_back(StringConvertionType(secondDelimeter));
    delimeters.push_back(StringConvertionType(thirdDelimeter));
    delimeters.push_back(StringConvertionType(forthDelimeter));
    delimeters.push_back(StringConvertionType(fifthDelimeter));
    return Splitter<String, std::vector<StringConvertionType>, true>::Split(
        line, delimeters, dropEmptyString);
}

template<class ContainerType, class LastT>
void addTail(std::vector<ContainerType>& container, bool& out, const LastT& last, bool in)
{
    container.push_back(ContainerType(last));
    out = in;
}

template<class ContainerType, class LastT>
void addTail(std::vector<ContainerType>& container, bool& out, const LastT& last)
{
    container.push_back(ContainerType(last));
    out = false; //simulate default false
}

template<class ContainerType, class HeadT, class... TailT>
void addTail(std::vector<ContainerType>& container, bool& out, const HeadT& head,  const TailT&... tail)
{
    container.push_back(ContainerType(head));
    addTail(container, out, tail...);
}

//template <class String, typename Delimeter, typename... Delimeters>
//vector<String> Split(
//    const String& line
//    , const Delimeter& delimiter
//    , const Delimeters&... tail
//    //, bool dropEmptyString = false
//        )
//{
//    typedef typename CharTraits<const Delimeter>::StringType StringConvertionType;
//    std::vector<StringConvertionType> delimeters;
//    delimeters.push_back(delimiter);
//    bool dropEmptyString = false;
//    addTail(delimeters, dropEmptyString, tail...);
//    return Splitter<String, std::vector<StringConvertionType>, true>::Split(
//        line, delimeters, dropEmptyString);
//}

};

#endif // SPLITTER
