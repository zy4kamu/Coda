#include "Controller.h"

#include "ChainCollectionLearner.h"
#include "DisambiguatorRelearner.h"
#include "DisambiguatorTester.h"
#include "TrainingAndTestDataPreparer.h"

namespace Disambiguation
{

Controller::Controller(
    shared_ptr<IDisambiguatorSettings> settings
    , shared_ptr<ITokenizer> tokenizer)
    : settingsHolder(std::make_shared<DisambiguatorSettingsHolder>(settings))
    , tokenizer(tokenizer)
{
}

void Controller::Learn()
{
    Learner learner(
        settingsHolder->GetTrainingSetFile()
        , settingsHolder->GetTestSetFile()
        , settingsHolder->GetIntermidiateOutputFile()
        , settingsHolder->GetChainTransformer()
        , settingsHolder->GetPossibleStateFinder()
        , settingsHolder->GetRegularizator());

    LinearCRF::CRF crf = learner.Learn(
       settingsHolder->GetMaxNumberOfIterations()
        , settingsHolder->GetPeriodOfTesting()
        , settingsHolder->GetPeriodOfTesting());

    LinearCRF::CRFBinaryWriter writer(
        settingsHolder->GetIntermidiateOutputFile());

    writer.Write(crf);
}

void Controller::Prepare()
{
    TrainingAndTestDataPreparer preparer(settingsHolder);
    preparer.PrepareTrainingAndTestSet();
}

void Controller::Relearn()
{
    DisambiguatorRelearner relearner(settingsHolder, tokenizer);
    relearner.RelearnDialog();
}

void Controller::TestFromFile()
{
    DisambiguatorTester tester(settingsHolder, tokenizer);
    tester.TestFromFile("REAL_TEST_FILE.txt");
}

void Controller::Test()
{
    DisambiguatorTester tester(settingsHolder, tokenizer);
    wcout << L"Input number of hypothesis (integer) or percentage of path coverage (double less than 1):\n";
    wstring wstringInput;
    std::wcin >> wstringInput;
    string stringInput;
    Tools::ConvertToString(wstringInput, stringInput);
    if (!Tools::ContainsSubstring(stringInput, "."))
    {
        int numberOfHypothesis = std::atoi(stringInput.c_str());
        if (numberOfHypothesis > 1) {
            tester.Test(numberOfHypothesis);
        } else {
            tester.Test(true);
        }
    } else {
        double percentage = std::atof(stringInput.c_str());
        wcout << L"Input max number of hypothesis:\n";
        std::wcin >> wstringInput;
        Tools::ConvertToString(wstringInput, stringInput);
        size_t numberOfHypothesis = std::atoi(stringInput.c_str());
        tester.Test(numberOfHypothesis, percentage);
    }
}

void Controller::Lemmatize()
{
    DisambiguatorTester tester(settingsHolder, tokenizer);
    tester.TestLemmatizing();
}

}
