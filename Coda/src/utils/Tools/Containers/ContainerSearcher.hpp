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

#ifndef CONTAINER_SEARCHER
#define CONTAINER_SEARCHER

namespace Tools
{

/* DEFINITIONS */

template <class Container>
bool Contains(const Container& container, const typename Container::value_type& elem);

template <class Container>
bool ContainsKey(const Container& container, const typename Container::key_type& key);

template <class Container>
bool ContainsValue(const Container& container, const typename Container::mapped_type& val);

/* REALIZATION */

template <class Container>
bool Contains(const Container& container, const typename Container::value_type& elem)
{
    return std::find(container.begin(), container.end(), elem) != container.end();
}

template <class Container>
bool ContainsKey(const Container& container, const typename Container::key_type& key)
{
    return container.find(key) != container.end();
}

template <class Container>
bool ContainsValue(const Container& container, const typename Container::mapped_type& val)
{
    return std::find_if(
        container.begin(), container.end()
        , [&](const typename Container::value_type& item) { return item.second == val; }) != container.end();
}

}


#endif // CONTAINER_SEARCHER
