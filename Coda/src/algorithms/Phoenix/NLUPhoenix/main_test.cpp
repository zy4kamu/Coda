#include "debug.h"
#ifdef _TEST_
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "SynTagRusTagInterpreter.h"
#include "tagsetConstants.h"
#include "PhoenixParser.h"
#include "PhoenixSlotGraph.h"
#include "dataTools.h"
#include "Word.h"
#include "DisambiguatorInterface.h"
#include "FilledFrame.h"
#include "NetEvaluator.h"
#include "QueryAssembler.h"
#include "ResponseAssembler.h"
#include "sqliteClient/sqlite_helper.h"
#include "StringTools.h"
#include "CommandInterpreter.h"

#if defined(COMPILER_MVS2010)
	#include "AtlConv.h"
	#include "AtlBase.h"
	#include "windows.h"
#endif

#define test

#ifdef test
#define MAIN main
#else
#define MAIN main_test
#endif

using namespace std;
using namespace Phoenix;

string dict_file = "";
string grammar_file = "";
string frames_file = "";
string disamb_dir = "";
string database_file = "";
string net_interp_file = "";

shared_ptr<Grammar> G;
shared_ptr<SlotTrellis> GS;
shared_ptr<PhoenixParser> parser;

static void readConfig(
		string& dict_file, string& grammar_file,
		string& frames_file,  string& disamb_dir,
		string& database_file, string& net_interp_file,
		const string& path)
{
	string path2 = path.substr(path.find_first_not_of(' '), path.size() - path.find_first_not_of(' '));
    std::getline(fin, dict_file);
	std::getline(fin, grammar_file);
	std::getline(fin, frames_file);
	std::getline(fin, disamb_dir);
	std::getline(fin, database_file);
	std::getline(fin, net_interp_file);
}

static void reportErrorLine(int line, const wstring& msg)
{
	wcout << endl << msg << endl;
	wcout << endl << "Line: " << line << endl;
}

//TEST(Conversions, UTF8ToWstring)
//{
//	string source = "\u042d\u0442\u043e \u0441\u0442\u0440\u043e\u043a\u0430 \u043d\u0430 \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435 \u0441 \u0432\u043a\u0440\u0430\u043f\u043b\u0435\u043d\u0438\u044f\u043c\u0438 \u0442\u0430\u043a\u0438\u0445 \u044f\u0437\u044b\u043a\u043e\u0432 \u043a\u0430\u043a English, Deutch etc.. \u041e\u043d\u0430 \u0437\u0430\u043a\u0430\u043d\u0447\u0438\u0432\u0430\u0435\u0442\u0441\u044f \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435\u0439.";//here was cyrrilic symbols: Это,строка,на,кириллице,с,вкраплениями,таких,языков,как,Она,заканчивается,кириллицей
////	wcout << source.length() <<L" "<< source.size() << endl;
//	wstring target = L"\u042d\u0442\u043e \u0441\u0442\u0440\u043e\u043a\u0430 \u043d\u0430 \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435 \u0441 \u0432\u043a\u0440\u0430\u043f\u043b\u0435\u043d\u0438\u044f\u043c\u0438 \u0442\u0430\u043a\u0438\u0445 \u044f\u0437\u044b\u043a\u043e\u0432 \u043a\u0430\u043a English, Deutch etc.. \u041e\u043d\u0430 \u0437\u0430\u043a\u0430\u043d\u0447\u0438\u0432\u0430\u0435\u0442\u0441\u044f \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435\u0439.";//here was cyrrilic symbols: Это,строка,на,кириллице,с,вкраплениями,таких,языков,как,Она,заканчивается,кириллицей
////	wcout << target.length() <<L" "<< target.size() << endl;
//	wstring result = Tools::ConvertToWstring(source);
//	EXPECT_EQ(result, target);
//}
//
//
//TEST(Conversions, WstringToUTF8)
//{
//	wstring source = L"\u042d\u0442\u043e \u0441\u0442\u0440\u043e\u043a\u0430 \u043d\u0430 \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435 \u0441 \u0432\u043a\u0440\u0430\u043f\u043b\u0435\u043d\u0438\u044f\u043c\u0438 \u0442\u0430\u043a\u0438\u0445 \u044f\u0437\u044b\u043a\u043e\u0432 \u043a\u0430\u043a English, Deutch etc.. \u041e\u043d\u0430 \u0437\u0430\u043a\u0430\u043d\u0447\u0438\u0432\u0430\u0435\u0442\u0441\u044f \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435\u0439.";//here was cyrrilic symbols: Это,строка,на,кириллице,с,вкраплениями,таких,языков,как,Она,заканчивается,кириллицей
////	wcout << source.length() <<L" " << source.size() << endl;
//	string target = "\u042d\u0442\u043e \u0441\u0442\u0440\u043e\u043a\u0430 \u043d\u0430 \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435 \u0441 \u0432\u043a\u0440\u0430\u043f\u043b\u0435\u043d\u0438\u044f\u043c\u0438 \u0442\u0430\u043a\u0438\u0445 \u044f\u0437\u044b\u043a\u043e\u0432 \u043a\u0430\u043a English, Deutch etc.. \u041e\u043d\u0430 \u0437\u0430\u043a\u0430\u043d\u0447\u0438\u0432\u0430\u0435\u0442\u0441\u044f \u043a\u0438\u0440\u0438\u043b\u043b\u0438\u0446\u0435\u0439.";//here was cyrrilic symbols: Это,строка,на,кириллице,с,вкраплениями,таких,языков,как,Она,заканчивается,кириллицей
////	wcout << target.length() <<L" " << target.size() << endl;
//	string result;
//	Tools::ConvertToString(source, result);
//	EXPECT_EQ(result, target);
//}

