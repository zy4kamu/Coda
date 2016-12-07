/** 
 * @file	SynTagRusTagInterpeter.h
 * @brief	Description of a class SynTagRusInterpeter
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


#ifndef SYNTTAGRUS_TAGINTERPETER
#define SYNTTAGRUS_TAGINTERPETER

#include <unordered_map>
#include <vector>
#include <string>

/** 
	@class SynTagRusTagInterpeter
	@brief Transforms SynTagRus tags into map
	@details Meyers' singleton class which provides interface for transformation of a vector of SynTagRus tags into map of the form FEATURE->VALUE
	@author M.Kudinov, SRR
*/
class SynTagRusTagInterpreter
{
public:
	/**
	@brief Standard singleton GetInstance
	*/
	static  SynTagRusTagInterpreter& GetInstance();

	/**
	@brief Transforms SynTagRus tagline into map of the form FEATURE->VALUE
	@param tags SynTagRus tags
	*/
	std::unordered_map<std::wstring, std::wstring> ProcessTags(const std::vector<std::wstring>&  tags);
	~SynTagRusTagInterpreter() {};

private:
	//constructor
	SynTagRusTagInterpreter();

	//transformations of verbs
	void processVerb(std::unordered_map<std::wstring, std::wstring>& feats);

	//transformations of adjectives
	void processAdj(std::unordered_map<std::wstring, std::wstring>& feats);

	std::unordered_map<std::wstring, std::wstring> map;
	SynTagRusTagInterpreter(const SynTagRusTagInterpreter& root);
	SynTagRusTagInterpreter& operator=(const SynTagRusTagInterpreter&);
};

#endif

