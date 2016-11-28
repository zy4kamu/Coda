#include "DisambiguatorTester.h"

#include <cassert>

#include "FileWriter.h"

namespace Disambiguation
{

DisambiguatorTester::DisambiguatorTester(
    shared_ptr<DisambiguatorSettingsHolder> settings
    , shared_ptr<ITokenizer> tokenizer)
    : settings(settings)
    , tokenizer(tokenizer)
{
    assert(settings->GetLanguage() == tokenizer->GetLanguage());
}

/************ TESTING FROM FILE ************************/

void DisambiguatorTester::TestFromFile(
    const string& outputFile, bool testOnlyOnTest)
{
    unordered_map<wstring, unordered_map<wstring, int> > statistics;
    wofstream out(outputFile);
	Tools::SetLocale(out);
    wifstream in(settings->GetTokenLabelSetFile());
	Tools::SetLocale(in);
    // Preparing variables
    shared_ptr<DataFunctor> testSetFunctor = settings->GetTestFunctor();
    int enumerator = 0;
    int correctNumber = 0;
    int totalNumber = 0;
    wstring line;
    vector<wstring> splitted;
    vector<Token> tokens;
    vector<wstring> labels;
    wstring sentence;
    std::set<wchar_t> delimeters;
    delimeters.insert(L'\t');
    delimeters.insert(L'|');

    double commonTime = 0;
    // Reading file
    while (getline(in, line))
    {
        splitted = Tools::Split(line, delimeters);
        if (splitted.size() == 2) {
            if (!testOnlyOnTest || (*testSetFunctor)(enumerator))
            {
                tokens.emplace_back(Tokenization::Token(
                    splitted[0]
                    , vector<wstring>()));
                labels.push_back(std::move(splitted[1]));
            }
        } else if (splitted.size() == 3) {
            if (!testOnlyOnTest || (*testSetFunctor)(enumerator))
            {
                tokens.emplace_back(Tokenization::Token(
                    splitted[0]
                    , vector<wstring>(1, splitted[1])));
                labels.push_back(std::move(splitted[2]));
            }
        } else {
            if (!testOnlyOnTest || (*testSetFunctor)(enumerator))
            {
                clock_t t0 = clock();
                vector<DisambiguatedData> disambiguated
                    = settings->GetDisambiguator()->Disambiguate(tokens);
                clock_t t1 = clock();
                commonTime += (double)(t1 - t0) / CLOCKS_PER_SEC;
                totalNumber += tokens.size();
                for (size_t tokenIndex = 0; tokenIndex < tokens.size()
                    ; ++tokenIndex)
                {
                    if (disambiguated[tokenIndex].label == labels[tokenIndex])
                    {
                        ++correctNumber;
                    } else {
                        Tools::AddToMapMap(&statistics, disambiguated[tokenIndex].label
                            , labels[tokenIndex], 1);
                    }
                }
                wcout << L"\rCorrect = " << correctNumber
                      << L"; Total = " << totalNumber
                      << L"; Accuracy = " << double(correctNumber) / double(totalNumber);
                for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
                {
                    out << tokens[tokenIndex].content;
                    for (size_t punctIndex = 0; punctIndex < disambiguated[tokenIndex].punctuation.size()
                         ; ++punctIndex)
                    {
                        out << disambiguated[tokenIndex].punctuation[punctIndex];
                    }
                    out << L" ";
                }
                out << std::endl;
                for (size_t tokenIndex = 0; tokenIndex < tokens.size(); ++tokenIndex)
                {
                    out << ((disambiguated[tokenIndex].label == labels[tokenIndex]) ? 1 : 0) << L"\t"
                        << tokens[tokenIndex].content << L"\t"
                        << disambiguated[tokenIndex].label << L"\t"
                        << labels[tokenIndex]
                        << std::endl;
                }
                out << std::endl << std::endl;
                tokens.clear();
                labels.clear();
            }
            ++enumerator;
        }
    }
    in.close();
    out.close();
    FileManager::WriteSortedTable("MORPHOLOGY_STATISTICS.csv", statistics, L',');
    std::wcout << L"\nCommon disambiguated time: " << commonTime << std::endl;
}

/************ TESTING FROM CONSOLE ************************/

void DisambiguatorTester::Test(
    size_t maxNumberOfHypothesis
    , double percentage)
{
    wstring sentence;
    vector<Token> tokens;
    vector<vector<DisambiguatedData> > disambiguatedData;
    vector<double> hypothesisDistribution;
    while(true)
    {
        wcout << "Input sentence:" << endl;
        getline(wcin, sentence);
        if (sentence == L"exit") break;
        wcout << endl;
        wcout << sentence << endl;
        clock_t t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        disambiguatedData = settings->GetDisambiguator()->Disambiguate(tokens
            , percentage, maxNumberOfHypothesis
            , &hypothesisDistribution);
        for (size_t hypothesisIndex = 0; hypothesisIndex < disambiguatedData.size()
            ; ++hypothesisIndex)
        {
            wcout << L"hypothesis: " << hypothesisIndex << L" "
                  << hypothesisDistribution[hypothesisIndex] << std::endl;
            for (size_t tokenIndex = 0
                ; tokenIndex < disambiguatedData[hypothesisIndex].size()
                ; ++tokenIndex)
            {
                wcout << L"\t"
                    << tokenIndex << L": "
                    << disambiguatedData[hypothesisIndex][tokenIndex].content << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].lemma << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].label << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].weight << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].lemmaId << L" "
                    << endl;
            }
        }
        clock_t t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) /
            CLOCKS_PER_SEC << endl << endl;
    }
}

