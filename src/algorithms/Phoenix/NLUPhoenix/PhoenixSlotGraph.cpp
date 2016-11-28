/** 
 * @file	PhoenixSlotGraph.cpp
 * @brief	Implementation of the methods in the corresponding header. If you need theoretical description please reference 'Spoken language processing' by Huang, Acero, Hon, chapters 12.3 and 17.4.1
 */

#include "PhoenixSlotGraph.h"
#include "CrossPlatformHeader.h"

using std::shared_ptr;
using std::vector;
using std::map;
using std::list;
using std::unordered_set;
using std::unordered_map;
using std::wstring;
using std::weak_ptr;

namespace Phoenix
{
SlotTrellis::SlotTrellis(const shared_ptr<Grammar>& G)
{
	gram = G;
	trellis[0] = shared_ptr<TrellisStep>(new TrellisStep());
}

void SlotTrellis::AddSlot(const vector<shared_ptr<ChartCell>>& matchedSlot)
{
	//int prevStep = findPointToAttach(matchedSlot[0]->GetStartPosition()); //all matches share the same left position. We will attach them to the neariest path to the left.
	//for each possible matches of the slot at position
	for (auto ccp = matchedSlot.begin(); ccp != matchedSlot.end(); ccp++)
	{
		shared_ptr<ChartCell> possibleMatch = *ccp;
		shared_ptr<TrellisStep> currentStep;
		//Here we calculate the scores of potential new paths (which we would get after attachment) and decide
		//if we prun them or their earlier competing matches or leave both 
		PrunningDecision prune = getMaxScoreDecision(possibleMatch); 

		if(possibleMatch->GetEndPosition() < possibleMatch->GetStartPosition())
		{
			throw EWildRule(possibleMatch->GetNet()->GetName().c_str());
		}

		switch (prune)
		{
		case PRUN_OLD: 
			clearStepAtPoint(possibleMatch->GetEndPosition());
			currentStep = trellis[possibleMatch->GetEndPosition()]; //clean all the scores in the step
			currentStep = shared_ptr<TrellisStep>(new TrellisStep()); //begin new path from the start
			trellis[possibleMatch->GetEndPosition()] = currentStep; //so old path is prunned out
			appendToPath(possibleMatch);
			break;
		case ADD:
			appendToPath(possibleMatch); //add new path at time step as new variant
			break;
		case PRUN_NEW:
			break; //new path is prunned out, hence we have nothing to do
		}

	}
}

int SlotTrellis::scorePhrase(const shared_ptr<ChartCell>& phrase)
{
	int startPos = phrase->GetStartPosition();
	int endPos = phrase->GetEndPosition();

	return endPos - startPos + 1;
}

int SlotTrellis::findPointToAttach(int lowerThan)
{
	//just nearies left
	for(auto sp = trellis.rbegin(); sp != trellis.rend(); sp++)
	{
		if(sp->first < lowerThan)
		{
			return sp->first;
		}
	}

	return 0;
}

void SlotTrellis::appendToPath(const shared_ptr<ChartCell>& possibleMatch)
{
	//the function adds new path to the step and updates all its scores according to the match score and path scores before attachment
	unordered_set<wstring> possibleFrames = gram->GetPossibleFramesForNet(possibleMatch->GetNet());

	int prevStep = findPointToAttach(possibleMatch->GetStartPosition());
	shared_ptr<TrellisStep> currentStep = trellis[possibleMatch->GetEndPosition()];
	shared_ptr<TrellisNode> newNode;
	#if defined(COMPILER_MVS2010)
        #undef max
    #endif
	int minFrames = std::numeric_limits<int>::max(); //infinity
	if (prevStep)
	{
		//if we continue path we must calculate new fragmentation scores and set frame labels for new node. See function comments.
		continuePath(newNode, minFrames, possibleMatch, possibleFrames, prevStep, currentStep);
	}
	else
	{
		//if we init new path we just set fragmentation to 1 and set possible frame labels
		initPath(newNode, minFrames, possibleMatch, possibleFrames, prevStep, currentStep);
	}

	//when we set frame labels we can add new (expanded with new slot) path to the step
	//currentStep->AddPath(newNode);

	//and update scores at the step
	//currentStep->SetScores(getScoreAtPoint(prevStep) + scorePhrase(possibleMatch), getSlotNumAtPoint(prevStep) + 1, minFrames);

}

int SlotTrellis::getScoreAtPoint(int step)
{
	if(trellis.find(step) != trellis.end())
	{
		return trellis.at(step)->GetScore();
	}
	
	return 0;
}

SlotTrellis::PrunningDecision SlotTrellis::getMaxScoreDecision(const shared_ptr<ChartCell>& possibleMatch)
{
	int prevStep = findPointToAttach(possibleMatch->GetStartPosition());
	int newWordScore = getScoreAtPoint(prevStep) + scorePhrase(possibleMatch);
	int newSlotScore = getSlotNumAtPoint(prevStep) + 1;
	int oldWordScore = getScoreAtPoint(possibleMatch->GetEndPosition());
	int oldSlotScore = getSlotNumAtPoint(possibleMatch->GetEndPosition());

	if(newWordScore > oldWordScore)
	{
		return PRUN_OLD;
	}
	else if(newWordScore < oldWordScore)
	{
		return PRUN_NEW;
	}
	else if(newSlotScore < oldSlotScore)
	{
		return PRUN_OLD;
	}
	else if(newSlotScore > oldSlotScore)
	{
		return PRUN_NEW;
	}
	else
	{
		return ADD;
	}
}

void SlotTrellis::clearStepAtPoint(int timeStep)
{
	auto sp = trellis.find(timeStep);

	if(sp != trellis.end())
	{
		sp->second.reset();
	}
}

int SlotTrellis::getSlotNumAtPoint(int step)
{
	if(trellis.find(step) != trellis.end())
	{
		return trellis.at(step)->GetSlotNum();
	}
	
	return 0;
}

void SlotTrellis::continuePath(shared_ptr<TrellisNode>& newNode, int &minFrames, const shared_ptr<ChartCell>& possibleMatch, const  unordered_set<wstring>& possibleFrames, int prevStep, std::shared_ptr<TrellisStep> currentStep)
{
	//The function calculates minimum fragmentation score and sets frame labeling in a bit tricky way. Briefly the function does the following:
	//1) We set to the new node N_t all the possible frame labels.
	//2) For all the labels of the new slot N_t:
	// a. if label L is also the label of the previous node N_[t-1] in the path we do not increment fragmentation for the label, so fragmentationScore(N_[t], L) = fragmentationScore(N_[t - 1], L)
	// b. otherwise, we set fragmentationScore(N_[t], L)  for the label L to fragmentationScore(N_[t-1]) + 1
	//3) fragmentationScore(N_[t]) is set to $\min_{L}fragmentationScore(N_[t], L)$ 

	if(trellis.rbegin()->second != trellis[prevStep])
	{
		if(trellis.rbegin()->second->GetLabelFragmentation() != 0)
			minFrames = trellis.rbegin()->second->GetLabelFragmentation();
	}

	for(auto pp = trellis[prevStep]->GetPathIterator(); pp != trellis[prevStep]->GetPathEndIterator(); pp++)
	{
        unordered_set<wstring> activeFrames;
        Tools::Intersect(possibleFrames, (*pp)->GetPossibleFrames(), &activeFrames); //active frames are the frames which were the labels of the previous node

		newNode = shared_ptr<TrellisNode>(new TrellisNode(possibleMatch, *pp));
		(*pp)->SetNext(newNode);

		if(!activeFrames.empty())
		{
			for(auto label = activeFrames.begin(); label != activeFrames.end(); label++ ) //for each active frame
			{
				newNode->SetFrameLabel(*label, (*pp)->GetLabelFragmentation(*label)); //set corresponding frame label to the node with fragmentationScore(N_[t], L) = fragmentationScore(N_[t - 1], L)
				if ((*pp)->GetLabelFragmentation(*label) < minFrames)
				{
					minFrames = (*pp)->GetLabelFragmentation(*label); //find current fragmentationScore(N_[t])
				}
			}
		}

        unordered_set<wstring> inactiveFrames;
        Tools::GetDifference(possibleFrames, activeFrames, &inactiveFrames);

		if(!inactiveFrames.empty())
		{
			for(auto label = inactiveFrames.begin(); label != inactiveFrames.end(); label++ ) //for all non-active frames
			{
				newNode->SetFrameLabel(*label, trellis[prevStep]->GetLabelFragmentation() + 1); //set corresponding frame label to the node with fragmentationScore(N_[t], L) = fragmentationScore(N_[t - 1]) + 1
				if (trellis[prevStep]->GetLabelFragmentation() + 1 < minFrames)
				{
					minFrames = trellis[prevStep]->GetLabelFragmentation() + 1; //find current fragmentationScore(N_[t])
				}
			}
		}

	//when we set frame labels we can add new (expanded with new slot) path to the step
	currentStep->AddPath(newNode);

	//and update scores at the step
	currentStep->SetScores(getScoreAtPoint(prevStep) + scorePhrase(possibleMatch), getSlotNumAtPoint(prevStep) + 1, minFrames);

	}
}

void SlotTrellis::initPath(shared_ptr<TrellisNode>& newNode, int &minFrames, const shared_ptr<ChartCell>& possibleMatch, const  unordered_set<wstring>& possibleFrames, int prevStep, std::shared_ptr<TrellisStep> currentStep)
{
	newNode = shared_ptr<TrellisNode>(new TrellisNode(possibleMatch, shared_ptr<TrellisNode>()));
	minFrames = 1;
	for(auto label = possibleFrames.begin(); label != possibleFrames.end(); label++ )
	{
		newNode->SetFrameLabel(*label, trellis[prevStep]->GetLabelFragmentation() + 1);
	}

	//when we set frame labels we can add new (expanded with new slot) path to the step
	currentStep->AddPath(newNode);

	//and update scores at the step
	currentStep->SetScores(getScoreAtPoint(prevStep) + scorePhrase(possibleMatch), getSlotNumAtPoint(prevStep) + 1, minFrames);

}

void SlotTrellis::Clear()
{
	trellis.clear();
	trellis[0] = shared_ptr<TrellisStep>(new TrellisStep());
}

vector<shared_ptr<Parse>> SlotTrellis::GetBestMatch() 
{
	vector<shared_ptr<Parse>> toReturn;

	int endPos = findBestEndPosition();

	if(!endPos)
	{
		return toReturn;
	}

	vector<shared_ptr<TrellisNode>> bestPaths = getLeastFragmentedPaths(endPos);

	if(bestPaths.empty())
	{
		return toReturn;
	}

	vector<shared_ptr<Parse>> parses = generateAllParses(bestPaths); 

	store.clear();

	vector<int> oneStepInconsistency;
	vector<int> nStepInconsistency;
	//there are two typees of consistency fragmentation - weak and strong. They are both used in lableling ranking.
	int minOneStepInconsistency = std::numeric_limits<int>::max(); 
	int minNStepInconsistency = std::numeric_limits<int>::max();

	//we calculate fragmentation for all possible labelings
	for(size_t i = 0; i < parses.size(); i++)
	{
		scoreFragmentation(oneStepInconsistency, nStepInconsistency, minOneStepInconsistency, minNStepInconsistency, parses[i], 5);
	}

	//and take the best ones
	for(size_t i = 0; i < parses.size(); i++)
	{
		if(oneStepInconsistency[i] == minOneStepInconsistency && nStepInconsistency[i] == minNStepInconsistency)
		{
			toReturn.push_back(parses[i]);
		}
	}

	return toReturn;
}

void SlotTrellis::scoreFragmentation(vector<int>& oneStepInconsistency, vector<int>& nStepInconsistency, 
									int& minOneStepInconsistency, int &minNStepInconsistency, 
									const shared_ptr<Parse>& parse, int weakFragmentation)
{

	const vector<wstring> frames = parse->GetFrameSequence();
	const vector<shared_ptr<ChartCell>> slots = parse->GetSlotSequence();
	int c1 = 0;
	int c2 = 0;

	if(frames.size() > 1)
	{
		for(size_t i = 1; i < frames.size(); i++) 
		{
			int frag2 = 0;

			if(frames[i] != frames[i-1] )  //if previous frame label is not equal to the current one increment fragmentation
			{
					c1++;
			}

			for(int j = i - 1; j >= 0; j--) //if there is no such frame label in the previous n=weakFragmentation frames increment weak fragmentation
			{
				if(frames[i] == frames[j])
				{
					break;
				}
				frag2++;
			}

			if(frag2 == weakFragmentation)
			{
				c2++;
			}
			
		}
	}
	else
	{
		c1 = 1;
		c2 = 1;
	}

	if( c1 < minOneStepInconsistency ) 
	{
		minOneStepInconsistency= c1; //minimum inconsistency for all labelings
		minNStepInconsistency = c2;
	}
	else if( c1 == minOneStepInconsistency ) 
	{
		if( c2 < minNStepInconsistency ) 
			minNStepInconsistency= c2;
	}

	oneStepInconsistency.push_back(c1);
	nStepInconsistency.push_back(c2);

}

vector<shared_ptr<Parse>> SlotTrellis::generateAllParses(const vector<shared_ptr<TrellisNode>>& bestPaths)
{

	labelSequenceGenerator g(bestPaths);

	return g.generate();
}

int SlotTrellis::findBestEndPosition()
{
	int best_wp= 0;

	for(auto sp = trellis.begin(); sp != trellis.end(); sp++) 
	{
		/* compare score */
		if( sp->second->GetScore() != trellis[best_wp]->GetScore() ) 
		{
			best_wp = sp->second->GetScore() > trellis[best_wp]->GetScore() ? sp->first : best_wp;
			continue;
		}
		/* compare number of slots */
		if( sp->second->GetSlotNum() != trellis[best_wp]->GetSlotNum() ) 
		{
			best_wp = sp->second->GetSlotNum() < trellis[best_wp]->GetSlotNum() ? sp->first : best_wp;
			continue;
		}
		/* compare number of frames */
		if(sp->second->GetLabelFragmentation() != trellis[best_wp]->GetLabelFragmentation() ) 
			best_wp = sp->second->GetLabelFragmentation() < trellis[best_wp]->GetLabelFragmentation() ? sp->first : best_wp;
			continue;
	}
	return best_wp;
}

TrellisStep::TrellisStep() : maxWordsScoreAtStep(0), minSlotScoreAtStep(0), frameFragmentation(0)
{
	alivePaths = list<shared_ptr<TrellisNode>>(0);
}

vector<shared_ptr<TrellisNode>> SlotTrellis::getLeastFragmentedPaths(int endPos)
{
	shared_ptr<TrellisStep> lastStep = trellis[endPos];

	vector<shared_ptr<TrellisNode>> bestPaths;

	for(auto pp = lastStep->GetPathIterator(); pp != lastStep->GetPathEndIterator(); pp++) 
	{
		shared_ptr<TrellisNode> temp = shared_ptr<TrellisNode>(new TrellisNode(**pp));
		unordered_set<wstring> possibleLabels = temp->GetPossibleFrames();
		int count = possibleLabels.size();
		for(auto fp = possibleLabels.begin(); fp != possibleLabels.end(); fp++) 
		{
			if( temp->GetLabelFragmentation(*fp) !=  lastStep->GetLabelFragmentation()) 
			{
				count--;
				temp->DeleteLabel(*fp);
			}
		}
		if( count >= 1 ) 
		{
			shared_ptr<TrellisNode> beg = shared_ptr<TrellisNode>(new TrellisNode(*temp));
			shared_ptr<TrellisNode> beg2 = shared_ptr<TrellisNode>(new TrellisNode(*temp));
			store.push_back(beg2);
			while(beg->ToPrevious()) //get to the start position of the path
			{
				beg = shared_ptr<TrellisNode>(new TrellisNode(*(beg->ToPrevious())));
				store.push_back(beg);
				beg->SetNext(beg2);
				beg2 = beg;
			}  
			bestPaths.push_back(beg);
		}
	}

	return bestPaths;
}

void TrellisStep::Clean()
{
	alivePaths.clear();
	maxWordsScoreAtStep = 0;
	minSlotScoreAtStep = 0;
	frameFragmentation = 0;
}

int TrellisStep::GetLabelFragmentation() const
{
	return frameFragmentation;
}

int TrellisStep::GetScore() const
{
	return maxWordsScoreAtStep;
}

int TrellisStep::GetSlotNum() const
{
	return minSlotScoreAtStep;
}

void TrellisStep::SetScores(int wordScore, int slotScore, int minFrames)
{
	maxWordsScoreAtStep = wordScore;
	minSlotScoreAtStep = slotScore;
	frameFragmentation = minFrames;
}
	
void TrellisStep::AddPath(const shared_ptr<TrellisNode>& newNode)
{
	alivePaths.push_back(newNode);
}

list<shared_ptr<TrellisNode>>::const_iterator TrellisStep::GetPathIterator() const
{
	return alivePaths.begin();
}

list<shared_ptr<TrellisNode>>::const_iterator TrellisStep::GetPathEndIterator() const
{
	return alivePaths.end();
}

TrellisNode::TrellisNode()
{
	previous = shared_ptr<TrellisNode>();
	next = weak_ptr<TrellisNode>();
	possibleFrames = unordered_map<wstring, int>();
	correspondingMatch = shared_ptr<ChartCell>();
}

TrellisNode::TrellisNode(const shared_ptr<ChartCell>& match, const shared_ptr<TrellisNode>& _previous)
{
	previous = _previous;
	next = weak_ptr<TrellisNode>();
	possibleFrames = unordered_map<wstring, int>();
	correspondingMatch = match;
}

std::shared_ptr<ChartCell> TrellisNode::GetMatch() const
{
	return correspondingMatch;
}

void TrellisNode::SetFrameLabel(const wstring& label, int fragmentScore)
{
	possibleFrames[label] = fragmentScore;
}

int TrellisNode::GetLabelFragmentation(const wstring& frameLabel) const
{
	return possibleFrames.at(frameLabel);
}

unordered_set<wstring> TrellisNode::GetPossibleFrames() const
{
	unordered_set<wstring> toReturn;
	for(auto p = possibleFrames.begin(); p != possibleFrames.end(); p++)
	{
		toReturn.insert(p->first);
	}
	return toReturn;
}

shared_ptr<TrellisNode> TrellisNode::ToPrevious() const
{
	return previous;
}

shared_ptr<TrellisNode> TrellisNode::ToNext() const
{
	return next.lock();
}

void TrellisNode::SetNext(const shared_ptr<TrellisNode>& node)
{
	next = weak_ptr<TrellisNode>(node);
}

void TrellisNode::DeleteLabel(const wstring& label)
{
	possibleFrames.erase(label);
}

SlotTrellis::labelSequenceGenerator::labelSequenceGenerator(const std::vector<std::shared_ptr<TrellisNode>>& _paths) : paths(_paths)  { }

void SlotTrellis::labelSequenceGenerator::generateLabelSequence(const shared_ptr<TrellisNode>& pathNode)
{
	unordered_set<wstring> frames =  pathNode->GetPossibleFrames();

	for(auto flp = frames.begin(); flp != frames.end(); flp++)
	{
		seqStack.push_back(*flp);
		if(!pathNode->ToNext())
		{
			labelSeqs.push_back(seqStack);
		}
		else
		{
			generateLabelSequence(pathNode->ToNext());
		}
		seqStack.pop_back();
	}
}

std::vector<std::shared_ptr<Parse>> SlotTrellis::labelSequenceGenerator::generate()
{
	vector<shared_ptr<Parse>> toReturn;

	for(auto pp = paths.begin(); pp != paths.end(); pp++)
	{
		generateLabelSequence(*pp);
		//generate slot sequences with different labelings recursively
		vector<shared_ptr<ChartCell>> slotSequence = generateSlotSequence(*pp);
		for(auto lsp = labelSeqs.begin(); lsp != labelSeqs.end(); lsp++)
		{
			//and construct parse objects based on such sequences
			toReturn.push_back(shared_ptr<Parse>(new Parse(*lsp, slotSequence)));
		}
		labelSeqs.clear();
	}

	return toReturn;
}

vector<shared_ptr<ChartCell>> SlotTrellis::labelSequenceGenerator::generateSlotSequence(shared_ptr<TrellisNode>& pathStart)
{
	vector<shared_ptr<ChartCell>> toReturn;

	shared_ptr<TrellisNode> node = pathStart;

	toReturn.push_back(node->GetMatch());

	while(node = node->ToNext())
	{
		toReturn.push_back(node->GetMatch());
	}

	return toReturn;

}

} //namespace Phoenix
