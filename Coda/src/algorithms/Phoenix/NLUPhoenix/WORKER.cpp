#include "WORKER.h"

using namespace Tools;

namespace Phoenix {

WORKER::WORKER(const string& path) {
	readConfig(path);
	G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));
	parser = shared_ptr<PhoenixParser>(new PhoenixParser(G, GS, ENGLISH, disamb_dir));

	qa = shared_ptr<QueryAssembler>(new QueryAssembler(database_file));
	evaluator = shared_ptr<NetEvaluator>(new NetEvaluator(net_interp_file));
	ra = shared_ptr<ResponseAssembler>(new ResponseAssembler(database_file));
}

WORKER::~WORKER() {
	// TODO Auto-generated destructor stub
}

void WORKER::readConfig(const string& path)
{
	string path2 = path.substr(path.find_first_not_of(' '), path.size() - path.find_first_not_of(' '));
	std::ifstream fin(path2);
	std::getline(fin, dict_file);
	std::getline(fin, grammar_file);
	std::getline(fin, frames_file);
	std::getline(fin, disamb_dir);
	std::getline(fin, database_file);
	std::getline(fin, net_interp_file);
}

wstring WORKER::ParseSentence( const wstring& sentence)
{
	wstring defaultAnswer = L"It seems that I could not find anything relevant. Please, clarify your question.";
	if (sentence.size() == 0) return defaultAnswer;
	wstring pattern = L"";
	wstring response = L"";
	if(Trim(sentence).length() == 0) return L"";
	ParseResult ret_code = parser->ParseLine(sentence);
	if(!(SUCCESS == ret_code))
	{
		wcout << L"ERROR!!!" << std::endl;
		return defaultAnswer;
	}
	vector<shared_ptr<Parse>> parses = parser->GetAllParses();
	vector<shared_ptr<IMatch>> allSlots;

	shared_ptr<Parse> parse = parses[0];

	wstring extraWords = parser->GetUnparsedSequences()[0];

	wstring parsePrint = parser->GetParsesAsStrings(CONCISE)[0];
	wcout << parsePrint << endl;
	parsePrint = parsePrint + L"\n";
	parser->Reset();
	wstring frameName = parse->GetFrameSequence()[0];
	shared_ptr<Frame> f = G->GetFrameByName(frameName);
	vector<shared_ptr<ChartCell>> phoenixSlots = parse->GetSlotSequence();
	for(auto pslot_p = phoenixSlots.begin(); pslot_p != phoenixSlots.end(); pslot_p++)
	{
		allSlots.push_back(*pslot_p);
	}

	FilledFrame ff(f, allSlots);

	wstring query;
	std::string a;
	try
	{
		wstring b = qa->CreateQuery(ff, *evaluator, query);
		wcout << b << endl;
		CONVERT_TO_UTF8_STRING(b, a);
	}
	catch(...)
	{
		return defaultAnswer;
	}
	if(query == L"")
	{
		for(auto p_slot = phoenixSlots.begin(); p_slot != phoenixSlots.end(); p_slot++)
		{
			if((*p_slot)->GetNetName() == L"[WhatCountry]")
				query = L"what_country";
			else if((*p_slot)->GetNetName() == L"[WhForSingle]")
				query = L"who_athlete";
			else if((*p_slot)->GetNetName() == L"[WhForSingleMan]")
				query = L"who_athlete";
			else if((*p_slot)->GetNetName() == L"[WhForSingleWoman]")
				query = L"who_athlete";
			else if((*p_slot)->GetNetName() == L"[WhatCountryBySportsman]")
				query = L"what_country";
			else if((*p_slot)->GetNetName() == L"[HowMany]")
				query = L"how_many";
			else if((*p_slot)->GetNetName() == L"[How_many_gold]")
				query = L"how_many";
			else if((*p_slot)->GetNetName() == L"[How_many_silver]")
				query = L"how_many";
			else if((*p_slot)->GetNetName() == L"[How_many_bronze]")
				query = L"how_many";
			else if((*p_slot)->GetNetName() == L"[How_many_any]")
				query = L"how_many";
			else if((*p_slot)->GetNetName() == L"[When]")
				query = L"when";
			else if((*p_slot)->GetNetName() == L"[WhereWas]")
				query = L"where";
			else if((*p_slot)->GetNetName() == L"[WhereWill]")
				query = L"where";
			else if((*p_slot)->GetNetName() == L"[WhatResult]")
				query = L"what_result";
			else if((*p_slot)->GetNetName() == L"[WhatSport]")
				query = L"what_sport";
			else if((*p_slot)->GetNetName() == L"[WhatDiscipline]")
				query = L"what_event";
			break;
		}
	}

	if(query == L"")
	{
		if(frameName == L"RusOlympQueryCountry" || frameName == L"RusOlympQueryCountryByMan")
			query = L"what_country";
		else if(frameName == L"RusOlympQueryPerson")
			query = L"what_athlete";
		else
			query = L"when";
	}

	SQLiteHelper m_helper(database_file, SQLStringExpressionQuery(""));
	m_helper.addQuery(SQLStringExpressionQuery(a));
	try
	{
		for(auto line_p = m_helper.begin(); line_p != m_helper.end(); line_p++)
		{
			auto container = (*line_p);
			{
				ra->AddRow(container);
			}
		}
	}
	catch(...)
	{
		wcout << L"ERROR!!!" << std::endl;
		return defaultAnswer;
	}
	response = ra->CreateResponse(ff, query);
	std::transform(response.begin(), response.end(), response.begin(), ::towlower);
	response = Trim(response);
	if (response.size() == 0) return defaultAnswer;
	return response;
}

}