void DisambiguatorTester::Test(
    int numberOfHypothesis)
{
    wstring sentence;
    vector<Token> tokens;
    vector<vector<DisambiguatedData> > disambiguatedData;
    shared_ptr<IDisambiguator> disambiguator = settings->GetDisambiguator();
    vector<double> hypothesisDistribution;
    while(true)
    {
        wcout << "Input sentence:" << endl;
        getline(wcin, sentence);
        if (sentence == L"exit") break;
        wcout << endl;
        wcout << sentence << endl;
        clock_t t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        disambiguatedData = disambiguator->Disambiguate(tokens
            , numberOfHypothesis, &hypothesisDistribution);
        for (size_t hypothesisIndex = 0; hypothesisIndex < disambiguatedData.size()
            ; ++hypothesisIndex)
        {
            wcout << L"hypothesis: " << hypothesisIndex << L" "
                  << hypothesisDistribution[hypothesisIndex] << std::endl;
            for (size_t tokenIndex = 0
                ; tokenIndex < disambiguatedData[hypothesisIndex].size()
                ; ++tokenIndex)
            {
                wcout << L"\t"
                    << tokenIndex << L": "
                    << disambiguatedData[hypothesisIndex][tokenIndex].content << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].lemma << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].label << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].weight << L" "
                    << disambiguatedData[hypothesisIndex][tokenIndex].lemmaId << L" "
                    << endl;
            }
        }
        clock_t t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) /
            CLOCKS_PER_SEC << endl << endl;
    }
}

void DisambiguatorTester::Test()
{
    wstring sentence;
    vector<Token> tokens;
    vector<DisambiguatedData> disambiguatedData;
    while(true)
    {
        wcout << L"Input sentence:" << endl;
        getline(wcin, sentence);
        if (sentence.empty())
        {
            continue;
        }
        if (sentence == L"exit") break;
        wcout << endl;
        wcout << sentence << endl;
        clock_t t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        disambiguatedData = settings->GetDisambiguator()->Disambiguate(tokens);
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
        clock_t t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) /
            CLOCKS_PER_SEC << endl << endl;
    }
}

void DisambiguatorTester::TestLemmatizing()
{
    wstring sentence;
    vector<Token> tokens;
    vector<wstring> lemmas;
    while(true)
    {
        wcout << L"Input sentence:" << endl;
        getline(wcin, sentence);
        if (sentence.empty())
        {
            continue;
        }
        if (sentence == L"exit") break;
        wcout << endl;
        wcout << sentence << endl;
        clock_t t0 = clock();
        tokens = tokenizer->Tokenize(sentence);
        lemmas = settings->GetDisambiguator()->Lemmatize(tokens);
        for (size_t tokenIndex = 0; tokenIndex < lemmas.size()
            ; ++tokenIndex)
        {
            wcout << tokenIndex << L": "
                << tokens[tokenIndex].content << L" "
                << lemmas[tokenIndex] << L" "
                << endl;
        }
        clock_t t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) /
            CLOCKS_PER_SEC << endl << endl;
    }
}


}

