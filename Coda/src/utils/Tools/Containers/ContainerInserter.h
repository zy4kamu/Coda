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

#ifndef CONTAINER_INSERTER
#define CONTAINER_INSERTER

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

/* DEFINITION */

template <class Container, class Key>
void AddToMap(Container* dictionary, const Key& key);

template <class Container, class Key, class Value>
void AddToMap(Container* dictionary, const Key& key, Value value);

template <class Container1, class Container2>
void AddMapToMap(Container1* toAdd, const Container2& fromAdd);

template <class Container, class FirstKey, class SecondKey, class Value>
void AddToMapMap(Container* dictionary, const FirstKey& firstKey
    , const SecondKey& secondKey, Value value);

template<class Container0, class Container1>
void Append(Container0* container0, const Container1& container1);

template<class Container0, class Container1>
Container0 Append(const Container0& container0, const Container1& container1);


/* REALIZATION */

enum InserterType
{
    INSERT,
    PUSH_BACK
};

template <class Container, InserterType typeOfInserting> struct UnitInserterByType;
template <class Container> struct InserterTypeIdentifier;

template<class T> 
struct InserterTypeIdentifier<vector<T> >
{
    static const InserterType Type = PUSH_BACK;
};

template<class T> 
struct InserterTypeIdentifier<set<T> >
{
    static const InserterType Type = INSERT;
};

template<class T> 
struct InserterTypeIdentifier<unordered_set<T> >
{
    static const InserterType Type = INSERT;
};

template<class Container>
struct UnitInserterByType<Container, INSERT>
{
    typedef typename Container::value_type Type;
    static void Insert(Container* container
        , Type&& data)
    {
        container->insert(std::forward<Type>(data));
    }
};

template<class Container>
struct UnitInserterByType<Container, PUSH_BACK>
{
    typedef typename Container::value_type Type;
    static void Insert(Container* container
        , Type&& data)
    {
        container->push_back(std::forward<Type>(data));
    }
};

template<class Container> struct UnitInserter
{
    typedef typename Container::value_type Type;
    static void Insert(Container* container, Type data)
    {
        UnitInserterByType<Container, InserterTypeIdentifier<Container>::Type >::Insert(
            container, std::forward<Type>(data)); 
    }
};

template <class Container, class Key>
void AddToMap(Container* dictionary, const Key& key)
{
    auto iter = dictionary->find(key);
    if (iter == dictionary->end())
    {
        (*dictionary)[key] = 1;
    } else {
        ++(iter->second);
    }
}

template <class Container, class Key, class Value>
void AddToMap(Container* dictionary, const Key& key, Value value)
{
    auto iter = dictionary->find(key);
    if (iter == dictionary->end())
    {
        (*dictionary)[key] = value;
    } else {
        iter->second += value;
    }
}

template <class Container1, class Container2>
void AddMapToMap(Container1* toAdd, const Container2& fromAdd)
{
    for (auto iter = fromAdd.begin(); iter != fromAdd.end(); ++iter)
    {
        AddToMap(toAdd, iter->first, iter->second);
    }
}

template <class Container, class FirstKey, class SecondKey, class Value>
void AddToMapMap(Container* dictionary, const FirstKey& firstKey
    , const SecondKey& secondKey, Value value)
{
    auto iter = dictionary->find(firstKey);
    if (iter == dictionary->end())
    {
        typename Container::mapped_type toAdd;
        toAdd[secondKey] = value;
        (*dictionary)[firstKey] = std::move(toAdd);
    } else {
        AddToMap(&(iter->second), secondKey, value);
    }
}

template<class Container0, class Container1>
void Append(Container0* container0, const Container1& container1)
{
    container0->insert(container0->end(), container1.begin(), container1.end());
}

template<class Container0, class Container1>
Container0 Append(const Container0& container0, const Container1& container1)
{
    Container0 copy0;
    Append(copy0, container1);
    return copy0;
}

}

#endif // CONTAINER_INSERTER
