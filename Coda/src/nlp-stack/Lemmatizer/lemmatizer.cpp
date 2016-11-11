/** 
 * @file    lemmatizer.cpp
 * @brief	source file with class and functions for lemmitization of words with help of dictionary
 */

#include "IDisambiguator.h"
#include "lemmatizer.h"
#include "ISentenceSplitter.h"
#include "Tools.h"
#include <iostream>

using namespace Disambiguation;

namespace lemmatizer
{

shared_ptr<ILemmatizer> ILemmatizer::CreateLemmatizer(
    shared_ptr<IDisambiguator> i_disambiguator)
{
    return shared_ptr<ILemmatizer>(new Lemmatizer(i_disambiguator));
}

Lemmatizer::Lemmatizer(shared_ptr<IDisambiguator> i_disambiguator)
{
    if (i_disambiguator == NULL)
    {
        m_disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
    }
    else
    {
        m_disambiguator = i_disambiguator;
    }
}

bool Lemmatizer::lemmatize(const vector<Token> &i_tokens, vector<wstring> &o_result, const std::function<wstring(const DisambiguatedData&)>& i_extractor)
{
    if (i_tokens.empty())
    {
        cout<<L"Nothing to lemmatize!\n";
        return false;
    }
    m_tokens = i_tokens;
    size_t size = m_tokens.size();
    m_result.clear();
    m_result.reserve(size);
    vector<DisambiguatedData> ddata = m_disambiguator->Disambiguate(m_tokens);
    addDDataToResult_(ddata, i_extractor);
    o_result = m_result;
    return true;
}

bool Lemmatizer::lemmatize(const vector<Token> &i_tokens, vector<wstring> &o_result)
{
    return lemmatize(i_tokens, o_result, extractLemma);
}

void Lemmatizer::addDDataToResult_(vector<DisambiguatedData>& i_ddata, const std::function<wstring(const DisambiguatedData&)>& i_extractor)
{
    for (size_t i = 0; i < i_ddata.size(); i++)
    {
        wstring extract = i_extractor(i_ddata[i]);
        m_result.push_back(extract);
    }
}

shared_ptr<ILemmatizer> GetLemmatizer(Tools::Language lang)
{

    shared_ptr<IDisambiguator> disambiguator;
    if (lang ==Tools::Language::EN)
        disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::EN_FAST);
    else
        disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
    shared_ptr<ILemmatizer> lemmatizer =
        ILemmatizer::CreateLemmatizer(disambiguator);
    return lemmatizer;
}

void Lemmatize(const char *fileIn, const char *fileOut, int& SentenceCounter, Tools::Language lang)
{

    int numberOfEvaluatedSentences = 0;
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(lang);
    shared_ptr<ILemmatizer> lemmatizer = GetLemmatizer(lang);
    shared_ptr<_sentence_splitter::ISentenceSplitter> sentenceSplitter = _sentence_splitter::ISentenceSplitter::GetSentenceSplitter(lang);

    wifstream in(fileIn);
	Tools::SetLocale(in);
    wofstream out(fileOut);
	Tools::SetLocale(out);

    wstring line;
    while (getline(in, line))
    {
        if (line.empty())
        {
            out << std::endl;
            continue;
        }
        vector<size_t> splitIndexes(1, -1);
        vector<size_t> splitIndexes1 = sentenceSplitter->split(line);
        splitIndexes.insert(splitIndexes.end(), splitIndexes1.begin(), splitIndexes1.end());

        splitIndexes.push_back(line.size());
        for (size_t dotIndex = 0; dotIndex + 1< splitIndexes.size(); ++dotIndex)
        {
            if ((numberOfEvaluatedSentences % 10 == 0) && (SentenceCounter == 1))
            {
                std::wcout << numberOfEvaluatedSentences << std::endl;
            }
            wstring currentSentence = Tools::Trim(
                line.substr(splitIndexes[dotIndex] + 1, splitIndexes[dotIndex + 1] - splitIndexes[dotIndex] + 1));
            if (currentSentence.empty()) continue;
            vector<Token> tokens = tokenizer->Tokenize(currentSentence);
            vector<wstring> lemmas;
            lemmatizer->lemmatize(tokens, lemmas,  Lemmatizer::extractLemma);
            if (lemmas.size() > 0)
            {
                out << lemmas[0];
                for (size_t index = 1; index < lemmas.size(); ++index)
                {
                    out << L" " << lemmas[index];
                }
                out << std::endl;
            }
            ++numberOfEvaluatedSentences;
        }
    }

    in.close();
    out.close();
}

}
