#include "DisambiguatorSettingsEn.h"

#include "ConjunctionsChainTransformer.h"
#include "DoublingChainTransformer.h"
#include "DoublingPossibleStateFinder.h"
#include "DefaultPossibleStateFinder.h"
#include "Disambiguator.h"
#include "FeatureCalculatorEn.h"
#include "L2Regularization.h"
#include "EmptyPossibleStateFinder.h"

using LinearCRF::ConjunctionsChainTransformer;
using LinearCRF::DoublingPossibleStateFinder;
using LinearCRF::L2Regularization;

namespace Disambiguation
{

const string configFolderEn = string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "EN/";

const string DisambiguatorSettingsEn::intermidiateOutputFile = configFolderEn + "intermediate.txt";
const size_t DisambiguatorSettingsEn::maxNumberOfIterations = 10000;
const string DisambiguatorSettingsEn::modelFile = configFolderEn + "model.txt";
const size_t DisambiguatorSettingsEn::periodOfTesting = 5;
const string DisambiguatorSettingsEn::trainingSetFile = configFolderEn + "train.txt";
const string DisambiguatorSettingsEn::testSetFile = configFolderEn + "test.txt";
const string DisambiguatorSettingsEn::tokenLabelSetFile = configFolderEn + "Token_Label.txt";
const string DisambiguatorSettingsEn::relearnSetFile = configFolderEn + "relearn.txt";
const string DisambiguatorSettingsEn::relearnInputSetFile = configFolderEn + "relearnInput.txt";
const bool DisambiguatorSettingsEn::isModelFileBinary = true;

const string DisambiguatorSettingsEn::tagsFile = configFolderEn + "Possible_labels.txt";
const string DisambiguatorSettingsEn::tokenFrequencyFile = configFolderEn + "Token_LabelFrequencyDistribution.txt";

DisambiguatorSettingsEn::DisambiguatorSettingsEn() : language(Tools::Language::EN)
{
}


shared_ptr<IChainTransformer>
DisambiguatorSettingsEn::GetChainTransformer()
{
//    if (chainTransformer == 0)
//    {
//        vector<int> conjunctions;
//        conjunctions.push_back(-2);
//        conjunctions.push_back(-1);
//        conjunctions.push_back(1);
//        conjunctions.push_back(2);
//        chainTransformer = shared_ptr<IChainTransformer>(new ConjunctionsChainTransformer(conjunctions));
//    }
//    return chainTransformer;

    if (chainTransformer == 0)
    {
        vector<shared_ptr<IChainTransformer> > chainTransformers;

        vector<int> conjunctions;
        conjunctions.push_back(-1);
        conjunctions.push_back(1);
        chainTransformers.push_back(
            shared_ptr<IChainTransformer>(
            new ConjunctionsChainTransformer(conjunctions)));

        chainTransformers.push_back(
           shared_ptr<IChainTransformer>(
           new LinearCRF::DoublingChainTransformer()));

        chainTransformer = shared_ptr<ChainTransformerType>(
            new ChainTransformerType(
            chainTransformers.begin(),
            chainTransformers.end()));
    }
    return chainTransformer;
}

shared_ptr<IPossibleStateFinder>
DisambiguatorSettingsEn::GetPossibleStateFinder()
{
//    if (possibleStateFinder == 0)
//    {
//        possibleStateFinder =
//            std::make_shared<LinearCRF::DefaultPossibleStateFinder>(tagsFile);
//    }
//    return possibleStateFinder;

    if (possibleStateFinder == 0)
    {
        possibleStateFinder =
            std::make_shared<DoublingPossibleStateFinder>(
                tagsFile);
    }
    return possibleStateFinder;
}

shared_ptr<IRegularization>
DisambiguatorSettingsEn::GetRegularizator()
{
    return shared_ptr<IRegularization>(
        new L2Regularization(100000));
}

shared_ptr<RelearnRegularization>
DisambiguatorSettingsEn::GetRelearnRegularizator()
{
    return shared_ptr<RelearnRegularization>(
        new RelearnRegularization(0.0001));
}

shared_ptr<MorphologicalDictionary>
DisambiguatorSettingsEn::GetDictionary()
{
    if (dictionary == 0) {
        dictionary = std::make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_ENGLISH);
        dictionary->enableRules(true);
    }
    return dictionary;
}

shared_ptr<IFeatureCalculator>
DisambiguatorSettingsEn::GetFeatureCalculator()
{
    if (featureCalculator == 0) {
//        featureCalculator
//            = std::make_shared<FeatureCalculatorEn>(tokenFrequencyFile, tagsFile);
        featureCalculator
            = std::make_shared<FeatureCalculatorEn>(this->GetDictionary(), tagsFile);
    }
    return featureCalculator;
}

shared_ptr<DataFunctor> DisambiguatorSettingsEn::GetTrainingFunctor()
{
    return std::make_shared<TrainingFunctor>();
}

shared_ptr<DataFunctor> DisambiguatorSettingsEn::GetTestFunctor()
{
    return std::make_shared<TestFunctor>();
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsEn::GetDisambiguator()
{
    return std::make_shared<Disambiguator>(
        modelFile
        , this->GetDictionary()
        , this->GetFeatureCalculator()
        , this->GetChainTransformer()
        , this->GetPossibleStateFinder()
        , Tools::Language::EN
        , this->IsModelFileBinary());
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsEn::ReloadDisambiguator()
{
    return GetDisambiguator();
}

}
