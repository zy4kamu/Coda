/** 
 * @file	NetChart.h
 * @brief	The file contains descriptions of two classes used by parser.
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


#ifndef PHOENIX_NETCHART_H
#define PHOENIX_NETCHART_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>
#include "GrammarNet.h"
#include "Grammar.h"
#include "TimexNormalizer.h"

namespace Phoenix
{

const int INITIAL_END = -1;

/** 
	@class MatchTree
	@brief Stores match of a particular net in the input line and stores pointers to its nested matches of child (argument) nets
	@details The main features of match objects are borders of the match, net matched and nested matches. Each object is used on two stages of analysis: 1) parsing and 2) hypothesis ranking. 
	The former makes use of methods  GetStartPosition, GetCurrentWordPosition, ConsumeWord, AppendChild and Finalize. These methods are used to control the analysis window and 
	construction of parse trees. When the method Finalize is called the match may be stored in the Chart. 
	The latter phase uses methods for parse tree navigation and getiing match positions: GetStartPosition, GetEndPosition, GetChildMatchesIterator and HasChildren
	Thus, on the 1st phase the match objects are constructed and on the 2nd phase they are used for hypothesis ranking and parse tree contruction.
	@author M.Kudinov, SRR
*/
class MatchTree
{
friend class ChartCell;

public:
	/**
	@brief Constructor
	@param currentNet Name of the net being matched
	@param _wordPosition Start position of the match
	*/
	MatchTree(const std::shared_ptr<GrammarNet>& currentNet, int _wordPosition);
	~MatchTree() {}

	/**
	@brief Returns start position of the match
	*/
	int GetStartPosition() const;

	/**
	@brief Returns end position of the match
	*/
	int GetEndPosition() const;

	/**
	@brief Returns position of the word analyzed if the match not yet finished. Returns GetEndPosition() otherwise
	*/
	int GetCurrentWordPosition() const;

	/**
	@brief Returns grammar net the match corresponds to
	*/
	std::shared_ptr<GrammarNet> GetNet() const;

	/**
	@brief Proceeds parsing getting to the next word to analyze
	@details In the example below if the word in line is successfuly matched against arc argument we turn the word pointer to the next position in the line and make expansion	
		if(matchWord(wordArgumentOfArc, wordInLine)
		{
			expansionPath = shared_ptr<MatchTree>(new MatchTree(*path));
			expansionPath->ConsumeWord(); //indicates that expansion path must proceed from the word next to the one just matched
			expandPath(GetDestinationOfArc(), expansionPath); //start expansion at destination state of the arc
		}
	*/
	void ConsumeWord(std::wstring word);

	/**
	@brief Adds child match
	@details Appends child net match to the match. The function takes care of the end position of the match.  
	*/
	void AppendChild(const std::shared_ptr<MatchTree>& tree);

	/**
	@brief Sets right boundary for the net expansion
	@details If the net is successfully matched Finalize should be called to establish correct end position of the match
	*/
	void Finalize();

	/**
	@brief Returns iterator on child nets matches
	*/
	std::vector<std::shared_ptr<MatchTree>>::const_iterator GetChildMatchesIterator() const;
	
	/**
	@brief Returns end iterator on child nets matches
	*/
	std::vector<std::shared_ptr<MatchTree>>::const_iterator GetChildMatchesEndIterator() const;

	/**
	@brief Returns true if the match has children
	*/
	bool HasChildren() const;

	/**
	@brief Returns grammar features of a syntactic group
	*/
	std::unordered_map<std::wstring, std::wstring> GetFeatures() const;

	/**
	@brief Sets grammar features to the match tree (considered as a syntactic group in this case).
	@details The method should be called when the ! sign is encountered
	*/
	void SetFeatures(const std::unordered_map<std::wstring, std::wstring>& features); 

