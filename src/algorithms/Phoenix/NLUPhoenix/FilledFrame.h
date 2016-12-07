/** 
 * @file	FilledFrame.h 
 * @brief	Class for filled frame.
 * @details The class is to be used generally in case when a frame can represent a parse of whole or most part of the utterance. In this case the utterance can be translated into SQL query (relatively) easily.
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


#ifndef PHOENIX_FRAMEFILLED_H
#define PHOENIX_FRAMEFILLED_H

#include "Frame.h"
#include "Parse.h"
#include "NetChart.h"
#include <memory>

namespace Phoenix 
{

/**
	@class FilledFrame
	@brief Filled target frame involved in interpretation of the utterance.
	@details Generally, parser is capable of generating parse with frame alternations, but single frame can be translated into SQL query more easily. The class represents objects to be translated into queries.
	@author M.Kudinov, SRR
*/
class FilledFrame
{
public:
	/**
	@brief Constructor
	@param semantic_frame frame to be filled
	@param all_slots All filled slots of the frame
	*/
	FilledFrame(const std::shared_ptr<Frame>& semantic_frame, const std::vector<std::shared_ptr<IMatch>>& all_slots);
	~FilledFrame() {}

	/**
	@brief Returns semantic frame whose object is dealt with
	*/
	std::shared_ptr<Frame> GetSemanticFrame() const;

	/**
	@brief Returns iterator on frame's nets
	*/
	std::vector<std::shared_ptr<IMatch>>::const_iterator GetSlotIterator() const;

	/**
	@brief Returns end iterator on frame's nets
	*/
	std::vector<std::shared_ptr<IMatch>>::const_iterator GetSlotEndIterator() const;

	/**
	@brief Returns tense of the frame
	*/
	Tense GetTense() const;
private:
	std::shared_ptr<Frame> frame;
	std::vector<std::shared_ptr<IMatch>> slots;
	Tense tense;
}; 

class ENoCommandFrame
{
public:
	ENoCommandFrame(const wchar_t * frameName) {fwprintf(stderr, L"ERROR: frame %s does not contain a command\n", frameName);}
};

} //namespace Phoenix

#endif /*PHOENIX_FRAMEFILLED_H*/

