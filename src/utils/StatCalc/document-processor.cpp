/** 
 * @file    document-processor.cpp
 * @brief	source file with class and functions for converting documents to bag of word form
 */

#include "document-processor.h"
#include "Tools.h"
#include <iostream>
#include <utility>
#include <functional>
#include "stat-calc.h"

using namespace statcalc;
using lemmatizer::ILemmatizer;
using _sentence_splitter::ISentenceSplitter;

DocumentProcessor::DocumentProcessor(shared_ptr<ITokenizer> i_tokenizer, shared_ptr<ILemmatizer> i_lemmatizer, shared_ptr<ISentenceSplitter> i_splitter)
{
    m_tokenizer = i_tokenizer;
    m_lemmatizer = i_lemmatizer;
    m_splitter = i_splitter;
    if (i_tokenizer->GetLanguage()==Tools::Language::RU)
        m_reTools.loadRegexpReplacementList(STAT_CALC_PREPROC_RULES_RU);
    else
        m_reTools.loadRegexpReplacementList(STAT_CALC_PREPROC_RULES_EN);
    m_docBOW.clear();
}

void DocumentProcessor::clean(void)
{
    m_docBOW.clear();
}

bool DocumentProcessor::processDocument(std::shared_ptr<Tools::Document> i_document, StatCalc& i_statcalc)
{
    if (i_document->content().empty())
    {
        clean();
        return false;
    }
    vector<size_t> splitIndexes;
    wstring currentSentence;

    m_docBOW.clear();
    for (size_t i = 0; i < i_document->content().size(); i++)
    {       
        splitIndexes = m_splitter->split(i_document->content()[i]);
        splitIndexes.push_back(i_document->content()[i].size());

        currentSentence = Tools::Trim(
                    i_document->content()[i].substr(0, splitIndexes[0] - 0 + 1));
        if (!currentSentence.empty())
        {
            this->processSentence_(currentSentence,i_statcalc);
        }
        for (size_t dotIndex = 1; dotIndex< splitIndexes.size(); ++dotIndex)
        {
            currentSentence = Tools::Trim(
                        i_document->content()[i].substr(splitIndexes[dotIndex-1] + 1, splitIndexes[dotIndex] - splitIndexes[dotIndex-1] ));
            if (currentSentence.empty()) continue;
            this->processSentence_(currentSentence,i_statcalc);
        }
    }
    return true;
}

void DocumentProcessor::processSentence_(wstring &currentSentence, StatCalc &i_statcalc)
{
    vector<Token> tokens;
    vector<wstring> result;
    int wordId;
    currentSentence = m_reTools.applyRegexReplacementList(currentSentence);
    tokens = m_tokenizer->Tokenize(currentSentence);
    for (size_t k = 0; k < tokens.size();k++)
    {
        if (tokens[k].content.empty())
        {
            tokens.erase(tokens.begin()+k);
            k--;
        }
    }
    m_lemmatizer->lemmatize(tokens,result);
    for (size_t j = 0; j < result.size(); j++)
    {
        auto it = i_statcalc.m_vocabulary.find(result[j]);
        if (it == i_statcalc.m_vocabulary.end())
        {
            if (!i_statcalc.predefinedVocabulary())
            {
                wordId = i_statcalc.m_vocabulary.size();
                i_statcalc.m_vocabulary[result[j]] = wordId;
            }
            else
            {
                continue;
            }
        }
        else
        {
            wordId = it->second;
        }
        Tools::AddToMap(&m_docBOW,wordId);
    }
}

bool DocumentProcessor::processDocument(const vector<vector<DisambiguatedData> > &i_doc, StatCalc &i_statcalc)
{
    if (i_doc.empty() || i_doc[0].empty())
        return false;

    m_docBOW.clear();
    int wordId;
    DisambiguatedData ddata;
    for (size_t i = 0; i < i_doc.size(); i++)
    {
        for (size_t j = 0; j < i_doc[i].size();j++)
        {
            auto it = i_statcalc.m_vocabulary.find(i_doc[i][j].lemma);
            if (it != i_statcalc.m_vocabulary.end())
            {
                wordId = i_statcalc.m_vocabulary.size();
            }
            else
            {
                wordId = it->second;
            }
            Tools::AddToMap(&m_docBOW,wordId);
        }
    }
    return true;
}
