/** 
 * @file	PhoenixParser.cpp
 * @brief	Implementation of the methods in the corresponding header
 */

/*
 PhoenixParser.cpp
 Implementation of the parsing algorithm from the original Phoenix library. It resembles Earley parser, but does not have explicit SCANNER, COMPLETER and PREDICTOR routines.
 You may reference the handbook 'Spoken language processing' by Huang, Acero, Hon, chapters 12.3 and 17.4.1

 Algorithm:

 I. Initialization
 Get active nets i.e. nets which can be slots of frames is grammar G. (constructor)
 II. Parsing
 Outline:
 We are passing through the word W sequence left-to-right. 
 We try to match active nets against subsequence of W from current position k
 The parsing search procedure is DFS.

 If net has an arc with net transition argument we recursively do matching of the net argument from the current position k + t (t >= 0)
 If matching of the argument is successful we proceed matching in the calling net from the position k + t + l , where l is the length of matching of the argument

 If null arc is encountered we just get to the next state and position k does not change

 If word arc is encountered we match word in sequence with the transition argument. If matching is successful k increments and matching proceeds. The transition fails otherwise.

 We try all arcs emanating from the state thus getting depth-first grapth search. Any path either fails or reaches final state.

 If we reach the final state we store left position of the match and the right position of the match in Chart structure for each net. Successful matching automatically means successful transition in calling net. 
 If calling net is also matches well we store in chart a match with child match, the net argument of one of its arcs (see expandPath)

 Net matching is finished when all paths are tried

 The chart is used for DFS in the following manner: each time we go from a state s_i to a state s_j we create an object MatchTree with fields startPosition (left position of a net being matched) and Net
 (net being matched) same as in the MatchTree for the previous state but with wordPosition incremented by the length of the transition argument (1 for words and some L for nets). 
 Thus MatchTree corresponds to the whole match of the net up to the position wordPosition. If final state is reached MatchTree object is stored in the chart, thus storing information about whole match.
 Then calling net can extract the match from the chart. If the match for argument net exists then calling net may do transition. (see expandPath)
*/

#include "PhoenixParser.h"
#include "tagsetConstants.h"
#include "Tools.h"

#include <algorithm>

using std::shared_ptr;
using std::wstring;
using std::vector;
using std::unordered_map;
using std::map;
using std::unordered_set;

using Disambiguation::IDisambiguator;
using Tokenization::ITokenizer;
using Tokenization::Token;
using Disambiguation::DisambiguatedData;


