/** 
 * @file	PhoenixParser.h
 * @brief	Definition of the class making parsing according to the algorithm from the original Phoenix library
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


#ifndef PHOENIX_PHOENIXPARSER_H
#define PHOENIX_PHOENIXPARSER_H

#include <vector>
#include <memory>
#include <string>
#include <cwctype>
#include "GrammarNet.h"
#include "Grammar.h"
#include "NetChart.h"
#include "PhoenixSlotGraph.h"
#include "Parse.h"
#include "Word.h"
#include "IDisambiguator.h"
#include "ITokenizer.h"
#include "debug.h"
#include "FilledFrame.h"
#include "Tools.h"

#ifdef _TEST_
class ParserTest;
#endif

using Disambiguation::IDisambiguator;

namespace Phoenix 
{
/**
@enum ParseResult
Parsing results: SUCCESS or NO_PARSE
*/
enum ParseResult {SUCCESS, NO_PARSE};

/**
@enum ParseMode
Parsing results: 
FULL stands for output bracketed notation when all the rules applied in parsing and all matched tokens are reflected e.g. [Rule](WORD [rule]([_rule] WORD))
CONCISE stands for the notation when only relevant information is preserved: e.g. [_rule] -> rule
@see GrammarNet::GetConciseName()
*/
enum ParseMode {FULL, CONCISE};

/**
@enum Language
Language of the input string:
@see GrammarNet::GetConciseName()
*/
enum Language {ENGLISH, RUSSIAN};

/** 
	@class PhoenixParser
	@brief Implements parsing algorithm based on recursive transition networks as it is done in the original CMU's Phoenix library 
	@details Parsing is based on the procedure similar to the Earley's chart parser. The search graph traversal is made in DFS manner. 
	The hypothesis ranking is made by SlotTrellis object.
	@see SlotTrellis
	@author M.Kudinov, SRR
*/
class PhoenixParser
{

#ifdef _TEST_
friend class ParserTest;
#endif

public:

    PhoenixParser();

	/**
	@brief Constructor
	@param G grammar the parser must reference to. Grammar contains dictionary and rules for the parser
	@param SG object making hypothesis ranking and finding the best of them
	*/
	PhoenixParser(const std::shared_ptr<Grammar>& G, const std::shared_ptr<SlotTrellis>& SG, Language language, std::string pathToDisambiguatorModel);
	~PhoenixParser();

	/**
	@brief Parses utterance
	*/
	ParseResult ParseLine(std::wstring utterance);

	/**
	@brief Prints best scored parses to console
	@param mode notation of the output
	@see ParseMode
	*/
	void PrintParses(ParseMode mode);

	/**
	@brief Clears all generated parses and prepares to parsing new input
	*/
	void Reset();

	/**
	@brief Returns number of parses
	*/
	int GetNumberOfParses() const;

	/**
	@brief Returns all generated parses
	*/
	std::vector<std::shared_ptr<Parse>> GetAllParses() const;

	/**
	@brief Returns printable form of all generated parses
	*/
	std::vector<std::wstring> GetParsesAsStrings(ParseMode mode);

	/**
	@brief Returns sequence of tokens which did not match any string
	*/
	std::vector<std::wstring> GetUnparsedSequences() const;

private:
	std::shared_ptr<Grammar> gram;
	//Nets to be matched against word sequence
	std::vector<std::shared_ptr<GrammarNet>> activeNets;
	//Chart is a list where for each net all its matches are registered
	NetChart chart;
	//Tries to match whole grammar net against word string. Creates a new parsing path and calls expandPath() to expand it. Returns true if succeeded. 
	bool matchNet(const std::shared_ptr<GrammarNet>& net, int wordPosition);
	//Matches word arc argument against word string. Returns true if succeeded.
	bool matchWord(const std::wstring &word, const std::unordered_set<std::wstring>& features, int wordPositionInAnalyzedString);
	//Matches grammar features of the word against features of arc arguments
	bool matchFeatures(const Word& word, const std::unordered_set<std::wstring>& features);
	//Expands current analysis path trying to traverse from any arc from current state and expand further recursively. Returns true if final state is reached.
	//Besides in case the final state is reached current path is added to the chart
	void expandPath(const std::shared_ptr<GrammarNode>& state, const std::shared_ptr<MatchTree>& path);
	//Expands path taking net arc
	void processCallArc(const std::shared_ptr<NetArc>& ap, std::shared_ptr<MatchTree>& expansionPath, const std::shared_ptr<MatchTree>& path);
	//Expands path taking epsilon arc
	void processEpsilonArc(const std::shared_ptr<NetArc>& ap, std::shared_ptr<MatchTree>& expansionPath, const std::shared_ptr<MatchTree>& path);
	//Expands path taking word arc
	void processWordArc(const std::shared_ptr<NetArc>& ap, std::shared_ptr<MatchTree>& expansionPath, const std::shared_ptr<MatchTree>& path);
	//Pre-processes a string being parsed: converts to uppercase and gets grammar features. Fills vector dis_words.
	void createWordSequence(std::wstring& utterance);
	//container for tokens of the string being processed 
	std::vector<std::wstring> words;
	//container for tokens after disambiguation
	std::vector<Word> dis_words;
	//Slot graph for hypothesis scoring
	std::shared_ptr<SlotTrellis> slotTrellis;
	//Parsing results
	std::vector<std::shared_ptr<Parse>> parses;
	//Disambiguator
    std::shared_ptr<Disambiguation::IDisambiguator> m_disambiguator;
    //Tokenizer
    shared_ptr<Tokenization::ITokenizer> m_tokenizer;
	//Language of the input
	Language lang;
};

} //namespace Phoenix

#endif //PHOENIX_PHOENIXPARSER_H
