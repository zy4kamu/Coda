#include "ChainCollectionTester.h"

#include <unordered_map>
using std::unordered_map;

#include "FileWriter.h"

using namespace LinearCRF;

const size_t MAX_NUMBER_IN_TEST_SET = 100000;

TestedResultsData::TestedResultsData(
    int numberOfCorrect, int totalNumber
    , int numberOfFullyRightChains
    , int numberOfAllChains)
    : numberOfCorrect(numberOfCorrect)
    , totalNumber(totalNumber)
    , numberOfFullyRightChains(numberOfFullyRightChains)
    , numberOfAllChains(numberOfAllChains)
{
}

void TestedResultsData::Print()
{
    double fraction = static_cast<double>(numberOfCorrect)
        / static_cast<double>(totalNumber);
    double frcationOfFullyRightChains = static_cast<double>(numberOfFullyRightChains)
        / static_cast<double>(numberOfAllChains);
     std::wcout << L"Number of correct = " << numberOfCorrect
           << L"; Total number = " << totalNumber
           << L"; Fraction = " << fraction
           << std::endl;
     std::wcout << L"Number of fully correct chains = " << numberOfFullyRightChains
           << L"; Number of chains = " << numberOfAllChains
           << L"; Fraction = " << frcationOfFullyRightChains
           << std::endl;
}

ChainCollectionTester::ChainCollectionTester()
{
}

ChainCollectionTester::ChainCollectionTester(
    shared_ptr<IChainTransformer> chainTransformer
    , shared_ptr<IPossibleStateFinder> possibleStateFinder
    , CRF* crf)
    : crf_(crf)
    , chainTransformer_(chainTransformer)
    , possibleStateFinder_(possibleStateFinder)
{
    inputAlphabet_ = shared_ptr<Alphabet>(new Alphabet(crf_->GetInputAlphabet()));
    outputAlphabet_ = shared_ptr<Alphabet>(new Alphabet(crf_->GetOutputAlphabet()));
}

void ChainCollectionTester::Initialize(const string& testSetFile)
{
    encodedData_.clear();
    decodedData_.clear();
    cout << "\nChainCollectionTester: starting reading file..." << std::endl;
    size_t enumerator = 0;
    wifstream in(testSetFile);
	Tools::SetLocale(in);
    Chain chain;
    // Iterate over file
    while(in >> chain)
    {
        if (chain.GetSize() == 0)
        {
            break;
        }
        // Add decoded data
        DecodedCRFData decodedDataToAdd;
        decodedDataToAdd.labels = chain.GetLabels();
        decodedDataToAdd.tokens = chain.GetTokens();
        decodedData_.push_back(decodedDataToAdd);
        // Add encoded data
        EncodedCRFData encodedDataToAdd;
        encodedDataToAdd.intLabels = getLabelIndexes(chain);
        chain = chainTransformer_->ForwardTransform(chain);
        possibleStateFinder_->FindPossibleStates(&chain);
        encodedDataToAdd.intFeatures = getFeatureIndexes(chain);
        encodedDataToAdd.possibleStates = getPossibleStates(
            chain);
        encodedData_.push_back(encodedDataToAdd);
        // Print status of data downloading
        ++enumerator;
        std::wcout << "\rHave evaluated " << enumerator << " chains...";
        if (enumerator > MAX_NUMBER_IN_TEST_SET)
        {
            break;
        }
    }
    in.close();
    printf("\rHave evaluated %lu chains...", decodedData_.size());
    printf("\nChainCollectionLearner: end reading file...\n");
}

vector<vector<int> > ChainCollectionTester::getFeatureIndexes(
    const Chain& chain)
{
    size_t numberOfNodes = chain.GetSize();
    const vector<vector<wstring> >& features = chain.GetFeatures();
    vector<vector<int> > intFeatures(numberOfNodes);
    for (size_t nodeIndex = 0; nodeIndex < numberOfNodes; ++nodeIndex)
    {
        const vector<wstring>& currentFeatures = features[nodeIndex];
        for (size_t featureIndex = 0; featureIndex < currentFeatures.size()
            ; ++featureIndex)
        {
            int toAddFeature = inputAlphabet_->LookUpIndex_NoAdding(
                currentFeatures[featureIndex]);
            if (toAddFeature >= 0) 
            {
                intFeatures[nodeIndex].push_back(toAddFeature);
            }
        }
    }
    return intFeatures;
}

vector<int> ChainCollectionTester::getLabelIndexes(
    const Chain& chain)
{
    const vector<wstring>& labels = chain.GetLabels();
    vector<int> labelIndexes(labels.size(), -1);
    for (size_t iter = 0; iter < labels.size(); ++iter)
    {
        labelIndexes[iter] = outputAlphabet_->LookUpIndex_NoAdding(labels[iter]);
    }
    return labelIndexes;
}

vector<vector<int> > ChainCollectionTester::getPossibleStates(
    const Chain& chain)
{
    const vector<vector<wstring> >& possibleStatesString 
        = chain.GetPossibleStates();
    vector<vector<int> > possibleStates(possibleStatesString.size());
    for (size_t stateIndex = 0; stateIndex < possibleStates.size()
        ; ++stateIndex)
    {
        for (size_t featIndex = 0; featIndex < possibleStatesString[stateIndex].size()
            ; ++featIndex)
        {
            int featureIndex = outputAlphabet_->LookUpIndex_NoAdding(
                possibleStatesString[stateIndex][featIndex]);
            if (featureIndex >= 0) 
            {
                possibleStates[stateIndex].push_back(featureIndex);
            }
        }
    }
    return possibleStates;
}

