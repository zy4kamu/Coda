/** 
 * @file	Token.h
 * @brief	header file for data describing token
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

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

using std::wstring;
using std::vector;

namespace Tokenization
{

/** 
 * @class	Token
 * @brief	Data describing minimum NLP unit
 */
	struct Token
	{
		wstring content; /**< words*/
		vector<wstring> punctuation; /**< punctuation followed after this token*/
		wstring source;
		bool isNextSpace;

		Token(const Token& token)
		{
			this->content = token.content;
			this->punctuation = token.punctuation;
			this->source = token.source;
			this->isNextSpace = token.isNextSpace;
		}

		Token(Token&& token)
		{
			*this = std::move(token);
		}

		Token& operator = (const Token& token)
		{
			this->content = token.content;
			this->punctuation = token.punctuation;
			this->source = token.source;
			this->isNextSpace = token.isNextSpace;
			return *this;
		}

		Token& operator = (Token&& token)
		{
			this->content = std::move(token.content);
			this->punctuation = std::move(token.punctuation);
			this->source = std::move(token.source);
			this->isNextSpace = std::move(token.isNextSpace);
			return *this;
		}


    Token() : isNextSpace(true)
    {
    }

    Token(const wstring& content 
        , const vector<wstring>& punctuation)
        : content(content)
        , punctuation(punctuation)
    {
    }

    Token(const wstring& content
      , const vector<wstring>& punctuation
      , const wstring& source
      , bool isNextSpace)
      : content(content)
      , punctuation(punctuation)
      , source(source)
      , isNextSpace(isNextSpace)
    {
    }

    /**
     * @brief	Converts token to wide string
     * @TODO: support source & isNextSpace fields
     */
    wstring ToWstring() const
    {
        wstring str = content + L"\t";
        for (size_t punctuationIndex = 0; punctuationIndex < punctuation.size()
            ; ++punctuationIndex)
        {
            str += punctuation[punctuationIndex] + L" ";
        }
        return str;
    }
};

/*
* @TODO: support source & isNextSpace fields for all Print functions
*/
class PrintPreservingPunctuation
{
public:
    std::wstring operator()(const Token& token) const
    {
        std::wstring str = token.content + L"\t";
        for (size_t punctuationIndex = 0; punctuationIndex < token.punctuation.size(); ++punctuationIndex)
        {
            str += token.punctuation[punctuationIndex] + L" ";
        }
        return str;
    }
};

class PrintSeparatingPunctuation
{
public:
    std::wstring operator()(const Token& token) const
    {
        std::wstring str = token.content;

        for(size_t punctuationIndex = 0; punctuationIndex < token.punctuation.size(); ++punctuationIndex)
        {
            str += L"\n" + token.punctuation[punctuationIndex];
        }
        return str;
    }
};

class PrintRemovingPunctuation
{
public:
    std::wstring operator()(const Token& token) const
    {
        return token.content;
    }
};

inline std::wstring ToWstring(const Token& token)
{
    PrintPreservingPunctuation f;
    return f(token);
}

template<typename PrintingFunction>
inline std::wstring ToWstring(const Token& token, const PrintingFunction& f)
{
    return f(token);
}

}

#endif // TOKEN_H
