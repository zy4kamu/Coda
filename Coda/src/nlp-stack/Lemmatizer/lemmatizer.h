/** 
 * @file    lemmatizer.h
 * @brief	header file with class and functions for lemmitization of words with help of dictionary
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


#ifndef _LEMMATIZER_
#define _LEMMATIZER_

#include <functional>
#include "lemmatizer-interface.h"

namespace lemmatizer
{

/**
 * @class	Lemmatizer
 * @brief	Class for lemmatizing words to its lemma with help of dictionary
 */
class Lemmatizer : public ILemmatizer
{
public:
    /**
     * @brief Default constructor
     * @param[in] i_disambiguator - pointer to disambiguator. If i_disambiguator is NULL,
     * create own disambiguator, otherwise (i_disambiguator not NULL) use outsider disambiguator
     */
    Lemmatizer(shared_ptr<IDisambiguator> i_disambiguator = NULL);

    /**
     * @brief lemmatize is the main function
     * @param[in] i_tokens is a number of tokens to lemmatization
     * @param[in] i_extractor custom processing function (see static methods)
     * @param[out] o_result is place to store result of lemmatization
     * @return true if dictionary has token, false otherwise
     */
    bool lemmatize(const vector<Token>& i_tokens, vector<wstring>& o_result, const std::function<wstring(const DisambiguatedData&)>& i_extractor);

    /**
     * @brief lemmatize is the main function
     * @param[in] i_tokens is a number of tokens to lemmatization
     * @param[out] o_result is place to store result of lemmatization
     * @return true if dictionary has token, false otherwise
     */
    bool lemmatize(const vector<Token>& i_tokens, vector<wstring>& o_result);

    /**
     * @brief tokens is getter for m_token
     * @return Function returns last token which was lemmatize
     */
    vector<Token> tokens(void)
    {
        return m_tokens;
    }

    /**
     * @brief result is getter for m_result.
     * @return Function returns result of last lemmatization
     */
    vector<wstring> result(void)
    {
        return m_result;
    }

    void clean(void);

    static wstring extractLemma(const DisambiguatedData& i_ddata)
    {
        return (i_ddata.lemma.empty()) ? i_ddata.content : i_ddata.lemma;
    }

    static wstring extractMorphology(const DisambiguatedData& i_ddata)
    {
        return i_ddata.label;
    }

    static wstring replaceUnknownTokens(const DisambiguatedData& i_ddata)
    {
        return (i_ddata.lemma.empty()) ? L"<unk>" : i_ddata.content;
    }

    static wstring lemmaAndMorphology(const DisambiguatedData& i_ddata)
    {
        wstring lemma = (i_ddata.lemmaId == -1) ? i_ddata.content : i_ddata.lemma;
        wstring morph = i_ddata.label;
        wstring o_result = lemma + L":" + morph;
        return o_result;
    }

    static wstring wordLemmaAndMorphology(const DisambiguatedData& i_ddata)
    {
        wstring lemma = (i_ddata.lemmaId == -1) ? L"#LemmaNotFoundError" : i_ddata.lemma;
        wstring morph = i_ddata.label;
        wstring o_result = i_ddata.content + L":" + lemma + L":" + morph;
        return o_result;
    }

protected:
    void addDDataToResult_(vector<DisambiguatedData>& i_ddata, const std::function<wstring(const DisambiguatedData&)>& i_extractor);

    bool m_ownDisambiguator;

    shared_ptr<DictionaryInterface> m_dict;
    shared_ptr<IDisambiguator> m_disambiguator;
    vector<Token> m_tokens;
    vector<wstring> m_result;
};

shared_ptr<ILemmatizer> GetLemmatizer(Tools::Language lang = Tools::Language::RU);
void Lemmatize(const char* fileIn, const char* fileOut, int &SentenceCounter, Tools::Language lang = Tools::Language::RU);

}
#endif //_LEMMATIZER_