int MAIN(int argc, char **argv) 
{
#if defined(COMPILER_MVS2010)
	system("chcp 1251");
	std::locale::global(std::locale("Russian"));
	string path = "../../src/NLUPhoenix/Config/phoenix_Win.cfg";
#elif defined(COMPILER_GPP47)
	setlocale(LC_ALL, "");
	std::locale::global(std::locale("ru_RU.UTF-8"));
	string path = "../../src/NLUPhoenix/Config/phoenix_Linux.cfg";
#else
    wcout << L"WRONG COMPILER HEADER";
#endif

	string control_file = "../../src/NLUPhoenix/Config/test_gram_sentences_rus_olymp";

	std::wofstream fout("result.txt");

	readConfig(dict_file, grammar_file, frames_file, disamb_dir, database_file, net_interp_file, path);
	G = shared_ptr<Grammar>(new Grammar(dict_file, grammar_file, frames_file));
	GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));
	parser = shared_ptr<PhoenixParser>(new PhoenixParser(G, GS, RUSSIAN, disamb_dir));
//	parser = shared_ptr<PhoenixParser>(new PhoenixParser(G, GS, ENGLISH, disamb_dir));

	QueryAssembler qa(database_file);
	NetEvaluator evaluator(net_interp_file);
	ResponseAssembler ra(database_file);

	wifstream in(control_file);

	wstring sentence = L"";
	wstring pattern = L"";
	wstring response = L"";

	if(in == NULL)
	{
		wcout << "cannot open control file" << endl;
		
	}

	wstring buffer;

	int i = 0;
	while(getline(in, sentence))
	{
		i++;
		response = L"";
		pattern = L"";

		if(Trim(sentence).length() == 0 || Trim(sentence)[0] == L'#')
			continue;

		ParseResult ret_code = parser->ParseLine(sentence);

		if(!(SUCCESS == ret_code))
		{
			reportErrorLine(i, L"No parse!");
			fout.close();
			exit(0);
		}

		{
		#if defined(COMPILER_MVS2010)
			//fout <<  CW2A(sentence.c_str(), CP_UTF8) << endl;
			string toPrint;
			ConvertWstringToString(sentence, toPrint);
			fout << ConvertWstringToString(sentence, toPrint) << endl;
		#elif defined(COMPILER_GPP47)
			string toPrint;
			ConvertWstringToString(sentence, toPrint);

			fout << sentence << endl;
		#else
		    wcout << L"WRONG COMPILER HEADER";
		#endif


		vector<shared_ptr<Parse>> parses = parser->GetAllParses();

		vector<shared_ptr<IMatch>> allSlots;

		//if(!(1 == parses.size()))
		//{
		//	reportErrorLine(i, L"Multiple parses!");
		//	fout.close();
		//	exit(0);
		//}

		shared_ptr<Parse> parse = parses[0];

		//if(parse->IsFragmented())
		//{
		//	reportErrorLine(i, L"Parse fragmentation!");
		//	fout.close();
		//	exit(0);
		//}

		wstring parsePrint = parser->GetParsesAsStrings(CONCISE)[0];

		wcout << parsePrint << endl;

		parsePrint = parsePrint + L"\n";


		#if defined(COMPILER_MVS2010)
			//fout <<  CW2A(parsePrint.c_str(), CP_UTF8);
		    //ConvertWstringToString(parsePrint, toPrint);
			fout << parsePrint;
		#elif defined(COMPILER_GPP47)
			//ConvertWstringToString(parsePrint, toPrint);
			fout << parsePrint;
		#else
		    wcout << L"WRONG COMPILER HEADER";
		#endif

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
		wstring b = qa.CreateQuery(ff, evaluator, query);

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

		#if defined(COMPILER_MVS2010)
			//fout <<  CW2A(b.c_str(), CP_UTF8);
		    //ConvertWstringToString(b, toPrint);
			fout <<  b << endl;
		#elif defined(COMPILER_GPP47)
			//ConvertWstringToString(b, toPrint);
			fout <<  b << endl;
		#else
		    wcout << L"WRONG COMPILER HEADER";
		#endif

		CONVERT_TO_UTF8_STRING(b, a);
		SQLiteHelper m_helper(database_file, SQLStringExpressionQuery(""));
		m_helper.addQuery(SQLStringExpressionQuery(a));
		try
		{
			for(auto line_p = m_helper.begin(); line_p != m_helper.end(); line_p++)
			{
				auto container = (*line_p);
				{
					ra.AddRow(container);
				}
			}
		}
		catch(...)
		{
			reportErrorLine(i, L"Wrong query!");
			fout.close();
			exit(0);
		}

		response = ra.CreateResponse(ff, query);
		std::transform(response.begin(), response.end(), response.begin(), ::towlower);

		#if defined(COMPILER_MVS2010)
			//ConvertWstringToString(response, toPrint);
			//fout <<  response << endl << endl;
			fout <<  response << endl;
			fout <<  CW2A(response.c_str(), CP_UTF8) << endl << endl;
		#elif defined(COMPILER_GPP47)
			ConvertWstringToString(response, toPrint);
		    fout <<  response << endl << endl;
		#else
		    wcout << L"WRONG COMPILER HEADER";
		#endif
		sentence = L"";

	}

	}
	fout.close();
}

#endif