	/**
	@brief Returns dictionary forms of matched words
	*/
	const std::vector<std::wstring>& GetMatchedWordEntries() const;

private:
	//Net subpath passes through
	std::shared_ptr<GrammarNet> net;
	//Start position of the net match
	int startWordPosition;
	//End position of the match
	int endWordPosition;
	//score of the match for sorting matching paths
	int score;
	//position of the word being analyzed
	int wordPosition;
	//variants of subpaths in case net transition is taken
	std::vector<std::shared_ptr<MatchTree>> children;
	//grammar features of syntactic group
	mutable std::unordered_map<std::wstring, std::wstring> features;
	//word argument which matched the token in the string being processed
	std::vector<std::wstring> consumedWords;
};

/**
	@class IMatch
	@brief Interface for external (processed by external class - e.g. TimexNormalizer) and internal (made by Phoenix) slots
	@author M.Kudinov, SRR
*/
class IMatch
{
public:
	/**
	@brief Returns slot name
	*/	
	virtual std::wstring GetNetName() const = 0;
	/**
	@brief Returns false if slot is generated by Phoenix parser, true otherwise
	*/	
	virtual bool IsExternal() const = 0;
};

/**
	@class ChartCell
	@brief Special class for working with slot graph: it reduces redundant interface of MatchTree to three components - two positions in chart
	@note SlotTrellis does not need to interact with full MatchTree object
	@author M.Kudinov, SRR+

*/
class ChartCell : public IMatch
{
public:
	/**
	@brief Constructor
	@param _match match stored in the chart cell
	*/
	ChartCell(const std::shared_ptr<MatchTree>& _match);
	~ChartCell() {}

	/**
	@brief Returns start position of the match
	*/
	int GetEndPosition() const;

	/**
	@brief Returns end position of the match
	*/
	int GetStartPosition() const;

	/**
	@brief Returns grammar net the match corresponds to
	*/
	std::shared_ptr<GrammarNet> GetNet() const;

	/**
	@brief Returns match in the cell
	*/
	const std::shared_ptr<MatchTree> GetMatch() const;

	/**
	@brief Returns net name
	*/
	std::wstring GetNetName() const;

	bool IsExternal() const {return false;}


private:
	std::shared_ptr<MatchTree> match;
};

/**
	@class TimexMatch
	@brief Slot for timex slots
	@author M.Kudinov, SRR
*/
class TimexMatch : public IMatch
{
public:
	/**
	@brief Constructor
	@param _timexSlots timex slots structure, containing timex match in the input line
	*/
	TimexMatch(Slots _timexSlots);

	/**
	@brief Returns structure describing timex
	*/
	Slots GetSlots() const;

	/**
	@brief Returns slot name based on the information in slots structure
	*/
	std::wstring GetNetName() const;

	bool IsExternal() const {return true;}

private:
	Slots timexSlots;
};

/**
	@class NetChart
	@brief Chart is a pseudo-table where successful matches are stored. All matches are objects of MatchTree
	@details Chart is a bank of tables for each grammar net. Each table may be thought of as a matrix X*Y where X is a right bound of the net expansion and Y is the right one.
	Besides, each cell of the table may contain several expansions
	@author M.Kudinov, SRR
*/
class NetChart
{
public:
	/**
	@brief Constructor
	*/
	NetChart();
	~NetChart() {}

	/**
	@brief Adds parse tree for net into chart at position startPosition and endPosition.
	@param match 
	@param startPosition
	@param endPosition
	@note Arguments startPosition,endPosition must be equal to the fields of the match and are added to make chart interface more descriptive. Otherwise, exception is thrown
	@exception EInconsistentInsertion Exception thrown if there is an attempt to insert match into chart position X,Y so that startPosition or endPosition not equal to X and Y correspondingly
	*/
	void AddMatch(const std::shared_ptr<MatchTree>& match, int startPosition, int endPosition);

