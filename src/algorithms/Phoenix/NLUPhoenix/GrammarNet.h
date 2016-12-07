/** 
 * @file	GrammarNet.h 
 * @brief	Header file contains definitions of classes describing grammar rules used by Phoenix::Grammar class
 * @details  GrammarNet - recursive network representing particular semantic unit of interest. 
 *			 The network contains nodes and arcs between them. They are represented by NetArc and GrammarNode classes correspondingly 
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


#ifndef PHOENIX_GRAMMARNET_H
#define PHOENIX_GRAMMARNET_H

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_set>

namespace Phoenix 
{

class NetArc;
class GrammarNet;

/**
@enum ArcType
Describes arc types in the grammar net:
TWORD is an arc which allows transition if the word is match (thus, word is an arc argument)
TNULL is an epsilon transition
TCALL is a transition made if the nested net match is successful (net argument match)
*/
enum ArcType {TWORD, TNULL, TCALL};  


/**
@enum NetType
Types of gramamr rules
 BASIC stands for rules needed for frame generation and generation  concise output (see Parse.h). BASIC rules must have conceptFlag set to true to appear in concise output (CO).
 Name pattern in grammar: [Net_Name]
 AUX stands for rules needed for rules which are non-essential for frame generation and do not appear in concise output
 Name pattern in grammar: [net_name]
 PRETERMINAL stands for rules mapping groups of synonyms onto one conventional terminal e.g. (closest|soonest|earliest) -> earliest. Do not appear in CO without leading '_'.
 Name pattern in grammar: [_netname]
 FUNCTION stands for terminals whose value must be post-evaluated for frame generation e.g. $tomorrow or $(week ago). Appear in CO with prefix $.
 Name pattern [$net_name]
*/
enum NetType {BASIC, AUX, PRETERMINAL, FUNCTIONAL};

/**
@class GrammarNode
@brief class for node of Phoenix grammar Net
@author M.Kudinov, SRR
*/
class GrammarNode
{
public:
	/**
	@brief Constructor
	@param isFinal true if the state is a final state
	*/
	explicit GrammarNode(bool isFinal);
	~GrammarNode() {};

	/**
	@brief Adds arc going out of the node
	@param outArc netArc which will emanate from the node
	*/
	void AddArc(const std::shared_ptr<NetArc>& outArc);

	/**
	@brief Returns iterator on outgoing arcs
	*/
	std::vector<std::shared_ptr<NetArc>>::const_iterator GetArcIterator() const;

	/**
	@brief Returns end iterator on outgoing arcs
	*/
	std::vector<std::shared_ptr<NetArc>>::const_iterator GetArcEndIterator() const;

	/**
	@brief Returns true if the state is final
	*/
	bool IsFinal() const;
private:
	std::vector<std::shared_ptr<NetArc>> outArcs;
	bool isFinal;
};

/**
@class NetArc
@brief class for arc in Phoenix Grammar Net
@remark There are three types of arcs depending on their transition arguments: word arc, net arcs and null arcs. The hierarchy corresponds to the class hierarchy.
@author M.Kudinov, SRR
*/
class NetArc
{
public:
	~NetArc() {};
	/**
	@brief Returns destination node of the arc
	*/
	virtual std::shared_ptr<GrammarNode> GetDestination() const = 0;

	/**
	@brief Returns type of the arc
	@todo It would be better to implement visitor as soon as the type is used only once when in during the parsing
	*/
	virtual ArcType GetType() const = 0;
};

/**
@class CallArc
@brief class for arc in Phoenix Grammar Net
@details Net arc with net thransition. Transition is made when the argument net is matched i.e. if net transition argument is encountered 
		 an algorithm should get to the start state of the net argument and match net argument at first. 
		 Then if matching is successful we can go to the next state of the calling net
@author M.Kudinov, SRR
*/
class CallArc : public NetArc
{
public:
	/**
	@brief Constructor
	@param destination destination state of the arc
	@param argNet net argument of the arc
	*/
	CallArc(const std::shared_ptr<GrammarNode>& destination, const std::shared_ptr<GrammarNet>& argNet, bool _isHeadConsuming);

	/**
	@brief Constructor
	@details This version of the constructor takes parameters accessible to the program while reading .net file
	As far as the arc may call a net or lead to a state not yet read we have to store context variables to be able to restore all necessary references after the file is read completely
	@param _context grammar net the arc belongs to
	@param netId id of the net argument read from the *.net file
	@param stateId id of the state the arc leads to
	@param netRetriever function restoring net reference by id
	@remark netRetriever (instead of e.g. pointer) was used to avoid loop references
	@todo It is reasonable to establish special class for reading the phoenix data files. Special constructor for the net looks weird.
	*/
	CallArc(const std::shared_ptr<const GrammarNet>& _context, int netId, int stateId, bool _isHeadConsuming, std::function<std:: shared_ptr<GrammarNet>()> netRetriever);

	/**
	@see NetArc::GetDestination()
	*/
	std::shared_ptr<GrammarNode> GetDestination() const;

	/**
	@brief Returns TCALL
	@see ArcType
	*/
	ArcType GetType() const;

	/**
	@brief Returns net argument for matching
	*/
	std::shared_ptr<GrammarNet> GetArgument() const;

	/**
	@brief Sets list of admissible features for the arc argument 
	@param _features List of admissible features for the argument
	*/
	void  SetFeatures(const std::vector<std::wstring>& _features);

