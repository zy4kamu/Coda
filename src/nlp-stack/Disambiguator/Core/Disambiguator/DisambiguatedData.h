/** 
 * @file	DisambiguatedData.h
 * @brief	header file for data returned by disambiguator
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


#ifndef DISAMBIGUATED_DATA
#define DISAMBIGUATED_DATA

#include <string>
#include "Token.h"

using std::wstring;

namespace Disambiguation
{

/** 
 * @class	DisambiguatedData
 * @brief	Data returned by disambiguator
 */
struct DisambiguatedData : public Tokenization::Token
{
    /**
     * @brief	Empty constructor
     */
    DisambiguatedData()
        : Tokenization::Token()
    {
    }

     /**
     * @brief	Copy constructor
     * @param[in]	data - data to copy
     */
    DisambiguatedData(const DisambiguatedData& data)
        : Tokenization::Token(data)
        , lemma(data.lemma), label(data.label)
        , weight(data.weight), lemmaId(data.lemmaId)
    {
    }

    /**
     * @brief	Copy constructor
     * @param[in]	data - data to copy
     */
    DisambiguatedData(DisambiguatedData&& data)
        : Tokenization::Token(std::move(data))
        , weight(data.weight), lemmaId(data.lemmaId)
    {
        lemma = std::move(data.lemma);
        label = std::move(data.label);
    }

    /**
     * @brief	Constructor
     * @param[in]	content - word
     * @param[in]	punctuation - punctuation followed by the token
     * @param[in]	lemma - initial form of the token
     * @param[in]	label - morphology label of the token
     * @param[in]	weight - weight assigned to the label by the classifier
     * @param[in]	lemmaId - index of lemma in database
     */
    DisambiguatedData(
        const wstring& content
        , const vector<wstring>& punctuation
        , const wstring& source
        , bool isNextSpace
        , const wstring& lemma
        , const wstring& label
        , double weight
        , int lemmaId)
        : Tokenization::Token(content, punctuation, source, isNextSpace)
        , lemma(lemma), label(label), weight(weight), lemmaId(lemmaId)
    {
    }

    DisambiguatedData& operator =(DisambiguatedData&& data)
    {
        this->content = std::move(data.content);
        this->label = std::move(data.label);
        this->lemma = std::move(data.lemma);
        this->lemmaId = data.lemmaId;
        this->punctuation = std::move(data.punctuation);
        this->weight = data.weight;
        return *this;
    }

    wstring lemma; /**< initial form of the token*/
    wstring label; /**< morphology label of the token*/
    double weight; /**< weight assigned to the label by the classifier*/
    int lemmaId; /**< index of lemma in database*/
};

}

#endif // DISAMBIGUATED_DATA 
