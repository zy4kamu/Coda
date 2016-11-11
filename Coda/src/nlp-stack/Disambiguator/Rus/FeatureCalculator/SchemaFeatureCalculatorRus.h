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

#ifndef SCHEMA_FEATURE_CALCULATOR
#define SCHEMA_FEATURE_CALCULATOR

#include <vector>
#include <string>
#include <memory>
#include <unordered_set>
#include <map>

#include "IFeatureCalculator.h"

using std::vector;
using std::string;
using std::wstring;
using std::shared_ptr;
using std::unordered_set;
using std::map;

namespace Disambiguation
{

wstring GetUnitedLabel(
        const vector<wstring>& labels);

wstring GetUnitedPOS(
        const vector<wstring>& labels);

enum SchemaType
{
    TOKEN_PLUS_PUNCUATION
    , TOKEN
    , UNITED_MORPHOLOGY_PLUS_PUNCTUATION
    , UNITED_MORPHOLOGY
    , UNITED_POS_PLUS_PUNCTUATION
    , UNITED_POS
    , PUNCTUATION
    , EMPTY
};

struct Schema
{
    vector<int> relativeDistances;
    vector<SchemaType> schemaTypes;

    Schema(const vector<int>& relativeDistances,
        const vector<SchemaType>& schemaTypes);

    Schema(vector<int>&& relativeDistances,
        vector<SchemaType>&& schemaTypes);
};

class SchemaFeatureCalculatorRus
    : public IFeatureCalculator
{
public:
    SchemaFeatureCalculatorRus(
        shared_ptr<IFeatureCalculator> simpleFeatureCalculatorRus);

    vector<PredisambiguatedData> CalculateFeatures(
        const vector<Tokenization::Token>& sentence) const;
private:
    shared_ptr<IFeatureCalculator> simpleFeatureCalculatorRus;
    vector<Schema> schemas;

    void initializeSchemas();

    void calculateSchemaFeatures(
        const vector<PredisambiguatedData>& data
        , map<SchemaType, vector<wstring> >* schemaFeatures) const;

    wstring calculateSchema(int tokenIndex
        , const map<SchemaType, vector<wstring> >& schemaFeatures
        , const Schema& schema, int sentenceLength) const;
};

};

#endif // SCHEMA_FEATURE_CALCULATOR
