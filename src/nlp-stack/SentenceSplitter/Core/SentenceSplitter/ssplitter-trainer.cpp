#include <memory>
#include <fstream>
#include <boost/regex.hpp>
#include "Tools.h"
#include "ssplitter-trainer.h"
#ifdef MSVC
    #include "dirent_win.h"
#else
	#include <dirent.h>
#endif

namespace _sentence_splitter
{

void Trainer::train(const std::string i_data, const std::string i_validPath, const std::string i_modelPath, double spaceImportanceParam)
{
    std::string marked = "/home/irina/SentenceSplitterData/trainSentences.en";
    loadData(i_data, &_sentence_splitter::Trainer::processTextForTrain, true, spaceImportanceParam);
    loadData(i_data, &_sentence_splitter::Trainer::processTextForTrain, false, spaceImportanceParam);
    _classifiers::LogisticRegressionManager trainManager;
    trainManager.train(m_model, i_modelPath);
    const std::string& valid = i_validPath == "" ? i_data : i_validPath;
    std::cout << "Param: " << spaceImportanceParam << "\tspaces: true" << std::endl;
    test(valid, i_modelPath, true);
    std::cout << "Param: " << spaceImportanceParam << "\tspaces: false" << std::endl;
    test(valid, i_modelPath, false);
}


// @TODO: leftContextLength should be class field (common for all methods)

void Trainer::loadData(const std::string i_data,
                       void(_sentence_splitter::Trainer::* processText)(const std::wstring&, const vector<int>&, int, double),
                       bool with_spaces,
                       double spaceImportanceParam)
{
    std::wifstream in(i_data);
    Tools::SetLocale(in);
    // number of sentences, combined to one text
    const int blockSize = 10;
    const size_t leftContextLength = 3;

    wstring text;
    wstring leftContext;
    vector<int> endPositions;

    std::wstring delimiter = ( with_spaces ? L" " : L"" );
    double param = ( with_spaces ? spaceImportanceParam : 1 - spaceImportanceParam );
    while(getTextBlock(in, blockSize, leftContext, text, endPositions, delimiter))
    {
        (this->*processText)(text, endPositions, leftContextLength, param);
        leftContext = text.substr(std::max(size_t(0), text.length() - leftContextLength));
        endPositions.clear();
        text.clear();
    }

    in.close();
}

bool Trainer::getTextBlock(std::wifstream& i_in, const int i_blockSize, const std::wstring& i_leftContext,
                           std::wstring& o_text, vector<int>& o_endPositions, std::wstring delimiter )
{
    std::wstring sentence;
    std::wstring text = i_leftContext;
    o_endPositions.clear();
    int sentenceCounter = 0;
    while(std::getline(i_in, sentence))
    {
        sentence = Tools::Trim(sentence);
        if(sentence.empty())
        {
            continue;
        }
        if(text.length() > 0)
        {
            o_endPositions.push_back(text.length()-1);
        }
        text+=delimiter;
        text+=sentence;
        sentenceCounter++;
        if(sentenceCounter >= i_blockSize)
        {
            o_text = text;
            return true;
        }
    }
    if(text.length() > i_leftContext.length())
    {
        o_endPositions.push_back(text.length()-1);
        o_text = text;
        return true;
    }
    return false;
}

void Trainer::openLog(std::string i_logPath)
{
    if(featureLogger.is_open())
    {
        featureLogger.close();
    }
    featureLogger.open(i_logPath);
}

void Trainer::processTextForTrain(const wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double weight)
{
    vector<dataEntry> dataEntries;
    getLabeledData(i_line_to_split,i_endPositions,i_ignoreLength, true, dataEntries);
    for(size_t i = 0; i < dataEntries.size(); i++)
    {
        m_model->addDataEntry(std::get<0>(dataEntries[i]), std::get<1>(dataEntries[i]), std::get<2>(dataEntries[i]), weight);
    }
}

void Trainer::processTextForSemiTrain(const wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double)
{
    vector<dataEntry> dataEntries;
    getLabeledData(i_line_to_split,i_endPositions,i_ignoreLength, false, dataEntries);
    for(size_t i = 0; i < dataEntries.size(); i++)
    {
        m_model->addDataEntry(std::get<0>(dataEntries[i]), std::get<1>(dataEntries[i]), std::get<2>(dataEntries[i]));
    }
}


void Trainer::processTextForTest(const wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double)
{
    vector<dataEntry> dataEntries;
    getLabeledData(i_line_to_split,i_endPositions,i_ignoreLength, true, dataEntries);
    for(size_t i = 0; i < dataEntries.size(); i++)
    {
        int tru_cl = std::get<2>(dataEntries[i]);
        int cl = m_testModel->getClass(std::get<0>(dataEntries[i]));
        if(cl == 1)
        {
            m_testResults.positive++;
        }
        if(tru_cl == 1)
        {
            m_testResults.tru++;
        }
        if(cl == tru_cl)
        {
            m_testResults.success++;
            if(tru_cl == 1)
            {
                m_testResults.tru_positive++;
            }
        }
        m_testResults.total++;
    }
}

void Trainer::getLabeledData(const wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, bool i_sure, vector<dataEntry>& result)
{
    result.clear();
    boost::wregex re_non_letter_symbols(L"[[:punct:]]");
    boost::wsregex_iterator m1(i_line_to_split.begin(), i_line_to_split.end(), re_non_letter_symbols);
    boost::wsregex_iterator m2;
    std::for_each(m1, m2,
        [this, &i_line_to_split, &i_endPositions, &i_ignoreLength, &result, &i_sure](const boost::match_results<std::wstring::const_iterator>&  entry) mutable
        {
            size_t position = entry.position(size_t(0));
            if(position >= i_line_to_split.length() - i_ignoreLength)
            {
                return;
            }
            std::vector<std::wstring> contextWindow = extractor.getFeatures(position, i_line_to_split);

            std::unordered_map<std::wstring, double> doubleFeatures = extractor.getDoubleFeatures(position, i_line_to_split);

            int label = i_sure ? 0 : -1;
            if(find( i_endPositions.begin(), i_endPositions.end(), position) != i_endPositions.end())
            {
                label = 1;
            }

            if(featureLogger.is_open())
            {
                for(size_t i = 0; i < contextWindow.size(); i++)
                {
                    featureLogger << (contextWindow[i]) << "\t";
                }

                for(auto iter = doubleFeatures.begin(); iter != doubleFeatures.end(); iter++)
                {
                    featureLogger << iter->first << "==" << iter->second <<  "\t";
                }

                featureLogger << label << std::endl;
            }

            result.push_back(dataEntry(contextWindow, doubleFeatures, label));
        }
    );
}

void Trainer::test(const std::string i_data, const std::string i_model, bool with_spaces)
{
    m_testResults = TestData();
    m_testModel = std::shared_ptr<_classifiers::LogisticRegressionModel>(new _classifiers::LogisticRegressionModel(i_model));
    loadData(i_data, &_sentence_splitter::Trainer::processTextForTest, with_spaces, 1.);
    m_testResults.printResult();
}

} //_sentence_splitter
