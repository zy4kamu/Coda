/** 
 * @file	Frame.h
 * @brief	Header file contains definitions of a class describing frames.
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


#ifndef PHOENIX_FRAME_H
#define PHOENIX_FRAME_H

#include <memory>
#include <vector>
#include <string>
#include "GrammarNet.h"

namespace Phoenix {

/**
	@class Frame
	@brief Class describing frame objects of Phoenix grammar
	@details Partially filled frame is a target object of the program. E.g. filled slots may be used as arguments in SQL WHERE-clauses while empty slots may be retrieved columns.
	Slot in Phoenix grammar corresponds to grammar net.
	@author M.Kudinov, SRR
*/
class Frame
{
public:
	/**
	@brief Constructor
	@param _frameName Name of the frame constructed.
	*/
	Frame(std::wstring _frameName);
	~Frame() {};

	/**
	@brief Returns number of slots in frame
	*/
	int GetNumberOfSlots() const;

	/**
	@brief Checks if there is particular slot in the frame
	@param slot Grammar net to be checked for being a slot of the frame
	*/
	bool DoesHaveSlot(const std::shared_ptr<GrammarNet>& slot) const;

	/**
	@brief Returns frame name
	*/
	const std::wstring& GetFrameName() const;

	/**
	@brief Adds slot to the frame
	@param net Grammar net going to be plugged to the frame as slot
	*/
	void AddSlot(const std::shared_ptr<GrammarNet>& net, bool isTenseSlot);

	/**
	@brief Returns iterator on frame's nets
	*/
	std::vector<std::shared_ptr<GrammarNet>>::const_iterator GetNetIterator() const;

	/**
	@brief Returns end iterator on frame's nets
	*/
	std::vector<std::shared_ptr<GrammarNet>>::const_iterator GetNetEndIterator() const;

	/**
	@brief Returns slot containing main grammar features
	*/
	std::wstring GetTenseSlotName() const;

private:
	std::wstring frameName;
	std::vector<std::shared_ptr<GrammarNet>> slots; //grammar nets in frame slots
	std::wstring tenseSlotName; //slot from which tense is extracted
};

} //namespace Phoenix

#endif /*PHOENIX_FRAME_H*/

