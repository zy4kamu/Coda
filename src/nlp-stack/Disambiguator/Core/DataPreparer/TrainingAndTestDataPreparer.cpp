#include "ConfigPreparerRus.h"
#include "TrainingAndTestDataPreparer.h"
#include "DisambiguatorSettingsRus.h"


namespace Disambiguation
{

TrainingAndTestDataPreparer::TrainingAndTestDataPreparer(
    shared_ptr<DisambiguatorSettingsHolder> settings)
    : settings(settings)
{
}

void TrainingAndTestDataPreparer::PrepareTrainingAndTestSet()
{
    wifstream in(settings->GetTokenLabelSetFile());
    wofstream outTrain(settings->GetTrainingSetFile());
    wofstream outTest(settings->GetTestSetFile());
    shared_ptr<DataFunctor> trainingSetFunctor = settings->GetTrainingFunctor();
    shared_ptr<DataFunctor> testSetFunctor = settings->GetTestFunctor();
    shared_ptr<IFeatureCalculator> calculator = settings->GetFeatureCalculator();
	Tools::SetLocale(in);
	Tools::SetLocale(outTrain);
	Tools::SetLocale(outTest);
    vector<Tokenization::Token> sentence;
    vector<wstring> labels;
    vector<PredisambiguatedData> evaluatedSentence;
    size_t sentenceEnumerator = 0;
    while (read(in, &sentence, &labels))
    {
        if ((*trainingSetFunctor)(sentenceEnumerator)) {
            evaluatedSentence = calculator->CalculateFeatures(sentence);
            print(outTrain, evaluatedSentence, labels);
        } else if ((*testSetFunctor)(sentenceEnumerator)) {
            evaluatedSentence = calculator->CalculateFeatures(sentence);
            print(outTest, evaluatedSentence, labels);
        }
        ++sentenceEnumerator;
        wcout << L"\rEvaluating sentence: "
              << sentenceEnumerator << L"...";
    }
    in.close();
    outTrain.close();
    outTest.close();
}

bool TrainingAndTestDataPreparer::read(wifstream& in
    , vector<Tokenization::Token>* sentence
    , vector<wstring>* labels)
{
    sentence->clear();
    labels->clear();
    wstring line;
    set<wchar_t> splitters;
    splitters.insert(L'\t');
    splitters.insert(L'|');
    vector<wstring> splitted;
    while (getline(in, line))
    {
        splitted = Tools::Split(line, splitters);
        if (splitted.size() == 2) {
            sentence->emplace_back(Tokenization::Token(
                splitted[0]
                , vector<wstring>()));
            labels->push_back(splitted[1]);
        } else if (splitted.size() == 3) {
            sentence->emplace_back(Tokenization::Token(
                splitted[0]
                , vector<wstring>(1, splitted[1])));
            labels->push_back(splitted[2]);
        } else {
            break;
        }
    }
    return sentence->size() > 0;
}

void TrainingAndTestDataPreparer::print(wofstream& out
    , const vector<PredisambiguatedData>& sentence
    , const vector<wstring>& labels)
{
    for (size_t tokenIndex = 0; tokenIndex < sentence.size()
        ; ++tokenIndex)
    {
        out << sentence[tokenIndex].content << L" ";
        if (sentence[tokenIndex].features.size() > 0)
        {
            out << sentence[tokenIndex].features[0];
        }
        for (size_t featureIndex = 1
            ; featureIndex < sentence[tokenIndex].features.size()
            ; ++featureIndex)
        {
            out << L"," << sentence[tokenIndex].features[featureIndex];
        }
        out << L" " << labels[tokenIndex];
        out << std::endl;
    }
    out << std::endl;
}


}
