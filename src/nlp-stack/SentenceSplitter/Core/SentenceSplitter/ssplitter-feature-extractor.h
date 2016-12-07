/**
 * @file    feature-extractor.h
 * @brief	header file with class implementing feature extraction sentence splitting algorithm
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


#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <unordered_map>

namespace _sentence_splitter
{
/**
 * @class	FeatureExtractor
 * @brief	Extracts features relevant for sentence splitting
 */
class FeatureExtractor
{
public:
    /**
     * @brief constructor
     * @param[in] i_left_context_length how many previous tokens to take into account
     * @param[in] i_right_context_length how many next tokens to take into account
     */
    FeatureExtractor(int i_left_context_length, int i_right_context_length);
    /**
     * @brief constructor with word model
     * @param[in] i_left_context_length how many previous tokens to take into account
     * @param[in] i_right_context_length how many next tokens to take into account
     * @param[in] i_data path to word model, containing word+punctuations frequencies
     */
    FeatureExtractor(int i_left_context_length, int i_right_context_length, std::string i_data);
    /**
     * @brief returns relevant features of the suspicious position in the input line
     * @param[in] i_position position in input line to extract features for
     * @param[in] i_line_to_split input text
     * @return vector of feature names which are true for the position
     */
    std::vector<std::wstring> getFeatures(size_t i_position, const std::wstring& i_line_to_split);
    /**
     * @brief returns relevant features with double values of the suspicious position in the input line
     * @param[in] i_position position in input line to extract features for
     * @param[in] i_line_to_split input text
     * @return vector of feature names and their values for the position
     */
    std::unordered_map<std::wstring, double> getDoubleFeatures(size_t i_position, const std::wstring& i_line_to_split);

private:
    // words model with their frequencies with/without end of line (end of paragraph)
    std::unordered_map<std::wstring, std::pair<double, double> > wordsMap;

    void windowFstMatch_(std::vector<std::wstring>& io_resultTmp, const std::wstring& i_line_to_split, int i_start_position_in_line, int i_start_position_in_window, std::function<bool(int i, int j)> i_predicate,  std::function<void(int&)> i_iterate, std::function<void(std::wstring&)> i_postProcess);
    void revertString_(std::wstring& io_string_to_revert);

    void getWordFeatures(size_t i_position, const std::wstring& i_line_to_split,
                                             std::unordered_map<std::wstring, double>& o_result);

    bool isNormalCharacter_( wchar_t i_character ) const;
    bool isDigit_( wchar_t i_character ) const;
    bool isPunctuator_( wchar_t i_character ) const;
    bool isQuotation_( wchar_t i_character ) const;
    bool isUppercase_( wchar_t i_character ) const;
    bool isSentenceEndPunctuator_( wchar_t i_character ) const;
    bool isCons_( wchar_t i_character ) const;

    std::vector<std::wstring> getFeatureCodes_(std::vector<std::wstring>& i_tokens);

    int m_left_context_length;
    int m_right_context_length;
    int m_effective_left;

};

} //namespace _sentence_splitter

#endif // FEATUREEXTRACTOR_H
