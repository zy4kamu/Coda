#include "UnitLabelChainCollectionTester.h"
#include "FileWriter.h"

namespace LinearCRF
{
    UnitLabelChainCollectionTester::UnitLabelChainCollectionTester()
        : ChainCollectionTester()
    {}

    UnitLabelChainCollectionTester::UnitLabelChainCollectionTester(
        shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , CRF* crf)
        : ChainCollectionTester(chainTransformer, possibleStateFinder, crf)
    {}

    TestedResultsData UnitLabelChainCollectionTester::Test()
    {
        int numberOfFullyRightChains = 0;
        size_t size = encodedData_.size();
        for (size_t instanceIndex = 0; instanceIndex < size; ++instanceIndex)
        {
            const vector<int>& realSequence = encodedData_[instanceIndex].intLabels;
            vector<int> bestSequence;
            vector<double> bestSequenceWeights;
            crf_->FindBestSequence_Digital(
                encodedData_[instanceIndex].intFeatures
                , encodedData_[instanceIndex].possibleStates
                , &bestSequence, &bestSequenceWeights);
            bestSequence = chainTransformer_->BackwardTransform(bestSequence);
            if (bestSequence[0] == realSequence[0]) {
                ++numberOfFullyRightChains;
            }
        }
        double frcationOfFullyRightChains = static_cast<double>(numberOfFullyRightChains)
            / static_cast<double>(size);
         std::wcout << L"Number of fully correct chains = " << numberOfFullyRightChains
               << L"; Number of chains = " << size
               << L"; Fraction = " << frcationOfFullyRightChains
               << std::endl;
         return TestedResultsData(numberOfFullyRightChains, numberOfFullyRightChains
            , numberOfFullyRightChains, size);
    }

    TestedResultsData UnitLabelChainCollectionTester::TestToFolder(
        const string& outputFolder)
    {
        unordered_map<wstring, unordered_map<wstring, int> > tablePredictedReal;
        unordered_map<wstring, unordered_map<wstring, vector<wstring> > > problemSentences;
        int numberOfFullyRightChains = 0;
        size_t size = encodedData_.size();
        for (size_t instanceIndex = 0; instanceIndex < size; ++instanceIndex)
        {
            const vector<int>& realSequence = encodedData_[instanceIndex].intLabels;
            vector<int> bestSequence;
            vector<double> bestSequenceWeights;
            crf_->FindBestSequence_Digital(
                encodedData_[instanceIndex].intFeatures
                , encodedData_[instanceIndex].possibleStates
                , &bestSequence, &bestSequenceWeights);
            bestSequence = chainTransformer_->BackwardTransform(bestSequence);
            // Create Sentence
            wstring sentence;
            for (size_t tokenIndex = 0; tokenIndex < decodedData_[instanceIndex].tokens.size()
                ; ++tokenIndex)
            {
                sentence += decodedData_[instanceIndex].tokens[tokenIndex] + L" ";
            }
            sentence = Tools::Trim(sentence);
            // Update table
            wstring real = outputAlphabet_->LookUpObject(realSequence[0]);
            wstring predicted = outputAlphabet_->LookUpObject(bestSequence[0]);
            if (tablePredictedReal.find(predicted) == tablePredictedReal.end()) {
                unordered_map<wstring, int> toAdd;
                toAdd[real] = 1;
                tablePredictedReal[predicted] = toAdd;
                unordered_map<wstring, vector<wstring> > toAddSentence;
                toAddSentence[real] = vector<wstring>(1, sentence);
                problemSentences[predicted] = toAddSentence;
            } else {
                unordered_map<wstring, int>& value = tablePredictedReal[predicted];
                unordered_map<wstring, vector<wstring> >& valueSentences = problemSentences[predicted];
                if (value.find(real) == value.end()) {
                    value[real] = 1;
                    valueSentences[real] = vector<wstring>(1, sentence);
                } else {
                    ++value[real];
                    valueSentences[real].push_back(sentence);
                }
            }
            // Update statistics
            if (bestSequence[0] == realSequence[0]) {
                ++numberOfFullyRightChains;
            }
        }
        double frcationOfFullyRightChains = static_cast<double>(numberOfFullyRightChains)
            / static_cast<double>(size);
         std::wcout << L"Number of fully correct chains = " << numberOfFullyRightChains
               << L"; Number of chains = " << size
               << L"; Fraction = " << frcationOfFullyRightChains
               << std::endl;
         FileManager::Write(outputFolder + "CRF_TEST_STATISTICS.txt"
             , tablePredictedReal, L'\t', L' ', L',');
         // Print
         for (auto firstIter = problemSentences.begin()
             ; firstIter != problemSentences.end(); ++firstIter)
         {
             for (auto secondIter = firstIter->second.begin()
                 ; secondIter != firstIter->second.end(); ++secondIter)
             {
                 string predictedLabel;
                 string realLabel;
                 if (
                    Tools::ConvertWstringToString(firstIter->first, predictedLabel)
                    && Tools::ConvertWstringToString(secondIter->first, realLabel)
                    )
                 {
                     FileManager::Write(
                         outputFolder 
                         + string("Predicted = ") +  predictedLabel 
                         + string("; Real = ") + realLabel 
                         + string(".txt"), secondIter->second);
                 } else {
                     std::wcout << L"Couldn't print results to file: "
                           << firstIter->first << L"+" << secondIter->first 
                           << std::endl;
                 }
             }
         }
         return TestedResultsData(numberOfFullyRightChains, numberOfFullyRightChains
            , numberOfFullyRightChains, size);
    }
}
