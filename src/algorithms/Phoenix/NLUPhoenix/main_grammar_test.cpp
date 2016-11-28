#include "debug.h"

#ifdef _GRAMTEST_
#ifndef _TEST_

#include <assert.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include "IDisambiguator.h"
#include "Grammar.h"
#include "PhoenixParser.h"
#include "CommandInterpreter.h"

using namespace std;
using namespace Phoenix;
using namespace Tools;

string dict_file = "";
string grammar_file = "";
string frames_file = "";
string disamb_dir = "";
string database_file = "";
string net_interp_file = "";
string dict_file_rus = "";
string grammar_file_rus = "";
string frames_file_rus = "";
string net_interp_file_rus = "";

shared_ptr<Grammar> G;
shared_ptr<Grammar> G_rus;
shared_ptr<SlotTrellis> GS;
shared_ptr<SlotTrellis> GS2;
shared_ptr<PhoenixParser> parser;
shared_ptr<PhoenixParser> parser_rus;

static void readConfig(
		string& dict_file, 
		string& grammar_file,
		string& frames_file,
		const string& path
		)
{
	string path2 = path.substr(path.find_first_not_of(' '), path.size() - path.find_first_not_of(' '));
	std::ifstream fin(path2);
	std::getline(fin, dict_file);
	std::getline(fin, grammar_file);
	std::getline(fin, frames_file);
}

static void reportErrorLine(int line, const wstring& msg)
{
    wcout << msg << ": " << line << endl<<endl;
}

int main(int argc, char **argv) 
{
    std::shared_ptr<Disambiguation::IDisambiguator> disambiguator
        = Disambiguation::IDisambiguator::GetDisambiguator(Tools::Language::RU);

    shared_ptr<Tokenization::ITokenizer> tokenizer =
        Tokenization::ITokenizer::GetTokenizer(Tools::Language::RU);

	string control_file(argv[4]);

	std::ofstream fout("result.txt");

    setlocale(LC_ALL, "");

    G = shared_ptr<Grammar>(new Grammar(argv[1], argv[2], argv[3]));
	GS = shared_ptr<SlotTrellis>(new SlotTrellis(G));
	parser = shared_ptr<PhoenixParser>(new PhoenixParser(G, GS, ENGLISH, ""));

	wifstream in(control_file);

	wstring sentence = L"";

	int i = 0;	

    std::wcout << endl;


    wstring SPACE = L" ";


	while(getline(in, sentence))
	{
        wstring sentence2 = L"";
		i++;
        if(i > 20000) break;
		if(Trim(sentence).length() == 0 || Trim(sentence)[0] == L'#')
				continue;

        std::wcout << sentence << std::endl;

        std::vector<Tokenization::Token> tokens = tokenizer->Tokenize(sentence);

        vector<Disambiguation::DisambiguatedData> disambiguatedData = disambiguator->Disambiguate(tokens);

        for(size_t i = 0; i < disambiguatedData.size(); i++)
        {
            sentence2.append(disambiguatedData[i].lemma);
            sentence2.append(SPACE);
        }

        sentence2.pop_back();

        ParseResult ret_code = parser->ParseLine(sentence2);

		if(!(SUCCESS == ret_code))
		{
			reportErrorLine(i, L"No parse!");
            parser->Reset();
            continue;
		}

		vector<shared_ptr<Parse>> parses = parser->GetAllParses();

		shared_ptr<Parse> parse = parses[0];

		wstring parsePrint = parser->GetParsesAsStrings(CONCISE)[0];

		wcout << parsePrint << endl;

		parser->Reset();
	}
	
	fout.close();
	in.close();
}


#endif
#endif
