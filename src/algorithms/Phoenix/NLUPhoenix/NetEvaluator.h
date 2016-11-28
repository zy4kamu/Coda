/** 
 * @file	NetEvaluator.h 
 * @brief	The file contains description of class NetEvaluator.
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


#ifndef PHOENIX_NETEVALUATOR
#define PHOENIX_NETEVALUATOR

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "NetChart.h"
#include "commonExceptions.h"
#include "Tools.h"
#include "FilledFrame.h"
#include "TimexFormatConverter.h"

namespace Phoenix
{

/** 
	@class NetEvaluator
	@brief Returns semantic value of grammar net being interpreted as a semantic function
	@details Meyers' singleton class which provides access to evaluation functions
	@author M.Kudinov, SRR
*/
class NetEvaluator
{
public:

	/**
	@brief Constructor
	*/
	NetEvaluator(const std::string& programFile);

	/**
	@brief Returns value of the net based on the matched text
	@param netToEval Net whose value will be evaluated
	*/
	std::unordered_map<std::wstring, std::wstring> Evaluate(const std::shared_ptr<IMatch>& netToEval, Tense tense) const;
	~NetEvaluator() {};
	static std::wstring VALUE;

private:
	std::wstring DICTIONARY_ENTRY;
	std::wstring TIMEX;

	NetEvaluator(const NetEvaluator& root);
	NetEvaluator& operator=(const NetEvaluator&);
	//reads configuration from file
	void readConfig(const std::string& programFile);
	//name table for functions
	std::unordered_map<std::wstring, std::wstring> functionTable;
	//default evaluation: just returns value of its child net and matched words if there is no children
	std::unordered_map<std::wstring, std::wstring> defaultEvaluation(const std::shared_ptr<MatchTree>& argument, Tense tense) const;
	//evaluator returns dictionary form of the words in the evaluated net
	std::unordered_map<std::wstring, std::wstring> dictionaryEvaluation(const std::shared_ptr<MatchTree>& argument) const;
	//evaluator for time expressinos
	std::unordered_map<std::wstring, std::wstring> timexEvaluation(const std::shared_ptr<TimexMatch>& argument, Tense tense) const;
	//init class constants
	void initConstants();
	//adds the entry to function table for leftVlue being computed according to rightValue
	void addToTable(const std::wstring& leftValue, const std::wstring& rightValue);
	//evaluation for recursive tree traverse
	std::unordered_map<std::wstring, std::wstring> evaluate(const std::shared_ptr<MatchTree>& netToEval, Tense tense) const;

	Tense tense;
};

class ESymbolRedefinition
{
public:
	ESymbolRedefinition(const wchar_t * symbol) { fprintf(stderr, "ERROR: Multiple definition of symbol\n"); }
};

class ENoConfigSpecified
{
public:
	ENoConfigSpecified() {fprintf(stderr, "ERROR: No config file specified for net evaluations\n");}
};

} //namespace Phoenix

#endif

