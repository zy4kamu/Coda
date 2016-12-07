#include "Controller.h"
#include "ChainCollectionLearner.h"
#include "ChainCollectionTester.h"
#include "CRFApplier.h"
#include "CRFBinaryWriter.h"

namespace LinearCRF
{

typedef PipeChainTransformer<vector<shared_ptr<IChainTransformer> >::iterator> ChainTransformerType;
typedef ChainCollectionLearner<Optimization::LBFGS,  ChainCollectionTester, DONT_USE_UNPRESENTED_IN_CLIQUES_FEATURES> Learner;

Controller::Controller(shared_ptr<ICRFSettings> settings)
    : settingsHolder(settings)
    , applier(
        settingsHolder.GetModel()
        , settingsHolder.GetChainTransformer()
        , settingsHolder.GetPossibleStateFinder())
{
}

void Controller::Learn()
{
    Learner learner(
        settingsHolder.GetTrainingSetFile()
        , settingsHolder.GetTestSetFile()
        , settingsHolder.GetIntermidiateOutputFile()
        , settingsHolder.GetChainTransformer()
        , settingsHolder.GetPossibleStateFinder()
        , settingsHolder.GetRegularizator());
    CRF crf = learner.Learn(
        settingsHolder.GetMaxNumberOfIterations()
        , settingsHolder.GetPeriodOfTesting()
        , settingsHolder.GetPeriodOfTesting());
    CRFBinaryWriter writer(settingsHolder.GetIntermidiateOutputFile());
    writer.Write(crf);
}

void Controller::Test()
{
    CRF crf(settingsHolder.GetModelFile(), true);
    ChainCollectionTester tester(
        settingsHolder.GetChainTransformer()
        , settingsHolder.GetPossibleStateFinder()
        , &crf);
    tester.Initialize(settingsHolder.GetTestSetFile());
    tester.TestToFile("REAL_TEST_FILE.txt");
}

void Controller::Truncate(
     const string& inputCRFfile
     , const string& outputCRFfile
     , double truncationLevel)
{
    CRFBinaryReader reader(inputCRFfile);
    CRF crf;
    reader.Read(crf);
    crf.Truncate(truncationLevel);
    CRFBinaryWriter writer(outputCRFfile);
    writer.Write(crf);
}

void Controller::TranslateTextModelToBinary(
     const string& fileText
     , const string& fileBin)
{
    CRF crf(fileText, false);
    CRFBinaryWriter writer(fileBin);
    writer.Write(crf);
}

void Controller::TranslateBinaryModelToText(
     const string& fileBin
     , const string& fileText)
{
    std::wcout << L"Not implemented yet" << std::endl;
}

void Controller::Apply(Chain& chain
    , vector<wstring>* bestSequence
    , vector<double>* bestSequenceWeights)
{
    applier.Apply(chain, bestSequence, bestSequenceWeights);
}

void Controller::Apply(Chain& chain
    , size_t numberOfHypothesis
    , vector<vector<wstring> >* bestSequences
    , vector<vector<double> >* bestSequenceWeights
    , vector<double>* hypothesisDistribution)
{
    applier.Apply(chain
        , numberOfHypothesis, bestSequences
        , bestSequenceWeights, hypothesisDistribution);
}

void Controller::Apply(Chain& chain
    , double percentage
    , size_t maxNumberOfPaths
    , vector<vector<wstring> >* bestSequences
    , vector<vector<double> >* bestSequenceWeights
    , vector<double>* hypothesisDistribution)
{
    applier.Apply(chain
        , percentage, maxNumberOfPaths
        , bestSequences, bestSequenceWeights
        , hypothesisDistribution);
}

}
