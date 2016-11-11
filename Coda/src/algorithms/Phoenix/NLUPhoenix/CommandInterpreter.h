/** 
 * @file	CommandInterpreter.h 
 * @brief	The file contains description of class CommandInterpreter.
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



#ifndef PHOENIX_COMMAND_INTERPRETER
#define PHOENIX_COMMAND_INTERPRETER

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "NetChart.h"
#include "NumbNormaliser.h"
#include "FilledFrame.h"

namespace Phoenix
{

class CommandInterpreter;

typedef std::wstring (CommandInterpreter::* FUNC)(std::shared_ptr<MatchTree>);

/** 
	@class CommandInterpreter
	@brief Executes command based on a filled frame
	@author M.Kudinov, SRR
*/
class CommandInterpreter
{
public:
	CommandInterpreter();

	/**
	@brief Takes filled frame assuming it contains a frame for voice command 
	@param targetFrame frame whose slots will be used in command interpretation
	*/
	void Execute(const FilledFrame& targetFrame);

	~CommandInterpreter(){};
private:
	//commands
	std::wstring muteOn(std::shared_ptr<MatchTree> command);
	std::wstring muteOff(std::shared_ptr<MatchTree> command);
	std::wstring setVolume(std::shared_ptr<MatchTree> command);
	std::wstring volumeUp(std::shared_ptr<MatchTree> command);
	std::wstring volumeDown(std::shared_ptr<MatchTree> command);
	std::wstring setChannel(std::shared_ptr<MatchTree> command);
	std::wstring channelUp(std::shared_ptr<MatchTree> command);
	std::wstring channelDown(std::shared_ptr<MatchTree> command);
	std::wstring poweroff(std::shared_ptr<MatchTree> command);
	//convert match from command to an integer arg if needed
	std::shared_ptr<MatchTree> getNumberArg(std::shared_ptr<MatchTree> cmd);

	NumbNormalizer normalizer;
	std::unordered_map<std::wstring, FUNC> functions;
};

} //namespace Phoenix

#endif //PHOENIX_COMMAND_INTERPRETER

