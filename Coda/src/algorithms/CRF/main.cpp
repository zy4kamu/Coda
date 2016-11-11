#include <string>
#include <ctime>
#include <iostream>

#include "CRF.h"
#include "CRFBinaryReader.h"
#include "CRFBinaryWriter.h"

#include "Chain.h"
#include "ChainOptimizable.h"
#include "ChainCollectionLearner.h"
#include "ConjunctionsChainTransformer.h"
#include "DefaultChainTransformer.h"
#include "DefaultPossibleStateFinder.h"
#include "IRegularization.h"
#include "L2Regularization.h"
#include "PipeChainTransformer.h"
#include "EmptyPossibleStateFinder.h"
#include "ChainCollectionTester.h"
#include "UnitLabelChainCollectionTester.h"
#include "LBFGS.h"

#include "ICRFSettings.h"
#include "Controller.h"

using std::string;
using std::clock_t;
using std::cout;
using std::endl;
using std::cin;
using std::wofstream;

namespace LinearCRF
{

class CRFSettingsExample : public ICRFSettings
{
public:
    virtual shared_ptr<IChainTransformer> GetChainTransformer()
    {
        vector<int> conjunctions;
        conjunctions.push_back(-3);
        conjunctions.push_back(-2);
        conjunctions.push_back(-1);
        conjunctions.push_back(1);
        conjunctions.push_back(2);
        conjunctions.push_back(3);

        return shared_ptr<IChainTransformer>(
            new ConjunctionsChainTransformer(conjunctions));
    }

    virtual shared_ptr<IPossibleStateFinder> GetPossibleStateFinder()
    {
        return shared_ptr<EmptyPossibleStateFinder>(
            new EmptyPossibleStateFinder(CRF_ALL_TAGS_FILE_PATH));
    }

    virtual shared_ptr<IRegularization> GetRegularizator()
    {
        return shared_ptr<IRegularization>(
            new L2Regularization(100000000000));
    }

    virtual const string& GetIntermidiateOutputFile() const
    {
        static const string retval(CRF_TESTING_MODEL_OUTPUT_PATH);
        return retval;
    }

    virtual const string& GetModelFile() const
    {
        static const string retval(CRF_TESTING_MODEL_OUTPUT_PATH);
        return retval;
    }

    virtual const string& GetTrainingSetFile() const
    {
        static const string retval(CRF_TRAINING_SET_PATH);
        return retval;
    }

    virtual const string& GetTestSetFile() const
    {
        static const string retval(CRF_TEST_SET_PATH);
        return retval;
    }

    virtual bool IsModelFileBinary() const
    {
        return true;
    }

    virtual size_t GetMaxNumberOfIterations() const
    {
        return 10000;
    }

    virtual size_t GetPeriodOfTesting() const
    {
        return 10;
    }
};

}

int main()
{
    Tools::PrepareConsole();
    LinearCRF::Controller controller(std::make_shared<LinearCRF::CRFSettingsExample>());


    clock_t t0 = clock();
    std::wcout << L"Input: learn or test: " << std::endl;
    wstring input;
    std::wcin >> input;
    if (input == L"learn") {
        controller.Learn();
    } else if (input == L"test") {
        controller.Test();
    }
	clock_t t1 = clock();
	std::wcout << L"time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
}
