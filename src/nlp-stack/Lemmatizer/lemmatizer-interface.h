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

#ifndef LEMMATIZER_INTERFACE
#define LEMMATIZER_INTERFACE


#include "DictionaryInterface.h"
#include "IDisambiguator.h"
#include "ITokenizer.h"

using namespace Disambiguation;
using namespace Tokenization;

namespace lemmatizer
{

/**
 * @class	ILemmatizer
 * @brief	Class for lemmatizing words to its lemma with help of dictionary
 */
class   ILemmatizer
{
public:
    /**
     * @brief constructor of interface
       @param[in] disambiguator for lemmatizer
     */
    static shared_ptr<ILemmatizer> CreateLemmatizer(
        shared_ptr<IDisambiguator> i_disambiguator);

    /**
     * @brief lemmatize is the main function
     * @param[in] i_tokens is a number of tokens to lemmatization
     * @param[in] i_extractor custom processing function
     * @param[out] o_result is place to store result of lemmatization
     * @return true if dictionary has token, false otherwise
     */
    virtual bool lemmatize(const vector<Token>& i_tokens, vector<wstring>& o_result, const std::function<wstring(const DisambiguatedData&)>& i_extractor) = 0;

    /**
     * @brief lemmatize is the main function
     * @param[in] i_tokens is a number of tokens to lemmatization
     * @param[out] o_result is place to store result of lemmatization
     * @return true if dictionary has token, false otherwise
     */
     virtual bool lemmatize(const vector<Token>& i_tokens, vector<wstring>& o_result) = 0;
};

}

#endif // LEMMATIZER_INTERFACE
