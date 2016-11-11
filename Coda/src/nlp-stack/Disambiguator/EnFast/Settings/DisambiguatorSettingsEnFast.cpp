#include "DisambiguatorSettingsEnFast.h"

#include "ConjunctionsChainTransformer.h"
#include "DoublingChainTransformer.h"
#include "DoublingPossibleStateFinder.h"
#include "DefaultPossibleStateFinder.h"
#include "Disambiguator.h"
#include "FeatureCalculatorEnFast.h"
#include "L2Regularization.h"
#include "EmptyPossibleStateFinder.h"

using LinearCRF::ConjunctionsChainTransformer;
using LinearCRF::DoublingPossibleStateFinder;
using LinearCRF::L2Regularization;

namespace Disambiguation
{

const string configFolderEnFast = string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "EN_FAST/";

const string DisambiguatorSettingsEnFast::intermidiateOutputFile = configFolderEnFast + "intermediate.txt";
const size_t DisambiguatorSettingsEnFast::maxNumberOfIterations = 10000;
const string DisambiguatorSettingsEnFast::modelFile = configFolderEnFast + "model.txt";
const size_t DisambiguatorSettingsEnFast::periodOfTesting = 5;
const string DisambiguatorSettingsEnFast::trainingSetFile = configFolderEnFast + "train.txt";
const string DisambiguatorSettingsEnFast::testSetFile = configFolderEnFast + "test.txt";
const string DisambiguatorSettingsEnFast::tokenLabelSetFile = configFolderEnFast + "Token_Label.txt";
const string DisambiguatorSettingsEnFast::relearnSetFile = configFolderEnFast + "relearn.txt";
const string DisambiguatorSettingsEnFast::relearnInputSetFile = configFolderEnFast + "relearnInput.txt";
const bool DisambiguatorSettingsEnFast::isModelFileBinary = true;

const string DisambiguatorSettingsEnFast::tagsFile = configFolderEnFast + "Possible_labels.txt";
const string DisambiguatorSettingsEnFast::tokenFrequencyFile = configFolderEnFast + "Token_LabelFrequencyDistribution.txt";

DisambiguatorSettingsEnFast::DisambiguatorSettingsEnFast() : language(Tools::Language::EN_FAST)
{
}


shared_ptr<IChainTransformer>
DisambiguatorSettingsEnFast::GetChainTransformer()
{
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
DisambiguatorSettingsEnFast::GetPossibleStateFinder()
{
    if (possibleStateFinder == 0)
    {
        possibleStateFinder =
            std::make_shared<DoublingPossibleStateFinder>(
                tagsFile);
    }
    return possibleStateFinder;
}

shared_ptr<IRegularization>
DisambiguatorSettingsEnFast::GetRegularizator()
{
    return shared_ptr<IRegularization>(
        new L2Regularization(100000));
}

shared_ptr<RelearnRegularization>
DisambiguatorSettingsEnFast::GetRelearnRegularizator()
{
    return shared_ptr<RelearnRegularization>(
        new RelearnRegularization(0.0001));
}

shared_ptr<MorphologicalDictionary>
DisambiguatorSettingsEnFast::GetDictionary()
{
    if (dictionary == 0) {
        dictionary = std::make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_ENGLISH);
    }
    return dictionary;
}

shared_ptr<IFeatureCalculator>
DisambiguatorSettingsEnFast::GetFeatureCalculator()
{
    if (featureCalculator == 0) {
        featureCalculator
            = std::make_shared<FeatureCalculatorEnFast>(this->GetDictionary(), tagsFile);
    }
    return featureCalculator;
}

shared_ptr<DataFunctor> DisambiguatorSettingsEnFast::GetTrainingFunctor()
{
    return std::make_shared<TrainingFunctor>();
}

shared_ptr<DataFunctor> DisambiguatorSettingsEnFast::GetTestFunctor()
{
    return std::make_shared<TestFunctor>();
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsEnFast::GetDisambiguator()
{
    return std::make_shared<Disambiguator>(
        modelFile
        , this->GetDictionary()
        , this->GetFeatureCalculator()
        , this->GetChainTransformer()
        , this->GetPossibleStateFinder()
        , Tools::Language::EN_FAST
        , this->IsModelFileBinary());
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsEnFast::ReloadDisambiguator()
{
    return GetDisambiguator();
}

}
