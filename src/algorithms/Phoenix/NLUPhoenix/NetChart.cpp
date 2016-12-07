/** 
 * @file	NetChart.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

#include "NetChart.h"
//#include "FileManager.h"

using std::shared_ptr;
using std::wstring;
using std::vector;
using std::unordered_map;
using std::map;


namespace Phoenix 
{

NetChart::NetChart()
{
	netToEntryPositions = unordered_map<wstring, shared_ptr<netInChartX>>();
}

void NetChart::RegisterEntry(const shared_ptr<GrammarNet>& net, int wordPosition)
{
	//if there is no net entry in chart yet we'll create it
	if(netToEntryPositions.find(net->GetName()) == netToEntryPositions.end())
	{
		netToEntryPositions[net->GetName()] = shared_ptr<netInChartX>(new netInChartX());
	}
	//and create empty list of MatchTrees beginning at position wordPosition
	netToEntryPositions[net->GetName()]->entriesByLeftPosition[wordPosition] = std::shared_ptr<netInChartY>(new netInChartY());
}

NetChart::netInChartY::netInChartY()
{
	entriesByRightPosition = chartColumn();
}

NetChart::netInChartX::netInChartX()
{
	entriesByLeftPosition = chartRow();
}

bool NetChart::CheckMatchSuccess(const shared_ptr<GrammarNet>& net, int wordPosition) const
{
	if(netToEntryPositions.find(net->GetName()) == netToEntryPositions.end())
	{
		throw ENoMatchingAttemptRegistered();
	}

	shared_ptr<netInChartX> netChartEntry = netToEntryPositions.at(net->GetName());
	if(netChartEntry->entriesByLeftPosition.find(wordPosition) == netChartEntry->entriesByLeftPosition.end())
	{
		throw ENoMatchingAttemptRegistered();
	}

	//if matching was successful NetEntries contains non empty list of MatchTrees at position wordPosition
	return !(netChartEntry->entriesByLeftPosition.at(wordPosition)->entriesByRightPosition.empty());
}

bool NetChart::CheckMatchTreeAttempt(const shared_ptr<GrammarNet>& net, int wordPosition) const
{
	//if either there is no net entry in chart at all or net entry does not contain key wordPosition on netChartEntry
	//net is considered not to be attempted
	if(netToEntryPositions.find(net->GetName()) == netToEntryPositions.end())
	{
		return false;
	}

	shared_ptr<netInChartX> netChartEntry = netToEntryPositions.at(net->GetName());
	if(netChartEntry->entriesByLeftPosition.find(wordPosition) == netChartEntry->entriesByLeftPosition.end())
	{
		return false;
	}

	return true;
}

//Arguments startPosition and endPosition must be equal to values GetStartPosition() and GetEndPosition() of the match(arg1)
//The arguments are provided explicitly just to make the interface more descriptive
void NetChart::AddMatch(const shared_ptr<MatchTree>& match, int startPosition, int endPosition)
{
	wstring netName = match->GetNet()->GetName();
	if(startPosition != match->GetStartPosition() || 
		endPosition != match->GetEndPosition())
	{
		throw EInconsistentInsertion();
	}

	//generally the situation when the entry does not yet exist must not arise, because RegisterEntry is always called before. 
	//Maybe throwing exception is better
	if(!CheckMatchTreeAttempt(match->GetNet(), startPosition))
	{
		RegisterEntry(match->GetNet(), startPosition);
	}

	shared_ptr<netInChartY> entriesAtPosition = netToEntryPositions[netName]->entriesByLeftPosition[startPosition];
	shared_ptr<vector<shared_ptr<MatchTree>>> netEntriesAtXY = entriesAtPosition->entriesByRightPosition[endPosition];

	if(!netEntriesAtXY.get())
	{
		netEntriesAtXY = shared_ptr<vector<shared_ptr<MatchTree>>>(new vector<shared_ptr<MatchTree>>(0));
		entriesAtPosition->entriesByRightPosition[endPosition] = netEntriesAtXY;
	}

	netEntriesAtXY->push_back(match);
}

vector<shared_ptr<MatchTree>> NetChart::GetMatch(const shared_ptr<GrammarNet>& net, int wordPosition) const
{
	if(!CheckMatchSuccess(net, wordPosition))
	{
		throw ENoMatchingAttemptRegistered();
	}

	chartColumn entriesAtPosition = netToEntryPositions.at(net->GetName())->entriesByLeftPosition.at(wordPosition)->entriesByRightPosition;

	vector<shared_ptr<MatchTree>> toReturn;

	for(auto cp = entriesAtPosition.begin(); cp != entriesAtPosition.end(); cp++)
	{
		toReturn.insert(toReturn.end(), cp->second->begin(), cp->second->end());
	}

	return toReturn;
}

vector<shared_ptr<ChartCell>> NetChart::GetNonEmptyCellsForNet(const shared_ptr<GrammarNet>& net) const
{
	//TODO: implementation if needed
	return vector<shared_ptr<ChartCell>>(0);
}

vector<std::shared_ptr<ChartCell>> NetChart::GetNonEmptyCellsForNet(const shared_ptr<GrammarNet>& net, int leftPosition) const
{
	if(!CheckMatchSuccess(net, leftPosition))
	{
		return vector<shared_ptr<ChartCell>>(0);
	}

	chartColumn entriesAtPosition = netToEntryPositions.at(net->GetName())->entriesByLeftPosition.at(leftPosition)->entriesByRightPosition;

	vector<shared_ptr<ChartCell>> toReturn;

	for(chartColumn::iterator cp = entriesAtPosition.begin(); cp != entriesAtPosition.end(); cp++)
	{
		toReturn.push_back(shared_ptr<ChartCell>(new ChartCell(cp->second->at(0))));
	}

	return toReturn;
}

ChartCell::ChartCell(const shared_ptr<MatchTree>& _match)
{
	match = _match;
}

int ChartCell::GetEndPosition() const
{
	return match->GetEndPosition();
}

int ChartCell::GetStartPosition() const
{
	return match->GetStartPosition();
}

const std::shared_ptr<MatchTree> ChartCell::GetMatch() const
{
	return match;
}

shared_ptr<GrammarNet> ChartCell::GetNet() const
{
	return match->GetNet();
}

wstring ChartCell::GetNetName() const
{
	return match->GetNet()->GetName();
}

MatchTree::MatchTree(const shared_ptr<GrammarNet>& currentNet, int _wordPosition)
{
	net= currentNet;
	startWordPosition = _wordPosition;
	endWordPosition = INITIAL_END;
	score= 0;
	children = vector<shared_ptr<MatchTree>>(0);
	wordPosition = _wordPosition;
}

void MatchTree::ConsumeWord(wstring word)
{
	wordPosition++;
	consumedWords.push_back(word);
}

void MatchTree::AppendChild(const shared_ptr<MatchTree>& subTree)
{
	wordPosition = subTree->GetEndPosition() + 1;
	children.push_back(subTree);
}

int MatchTree::GetStartPosition() const
{
	return startWordPosition;
}

int MatchTree::GetEndPosition() const
{
	return endWordPosition;
}

int MatchTree::GetCurrentWordPosition() const
{
	return wordPosition;
}

shared_ptr<GrammarNet> MatchTree::GetNet() const
{
	return net;
}

void MatchTree::Finalize()
{
	endWordPosition = wordPosition - 1;
}

vector<shared_ptr<MatchTree>>::const_iterator MatchTree::GetChildMatchesIterator() const
{
	return children.begin();
}

vector<shared_ptr<MatchTree>>::const_iterator MatchTree::GetChildMatchesEndIterator() const
{
	return children.end();
}

bool MatchTree::HasChildren() const 
{
	return !children.empty();
}

std::unordered_map<std::wstring, std::wstring> MatchTree::GetFeatures() const
{
	//Print("/home/stepan/aux.txt", features);

	return features;
}

void MatchTree::SetFeatures(const unordered_map<wstring, wstring>& _features) //
{
//	Print("/home/stepan/aux.txt", _features);

	features = _features;

}

const vector<wstring>& MatchTree::GetMatchedWordEntries() const
{
	return consumedWords;
}

TimexMatch::TimexMatch(Slots _timexSlots)
{
	timexSlots = _timexSlots;
}

Slots TimexMatch::GetSlots() const
{
	return timexSlots;
}

wstring TimexMatch::GetNetName() const
{
	TimexFormatConverter tconv;

	wstring toReturn;

	tconv.ReturnTimeAndDateForPhoenix(timexSlots, toReturn);

	return toReturn;
}

} //namespace Phoenix