	/**
	@brief Creates an empty entry for match of grammar net beginning at position wordPosition and establishes pseudo-row in the chart
	@param net net for which the entry is created
	@param wordPosition start position of the possible match. It will correspond to the wordPosition-th row in the chart->net
	*/
	void RegisterEntry(const std::shared_ptr<GrammarNet>& net, int wordPosition);

	/**
	@brief Returns true if the net was attempted
	@param net
	@param wordPosition
	*/
	bool CheckMatchTreeAttempt(const std::shared_ptr<GrammarNet>& net, int wordPosition) const;

	/**
	@brief Returns true if the match was successful
	@param net net to be checked
	@param wordPosition position of the match
	*/
	bool CheckMatchSuccess(const std::shared_ptr<GrammarNet>& net, int wordPosition) const;

	/**
	@brief Returns all matches of the net at word position. Should be used with CheckMatchTreeAttempt()
	@param net net whose matches are to be returned
	@param wordPosition position of the matches
	@exception ENoMatchingAttemptRegistered Exception thrown if a non existent entry in the chart is referenced
	*/
	std::vector<std::shared_ptr<MatchTree>> GetMatch(const std::shared_ptr<GrammarNet>& net, int wordPosition) const;

	/**
	@brief Returns pairs left-rigth position for all matches of the net
	@param net net whose matches are to be returned
	@exception ENoMatchingAttemptRegistered Exception thrown if a non existent entry in the chart is referenced
	*/
	std::vector<std::shared_ptr<ChartCell>> GetNonEmptyCellsForNet(const std::shared_ptr<GrammarNet>& net) const;

	/**
	@brief Returns all net matches which begun at leftPositon ordered by their right (end) position
	@param net net whose matches are to be returned
	@param leftPosition position of the matches
	*/
	std::vector<std::shared_ptr<ChartCell>> GetNonEmptyCellsForNet(const std::shared_ptr<GrammarNet>& net, int leftPosition) const;

private:
	//Chart is a mapping from net names into a sparse matrix of its word positions 
	//Sparse matrix is implemented as double mapping
	//It is convenient because searching of a net entry by left position is often needed while sorting is needed for both positions
	//Furhter more each entry at position may not be unique e.g. net rule may be expanded into to different rules within the same word bounds
	//Thus the full path to the particular entry is schematically as follows: netEntryPositions[netName]->entriesByLeftPosition[leftPosition]->entriesByRightPosition[rightPosition]->list[NumberOfEntry]
	//EXAMPLE: entriesOfNetAtPositionXY netToEntryPositions.at(net->GetName())->entriesByLeftPosition.at(X)->entriesByRightPosition(Y)
	//X in 'matrix' is left position of occurecnce and Y is right
	struct netInChartY
	{
		netInChartY();
		~netInChartY() {}
		std::map<int, std::shared_ptr<std::vector<std::shared_ptr<MatchTree>>>> entriesByRightPosition;
	};
	struct netInChartX
	{
		netInChartX();
		~netInChartX() {}
		std::map<int, std::shared_ptr<netInChartY>> entriesByLeftPosition;
	};

	typedef std::map<int, std::shared_ptr<std::vector<std::shared_ptr<MatchTree>>>> chartColumn;
	typedef std::map<int, std::shared_ptr<netInChartY>> chartRow;

	std::unordered_map<std::wstring, std::shared_ptr<netInChartX>> netToEntryPositions;
};

/**
@class ENoMatchingAttemptRegistered
@brief Exception thrown if a non existent entry in the chart is referenced
*/
class ENoMatchingAttemptRegistered
{
public:
	ENoMatchingAttemptRegistered() {}
};

/**
@class ENoMatchingAttemptRegistered
@brief Exception thrown if there is an attempt to insert match into chart position X,Y so that startPosition or endPosition not equal to X and Y correspondingly
*/
class EInconsistentInsertion
{
public:
	EInconsistentInsertion() {}
};

} //namespace Phoenix

#endif //PHOENIX_NETCHART_H
