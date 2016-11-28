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

#ifndef CONFIGURATION_FILES_PREPARER
#define CONFIGURATION_FILES_PREPARER

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

#include "Chain.h"
#include "MorphologicalDictionary.h"
#include "DisambiguatorSettingsRus.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "IDisambiguatorSettings.h"
#include "OpenCorporaToSynagrusConverter.h"
#include "SchemaFeatureCalculatorRus.h"
#include "Token.h"
#include "Tools.h"

using std::vector;
using std::wstring;
using std::map;
using std::set;
using std::string;

namespace Disambiguation
{

const int FREQUENT_TOKENS_THRESHOLD = 300;

struct SentenceItem : public Tokenization::Token
{
    wstring label;
    vector<wstring> possibleStates;
};


class ConfigPreparerRus
{
typedef vector<SentenceItem> Sentence;
typedef unordered_map<wstring, unordered_map<wstring, int> > MapMapInt;
typedef TagsetConverting::OpenCorporaToSynagrusConverter ConverterType;

public:
    ConfigPreparerRus(
        shared_ptr<IDisambiguatorSettings> settings);
    void PrepareConfigurationFiles();
private:
    MapMapInt tokenLabelFrequencyDistribution;
    map<wstring, int> unitedLabels;
    map<wstring, int> unitedPOSs;
    MapMapInt translationsForUntranslatedTokens;
    vector<Sentence> sentences;
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<ConverterType> converter;
    shared_ptr<DataFunctor> trainingSetFunctor;

    void readSentences();
    void findPossibleStates();
    void getTokenFrequency();
    void getUnitedLabelsFrequency();
    void getUnitedPOSFrequency();
    void getTranslationsForUntranslatedTokens();
    void prepareFileWithTranslationsForFeatureCalculatorRus();

    wstring translateInputToOutputProperty(
        const wstring& prop) const;
};

}

#endif // CONFIGURATION_FILES_PREPARER
