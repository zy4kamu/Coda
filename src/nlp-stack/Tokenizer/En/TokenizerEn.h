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


#ifndef TOKENIZER_EN
#define TOKENIZER_EN

#include <unordered_set>

#include "ITokenizer.h"

using std::unordered_set;

namespace Tokenization
{

struct TokenizedSentence
{
    wstring sentence;
    vector<Token> tokens;
    vector<size_t> positions;

    TokenizedSentence(wstring& i_sentence) : sentence(i_sentence) {};
};

/**
 * @class	Tokenizer
 * @brief	Tokenizes sentence in PennTreeBank style
 * @remark  Tokenization rules:
 * @remark  1. most punktuation is splitted from adjoint token to separate token 2. children's --> children 's, gonna --> gon na, I'm --> I 'm etc 3. some punktuators are normalized
 * @remark  4. bracket are replaced to secial notation (acronims): ( ) [ ] { } --> -LRB- -RRB- -RSB- -RSB- -LCB- -RCB- ((Left|Right) (Round|Square|Curly) Bracket)
 */
class TokenizerEn : public ITokenizer
{
public:
    /**
     * @brief	Constructor
     */
    TokenizerEn();

    /**
     * @brief	Tokenizes input sentence
     * @param[in]	sentence - input sentence
     * @param[out]	tokens - tokens of the sentence
     */
    vector<Token> Tokenize(const wstring& sentence) const;
    /**
     * @brief	Tokenizes input sentence, saving alignment information (token positions in source sentence)
     * @param[in]	sentence - input sentence
     * @param[out]	o_positions - token positions in source sentence
     * @param[out]	tokens - tokens of the sentence
     */
    virtual vector<Token> TokenizeWithAlignment(
            const wstring& sentence, vector<size_t>& o_positions) const;

    virtual Tools::Language GetLanguage() const;

private:
    vector<Token> processSentence(const wstring& i_sentence, vector<wstring>& o_content, vector<size_t>& o_positions) const;
    void splitLine(const wstring& i_sentence, vector<wstring>& o_content, vector<size_t>& o_positions) const;
    void splitSpecialSymbols(const wstring& i_word, vector<wstring>& o_content, vector<size_t>& o_positions, bool isSentenceEnd) const;
    wstring splitEndingPeriod(const wstring& i_word) const;
    vector<Token> fillSourceTokens(const wstring& i_sentence, const vector<wstring>& i_content, const vector<size_t>& i_positions) const;
    wstring replaceBracketsAndSpecialCharacters(const wstring& i_word) const;
    bool isSpace(wchar_t i_char) const;
    bool isPunct(wchar_t i_char) const;
    bool isWord(const wstring& word) const;
    wstring normalizeSpaces(const wstring& str) const;
    void replaceQuotations(const wstring& i_sentence, const vector<size_t>& i_positions, vector<wstring>& o_content) const;

};

}

#endif // TOKENIZER_EN


