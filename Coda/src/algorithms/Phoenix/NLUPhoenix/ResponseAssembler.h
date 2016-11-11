/**
 * @file	ResponseAssembler.h
 * @brief	Main module for answer generation
 * @details Class operates on objects defined in sqlite_helper.h which function like Java-object i.e. are type independent. All type casts are made inside functions called by the module.
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


#ifndef PHOENIX_RESPONSEASSEMBLER_H_
#define PHOENIX_RESPONSEASSEMBLER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include "FilledFrame.h"
#include "sqliteClient/sqlite_helper.h"
#include "TemplateParser.h"
#include "SlotTransformer.h"

namespace Phoenix
{

static const char * TEMPLATE_TYPE_SIMPLE = "simple";
static const char * TEMPLATE_TYPE_MULTIROW = "multirow";
static const char * TEMPLATE_TYPE_NO_DATA = "nodata";

struct TemplateInfo
{
	std::vector<std::wstring> textPartsOfTemplate;
	std::vector<TemplateData> dataPartsOfTemplate;
	bool beginsWithText;
};

/** 
	@class ResponseAssembler
	@brief Creates response in the natural language based on the response from the database
	@author M.Kudinov, SRR
*/
class ResponseAssembler
{
public:
	/**
	@brief Constructor
	@param pathToDb path to the database file containing templates
	*/
	ResponseAssembler(const string& pathToDb);

	/**
	@brief Adds row from the database for answer generation
	@param responseRow row retrieved from the database with fields of empty type
	*/
	void AddRow(const SQLColumnDataQueryCollections<List, std::shared_ptr<ISQLColumnDataQuery>>& responseRow);

	/**
	@brief Creates response based on template and rows added
	@param frameToResponse frame from which the question was generated
	@param _question type of the question to answer
	*/
	std::wstring CreateResponse(const FilledFrame& frameToResponse, const std::wstring& _question);
private:
	std::wstring currentQuestion;
	std::wstring currentFrameName;
	TemplateParser tparser;
	SlotTransformer library;
	std::list<std::unordered_map<std::string, std::shared_ptr<ISQLQuery>>> responseData;
	TemplateInfo tinfo;
	std::wstring getResponseTemplateFromDB(const std::wstring& frameName, const std::wstring& _question, const std::string& _templateType);
	std::wstring assembleRow(std::unordered_map<std::string, std::shared_ptr<ISQLQuery>> row);
	std::wstring insertMultipleStringsTemplate();
	std::wstring reportNoData();
};

} /* namespace Phoenix */
#endif /* PHOENIX_RESPONSEASSEMBELER_H_ */