	/**
	@brief Returns list of admissible grammar features of the argument 
	*/
	std::unordered_set<std::wstring> GetFeatures() const;

	/**
	@brief Returns true if processed net must push its grammar features up through the parsing tree
	*/
	bool IsHeadConsuming() const;

private:
	int destinationStateId; // temporary index indicating destination node until all net is read
	int netIndex; // temporary index for setting netsted net after all nets are read
	const std::weak_ptr<const GrammarNet> context; //net object containing arc
	ArcType type;
	std::function<std::shared_ptr<GrammarNet>()> getNet;  //Necessary to avoid problems with constants and references without direct working with GrammarObject

	mutable std::shared_ptr<GrammarNet> net; //net is a transition argument of net arc
	mutable std::shared_ptr<GrammarNode> destinationNode; // see comment to GetDestination()

	std::unordered_set<std::wstring> features;

	bool isHeadConsuming;

	CallArc( const CallArc &); //Necessary to avoid problems with constants and references
	CallArc &operator =( const CallArc & ); //Necessary to avoid problems with constants and references
};

/**
@class WordArc
@brief Word arc with word transition. Meaning of fields and methods is mostly the same as for CallArc
@see CallArc
@author M.Kudinov, SRR
*/
class WordArc : public NetArc
{
public:
	/**
	@brief Constructor
	@param destination destination state of the arc
	@param argWord word argument of the arc
	@param isHeadConsuming true if grammar features of the parent net must be borrowed from the word argument
	*/
	WordArc(const std::shared_ptr<GrammarNode>& destination, const std::wstring& argWord, bool isHeadConsuming);

	/**
	@brief Reading-safe constructor
	@see CallArc
	@param _context grammar net the arc belongs to
	@param argWord argument word
	@param stateId id of the state the arc leads to
	@param isHeadConsuming true if grammar features of the parent net must be borrowed from the word argument
	*/
	WordArc(const std::shared_ptr<const GrammarNet>& _context, const std::wstring& argWord, int stateId, bool isHeadConsuming);

	/**
	@see NetArc::GetDestination()
	*/
	std::shared_ptr<GrammarNode> GetDestination() const;

	/**
	@brief Returns TWORD
	@see ArcType
	*/
	ArcType GetType() const;

	/**
	@brief Returns word argument for matching
	*/
	const std::wstring& GetArgument() const;

	/**
	@brief Sets list of admissible features for the arc argument 
	@param _features List of admissible features for the argument
	*/
	void  SetFeatures(const std::vector<std::wstring>& _features);

	/**
	@brief Returns list of admissible grammar features of the argument 
	*/
	std::unordered_set<std::wstring> GetFeatures() const;

	/**
	@brief Returns true if processed word must push its grammar features up through the parsing tree
	*/
	bool IsHeadConsuming() const;

private:
	const std::wstring& word; //word is a transition argument of word arc
	mutable std::shared_ptr<GrammarNode> destinationNode;
	ArcType type;

	const std::weak_ptr<const GrammarNet> context;
	int destinationStateId;

	WordArc( const WordArc &);
	WordArc &operator =( const WordArc & ); 

	bool isHeadConsuming;

	std::unordered_set<std::wstring> features;
};

/**
@class NullArc
@brief Arc for epsilon-transition. Meaning of fields and methods is mostly the same as for CallArc
@author M.Kudinov, SRR
*/
class NullArc : public NetArc
{
public:
	/**
	@brief Constructor
	@param destination destinationNode
	*/
	NullArc(const std::shared_ptr<GrammarNode>& destination);

	/**
	@brief reading-safe constructor
	@see CallArc
	*/
	NullArc(const std::shared_ptr<const GrammarNet>& _context, int stateId);

	/**
	@brief reading-safe constructor
	@see NetArc
	*/
	std::shared_ptr<GrammarNode> GetDestination() const;

	/**
	@brief reading-safe constructor
	@see NetArc
	*/
	ArcType GetType() const;

private:
	int destinationStateId;
	const std::weak_ptr<const GrammarNet> context;

	mutable std::weak_ptr<GrammarNode> destinationNode;
	ArcType type;

	NullArc( const NullArc &);
	NullArc &operator =( const NullArc & ); 

};

/**
@class GrammarNet
@brief Phoenix grammar rule. Contains nodes, each of which has a list of outgoing arcs.
@author M.Kudinov, SRR
*/
class GrammarNet
{
friend class CallArc; 
friend class WordArc;
friend class NullArc;

public:
	GrammarNet(std::wstring netName, bool isConceptLeaf);
	~GrammarNet() {};

	/**
	@brief Adds node to the net
	@param node New state for the net
	*/
	void AddNode(const std::shared_ptr<GrammarNode>& node);

	/**
	@brief Returns alias of the net
	*/
	const std::wstring& GetName() const;

	/**
	@brief Returns first start node of the grammar net
	*/
	std::shared_ptr<GrammarNode> GetStartNode() const;

	/**
	@brief Returns true if the net should preserve information about its terminals
	*/
	bool IsConceptLeaf() const;

	/**
	@brief Returns nickname for frame generation and concise output
	*/
	std::wstring GetConciseName() const;
	/**
	@brief Returns NetType
	@see NetType
	*/
	NetType GetType() const;
private:
	NetType computeType();
	std::wstring netName;
	std::vector<std::shared_ptr<GrammarNode>> nodes;
	bool conceptLeafFlag;
	NetType type;
};

} //namespace Phoenix

#endif
