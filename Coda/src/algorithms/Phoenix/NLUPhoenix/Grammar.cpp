/** 
 * @file	Grammar.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Grammar.h"
#include "Tools.h"
#include <locale>
#include <algorithm>

using std::shared_ptr;
using std::string;
using std::wstring;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using namespace Tools;

namespace Phoenix {

Grammar::Grammar(const string& _dict_file, const string& _grammar_file, const string& _frames_file)
{
	dict_file = _dict_file;
	grammar_file = _grammar_file;
	frames_file =  _frames_file;

	read_dict();
	read_nets();
	read_frames();
}

// Reads dictionary from file. Each entry is fomatted as:
// WORD wordId
int Grammar::read_dict()
{
	wifstream in(dict_file);
    if(in == NULL)
    	throw "Grammar::read_dict(): File not found " + dict_file;

    int		wordId;
    wstring utterance;
    wstring buffer;

	while(getline(in, buffer))
	{
		vector<wstring> splitted = Split(
            buffer, L" ");
		if(Trim(buffer).length() == 0)
			continue;
		utterance = splitted[0];
		wordId = stoi(splitted[1]);
		if( (wordId < 0) || (wordId > MAX_WRDS) ) 
			{ throw EErrorReadingDictionary(); }
		std::transform(utterance.begin(), utterance.end(), utterance.begin(), ::towupper);
		wrds[wordId] = utterance;
	}

	in.close();
	return wrds.size();
}

// Reads grammar networks from file. 
// Each entry in the file is one of the following:
// a) title ("Number of Nets=...)
// b) net description in format: <NET_ALIAS> <INDEX NUMBER_OF_NODES> <CONCEPT_LEAF_FLAG>
// The purpose of the latter remains unclear yet.
// c) node description in format: <NODE_NUMBER> <NUMBER_OF_OUTGOING_ARCS> <FINAL_FLAG>
// d) outgoing arc description in format: <WORD_NUMBER> <NET_NUMBER> <TO_NODE>
// An arc may be a:
// word arc, indicated by WORD_ID != 0 and NET_ID = 0
// null arc, indicated by WORD_ID = 0 and NET_ID = 0
// call arc, indicated by NET_ID > 0 (WORD_ID is ignored in this case)
void Grammar::read_nets()
{
	wifstream in(grammar_file);
	if(in == NULL)
		throw "Grammar::read_nets(): File not found " + grammar_file;

	wstring buffer;
	vector<wstring> vectorBuffer;

	wstring name;
	wstring feats;
	int	num_nodes, net_num, num_nets;
	int leafFlag;

	getline(in, buffer);
	vectorBuffer = Tools::Split(buffer, L" ");
	num_nets = stoi(vectorBuffer[vectorBuffer.size() - 1]);

	nets = vector<shared_ptr<GrammarNet>>(num_nets);

	while(getline(in, buffer))
	{
        vectorBuffer = Tools::Split(buffer, L" ", true);
    //	if (vectorBuffer.size() != 5) break;
		name = vectorBuffer[0];
		net_num = stoi(vectorBuffer[1]);
		num_nodes = stoi(vectorBuffer[2]);
		leafFlag = stoi(vectorBuffer[3]);
    //	feats = vectorBuffer[4];
		shared_ptr<GrammarNet> tempNet = shared_ptr<GrammarNet>(new GrammarNet(wstring(name), leafFlag == 1));
		wstring commonFeats = Trim(feats);
		for(int  i = 0; i < num_nodes; i++)
		{
			int nodeNumber = 0, numberOfSuccessors = 0;
			int isFinal = 0;
			getline(in, buffer);
			vectorBuffer = Tools::Split(buffer, L" ", true);
			if (vectorBuffer.size() != 3) throw EErrorReadingGrammar();
			nodeNumber = stoi(vectorBuffer[0]);
			numberOfSuccessors = stoi(vectorBuffer[1]);
			isFinal = stoi(vectorBuffer[2]);

			shared_ptr<GrammarNode> tempNode = shared_ptr<GrammarNode>(new GrammarNode(isFinal == 1));

			if(numberOfSuccessors == 0)
			{
				tempNet->AddNode(tempNode);
			}
			else
			{
				for(int j = 0; j < numberOfSuccessors; j++)
				{
					int wordId, netId, toNode, isHead;

					getline(in, buffer);
					vectorBuffer = Tools::Split(buffer, L" ", true);
//					if (vectorBuffer.size() != 5) throw EErrorReadingGrammar();
					wordId = stoi(vectorBuffer[0]);
					netId = stoi(vectorBuffer[1]);
					toNode = stoi(vectorBuffer[2]);
//					isHead = stoi(vectorBuffer[3]);
                    //feats = Trim(vectorBuffer[4]);

					shared_ptr<NetArc> arc;

						if (wordId == 0 && netId == 0)
						{
							arc = shared_ptr<NetArc>(new NullArc(tempNet, toNode));
						}
						else if (netId > 0)
						{
							arc = shared_ptr<NetArc>(new CallArc(tempNet, netId, toNode, (isHead == 1), [this, netId] () -> shared_ptr<GrammarNet> { return getNetByIndex(netId - 1);})); //-1 because in *.net nets are indexed from 1
							
//							if(feats != L"_")
//							{
//								vector<wstring> buf = parseFeats(feats);
//								std::dynamic_pointer_cast<CallArc>(arc)->SetFeatures(buf);
//							}
//							else
//							{
//								vector<wstring> buf = parseFeats(commonFeats);
//								std::dynamic_pointer_cast<CallArc>(arc)->SetFeatures(buf);
//							}
						}
						else
						{
							arc = shared_ptr<NetArc>(new WordArc(tempNet, wrds[wordId], toNode, (isHead == 1)));

//							if(feats != L"_")
//							{
//								vector<wstring> buf = parseFeats(feats);
//								std::dynamic_pointer_cast<WordArc>(arc)->SetFeatures(buf);
//							}
//							else
//							{
//								vector<wstring> buf = parseFeats(commonFeats);
//								std::dynamic_pointer_cast<WordArc>(arc)->SetFeatures(buf);
//							}
						}

					tempNode->AddArc(arc);
				}
				tempNet->AddNode(tempNode);
			}
		}
		nets[net_num - 1] = tempNet;
	}

	in.close();
}

shared_ptr<GrammarNet> Grammar :: findNet(const std::wstring& name) const
{
	if( name.empty() )
	{
		return 0;
	}

	for(unsigned int i = 0; i < nets.size(); i++ ) 
	{
		if(nets[i]->GetName().empty()) 
			continue;

		if(nets[i]->GetName() == name) 
			return nets[i];
	}

	return 0;
}

// Reads frames from file.
// Each entry in the file is one of the following:
// a) #comment
// b,c,d) frame description in format 
// FUNCTION: <FRAME_NAME>
// NETS: 
// SLOT_NAME1
// SLOT_NAME2
// ...
// SLOT_NAME_N
// ;
// see also: Frame.h, Frame.cpp and Phoenix manual
void  Grammar::read_frames()
{
	/* read frames */
	wifstream in(frames_file);
	if(in == NULL)
		throw "Grammar::read_frames(): File not found " + frames_file;

	vector<wstring> splitted;
	wstring res;
	wstring line;
	wstring bufer;
	wstring prompt;
	wstring frameName;

	while(1)
	{
		/* scan for start of frame */
		bool foundFunction = false;
		while (getline(in, res))
		{
			splitted = Split(res, L" ");
			if (splitted.size() < 2 || !StartsWith(res, L"FUNCTION:")) continue;
			else
			{
				foundFunction = true;
				frameName = splitted[1];
			    break;
			}
		}
		if (!foundFunction)
			break;

		/* copy frame name */
		shared_ptr<Frame> currentFrame = shared_ptr<Frame>(new Frame(wstring(frameName)));

		/* scan for net declarations */
		bool foundNets = false;
		while (getline(in, res))
		{
			if (StartsWith(res, L"NETS:"))
			{
				foundNets = true;
				break;
			}
		}
		if( !foundNets ) throw "Grammar::read_frames(): File not found " + frames_file;

		/* read net names */
		bool finishedFrame = false;

		while (getline(in, res))
		{
			bool isTenseSlot = false;
			res = Trim(res);
			if (res[0] == ';')
			{
				finishedFrame = true;
				break;
			}
			if(res[res.length() - 2] == L'!')
			{
				isTenseSlot = true;
				res = res.substr(0, res.length() - 2) + L']';
			}
			if (res[0] != '[') continue;
			shared_ptr<GrammarNet> net = findNet(res);
			if (net) currentFrame->AddSlot(net, isTenseSlot);
			else fprintf(stderr, "WARNING: can't find net for frame\n");
			isTenseSlot = false;
		}
		if( !finishedFrame ) throw "Grammar::read_frames(): File not found " + frames_file;

	    frames.push_back(currentFrame);
	}
	in.close();
}

