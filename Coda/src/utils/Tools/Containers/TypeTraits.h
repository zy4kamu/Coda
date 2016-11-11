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

#ifndef CONTAINER_TYPE_IDENTIFIER
#define CONTAINER_TYPE_IDENTIFIER

#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

using std::vector;
using std::set;
using std::map;
using std::unordered_map;
using std::unordered_set;

namespace Tools
{

/*********** CONST REMOVER *************/

template<class T>
struct ConstRemover
{
    typedef typename std::remove_const<T> Type;
};

template<class T1, class T2>
struct ConstRemover<std::pair<T1, T2> >
{
    typedef typename std::pair<typename std::remove_const<T1>::type, typename std::remove_const<T2>::type > Type;
};

/********** CONTAINER IDENTIFIER ***************/


static const int UNARY = 0;
static const int COUPLING = 1;
static const int NOT_IMPLEMENTED = 2;

static const int EXTENDED_UNARY = 0; // vector<int>
static const int EXTENDED_UNARY_UNARY = 1; // vector<vector<int> >
static const int EXTENDED_UNARY_COUPLING = 2; // vector<map<int, int> >
static const int EXTENDED_COUPLING = 3; // map<int, int>
static const int EXTENDED_COUPLING_UNARY = 4; // map<int, vector<int> >
static const int EXTENDED_COUPLING_COUPLING = 5; // map<int, map<int, int> >
static const int EXTENDED_NOT_IMPLEMENTED = 6;

template <class Container>
struct TypeIdentifier
{
    static const int containerType = NOT_IMPLEMENTED;
};

template <class T>
struct TypeIdentifier<vector<T> >
{
    static const int containerType = UNARY;
};

template <class T>
struct TypeIdentifier<set<T> >
{
    static const int containerType = UNARY;
};

template <class T>
struct TypeIdentifier<unordered_set<T> >
{
    static const int containerType = UNARY;
};

template <class Key, class Value>
struct TypeIdentifier<map<Key, Value> >
{
    static const int containerType = COUPLING;
};

template <class Key, class Value>
struct TypeIdentifier<unordered_map<Key, Value> >
{
    static const int containerType = COUPLING;
};

template <class Container, int _containerType, int valueType>
struct EEExtendedTypeIdentifier
{
    static const int containerType = EXTENDED_NOT_IMPLEMENTED;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, UNARY, NOT_IMPLEMENTED>
{
    static const int containerType = EXTENDED_UNARY;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, UNARY, UNARY>
{
    static const int containerType = EXTENDED_UNARY_UNARY;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, UNARY, COUPLING>
{
    static const int containerType = EXTENDED_UNARY_COUPLING;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, COUPLING, NOT_IMPLEMENTED>
{
    static const int containerType = EXTENDED_COUPLING;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, COUPLING, UNARY>
{
    static const int containerType = EXTENDED_COUPLING_UNARY;
};

template <class Container>
struct EEExtendedTypeIdentifier<Container, COUPLING, COUPLING>
{
    static const int containerType = EXTENDED_COUPLING_COUPLING;
};

template <class Container, int memberType>
struct EExtendedTypeIdentifier;

template <class Container>
struct EExtendedTypeIdentifier<Container, COUPLING>
{
    static const int containerType = EEExtendedTypeIdentifier
    <
        Container
        , COUPLING
        , TypeIdentifier<typename Container::mapped_type>::containerType
    >::containerType;
};

template <class Container>
struct EExtendedTypeIdentifier<Container, UNARY>
{
    static const int containerType = EEExtendedTypeIdentifier
    <
        Container
        , UNARY
        , TypeIdentifier<typename Container::value_type>::containerType
    >::containerType;
};

template <class Container>
struct EExtendedTypeIdentifier<Container, NOT_IMPLEMENTED>
{
    static const int containerType = EXTENDED_NOT_IMPLEMENTED;
};

template <class Container>
struct ExtendedTypeIdentifier
{
    static const int containerType = EExtendedTypeIdentifier
    <
        Container
        , TypeIdentifier<Container>::containerType
    >::containerType;
};

}

#endif // CONTAINER_TYPE_IDENTIFIER