TestedResultsData ChainCollectionTester::Test()
{
    unordered_map<wstring, unordered_map<wstring, int> > tablePredictedReal;
    int numberOfCorrect = 0;
    int totalNumber = 0;
    int numberOfFullyRightChains = 0;
    size_t size = encodedData_.size();
    for (size_t instanceIndex = 0; instanceIndex < size; ++instanceIndex)
    {
        const vector<int>& realSequence = encodedData_[instanceIndex].intLabels;
        totalNumber += realSequence.size();
        vector<int> bestSequence;
        vector<double> bestSequenceWeights;
        crf_->FindBestSequence_Digital(
            encodedData_[instanceIndex].intFeatures
            , encodedData_[instanceIndex].possibleStates
            , &bestSequence, &bestSequenceWeights);
        bestSequence = chainTransformer_->BackwardTransform(bestSequence);
        bool isChainRight = true;
        for (size_t nodeIndex = 0; nodeIndex < realSequence.size(); ++nodeIndex)
        {
            wstring real = outputAlphabet_->LookUpObject(realSequence[nodeIndex]);
            wstring predicted = outputAlphabet_->LookUpObject(bestSequence[nodeIndex]);
            if (tablePredictedReal.find(predicted) == tablePredictedReal.end()) {
                unordered_map<wstring, int> toAdd;
                toAdd[real] = 1;
                tablePredictedReal[predicted] = toAdd;
            } else {
                unordered_map<wstring, int>& value = tablePredictedReal[predicted];
                if (value.find(real) == value.end()) {
                    value[real] = 1;
                } else {
                    ++value[real];
                }
            }
            if (bestSequence[nodeIndex] == realSequence[nodeIndex]) {
                ++numberOfCorrect;
            } else {
                isChainRight = false;
            }
        }
        if (isChainRight) {
            ++numberOfFullyRightChains;
        }
    }
    FileManager::Write("CRF_TEST_STATISTICS.txt", tablePredictedReal, L'\t', L' ', L',');
    return TestedResultsData(numberOfCorrect, totalNumber, numberOfFullyRightChains, size);
}

TestedResultsData ChainCollectionTester::TestToFile(
    const string& outputFile)
{
    int numberOfCorrect = 0;
    int totalNumber = 0;
    int numberOfFullyRightChains = 0;
    wofstream out(outputFile);
	Tools::SetLocale(out);
    size_t size = encodedData_.size();
    std::wcout << std::endl;
    for (size_t instanceIndex = 0; instanceIndex < size; ++instanceIndex)
    {
        // Calculate best sequence (int)
        vector<int> bestSequenceInt;
        vector<double> bestSequenceWeights;
        crf_->FindBestSequence_Digital(
            encodedData_[instanceIndex].intFeatures
            , encodedData_[instanceIndex].possibleStates
            , &bestSequenceInt, &bestSequenceWeights);
        bestSequenceInt = chainTransformer_->BackwardTransform(bestSequenceInt);
        // Calculate statistics
        const vector<int>& realSequence = encodedData_[instanceIndex].intLabels;
        totalNumber += realSequence.size();
        bool isChainRight = true;
        for (size_t nodeIndex = 0; nodeIndex < realSequence.size(); ++nodeIndex)
        {
            if (bestSequenceInt[nodeIndex] == realSequence[nodeIndex]) {
                ++numberOfCorrect;
            } else {
                isChainRight = false;
            }
        }
        if (isChainRight) {
            ++numberOfFullyRightChains;
        }
        // Caclualte best seqeunce (string)
        vector<wstring> bestSequence(bestSequenceInt.size());
        for (size_t tokenIndex = 0; tokenIndex < bestSequence.size(); ++tokenIndex)
        {
            bestSequence[tokenIndex] =
                outputAlphabet_->LookUpObject(bestSequenceInt[tokenIndex]);
        }
        // Print
        const vector<wstring>& tokens = decodedData_[instanceIndex].tokens;
        const vector<wstring>& labels = decodedData_[instanceIndex].labels;
        for (size_t tokenIndex = 0; tokenIndex < bestSequence.size(); ++tokenIndex)
        {
            out << ((bestSequence[tokenIndex] == labels[tokenIndex]) ? 1 : 0) << L"\t"
                << tokens[tokenIndex] << L"\t"
                << bestSequence[tokenIndex] << L"\t"
                << labels[tokenIndex]
                << std::endl;
        }
        printf("\rHave evaluated %lu chains...", instanceIndex);
        out << std::endl << std::endl;
    }
    out.close();
    return TestedResultsData(numberOfCorrect, totalNumber, numberOfFullyRightChains, size);
}

TestedResultsData ChainCollectionTester::TestToFolder(
        const string& outputFolder)
{
    std::wcout << L"ChainCollectionTester::TestToFolder NOT IMPLEMENTED YET"
          << std::endl;
    return TestedResultsData(-1, -1, -1, -1);
}
