/**
 * @file    stat-calc.h
 * @brief	header file with class and functions for converting documents to bag of word form
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


#ifndef _DOCUMENT_PROCESSOR_
#define _DOCUMENT_PROCESSOR_

#include "statcalc-data-types.h"
#include "lemmatizer.h"
#include "ISentenceSplitter.h"
#include "RegExp.h"

using namespace Tokenization;
using lemmatizer::ILemmatizer;
using _sentence_splitter::ISentenceSplitter;

namespace statcalc
{

/**
 * @class	DocumentProcessor
 * @brief	Class for converting documents to bag-of-words format and storing others good statistics...
 */
class DocumentProcessor
{
public:
    /**
     * @brief Default constructor for document processor
     * @param[in] i_tokenizer is shared pointer to object tokenizer
     * @param[in] i_lemmatizer is shared pointer to object lemmatizer
     */
    DocumentProcessor(shared_ptr<ITokenizer> i_tokenizer, shared_ptr<ILemmatizer> i_lemmatizer, shared_ptr<ISentenceSplitter> i_splitter);

    /**
     * @brief main function for converting document from plain text to bag of words
     * @param[in] i_document is document for procesing
     * @param[out] o_docBOW structure for storing result
     * @return true in case of success, false otherwise
     */
    bool processDocument(std::shared_ptr<Tools::Document> i_document, StatCalc &i_statcalc);

    /**
     * @overload processDocument(const vector<vector<DisambiguatedData> >& i_doc, WordIntStat& o_docBOW);
     * @param[in] i_doc is already disambiguated data for processing
     * @param[out] o_docBOW structure for storing result
     * @return true in case of success, false otherwise
     */
    bool processDocument(const vector<vector<DisambiguatedData> >& i_doc, StatCalc &i_statcalc);

    /**
     * @brief docBOW is getter for bag-of-words form of document
     * @return m_docBOW
     */
    unordered_map<int,int>& docBOW(void)
    {
        return m_docBOW;
    }

    /**
     * @brief cleaning m_docBOW and m_docPlain
     */
    void clean(void);

private:
    void processSentence_(wstring& currentSentence, StatCalc &i_statcalc);

    unordered_map<int,int> m_docBOW;
    RegExpTools m_reTools;
    shared_ptr<ITokenizer> m_tokenizer;
    shared_ptr<ILemmatizer> m_lemmatizer;
    shared_ptr<ISentenceSplitter> m_splitter;
};

}
#endif //_DOCUMENT_PROCESSOR_
