/** 
 * @file	Tokenizer.h
 * @brief	header file for tokenizer
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


#ifndef TOKENIZER_RUS
#define TOKENIZER_RUS

#include <unordered_set>

#include "ITokenizer.h"

using std::unordered_set;

namespace Tokenization
{

/** 
 * @class	Tokenizer
 * @brief	Enables to split sentence vy tokens
 */
class TokenizerRus : public ITokenizer
{
public:  
    /**
     * @brief	Constructor
     * @param[in]	muliplePartTokensFile - file with exception tokens (êàê áóäòî)
     * @param[in]	dashTokensFile - file with exception tokens (êàê-íèáóäü)
     */
    TokenizerRus(
        const string& muliplePartTokensFile = string(TOKENIZER_CONFIG_FOLDER) + "RU/multiplePartTokens.txt",
        const string& dashTokensFile = string(TOKENIZER_CONFIG_FOLDER) + "RU/dashTokens.txt");

    /**
     * @brief	Tokenizeses input sentence
     * @param[in]	sentence - input sentence
     * @param[out]	tokens - tokens of the sentence
     */
	vector<Token> Tokenize(const wstring& sentence) const;

    /**
     * @TODO: not implemented for Russian. Simply tokenize
     */
    virtual vector<Token> TokenizeWithAlignment(
            const wstring& sentence, vector<size_t>& ) const { return Tokenize(sentence); };

    virtual Tools::Language GetLanguage() const;
private:
    unordered_set<wstring> muliplePartTokens; /**< Tokens that consist of more than two parts*/
    unordered_set<wstring> dashTokens; /**< Tokens that contains dash*/

    wstring replaceSpecialSymbols(const wstring& sentence) const;
    wstring alignSentence(const wstring& sentence) const;
    wstring applyMultipleTokensRule(const wstring& sentence) const;
    void applyFirstSymbolPunctuationRule(vector<Token>* tokens) const;
    void removePunctuation(wstring* token, vector<wstring>* punctuation) const;

    /**
     * @brief	Adds spaces from left and right to the sentence
     * @param[in]	sentence - input sentence
     * @param[out]  untrimmed - input sentence with spaces
     */
    wstring inverseTrim(const wstring& sentence) const;
};

}

#endif // TOKENIZER_RUS

