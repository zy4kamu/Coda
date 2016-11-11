/** 
 * @file	GrammarNet.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

#include "GrammarNet.h"
#include "Grammar.h"
#include <locale>

using std::shared_ptr;
using std::wstring;
using std::vector;
using std::function;
using std::unordered_set;

namespace Phoenix {
/*
class GrammarNet
*/
GrammarNet::GrammarNet(wstring _netName, bool isConceptLeaf) : netName(_netName), conceptLeafFlag(isConceptLeaf)
{
	nodes = vector<shared_ptr<GrammarNode>>(); 
	type = computeType();
}

void GrammarNet::AddNode(const shared_ptr<GrammarNode>& node)
{
	nodes.push_back(node);
}

const std::wstring& GrammarNet::GetName() const
{
	return netName;
}

shared_ptr<GrammarNode> GrammarNet::GetStartNode() const
{
	return nodes.at(0);
}

bool GrammarNet::IsConceptLeaf() const
{
	return conceptLeafFlag;
}

NetType GrammarNet::computeType()
{
	wchar_t netMark = netName[1];

	if(netMark == L'_')
	{
		return PRETERMINAL;
	}
	else if(netMark == L'$')
	{
		return FUNCTIONAL;
	}
	else if(toupper(netMark) == netMark)
	{
		return BASIC;
	}
	else
	{
		return AUX;
	}
}

std::wstring GrammarNet::GetConciseName() const
{
	switch(type)
	{
	case BASIC:
		return netName;
	case AUX:
		return L"";
	case PRETERMINAL:
		return (netName.substr(2, netName.size() - 3));
	case FUNCTIONAL:
		return netName.substr(1, netName.size() - 2);
		break;
	}

	return L"";
}

NetType GrammarNet::GetType() const
{
	return type;
}

/*
NetArc descendants
*/
CallArc::CallArc(const shared_ptr<GrammarNode>& destination, const shared_ptr<GrammarNet>& argNet, bool _isHeadConsuming) : net(argNet), type(TCALL), isHeadConsuming(isHeadConsuming) {}

//contructor for lazy initialization of destination node and argument net. Necessary because arc my generally lead to net or node not yet created. (same for word and null arcs). See GetDestination()
//besides call nets should be able to work with other nets in the same manner. It could require storing shared_ptr to grammar object and thus loop reference. So, we use callback netRetriever
CallArc::CallArc(
		const shared_ptr<const GrammarNet>& _context, int netId,
		int stateId, bool _isHeadConsuming, function<shared_ptr<GrammarNet>()> netRetriever) :
		destinationStateId(stateId), netIndex(netId), context(_context), type(TCALL), isHeadConsuming(_isHeadConsuming)
{
	destinationNode = shared_ptr<GrammarNode>();
	net = shared_ptr<GrammarNet>();
	getNet = netRetriever; // net Retriever normally should be curried when the constructor is called
}

// As far as reading the net is carried out line by line the situation when the arc being constructed references non-existing node often is caused.
// To prevent constructor 1 (above) stores not nodes themselves but their indeces in the configuration file until the first reference
shared_ptr<GrammarNode> CallArc::GetDestination() const
{
	if (!destinationNode.get())
	{
		destinationNode = context.lock()->nodes.at(destinationStateId);
	}
	return destinationNode;
}

ArcType CallArc::GetType() const
{
	return type;
}

shared_ptr<GrammarNet> CallArc::GetArgument() const 
{
	if(!net)
	{
		net = getNet();
	}
	return net;
}

unordered_set<wstring> CallArc::GetFeatures() const
{
	return features;
}

void CallArc::SetFeatures(const vector<wstring>& _features)
{
	if(_features.empty())
	{
		return;
	}

	for(auto feat_p = _features.begin(); feat_p != _features.end(); feat_p++)
	{
		features.insert(*feat_p);	
	}
}

bool CallArc::IsHeadConsuming() const
{
	return isHeadConsuming;
}

WordArc::WordArc(const shared_ptr<GrammarNode>& destination, const wstring& argWord, bool _isHeadConsuming) : word(argWord), destinationNode(destination), type(TWORD), isHeadConsuming(_isHeadConsuming)  {}

unordered_set<wstring> WordArc::GetFeatures() const
{
	return features;
}

void WordArc::SetFeatures(const vector<wstring>& _features)
{
	if(_features.empty())
	{
		return;
	}

	for(auto feat_p = _features.begin(); feat_p != _features.end(); feat_p++)
	{
		features.insert(*feat_p);	
	}
}

//The same motivation for _context as for call arc except the dictionary is expected to be read
WordArc::WordArc(const shared_ptr<const GrammarNet>& _context, const wstring& argWord, int stateId, bool _isHeadConsuming) : destinationStateId(stateId), word(argWord), context(_context), type(TWORD), isHeadConsuming(_isHeadConsuming)
{
	destinationNode = shared_ptr<GrammarNode>();
}

ArcType WordArc::GetType() const
{
	return type;
}

shared_ptr<GrammarNode> WordArc::GetDestination() const
{
	if (!destinationNode.get())
	{
		destinationNode = context.lock()->nodes.at(destinationStateId);
	}
	return destinationNode;
}

const wstring& WordArc::GetArgument() const 
{
	return word;
}

bool WordArc::IsHeadConsuming() const
{
	return isHeadConsuming;
}

NullArc::NullArc(const shared_ptr<GrammarNode>& destination) : type(TNULL) {}

//The same motivation as for call arc except no arguments are needed
NullArc::NullArc(const shared_ptr<const GrammarNet>& _context, int stateId) : destinationStateId(stateId), context(_context), type(TNULL) 
{
	destinationNode = shared_ptr<GrammarNode>();
}

ArcType NullArc::GetType() const
{
	return type;
}

shared_ptr<GrammarNode> NullArc::GetDestination() const
{
	if (!destinationNode.lock())
	{
		destinationNode = context.lock()->nodes.at(destinationStateId);
	}
	return destinationNode.lock();
}

/*
class GrammarNode
*/
GrammarNode::GrammarNode(bool _isFinal) : isFinal(_isFinal)
{
	outArcs = vector<shared_ptr<NetArc>>(0);
}

void GrammarNode::AddArc(const shared_ptr<NetArc>& outArc)
{
	outArcs.push_back(outArc);
}

vector<shared_ptr<NetArc>>::const_iterator GrammarNode::GetArcIterator() const
{
	return outArcs.begin();
}

vector<shared_ptr<NetArc>>::const_iterator GrammarNode::GetArcEndIterator() const
{
	return outArcs.end();
}

bool GrammarNode::IsFinal() const
{
	return isFinal;
}

}  // namespace Phoenix
