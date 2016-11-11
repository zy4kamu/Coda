#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

#include <boost/regex.hpp>

using Tokenization::Token;
using Tokenization::ITokenizer;
using Tokenization::TokenizerEn;

// check tokenizer using file with sentence pairs in simple format: Raw sentence 1\n Tokenized sentence 1\n Raw sentence 2 ...etc
const std::string testData = "/home/irina/PennTreeBank/tokens";
const std::string testResult = "/home/irina/PennTreeBank/tokenTest";

wstring prepareSentence(const wstring& sentence)
{
    wstring result = sentence;
    result = boost::regex_replace(result, boost::wregex(L"-LRB-"), L"\\(");
    result = boost::regex_replace(result, boost::wregex(L"-RRB-"), L"\\)");
    result = boost::regex_replace(result, boost::wregex(L"-LSB-"), L"\\[");
    result = boost::regex_replace(result, boost::wregex(L"-RSB-"), L"\\]");
    result = boost::regex_replace(result, boost::wregex(L"-LCB-"), L"\\{");
    result = boost::regex_replace(result, boost::wregex(L"-RCB-"), L"\\}");
    result = boost::regex_replace(result, boost::wregex(L"''"), L"\"");
    result = boost::regex_replace(result, boost::wregex(L"``"), L"\"");

    return result;
}

void checkEnTokenizer()
{
    wstring sentence;
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(Tools::Language::EN);
    TokenizerEn* enTokenizer = dynamic_cast<TokenizerEn*>(tokenizer.get());
    if(enTokenizer == 0) {
        std::wcout << L"Wrong language" << endl;
        return;
    }
    std::wifstream in(testData);
    std::wofstream out(testResult);
    int errors = 0;
    double time = 0;
    int count = 0;
    while(getline(in, sentence)) {
        wstring tokenizedSentence;
        if(!getline(in, tokenizedSentence)) {
            std::wcout << L"Incomplete pair: " << sentence;
            break;
        }
        wstring unNormalized = prepareSentence(sentence);
        clock_t start_time = clock();
        vector<Token> tokens = enTokenizer->Tokenize(unNormalized);
        clock_t end_time = clock();
        time += (double)(end_time-start_time);
        count++;
        wstring testString;
        for(size_t index = 0; index < tokens.size(); index++) {
            testString+=tokens[index].content;
            if(index != tokens.size()-1) {
                testString += L' ';
            }
        }
        if(testString.compare(Tools::Trim(tokenizedSentence)) != 0) {
            out << L"source:\t\t" << sentence << endl;
            out << L"tokenized:\t" << tokenizedSentence << endl;
            out << L"result:\t\t" << testString << endl << endl;
            errors++;
        }
    }
    wcout << L"Errors: " << errors << endl;
    wcout << L"time: " << time/CLOCKS_PER_SEC << L" for " << count << L" sentences\t" << time/count << L" per sentence" << endl;
}

int main()
{
    Tools::PrepareConsole();
    wstring sentence;
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(Tools::Language::EN);
    while(true)
    {
        std::wcout << L"Input sentence:\n";
        getline(std::wcin, sentence);
        //vector<Token> tokens = tokenizer->Tokenize(sentence);
        vector<size_t> positions;
        vector<Token> tokens = tokenizer->TokenizeWithAlignment(sentence, positions);
        for (auto& token : tokens)
        {
            std::wcout << token.content << std::endl;
        }
        for (auto& token : tokens)
        {
            wstring delimeter = token.isNextSpace ? L" " : L"";
            std::wcout << token.source << delimeter;
        }
        std::wcout << std::endl;

    }
    //checkEnTokenizer();
}

