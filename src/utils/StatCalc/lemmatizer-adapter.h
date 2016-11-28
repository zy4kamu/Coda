/**
 * @file    lemmatizer-adapter.h
 * @brief	header file with adoptation lemmatizers and stemmers interface to each other
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


#ifndef _LEMMATIZER_ADAPTER_
#define _LEMMATIZER_ADAPTER_

#include "lemmatizer.h"
#include "stemmer.h"
using namespace lemmatizer;

namespace statcalc
{

template<class T>
class LemmatizerStemmerAdapter : public ILemmatizer
{
public:
    LemmatizerStemmerAdapter(const string& language = "english", bool isEmpty = false)
    {
        m_stemmer = shared_ptr<stemmer::PorterStemmer<T>>(new stemmer::PorterStemmer<T>(language));
        m_isEmpty = isEmpty;
    }


    bool lemmatize(const vector<Token> &i_tokens, vector<wstring> &o_result)
    {
        clean();
        m_tokens = i_tokens;
        m_result = vector<wstring>(i_tokens.size(),L"");
        if (m_isEmpty)
        {
            for (size_t i = 0; i < i_tokens.size(); i++)
            {
                m_result[i] = i_tokens[i].content;
            }
//            wcout<<L"here"<<std::endl;
        }
        else
        {
            Token temp;
            for (size_t i = 0; i < i_tokens.size(); i++)
            {
                temp = i_tokens[i];
                temp = m_stemmer->stemmStr(temp);
                m_result[i] = temp.content;
            }
        }
        o_result = m_result;
        return true;
    }

    bool lemmatize(const vector<Token>& i_tokens, vector<wstring>& o_result, const std::function<wstring(const DisambiguatedData&)>& i_extractor)
    {
        return lemmatize(i_tokens, o_result);
    }

    vector<wstring> result()
    {
        return m_result;
    }

    vector<Token> tokens()
    {
        return m_tokens;
    }

    void clean()
    {
        m_tokens.clear();
        m_result.clear();
    }

private:
    shared_ptr<stemmer::PorterStemmer<T>> m_stemmer;
    vector<Token> m_tokens;
    vector<wstring> m_result;
    bool m_isEmpty;
};

}

#endif // _LEMMATIZER_ADAPTER_
