/**
 * @file	ISentenceSplitter.h
 * @brief	header file for interfece of sentence splitter
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


#ifndef SSPLITTER_INTERFACE
#define SSPLITTER_INTERFACE

#include <vector>
#include <string>
#include <memory>

#include "Tools.h"

using std::vector;
using std::wstring;
//using std::string;
using std::shared_ptr;

namespace _sentence_splitter
{

/**
 * @class	ISentenceSplitter
 * @brief	Interface of the instrument enabling to split sentences by tokens
 */
class ISentenceSplitter
{
public:
    /**
     * @brief constructor of interface
     */
    static shared_ptr<ISentenceSplitter> GetSentenceSplitter(Tools::Language language);

    /**
     * @brief destructor of interface
     */
    virtual ~ISentenceSplitter() {};

    /**
     * @brief returns positions in the input string where the text is split into sentences
     * @param[in] i_line_to_split line to split
     * @return true in case of success, false otherwise
     */
    virtual std::vector<size_t> split(const std::wstring& i_line_to_split) = 0;

    virtual Tools::Language GetLanguage() const = 0;
};

}


#endif // TOKENIZER_INTERFACE
