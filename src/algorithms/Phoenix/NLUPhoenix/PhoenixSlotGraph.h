/** 
 * @file	PhoenixSlotGraph.h
 * @brief	Classes for the representation of the trellis for parsing hypotheses ranking and corresponding algorithm 
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


#ifndef PHOENIX_PHOENIXSLOTGRAPH_H
#define PHOENIX_PHOENIXSLOTGRAPH_H

#include <vector>
#include <memory>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include <limits>
#include <memory>

#include "NetChart.h"
#include "Frame.h"
#include "Parse.h"
#include "Tools.h"

using std::shared_ptr;

namespace Phoenix {

/** 
	@class TrellisNode
	@brief Node in the trellis
	@details Every node contains frame slot the path passes through (i.e. net match), all possible frame labels (because the slot may be included in many frames),
	methods for manipulating such labels and means for navigation along the path.
	@see SlotTrellis
	@author M.Kudinov, SRR
*/
class TrellisNode
{
public:
	/**
	@brief Constructor. Creates empty node without previous element. May be used for constructing root node of the path.
	*/	
	TrellisNode();

	/**
	@brief Constructor. Creates a node corresponding to a match and places it after the specified element in the path.
	*/	
	TrellisNode(const std::shared_ptr<ChartCell>& match, const std::shared_ptr<TrellisNode>& _previous);
	~TrellisNode() {}

	/**
	@brief Returns all possible frame labels for the slot corresponding to the node
	*/	
	std::unordered_set<std::wstring> GetPossibleFrames() const;

	/**
	@brief Returns previous node in the path
	*/	
	std::shared_ptr<TrellisNode> ToPrevious() const;

	/**
	@brief Returns next node in the path
	*/	
	std::shared_ptr<TrellisNode> ToNext() const;

	/**
	@brief Returns next node in the path
	@param frameLabel Name of the frame whose fragmentation score is to be calculated
	@see SlotTrellis
	*/	
	int GetLabelFragmentation(const std::wstring& frameLabel) const;

	/**
	@brief Returns match corresponding to the node
	*/	
	std::shared_ptr<ChartCell> GetMatch() const;

	/**
	@brief Sets new possible frame label
	@param label Name of the frame
	@param fragmentScore Fragmentaion of the label at the node
	*/	
	void SetFrameLabel(const std::wstring& label, int fragmentScore);

	/**
	@brief Sets next node in the path for the node
	@param node Slot which will be the next in the path
	*/	
	void SetNext(const std::shared_ptr<TrellisNode>& node);

	/**
	@brief Removes possible frame label
	@param label Name of the frame
	*/	
	void DeleteLabel(const std::wstring& label);

private:
	//match stored in the node
	std::shared_ptr<ChartCell> correspondingMatch;
	//possible frame labels
	std::unordered_map<std::wstring, int> possibleFrames; 
	std::shared_ptr<TrellisNode> previous;
	std::weak_ptr<TrellisNode> next;
};

/** 
	@class TrellisStep
	@brief Step of the trellis matching slots against the word sequence
	@details In principle each time step (word position in our case) should be able to retrieve all the paths existing up to the moment of the step.
	In this case the trellis would have been represented as a tree with with number of nodes O(L*P), where L is length of the sequence and P is the number of parses.
	If we preserve only steps where net match ends we get the tree with size proportional to the number of possible slots in all parses.
	Trellis step represents a step in such tree.
	@see SlotTrellis
	@author M.Kudinov, SRR
*/
class TrellisStep
{
public:
	/**
	@brief Constructor. Creates empty step without any path.
	*/	
	TrellisStep();
	~TrellisStep() {}

	/**
	@brief Returns minimal accumulated label fragmentation over all labels over all paths which have reached
	*/	
	int GetLabelFragmentation() const;

	/**
	@brief Returns maximum word matching score over all paths which have reached the node.
	*/	
	int GetScore() const;

	/**
	@brief Returns minimal number of slots over all paths the path passed through.
	*/	
	int GetSlotNum() const;

	/**
	@brief Returns iterator over all paths which have reached the node.
	*/	
	std::list<std::shared_ptr<TrellisNode>>::const_iterator GetPathIterator() const;

	/**
	@brief Returns end iterator over all paths which have reached the node.
	*/	
	std::list<std::shared_ptr<TrellisNode>>::const_iterator GetPathEndIterator() const;

	/**
	@brief Deletes all the paths which have reached the step.
	*/
	void Clean();

	/**
	@brief Sets new score features for the step.
	*/
	void SetScores(int wordScore, int slotScore, int frameScore);

	/**
	@brief Adds new path which have reached the step.
	*/
	void AddPath(const std::shared_ptr<TrellisNode>& newNode);
private:
	//paths which have reached the step
	std::list<std::shared_ptr<TrellisNode>> alivePaths;
	//maximum word score at step
	int maxWordsScoreAtStep;
	//minimum slot score at step
	int minSlotScoreAtStep;
	//minimum frame fragmentation
	int frameFragmentation; 
};

