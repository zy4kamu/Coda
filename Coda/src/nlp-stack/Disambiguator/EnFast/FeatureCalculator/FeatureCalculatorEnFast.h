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

#ifndef _FEATURE_CALCULATOR_EN_FAST
#define _FEATURE_CALCULATOR_EN_FAST

#include "IFeatureCalculator.h"
#include "PreDisambiguatedData.h"
#include "MorphologicalDictionary.h"
#include "OpenCorporaToSynagrusConverter.h"
#include "Token.h"
#include "SimpleFeatureCalculatorRus.h"

using std::vector;
using std::string;
using std::wstring;
using std::unordered_map;
using std::shared_ptr;

using Tokenization::Token;

namespace Disambiguation
{

class FeatureCalculatorEnFast
    : public IFeatureCalculator
{

public:
    FeatureCalculatorEnFast(shared_ptr<MorphologicalDictionary> dictionary
        , const string& possibleTagsFile);

    virtual vector<PredisambiguatedData> CalculateFeatures(
        const vector<Tokenization::Token>& sentence) const;

private:
    shared_ptr<MorphologicalDictionary> dictionary;
    vector<wstring> possibleTags;

    wstring capitalizedFeature(const wstring& word) const;
    vector<wstring> dictionaryFeatures(const wstring& word, PredisambiguatedData* predisambiguated) const;
    wstring suffixFeature(const wstring& word, size_t size) const;
    wstring prefixFeature(const wstring& word, size_t size) const;
};

}

#endif // _FEATURE_CALCULATOR_EN_FAST
