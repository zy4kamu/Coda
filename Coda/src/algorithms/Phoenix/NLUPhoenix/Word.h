/** 
 * @file	Word.h
 * @brief	Class for word forms with morphological features
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


#ifndef PHOENIX_WORD_H
#define PHOENIX_WORD_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SynTagRusTagInterpreter.h"

namespace Phoenix
{

/** 
	@class Word
	@brief Word with grammar features
	@details Provides interface for getting grammar features of a word
	@author M.Kudinov, SRR
*/
class Word
{
public:
	/**
	@brief Constructor
	@param _lemma lemma
	@param _tags tags
	@param _word wordform
	*/
	Word(const std::wstring& _lemma, const std::vector<std::wstring>& _tags, const std::wstring& _word);
	~Word() {} 

	/**
	@brief Returns lemma of the word
	*/
	std::wstring GetLemma() const;

	/**
	@brief Returns value of a grammatical feature
	@param feature Feature whose value is to be returned
	@exception ENoFeature
	*/
	std::wstring GetFeature(const std::wstring& feature) const;
	
	/**
	@brief Returns list of grammar features the word has
	*/
	std::vector<std::wstring> GetAvailableFeatures() const;

	/**
	@brief Returns map containing all pairs feature-value which the word has
	*/
	std::unordered_map<std::wstring, std::wstring> GetAllFeatures() const;

	/**
	@brief Returns set containing all feature values
	*/
	std::unordered_set<std::wstring> GetAllFeaturesSet() const;

	/**
	@brief Returns true if the word has the feature
	@param feature feature whose presense is to be cleared
	*/
	bool DoesHaveFeature(const std::wstring& feature) const;

private:
	std::wstring lemma;
	std::unordered_map<std::wstring, std::wstring> tags;
	std::wstring word;
};

/**
@class ENoFeature 
@brief Exception thrown if some non-existing feature is requested
@author M.Kudinov, SRR
 */
class ENoFeature
{
public:
	ENoFeature() {}
};

} //namespace Phoenix

#endif //PHOENIX_WORD_H
