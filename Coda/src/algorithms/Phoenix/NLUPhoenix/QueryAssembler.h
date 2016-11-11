/** 
 * @file	QueryAssembler.h 
 * @brief	The file contains description of class QueryAssembler.
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


#ifndef PHOENIX_QUERY_ASSEMBLER
#define PHOENIX_QUERY_ASSEMBLER

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "commonExceptions.h"
#include "FilledFrame.h"
#include "NetEvaluator.h"
#include "sqliteClient/sqlite_helper.h"
#include "debug.h"
#include "Tools.h"

#ifdef _TEST_
class QueryAssemblerTest;
#endif

namespace Phoenix
{

/** 
	@class QueryAssembler
	@brief Creates SQL query
	@details Object which assembles SQL query based on semantic frame
	@author M.Kudinov, SRR
*/
static const wchar_t * CONST_QUERY = L"[Query]";

class QueryAssembler
{
#ifdef _TEST_
friend class QueryAssemblerTest;
#endif

public:
	/**
	@brief Constructor
	@param pathToDb path to the database file containing templates
	*/
	QueryAssembler(const std::string& pathToDb);

	/**
	@brief Creates SQL query based on the input frame
	@param targetFrame frame whose slots will be used in query generation
	*/
	std::wstring CreateQuery(const FilledFrame& targetFrame, const NetEvaluator& evaluator, wstring& question);

	~QueryAssembler() {};

private:
	//returns query template from the resource
	std::wstring getQueryTemplateFromDB(const std::wstring& frameName, const std::string& pathToDb);
	//transforms text template into map
	std::unordered_map<std::wstring, std::wstring> parseTemplate(const std::wstring& templateString);
	//returns default timex structure in case no time slot is filled based on tense
	Slots createDefaultTimex(Tense tense);
	//returns string with placeholders filled with the values returned by fields from match
	std::wstring interpolate(const std::wstring& rule, const std::shared_ptr<IMatch>& match, const NetEvaluator& evaluator);
	std::unordered_map<std::wstring, std::wstring> queryTemplate;
	std::string pathToDb;
	std::wstring currentFrameName;
	Tense currentFrameTense;
};

} //namespace Phoenix

#endif
