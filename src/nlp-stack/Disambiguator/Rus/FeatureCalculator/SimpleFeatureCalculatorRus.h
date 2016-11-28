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

#ifndef SIMPLE_FEATURE_CALCULATOR
#define SIMPLE_FEATURE_CALCULATOR

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "MorphologicalDictionary.h"
#include "IFeatureCalculator.h"
#include "NameEntityFeatureCalculatorRus.h"
#include "PreDisambiguatedData.h"
#include "OpenCorporaToSynagrusConverter.h"

using std::vector;
using std::string;
using std::wstring;
using std::unordered_map;

using TagsetConverting::OpenCorporaToSynagrusConverter;

namespace Disambiguation
{

typedef unordered_map<wstring, unordered_map<wstring, int> > MapMapInt;
typedef unordered_map<wstring, vector<wstring> > MapVec;

class SimpleFeatureCalculatorRus
    : public IFeatureCalculator
{

public:
    SimpleFeatureCalculatorRus(
        const string& tokenLabelFrequencyDistributionFile
        , const string& translationsForUntranslatedTokens
        , const string& abbreviatureFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<TagsetConverting::OpenCorporaToSynagrusConverter> converter);

    virtual vector<PredisambiguatedData> CalculateFeatures(
        const vector<Tokenization::Token>& sentence) const;
protected:
    wstring translateInputToOutputProperty(const wstring& prop) const;
private:
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<TagsetConverting::OpenCorporaToSynagrusConverter> converter;
    MapMapInt tokenLabelFrequencyDistribution;
    MapMapInt translationsForUntranslatedTokens;
    MapVec abbreviatureTags; /**< Produces conversion from abbreviature to possible Syntagrus tag*/
    NameEntityFeatureCalculatorRus nameEntityFeatureCalculatorRus;

    PredisambiguatedData calculateFeatures(
        const Tokenization::Token& token, bool isLastInSentence) const;
};

}

#endif // SIMPLE_FEATURE_CALCULATOR
