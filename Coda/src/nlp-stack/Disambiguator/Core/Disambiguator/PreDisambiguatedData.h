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

#ifndef PRE_DISAMBIGUATED_DATA
#define PRE_DISAMBIGUATED_DATA

#include <vector>
#include <string>
#include <unordered_map>

#include "Token.h"
#include "MorphologicalDictionary.h"

using std::vector;
using std::wstring;
using std::unordered_map;

namespace Disambiguation
{

struct PredisambiguatedData 
    : public Tokenization::Token
{
    vector<wstring> features;
    unordered_map<wstring, vector<shared_ptr<Morphology> > > gramInfo;

    PredisambiguatedData(const Tokenization::Token& data)
        : Tokenization::Token(data)
    {}

    PredisambiguatedData(Tokenization::Token&& data)
        : Tokenization::Token(std::move(data))
    {}

    PredisambiguatedData(const PredisambiguatedData& data)
        : Tokenization::Token(data)
        , features(data.features)
        , gramInfo(data.gramInfo)
    {}

    PredisambiguatedData(PredisambiguatedData&& data)
        : Tokenization::Token(std::move(data))
        , features(std::move(data.features))
        , gramInfo(std::move(data.gramInfo))
    {}
};

}

#endif // PRE_DISAMBIGUATED_DATA
