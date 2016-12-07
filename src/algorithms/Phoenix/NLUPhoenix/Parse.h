/** 
 * @file	Parse.h
 * @brief	Definition of Parse class. 
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


#ifndef PHOENIX_PARSE_H
#define PHOENIX_PARSE_H

#include <string>
#include <memory>
#include <vector>
#include "NetChart.h"

namespace Phoenix
{
/**
	@class Parse
	@brief Particular parse of the line normally produced by SlotGraph
	@details The class contains alignment between frame slots (matched rules) and frames (frame labels). Together these parts represent parse.
	@author M.Kudinov, SRR
*/
class Parse
{
public:
	/**
	@brief Constructor
	@param frameLabels Sequence of frames corresponding to matched nets
	@param _slotSequence Sequence of net matches
	*/
	Parse(const std::vector<std::wstring>& frameLabels, const std::vector<std::shared_ptr<ChartCell>>& _slotSequence);
	~Parse() {};

	/**
	@brief Returns sequence of frames for the parse
	*/
	const std::vector<std::wstring>& GetFrameSequence() const ;

	/**
	@brief Returns sequence of net matches
	*/
	const std::vector<std::shared_ptr<ChartCell>>& GetSlotSequence() const;

	/**
	@brief Prints utterance with bracketed structure generated according to the parse
	@param utteranceTokens Sequence of words
	@see ParseMode
	*/
	void Print(const std::vector<std::wstring>& utteranceTokens);

	/**
	@brief Prints utterance with concise bracketed structure generated according to the parse
	@param utteranceTokens Sequence of words
	@see ParseMode
	*/
	void PrintConcise(const std::vector<std::wstring>& utteranceTokens);

	/**
	@brief Prints utterance with bracketed structure generated according to the parse with printed in dictionary form
	*/
	void PrintNF();

	/**
	@brief Prints utterance with concise bracketed structure generated according to the parse with words printed in dictionary form
	*/
	void PrintConciseNF();

	/**
	@brief Returns true if parse is fragmented i.e. contains more than one different frame labels
	*/
	bool IsFragmented() const;

	/**
	@brief printable form of the parse
	@param utteranceTokens Sequence of words
	*/
	std::wstring ToString(const std::vector<std::wstring>& utteranceTokens);

	std::wstring ToStringConciseNF(const std::vector<std::wstring>& utteranceTokens);

	std::vector<std::wstring> GetUnparsedSequence(const std::vector<std::wstring>& words) const;

private:
	std::wstring printSlotRecursively(const std::shared_ptr<MatchTree>& slotSequence, const std::vector<std::wstring>& words);
	std::wstring printConciseRecursively(const std::shared_ptr<MatchTree>& slotSequence, const std::vector<std::wstring>& words);
	std::wstring printSlotRecursivelyNF(const std::shared_ptr<MatchTree>& slotSequence);
	std::wstring printConciseRecursivelyNF(const std::shared_ptr<MatchTree>& slotSequence);
	std::vector<std::wstring> frameSequence; 
	std::vector<std::shared_ptr<ChartCell>> slotSequence;
};

} //namespace Phoenix

#endif //PHOENIX_PARSE_H

