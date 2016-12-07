/** 
 * @file	Grammar.h 
 * @brief	The file contains description of class Grammar.
 * @details The first is parse tree containing pointers to child nodes and bounds of expansion for each non-terminal (grammar net in this case)
 *			The second is chart. Logically chart is a bank of tables so that for each grammar net it contains all its correct expansions stored as trees. Thus the two classes are closely related
 *			Both classes are used by PhoenixParser
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


#ifndef PHOENIX_GRAMMAR_H
#define PHOENIX_GRAMMAR_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "commonExceptions.h"
#include "GrammarNet.h"
#include "TimexFormatConverter.h"
#include "Frame.h"
#include "debug.h"

namespace Phoenix
{

const int WORD_NOT_FOUND = -1;
#define START_SYMBOL L"<s>"
#define END_SYMBOL L"<e>"
//static const wchar_t * START_SYMBOL = L"<s>";
//static const wchar_t * END_SYMBOL = L"<e>";
static const int LINE_LEN = 10000;
static const int MAX_WRDS = 100000;
static const int LABEL_LEN = 200;

/**
@class Grammar 
@brief Basic class containing grammar rules and dictionary for parsing.  
@details Grammar contains semantic frames, grmmar rules (nets), dictionary and
basic grammar constants necessary for parsing sentences in natural language and provide interface for manipulating them.
@note The class is to be used by PhoenixParser class.
@author M.Kudinov, SRR
 */
class Grammar
{

#ifdef _TEST_
friend class GrammarTest;
#endif

public: 
	/**
	@brief Constructor
	@param dict_file path to the file containing dictionary
	@param grammar_file path to the file containing grammar rules
	@param frames_file path to the file containing frames
	@exception EFileNotFound One of the configuration files was not found
	@exception EBadFileFormat One of the configuration files is not well-formed
	@exception EErrorReadingGrammar Error in grammar net description in .net file
	@exception EUnableToInterpretNode Error in grammar net description in .net file
	@exception EErrorReadingDictionary Error in dictionary file
	*/
	Grammar(const std::string& dict_file, const std::string& grammar_file, const std::string& frames_file);
	~Grammar() {};

	/**
	@brief Returns iterator on frames
	*/
	std::vector<std::shared_ptr<Frame>>::const_iterator GetFrameIterator() const;

	/**
	@brief Returns end iterator on frames
	*/
	std::vector<std::shared_ptr<Frame>>::const_iterator GetFrameEndIterator() const;

	/**
	@brief Returns all frames which the net can be attributed to
	*/
	std::unordered_set<std::wstring> GetPossibleFramesForNet(const std::shared_ptr<GrammarNet>& net) const;

	/**
	@brief Returns frame by name
	@param frameName Name of the frame
	*/
	std::shared_ptr<Frame> GetFrameByName(std::wstring& frameName); 

private:
// Returns grammar net by its alias
	std::shared_ptr<GrammarNet> findNet(const std::wstring& name) const;
// Returns word's index in dicitionary
	int findWord(const std::wstring& word) const;
// initializes grammar nets from grammar_file
	void read_nets();
// initializes frames nets from frames_file
	void read_frames();
// initializes dictionary nets from dict_file
	int read_dict();
// Returns net by index needed because while reading *.net file nets which are not read yet may be referenced
	std::shared_ptr<GrammarNet> getNetByIndex(int id);
//Returns vector of grammar marks generated from feature string in format {f1|f2|...|fn}
	std::vector<std::wstring> parseFeats(const std::wstring& featLine);
	std::vector<std::shared_ptr<Frame>> frames;		
	std::vector<std::shared_ptr<GrammarNet>> nets;		
// Dicionary is stored as unordered map rather than vector because of unclear procedure of index assignment during grammar compilation. 
// Word indeces are not sorted nor fall into scope [1..NUMBER_OF_WORDS]. However, net file references words by these indeces, so they are preserved as keys in unordered map
	std::unordered_map<int, std::wstring> wrds;

	std::string dict_file;
	std::string grammar_file;
	std::string frames_file;
};

/**
@class EErrorReadingGrammar 
@brief Exception thrown if the grammar file is ill-formed (error in a net description)
@author M.Kudinov, SRR
 */
class EErrorReadingGrammar
{
public:
	EErrorReadingGrammar() {fprintf(stderr, "ERROR: Error reading grammar\n");}
};

/**
@class EUnableToInterpretNode 
@brief Exception thrown if the grammar file is ill-formed (error in node description)
@author M.Kudinov, SRR
 */
class EUnableToInterpretNode
{
public:
	EUnableToInterpretNode() {fprintf(stderr, "ERROR: failed reading node succs\n");}
};

/**
@class EUnableToInterpretNode 
@brief Exception thrown if dictionary file is ill-formed
@author M.Kudinov, SRR
 */
class EErrorReadingDictionary
{
public:
	EErrorReadingDictionary() {fprintf(stderr, "ERROR: word number out of range\n");}
};

} //namespace Phoenix

#endif
