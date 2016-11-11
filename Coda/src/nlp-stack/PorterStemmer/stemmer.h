/**
 * @file	stemmer.h
 * @brief	header file for PorterStemmer class with interface for stemming for different languages
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


#ifndef _STEMMER_
#define _STEMMER_

#include <stdio.h>
#include <stdlib.h> /* for malloc, free */
#include <string.h> /* for memmove */
#include <ctype.h>  /* for isupper, tolower */
#include <iostream>
#include <vector>
#include <memory>
#include "Tools.h"
#include "libstemmer.h"
#include "Token.h"

using std::string;
using std::vector;
using std::shared_ptr;
using Tokenization::Token;

namespace stemmer
{

typedef string strtype;


template<class T>
class StemmerBase
{
public:
    StemmerBase(){}

    std::vector<T>
    stemmVectorStr(const std::vector<T>& i_vstr);

    virtual T stemmStr(T& i_str) = 0;

    virtual ~StemmerBase(){}
};



template<class T  = std::string>
class PorterStemmer : public StemmerBase<T>{
public:
    PorterStemmer(string language = "english");
    shared_ptr<sb_stemmer> stemmer(void);
    T stemmStr(T& i_str);

    std::string strToString(T &i_str);

    T StringToStr(string& i_str);

    ~PorterStemmer(){}

private:
    std::shared_ptr<sb_stemmer> m_stemmer;
};

template<class T>
std::vector<T>
StemmerBase<T>::stemmVectorStr(const std::vector<T> &i_vstr)
{
    std::vector<T> result(i_vstr.size());
    for (size_t i = 0; i < i_vstr.size(); i++)
        result[i] = this->stemmStr(i_vstr[i]);
    return result;
}

template<class T>
PorterStemmer<T>::PorterStemmer(std::string language)
{
    m_stemmer = shared_ptr<struct sb_stemmer>(
                sb_stemmer_new(language.c_str(), NULL),
                [](sb_stemmer* p){sb_stemmer_delete(p);}
            );
}

template<class T>
T
PorterStemmer<T>::stemmStr(T &i_str)
{
    string str = this->strToString(i_str);
    string delim = " ";
    string partStr;
    vector<string> vstr = Tools::Split(str," ","\n","\r","\t",true);
    str.clear();
    for (size_t i = 0; i < vstr.size(); i++)
    {
        unsigned char* a = (unsigned char*)vstr[i].c_str();
        sb_symbol* source = a;
        const sb_symbol* res = sb_stemmer_stem(this->stemmer().get(),source,vstr[i].size());
        char* ptr = (char*)res;
        partStr = string(ptr);
        str += partStr;
        if (i!=vstr.size()-1)
            str+=delim;
    }
    T result = this->StringToStr(str);
    return result;
}

template<class T>
std::shared_ptr<sb_stemmer>
PorterStemmer<T>::stemmer()
{
    return this->m_stemmer;
}

}

#endif // _STEMMER_