/** 
	@class SlotTrellis
	@brief Slot graph for ranking parsing hypotheses. Implements the ranking algorithm. 
	@details The basic search strategy is BFS. More precisely it is a beam search with all by the best paths are prunned. 
	Each matched active slot is added to the graph. If its right end coincide with some of the matches added before the matches compete based on two criteria:
	1) the total number of words matched along the path (the more the better)
	2) the total number of slots the hypothetical path passes through (the less the better)
	The loser is prunned. If both scores are equal the new alternative path is added to the trellis at step equal to the right position of the match (and correspondingly path)
	After all paths are built they are sorted according to the Frame fragmentation score i.e. the number of times the path switches the frame being filled.
	@remark Ideally, one utterance must correspond to one frame (question on one topic or a command or request) but complex utterances may contain information allowing
	to fill mor than one frame. Anyway, it is reasonable to prefer parses more homogeneous in its labeling.
	@remark The same motivation stands behind the slot score.
	@author M.Kudinov, SRR
*/
class SlotTrellis
{
public:
	/**
	@brief Constructor.
	@param G Grammar to reference
	*/	
	SlotTrellis(const std::shared_ptr<Grammar>& G);
	~SlotTrellis() {};

	/**
	@brief Attempts to add a new matched active slot to some path of the trellis. The algorithm guarantees to save only the best path in the trellis.
	@param matchedSlot The slot to add. In fact these are the row from the parsing chart: all matches of the net begun at some particular left position
	@see NetChart
	*/
	void AddSlot(const std::vector<std::shared_ptr<ChartCell>>& matchedSlot);

	/**
	@brief Delete all steps
	*/
	void Clear();

	/**
	@brief Returns best scoring parses from the trellis.
	*/
	std::vector<std::shared_ptr<Parse>> GetBestMatch();
private:
	//return values for the procedure detecting if we should add new slot to the path or not
	enum PrunningDecision {PRUN_OLD, PRUN_NEW, ADD};
	//Returns one of the decisions PrunningDecision
	PrunningDecision getMaxScoreDecision(const std::shared_ptr<ChartCell>& possibleMatch);
	//Finds the number of the trellis step with the path to which the slot will be attached
	int findPointToAttach(int lowerThan);
	//Calculate the score of the match. In this case number of the words between left and right positions.
	int scorePhrase(const std::shared_ptr<ChartCell>& phrase);
	//Calculates accumulated score along the path
	int getScoreAtPoint(int step);
	//Calculates accumulated slot number along the path
	int getSlotNumAtPoint(int step);
	//Finds best candidate to being the end position of the utterance
	int findBestEndPosition();
	//Finds paths with least accumulated fragmentation
	std::vector<std::shared_ptr<TrellisNode>> getLeastFragmentedPaths(int endPos);
	//Generates parse objects from paths
	std::vector<std::shared_ptr<Parse>> generateAllParses(const std::vector<std::shared_ptr<TrellisNode>>& paths);
	//Deletes step point (i.e. no path begins or end there)
	void clearStepAtPoint(int timeStep);
	//Appends new slot to the path
	void appendToPath(const std::shared_ptr<ChartCell>& possiblematch);
	//Appends the slot to a non-start position
	void continuePath(std::shared_ptr<TrellisNode>& newNode, int &minFrames, const std::shared_ptr<ChartCell>& possibleMatch, const std::unordered_set<std::wstring>& possibleFrames, int prevStep, std::shared_ptr<TrellisStep> currentStep);
	//Appends the slot to the start position
	void initPath(std::shared_ptr<TrellisNode>& newNode, int &minFrames, const std::shared_ptr<ChartCell>& possibleMatch, const  std::unordered_set<std::wstring>& possibleFrames, int prevStep, std::shared_ptr<TrellisStep> currentStep);
	//Calculates step fragmentation scores of two types for each possible parse
	void scoreFragmentation(std::vector<int>& oneStepInconsistency, std::vector<int>& nStepInconsistency, int& minOneStepInconsistency, int &minNStepInconsistency, const std::shared_ptr<Parse>& parse, int weakFragmentation);

	std::shared_ptr<Grammar> gram;
	std::map<int, std::shared_ptr<TrellisStep>> trellis;
	vector<shared_ptr<TrellisNode>> store;

	//class for generating parses from slots. Added for making class implementation less cluttered and reduce the number of fields.
	//generation is based on the recursive generation of subpaths
	class labelSequenceGenerator
	{
	public:
		labelSequenceGenerator(const std::vector<std::shared_ptr<TrellisNode>>& _paths);
		~labelSequenceGenerator() {};
		std::vector<std::shared_ptr<Parse>> generate();
	private:
		void generateLabelSequence(const std::shared_ptr<TrellisNode>& pathNode);
		std::vector<std::shared_ptr<ChartCell>> generateSlotSequence(std::shared_ptr<TrellisNode>& pathStart);
		std::vector<std::shared_ptr<TrellisNode>> paths;
		std::vector<std::wstring> seqStack;
		std::vector<std::vector<std::wstring>> labelSeqs;
	};
};

class EWildRule
{
public:
	EWildRule(const wchar_t * rulename) {fwprintf(stderr, L"ERROR: rule may be matched with any string. Check grammar near the rule %s\n", rulename);}
};

} //namespace Phoenix

#endif //PHOENIX_PHOENIXSLOTGRAPH_H

