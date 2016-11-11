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

#ifndef SORTING
#define SORTING

#include <algorithm>
#include <cassert>
#include <functional>

namespace Tools
{

template<class Container>
void Sort(Container& data, bool useLessFunctor = true)
{
    if (useLessFunctor) {
        std::sort(data.begin(), data.end(), std::less<typename Container::value_type>());
    } else {
        std::sort(data.begin(), data.end(), std::greater<typename Container::value_type>());
    }
}

template <class Container1, class Container2>
void Sort(Container1& data1, Container2& data2, bool useLessFunctor = true)
{
    typedef typename Container1::value_type T1;
    typedef typename Container2::value_type T2;
    assert(data1.size() == data2.size());
    size_t size = data1.size();
    vector<std::pair<T1,T2> > data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i)
    {
        data.push_back(std::make_pair(data1[i], data2[i]));
    }
    Sort(data, useLessFunctor);
    for (size_t i = 0; i < size; ++i)
    {
        data1[i] = data[i].first;
        data2[i] = data[i].second;
    }
}

template<class T>
struct less_second
: std::binary_function<T,T,bool>
{
   inline bool operator()(const T& lhs, const T& rhs)
   {
      return lhs.second > rhs.second;
   }
};

template<class T>
struct great_first
: std::binary_function<T,T,bool>
{
   inline bool operator()(const T& lhs, const T& rhs)
   {
      return lhs.first < rhs.first;
   }
};

}

#endif // SORTING
