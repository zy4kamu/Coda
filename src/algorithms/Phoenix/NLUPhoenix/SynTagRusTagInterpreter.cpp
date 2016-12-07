#include "SynTagRusTagInterpreter.h"
#include "tagsetConstants.h"

using std::unordered_map;
using std::vector;
using std::wstring;

SynTagRusTagInterpreter& SynTagRusTagInterpreter::GetInstance()
{
	static SynTagRusTagInterpreter instance; 
	return instance;
}

SynTagRusTagInterpreter::SynTagRusTagInterpreter()
{
	map[SynTagRusTagset::POS_NOUN] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_VERB] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_NUMR] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_ADVB] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_PREP] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_CONJ] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_INTJ] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_UNKN] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_SENT] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_PRCL] = PrivateTagset::CAT_POS;
	map[SynTagRusTagset::POS_ADJ] = PrivateTagset::CAT_POS;

	map[SynTagRusTagset::ANIM_N] = PrivateTagset::CAT_ANIM;
	map[SynTagRusTagset::ANIM_Y] = PrivateTagset::CAT_ANIM;

	map[SynTagRusTagset::C_NOM] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_DAT] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_ACC] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_ABL] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_GEN1] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_GEN2] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_LOC1] = PrivateTagset::CAT_CASE;
	map[SynTagRusTagset::C_LOC2] = PrivateTagset::CAT_CASE;

	map[SynTagRusTagset::N_PLUR] = PrivateTagset::CAT_NUM;
	map[SynTagRusTagset::N_SING] = PrivateTagset::CAT_NUM;

	map[SynTagRusTagset::DEG_COMP] = PrivateTagset::CAT_DEG;
	map[SynTagRusTagset::DEG_SUPERL] = PrivateTagset::CAT_DEG;

	map[SynTagRusTagset::A_IMPRF] = PrivateTagset::CAT_ASPECT;
	map[SynTagRusTagset::A_PERF] = PrivateTagset::CAT_ASPECT;

	map[SynTagRusTagset::D_SHORT] = PrivateTagset::CAT_SHORT;

	map[SynTagRusTagset::G_FEMN] = PrivateTagset::CAT_GEN;
	map[SynTagRusTagset::G_MASC] = PrivateTagset::CAT_GEN;
	map[SynTagRusTagset::G_NEUT] = PrivateTagset::CAT_GEN;

	map[SynTagRusTagset::VT_GRND] = PrivateTagset::CAT_VERB_FORM;
	map[SynTagRusTagset::VT_INF] = PrivateTagset::CAT_VERB_FORM;
	map[SynTagRusTagset::VT_PRT] = PrivateTagset::CAT_VERB_FORM;

	map[SynTagRusTagset::M_IMPER] = PrivateTagset::CAT_MODE;
	map[SynTagRusTagset::M_INDC] = PrivateTagset::CAT_MODE;

	map[SynTagRusTagset::A_IMPRF] = PrivateTagset::CAT_ASPECT;
	map[SynTagRusTagset::A_PERF] = PrivateTagset::CAT_ASPECT;

	map[SynTagRusTagset::TN_NONPAST] = PrivateTagset::CAT_TENSE;
	map[SynTagRusTagset::TN_PAST] = PrivateTagset::CAT_TENSE;

	map[SynTagRusTagset::P_1] = PrivateTagset::CAT_PERSON;
	map[SynTagRusTagset::P_2] = PrivateTagset::CAT_PERSON;
	map[SynTagRusTagset::P_3] = PrivateTagset::CAT_PERSON;
}

unordered_map<wstring, wstring> SynTagRusTagInterpreter::ProcessTags(const vector<wstring>& tags)
{
	unordered_map<wstring, wstring> toReturn;

	if(tags.empty())
	{
		return toReturn;	
	}
	
	for(auto tag_p = tags.begin(); tag_p != tags.end(); tag_p++)
	{
		wstring tag = *tag_p;
		wstring catName = map[tag];
		toReturn[catName] = tag;
	}

	toReturn.erase(L"");

	wstring pos = toReturn.at(PrivateTagset::CAT_POS);

	if(pos == SynTagRusTagset::POS_VERB)
	{
		processVerb(toReturn);
	}
	else if(pos == SynTagRusTagset::POS_ADJ)
	{
		processAdj(toReturn);
	}

	return toReturn;
}

void SynTagRusTagInterpreter::processVerb(unordered_map<wstring, wstring>& feats)
{

	if(feats.find(PrivateTagset::CAT_MODE) != feats.end() && feats[PrivateTagset::CAT_MODE] == SynTagRusTagset::M_IMPER)
	{
		return;
	}

	if(feats.find(PrivateTagset::CAT_VERB_FORM) == feats.end())
	{
		feats[PrivateTagset::CAT_VERB_FORM] = SynTagRusTagset::VT_FIN; 
	}

	if(feats[PrivateTagset::CAT_VERB_FORM] == SynTagRusTagset::VT_INF)
	{
		return;
	}

	wstring aspect = feats[PrivateTagset::CAT_ASPECT];
	wstring tense = feats[PrivateTagset::CAT_TENSE];

	if(tense == SynTagRusTagset::TN_PRES)
	{
		feats[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_PRESENT;
	}
	else if(tense == SynTagRusTagset::TN_PAST)
	{
		feats[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_PAST;
	}
	else if(aspect == SynTagRusTagset::A_PERF && tense == SynTagRusTagset::TN_NONPAST)
	{
		feats[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_FUTURE;
	}
	else if(aspect == SynTagRusTagset::A_IMPRF && tense == SynTagRusTagset::TN_NONPAST || tense == SynTagRusTagset::TN_PRES)
	{
		feats[PrivateTagset::CAT_TIME] = SynTagRusTagset::TIME_PRESENT;
	}

	if(feats.find(PrivateTagset::CAT_VOICE) == feats.end())
	{
		feats[PrivateTagset::CAT_VOICE] = SynTagRusTagset::V_ACT;
	}
}

void SynTagRusTagInterpreter::processAdj(unordered_map<wstring, wstring>& feats)
{
	if(feats.find(PrivateTagset::CAT_DEG) == feats.end() && feats.find(PrivateTagset::CAT_SHORT) == feats.end())
	{
		feats[PrivateTagset::CAT_DEG] = SynTagRusTagset::DEG_POSITIVE;
	}

	if(feats.find(PrivateTagset::CAT_SHORT) == feats.end())
	{
		feats[PrivateTagset::CAT_SHORT] = SynTagRusTagset::D_FULL;
	}
}
