#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <locale>

#include "IDisambiguator.h"
#include "ITokenizer.h"
#include "SyntaxChunker.h"
#include "ChunkingProcessor.h"
#include "Tools.h"

using std::wstring;
using std::cin;
using std::cout;
using std::vector;
using std::clock_t;
using std::wcout;
using std::getline;
using std::wcin;
using std::endl;

using Disambiguation::IDisambiguator;
using Tokenization::ITokenizer;
using Tokenization::Token;

static const string CHUNCKER_CONFIG = string(CHUNKER_CONFIG_PATH)+string("Chunker.bin");

void TestDisambiguatorAndChunker()
{
	clock_t t0;
	clock_t t1;
	try
	{
		t0 = clock();
        shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(
            Tools::Language::RU);
        shared_ptr<IDisambiguator> disambiguator =
            IDisambiguator::GetDisambiguator(Tools::Language::RU);
		ChunkingProcessor processor(CHUNCKER_CONFIG, disambiguator);
		t1 = clock();
		cout << "time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
		wstring sentence;
        vector<Token> tokens;
        vector<DisambiguatedData> disambiguatedData;
 		while(true)
		{
			wcout << "Input sentence:" << endl;
			getline(wcin, sentence);
			if (sentence == L"exit") break;
			t0 = clock();
			wcout << endl;

            tokens = tokenizer->Tokenize(sentence);
            disambiguatedData = disambiguator->Disambiguate(tokens);

			processor.Process(sentence, disambiguatedData);
			for (size_t tokenIndex = 0; tokenIndex < disambiguatedData.size()
                ; ++tokenIndex)
            {
			    wcout << tokenIndex << L": " 
                    << disambiguatedData[tokenIndex].content << L" " 
                    << disambiguatedData[tokenIndex].lemma << L" "
                    << disambiguatedData[tokenIndex].label << L" "
                    << disambiguatedData[tokenIndex].weight << L" "
                    << disambiguatedData[tokenIndex].lemmaId << L" "
                    << endl;
            }
			t1 = clock();
			cout << "time in seconds: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl << endl;
		}
	}
	catch (...)
	{
		wcout << "The problem occured while testing disambiguator";
	}
}

int main()
{
    #ifdef MSVC
        system("chcp 1251");
        std::locale::global(std::locale("Russian"));
    #else
        setlocale(LC_ALL, "");
	#endif

	TestDisambiguatorAndChunker();
	return 0;
}