namespace Phoenix
{
const wstring MATCH_EXACT_FORM = L"=";

PhoenixParser::PhoenixParser()
{
}

PhoenixParser::PhoenixParser(const shared_ptr<Grammar>& G, const shared_ptr<SlotTrellis>& SG, Language _lang, string pathToDisambiguatorModel)
{
	words = vector<wstring>();

	gram = G;
	slotTrellis = SG;
	std::unordered_set<std::wstring> registry;

	//Detect active nets: iterate through all slots of all frames and store nets in activeNets
	for(auto fp = gram->GetFrameIterator(); fp != gram->GetFrameEndIterator(); fp++)
	{
		for(vector<shared_ptr<GrammarNet>>::const_iterator np = (*fp)->GetNetIterator();
			np != (*fp)->GetNetEndIterator();
			np++)
		{
			if(registry.find((*np)->GetName()) == registry.end())
			{
				activeNets.push_back(*np);
				registry.insert((*np)->GetName());
			}
		}
	}
	lang = _lang;
	if(lang != ENGLISH)
	{
       m_disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
       m_tokenizer = Tokenization::ITokenizer::GetTokenizer(Tools::Language::RU);
	}
	wcout << L"PhoenixParser is ready!!!" << endl;
}

PhoenixParser::~PhoenixParser()
{
}

ParseResult PhoenixParser::ParseLine(wstring utterance)
{
	parses.clear();
	words.clear();
	chart = NetChart();
	slotTrellis->Clear();

    utterance = Tools::Trim(utterance);

	createWordSequence(utterance);

	if(dis_words.empty())
	{
	//	return NO_PARSE;
	}

	for(unsigned int wp = 0; wp < words.size(); wp++) //go left-to-right through the word sequence
	{
		for(auto np = activeNets.begin(); np != activeNets.end(); np++) //match all active nets against sequence from position wp
		{
			if(matchNet(*np, wp))
			{
				slotTrellis->AddSlot(chart.GetNonEmptyCellsForNet(*np, wp));
			}
		}
	}

	parses = slotTrellis->GetBestMatch();

	if(parses.empty())
	{
		return NO_PARSE;
	}
	else
	{
		return SUCCESS;
	}

}

bool PhoenixParser::matchNet(const shared_ptr<GrammarNet>& net, int wordPosition)
{
	if(chart.CheckMatchTreeAttempt(net, wordPosition))
	{
		return chart.CheckMatchSuccess(net, wordPosition); //lookup in the chart existing matches at the position
	}

	shared_ptr<MatchTree> netMatch = shared_ptr<MatchTree>(new MatchTree(net, wordPosition)); 
	chart.RegisterEntry(net, wordPosition); //create new cells in the chart (allocate 'row' in chart 'matrix')
	shared_ptr<GrammarNode> state = net->GetStartNode(); //begin traversal of the net from the beginning

	expandPath(state, netMatch);

	return chart.CheckMatchSuccess(net, wordPosition); //lookup in the chart if the matching was successful
}

bool PhoenixParser::matchWord(const wstring& word, const unordered_set<wstring>& features, int wordPositionInAnalyzedString)
{
	if(features.empty() || features.find(MATCH_EXACT_FORM) != features.end())
	{
		return word == words[wordPositionInAnalyzedString];
	}

	if(words[wordPositionInAnalyzedString] == START_SYMBOL || words[wordPositionInAnalyzedString] == END_SYMBOL)
	{
		return false;
	}

	if(word != dis_words[wordPositionInAnalyzedString - 1].GetLemma())
	{
		return false;
	}

	if(matchFeatures(dis_words[wordPositionInAnalyzedString - 1], features))
		return true;
	return false;
}

bool PhoenixParser::matchFeatures(const Word& word, const unordered_set<wstring>& features)
{
	unordered_set<wstring> allFeats = word.GetAllFeaturesSet();

	for(auto feat_p = features.begin(); feat_p != features.end(); feat_p++)
	{
		wstring feat_seq_line = *feat_p;
		vector<wstring> feat_seq;
        feat_seq = Tools::Split(feat_seq_line, L".");

		bool matches = true;
		for(size_t i = 0; i < feat_seq.size(); i++)
		{
			if(allFeats.find(feat_seq[i]) == allFeats.end())
			{
				matches = false;
				break;
			}
		}

		if(matches)
		{
			return true;
		}
	}

	return false;
}

void PhoenixParser::expandPath(const shared_ptr<GrammarNode>& state, const shared_ptr<MatchTree>& path)
{
	for(auto ap = state->GetArcIterator(); ap != state->GetArcEndIterator(); ap++) //try all arcs emanating from the state we recursively descended to
	{
		shared_ptr<NetArc> netArc = *ap;
		shared_ptr<MatchTree> expansionPath;

		switch(netArc->GetType())
		{
			case TCALL: //the arc has net argument
				processCallArc(netArc, expansionPath, path);
				break;
			case TNULL:
				processEpsilonArc(netArc, expansionPath, path);
				break;
			case TWORD:
				processWordArc(netArc, expansionPath, path);
		}
	}
}

vector<wstring> PhoenixParser::GetParsesAsStrings(ParseMode mode)
{
	vector<wstring> toReturn;

	switch (mode)
	{
		case FULL:
			for(auto parse_p = parses.begin(); parse_p != parses.end(); parse_p++)
			{
				toReturn.push_back((*parse_p)->ToString(words));
			}
			break;
		case CONCISE:
			for(auto parse_p = parses.begin(); parse_p != parses.end(); parse_p++)
			{
				toReturn.push_back((*parse_p)->ToStringConciseNF(words));
			}
	}

	return toReturn;
}

void PhoenixParser::PrintParses(ParseMode mode)
{
	switch (mode)
	{
		case FULL:
			for(auto parse_p = parses.begin(); parse_p != parses.end(); parse_p++)
			{
				(*parse_p)->Print(words);
			}
			break;
		case CONCISE:
			for(auto parse_p = parses.begin(); parse_p != parses.end(); parse_p++)
			{
				(*parse_p)->PrintConciseNF();
			}
	}
}

void PhoenixParser::Reset()
{
	parses.clear();
	words.clear();
	chart = NetChart();
	slotTrellis->Clear();
}

void PhoenixParser::processCallArc(const shared_ptr<NetArc>& ap, shared_ptr<MatchTree>& expansionPath, const shared_ptr<MatchTree>& path)
{
	shared_ptr<GrammarNet> netArg = std::dynamic_pointer_cast<CallArc>(ap)->GetArgument();
	if(matchNet(netArg, path->GetCurrentWordPosition())) 
	{
		vector<shared_ptr<MatchTree>> children = chart.GetMatch(netArg, path->GetCurrentWordPosition()); //if matching was successful it is stored in the chart
		if(!children.empty())
		{
			for(auto cp = children.begin(); cp != children.end(); cp++) //"children" are all matches of netArg. They are going to be child paths of expansionPath.
			{
				expansionPath = shared_ptr<MatchTree>(new MatchTree(*path)); //if there are N argument net matches we construct N new paths
				
				if(std::dynamic_pointer_cast<CallArc>(ap)->IsHeadConsuming())
				{
					expansionPath->SetFeatures((*cp)->GetFeatures());
				}
				
				expansionPath->AppendChild(*cp); //each path is also a tree with child corresponding to one of N arguemnt net matches
				if(ap->GetDestination()->IsFinal())
				{
					expansionPath->Finalize(); //if out transition led to the final state we add all tree variants to the chart
					chart.AddMatch(expansionPath, expansionPath->GetStartPosition(), expansionPath->GetEndPosition());
				}
				expandPath(ap->GetDestination(), expansionPath);  //nevertheless we try to expand path further to find the longest match
			}
		}
	} //if not the transition fails and we must search another path
}

void PhoenixParser::processEpsilonArc(const shared_ptr<NetArc>& ap, shared_ptr<MatchTree>& expansionPath, const shared_ptr<MatchTree>& path)
{
	expansionPath = shared_ptr<MatchTree>(new MatchTree(*path));
	if(ap->GetDestination()->IsFinal())
	{
		expansionPath->Finalize();
		chart.AddMatch(expansionPath, expansionPath->GetStartPosition(), expansionPath->GetEndPosition());
	}
	expandPath((ap)->GetDestination(), expansionPath);
}

void PhoenixParser::processWordArc(const shared_ptr<NetArc>& ap, shared_ptr<MatchTree>& expansionPath, const shared_ptr<MatchTree>& path)
{
	shared_ptr<WordArc> arc = std::dynamic_pointer_cast<WordArc>(ap);

	if(matchWord(arc->GetArgument(), arc->GetFeatures(), path->GetCurrentWordPosition()))
	{
		expansionPath = shared_ptr<MatchTree>(new MatchTree(*path));
		//we extract featurese either from the word marked by ! or the last term in the rule
		//it is needed for support of rules with of the kind NT->t i.e. only single terminal symbol on the rhs is present
		//it is a bit weird solution because it causes pushing unnecessary features up the tree
		if(lang == RUSSIAN)
		{
			if(arc->IsHeadConsuming() || ((ap)->GetDestination()->IsFinal() && expansionPath->GetFeatures().empty()))
			{
				expansionPath->SetFeatures(dis_words[path->GetCurrentWordPosition() - 1].GetAllFeatures());
			}
		}
		expansionPath->ConsumeWord(arc->GetArgument()); //indicates that expansion path must proceed from the word next to the one just matched
		if((ap)->GetDestination()->IsFinal())
		{
			expansionPath->Finalize();
			chart.AddMatch(expansionPath, expansionPath->GetStartPosition(), expansionPath->GetEndPosition());
		}
		expandPath((ap)->GetDestination(), expansionPath);
	}
}

void PhoenixParser::createWordSequence(wstring& utterance)
{
	dis_words = vector<Word>();

	if(utterance.empty())
	{
		return;
	}

	if(utterance.find_first_not_of(L' ') == wstring::npos)
	{
		return;
	}

	utterance = utterance.substr(utterance.find_first_not_of(L' '));
	if(utterance[utterance.length() - 1] == L' ')
	{
		utterance = utterance.substr(0, utterance.find_last_of(L' '));
	}

	if(utterance == L"")
	{
		return;
	}

	//we add two special symbols (see Grammar.h) to support rules considering start and end positions in the string
	words.push_back(wstring(START_SYMBOL));

	vector<wstring> lemmas;
	vector<wstring> tags;
	std::locale lolo;
	std::transform(utterance.begin(), utterance.end(), utterance.begin(), ::towupper);
	vector<wstring> vsplit;
    vsplit = Tools::Split(utterance, L" ");

    if(lang != RUSSIAN)
    {
        words.insert(words.end(), vsplit.begin(), vsplit.end());
        words.push_back(wstring(END_SYMBOL));
    }
	if(lang == RUSSIAN)
	{   
       // m_disambiguator->Disambiguate(utterance, lemmas, stress, tags, true);

        vector<Token> tokens;
        vector<DisambiguatedData> disambiguatedData;
        tokens = m_tokenizer->Tokenize(utterance);
        disambiguatedData = m_disambiguator->Disambiguate(tokens);

        for(int i = 0; i < disambiguatedData.size(); i++)
        {
            words.push_back(disambiguatedData[i].content);
            lemmas.push_back(disambiguatedData[i].lemma);
            tags.push_back(disambiguatedData[i].label);
        }
        words.push_back(wstring(END_SYMBOL));

	#if defined(COMPILER_MVS2010)
		lolo = std::locale::global(std::locale("Russian"));
	#elif defined(COMPILER_GPP47)
		setlocale(LC_ALL, "");
	#else
		wcout << L"WRONG COMPILER HEADER";
		throw ("main: WRONG COMPILER HEADER");
	#endif

        for(size_t i = 0; i < vsplit.size(); ++i)
        {
            vector<wstring> toPushBack = Tools::Split(tags[i], L"@");
            if(lemmas[i] != L"")
            {
                if(toPushBack.empty() || toPushBack[0] == L"UNKNOWN")
                {
                    toPushBack.clear();
                    toPushBack.push_back(SynTagRusTagset::POS_UNKN);
                }
                std::transform(lemmas[i].begin(), lemmas[i].end(), lemmas[i].begin(), ::towupper);
                dis_words.push_back(Word(lemmas[i], toPushBack, vsplit[i]));
            }
            else
            {
                toPushBack.push_back(SynTagRusTagset::POS_UNKN);
                dis_words.push_back(Word(words[i + 1], toPushBack, vsplit[i]));
            }
	}
	#if defined(COMPILER_MVS2010)
		std::locale::global(lolo);
	#endif
	}
}

int PhoenixParser::GetNumberOfParses() const
{
	return parses.size();
}

vector<shared_ptr<Parse>> PhoenixParser::GetAllParses() const
{
	return parses;
}

vector<wstring> PhoenixParser::GetUnparsedSequences() const
{
	vector<wstring> toReturn;

	for(auto parse_p = parses.begin(); parse_p != parses.end(); parse_p++)
	{
		vector<wstring> wrds = (*parse_p)->GetUnparsedSequence(words);
		wstring toAdd = L"";

		for(size_t i = 0; i < wrds.size(); i++)
		{
			toAdd = toAdd + L" " + wrds[i];
		}

		toReturn.push_back(toAdd);	
	}

	return toReturn;
}

} //namespace Phoenix
