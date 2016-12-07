/** 
 * @file	SchemaCalculator.h
 * @brief	header file for special feature calculator based on common sentence schema representation
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


#ifndef SCHEMA_CALCULATOR
#define SCHEMA_CALCULATOR

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>

#include "SyntaxNode.h"

using std::vector;
using std::unordered_map;
using std::wstring;
using std::tuple;

namespace SyntaxParser
{

enum SchemaType
{
    EMPTY,
    PUNCTUATION,
    PUNCTUATION_WITH_PLACE_FOR_TOKEN,
    POS_PLUS_PUNCTUATION,
    EXTENDED_POS_PLUS_PUNCTUATION,
    FULL_POS_PLUS_PUNCTUATION
};

enum PriorityLevel
{
    A_PRIORITY,
    B_PRIORITY,
    C_PRIORITY,
    D_PRIORITY
};

/** 
 * @class	SchemaCalculatorRus
 * @brief	Feature calculator based on common sentence schema representation
 */
class SchemaCalculatorRus
{
public:
    /**
     * @brief	Constructor
     * @param[in]	nodes - syntax nodes to process
     */
    SchemaCalculatorRus(const vector<SyntaxNode>* nodes);

    /**
     * @brief	Calculates several schemas depending on distance between them
     * @param[in]	leftIndex - index of the left node
     * @param[in]	rightIndex - index of the right node
     */
    vector<wstring> CalculateSchema(int leftIndex, int rightIndex);

    /**
     * @brief	Calculates one schema between two nodes
     * @param[in]	level - estimates distance between tokens
     * @param[in]	leftIndex - index of the left node
     * @param[in]	rightIndex - index of the right node
     */
    wstring CalculateSchema(PriorityLevel level, int leftIndex, int rightIndex);
private:
    const vector<SyntaxNode>* nodes; /**< syntax nodes to process*/
    vector<wstring> punctuation; /**< punctuation after each token in the sentence*/
    unordered_map<wstring, int> tokenFrequency; /**< most common tokens in the corpus*/

    vector<wstring> punctuationUnits;
    vector<wstring> posUnits;
    vector<wstring> extendedPOSUnits;
    vector<wstring> fullPOSUnits;
    vector<wstring> tokenUnits;

    void initializePunctuation();

    void initializeUnits();

    /**
     * @brief	Calculates one token representation in schema
     * @param[in]   index - index of token in the sentence
     * @param[in]	level - type of schema
     */
    wstring getUnit(int index, SchemaType level);

    tuple<SchemaType, SchemaType, SchemaType, SchemaType> 
        calculateSchemaTypes(
        PriorityLevel distancelevel, int leftIndex, int rightIndex);

    wstring calculatePOSUnit(int index);

    wstring calculateExtendedPOSUnit(int index);

    wstring calculateFullPOSUnit(int index);

    wstring calculateTokenUnit(int index);

    wstring calculatePunctuationUnit(int index);
};

}

#endif // SCHEMA_CALCULATOR
