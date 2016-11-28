#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <locale>

#include "IDisambiguator.h"
#include "IStresser.h"
#include "ITokenizer.h"

using Disambiguation::DisambiguatedData;
using Disambiguation::IDisambiguator;

using std::wstring;
using std::cin;
using std::cout;
using std::vector;
using std::clock_t;
using std::wcout;
using std::getline;
using std::wcin;
using std::endl;

using Tokenization::ITokenizer;
using Stresser::IStresser;

void TestStresser(int numberOfHypothesis, bool useCyrillic)
{
    // Creating disambiguator
    clock_t t0 = clock();
	clock_t t1; 
    shared_ptr<ITokenizer> tokenizer = 
        ITokenizer::GetTokenizer(Tools::Language::RU);
    shared_ptr<IDisambiguator> disambiguator =
        Disambiguation::IDisambiguator::GetDisambiguator(Tools::Language::RU);
    shared_ptr<IStresser> stresser =
        Stresser::IStresser::GetStresser(Tools::Language::RU);
	t1 = clock();
	wcout << L"time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
    // Test
	wstring sentence;
	vector<Token> tokens;
    vector<vector<DisambiguatedData> > disambiguatedData;
 	while(true)
	{
		wcout << L"Input sentence:" << endl;
		getline(wcin, sentence);
		if (sentence == L"exit") break;
		wcout << endl;
		wcout << sentence << endl;
		t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        vector<double> hypothesisWeights;
        disambiguatedData = disambiguator->Disambiguate(
            tokens, numberOfHypothesis, &hypothesisWeights);
        for (size_t hypothesisIndex = 0; hypothesisIndex < disambiguatedData.size()
            ; ++hypothesisIndex)
        {
            wcout << L"hypothesis: " << hypothesisIndex << std::endl;
            vector<wstring> stress = stresser->SetStress(
                disambiguatedData[hypothesisIndex]);
		    for (size_t tokenIndex = 0
                ; tokenIndex < disambiguatedData[hypothesisIndex].size()
                ; ++tokenIndex)
            {
			    wcout << L"\t"
                    << tokenIndex << L": " 
                    << disambiguatedData[hypothesisIndex][tokenIndex].content << L" "
                    << stress[tokenIndex] << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].lemma << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].label << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].weight << L" "
                    << endl;
            }
        }
		t1 = clock();
		wcout << L"time in seconds: " << (double)(t1 - t0) / 
            CLOCKS_PER_SEC << endl << endl;
	}
}

void TestStresser(bool useCyrillic)
{
    // Creating disambiguator
    clock_t t0 = clock();
	clock_t t1;
    shared_ptr<ITokenizer> tokenizer =
        ITokenizer::GetTokenizer(Tools::Language::RU);
    shared_ptr<IDisambiguator> disambiguator =
        Disambiguation::IDisambiguator::GetDisambiguator(Tools::Language::RU);
    shared_ptr<IStresser> stresser =
        Stresser::IStresser::GetStresser(Tools::Language::RU);
    t1 = clock();
	wcout << L"time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
    // Test
	wstring sentence;
	vector<Token> tokens;
    vector<DisambiguatedData> disambiguatedData;
 	while(true)
	{
		wcout << L"Input sentence:" << endl;
		getline(wcin, sentence);
		if (sentence == L"exit") break;
		wcout << endl;
		wcout << sentence << endl;
		t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        disambiguatedData = disambiguator->Disambiguate(tokens);
        vector<wstring> stress = stresser->SetStress(
            disambiguatedData);
		for (size_t tokenIndex = 0; tokenIndex < disambiguatedData.size()
            ; ++tokenIndex)
        {
			wcout << tokenIndex << L": " 
                << disambiguatedData[tokenIndex].content << L" "
                << stress[tokenIndex] << L" "
                << disambiguatedData[tokenIndex].lemma << L" "
                << disambiguatedData[tokenIndex].label << L" "
                << disambiguatedData[tokenIndex].weight << L" "
                << endl;
        }
		t1 = clock();
		wcout << L"time in seconds: " << (double)(t1 - t0) / 
            CLOCKS_PER_SEC << endl << endl;
	}
}

int main()
{ 
    Tools::PrepareConsole();
    TestStresser(3, true);
}