int Grammar::findWord(const wstring& word) const
{
	for(auto p = wrds.begin(); p != wrds.end(); p++)
	{
		if(p->second == word)
		{
			return p->first;
		}
	}
	return WORD_NOT_FOUND;
}

vector<shared_ptr<Frame>>::const_iterator Grammar::GetFrameIterator() const
{
	return frames.begin();
}

vector<shared_ptr<Frame>>::const_iterator Grammar::GetFrameEndIterator() const
{
	return frames.end();
}

shared_ptr<GrammarNet> Grammar::getNetByIndex(int id)
{
	return nets.at(id);
}

unordered_set<wstring> Grammar::GetPossibleFramesForNet(const shared_ptr<GrammarNet>& net) const
{
	unordered_set<wstring> toReturn;
	for(auto fp = frames.begin(); fp != frames.end(); fp++)
	{
		if((*fp)->DoesHaveSlot(net))
		{
			toReturn.insert((*fp)->GetFrameName());
		}
	}
	return toReturn;
}

vector<wstring> Grammar::parseFeats(const wstring& featLine)
{
	if(featLine == L"_")
	{
		return vector<wstring>();
	}

	wstring featLine2 = featLine.substr(featLine.find_first_of(L'{') + 1);
	featLine2 = featLine2.substr(0, featLine2.find_first_of(L'}'));

	vector<wstring> retval = Split(featLine2, L"|");
	return retval;
}

shared_ptr<Frame> Grammar::GetFrameByName(wstring& frameName)
{
	for(size_t i = 0; i < frames.size(); i++)
	{
		if(frames[i]->GetFrameName() == frameName)
		{
			return frames[i];
		}
	}

	return shared_ptr<Frame>();
}

}  // namespace Phoenix
