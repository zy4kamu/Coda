/**
 * @file    sentence-splitter.h
 * @brief	header file with class implementing sentence splitting algorithm
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


#ifndef SENTENCESPLITTER_H
#define SENTENCESPLITTER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <boost/regex.hpp>
#include <fstream>
#include "logistic-regression.h"
#include "Tools.h"
#include "ISentenceSplitter.h"


namespace _sentence_splitter
{

/**
 * @class	SentenceSplitter
 * @brief	sentence splitter class
 * @example
 * setlocale(LC_ALL,"");
 * _sentence_splitter::SentenceSplitter spltr("/home/mkudinov/sent_split_model");
 * std::vector<size_t> borders = spltr.split(L"\u0420\u043e\u0431\u0435\u0440\u0442 \u0414\u0430\u0443\u043d\u0438 \u043c\u043b. \u0438 \u0421\u044d\u043c\u044e\u044d\u043b \u041b. \u0414\u0436\u0435\u043a\u0441\u043e\u043d \u0443\u0441\u0442\u0440\u043e\u0438\u043b\u0438 \u043f\u043e\u0431\u043e\u0438\u0449\u0435 \u0432 \u0446\u0435\u043d\u0442\u0440\u0435 \u041a\u0438\u0435\u0432\u0430.");//here was cyrrilic symbols: Роберт,Дауни,мл,и,Сэмюэл,Л,Джексон,устроили,побоище,в,центре,Киева
 * // in this case the function returns the position of the leftmost period sign
 */
class SentenceSplitter : public ISentenceSplitter
{
public:
    /**
     * @brief Standard language dependent constructor
     * @param[in] i_model path to the logistic regression model file
     */
    SentenceSplitter(const Tools::Language i_langID);

    /**
     * @brief Constructor
     * @param[in] i_model path to the logistic regression model file
     */
    SentenceSplitter(const std::string& i_model);
    /**
     * @brief returns positions in the input string where the text is split into sentences
     * @param[in] i_line_to_split line to split
     * @return true in case of success, false otherwise
     */
    std::vector<size_t> split(const std::wstring& i_line_to_split);

    virtual Tools::Language GetLanguage() const {return language; }

private:
    const Tools::Language language;

    //returns true if text must be split at the position
    bool testEntryOnSentenceSplit_(size_t i_position, const std::wstring& i_line_to_split);
    _classifiers::LogisticRegressionModel m_model;

  //  std::wofstream featureLogger;
};

} //_sentence_splitter

#endif // SENTENCESPLITTER_H
