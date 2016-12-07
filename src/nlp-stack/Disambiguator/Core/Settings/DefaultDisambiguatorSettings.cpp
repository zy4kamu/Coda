#include "DefaultDisambiguatorSettings.h"
#include "DefaultChainTransformer.h"
#include "Disambiguator.h"
#include "EmptyPossibleStateFinder.h"
#include "L2Regularization.h"

using LinearCRF::DefaultChainTransformer;
using LinearCRF::EmptyPossibleStateFinder;
using LinearCRF::L2Regularization;

namespace Disambiguation
{

DefaultDisambiguatorSettings::DefaultDisambiguatorSettings(
    Tools::Language language)
    : language(language)
    , intermediateOutputPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/Intermidiate_Model.txt")
    , modelPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/Model.txt")
    , relearnInputSetPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/RelearnInputSet.txt")
    , relearnSetPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/RelearnSet.txt")
    , testSetPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/TestSet.txt")
    , tokenLabelSetPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/Token_Label.txt")
    , trainingSetPath(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + Tools::LanguageToString(language) + "/TrainingSet.txt")
{
}

const string& DefaultDisambiguatorSettings::GetIntermidiateOutputFile() const
{
    return intermediateOutputPath;
}

const string& DefaultDisambiguatorSettings::GetModelFile() const
{
    return modelPath;
}

const string& DefaultDisambiguatorSettings::GetTrainingSetFile() const
{
    return trainingSetPath;
}

const string& DefaultDisambiguatorSettings::GetTestSetFile() const
{
    return testSetPath;
}

const string& DefaultDisambiguatorSettings::GetRelearnSetFile() const
{
    return relearnSetPath;
}

const string& DefaultDisambiguatorSettings::GetRelearnInputSetFile() const
{
    return relearnInputSetPath;
}

const string& DefaultDisambiguatorSettings::GetTokenLabelSetFile() const
{
    return tokenLabelSetPath;
}

shared_ptr<IChainTransformer>
DefaultDisambiguatorSettings::GetChainTransformer()
{
    if (chainTransformer == 0)
    {
        chainTransformer = std::make_shared<DefaultChainTransformer>();
    }
    return chainTransformer;
}

shared_ptr<IPossibleStateFinder>
DefaultDisambiguatorSettings::GetPossibleStateFinder()
{
    if (possibleStateFinder == 0)
    {
        possibleStateFinder = std::make_shared<EmptyPossibleStateFinder>();
    }
    return possibleStateFinder;
}

shared_ptr<IRegularization>
DefaultDisambiguatorSettings::GetRegularizator()
{
    return shared_ptr<IRegularization>(
        new L2Regularization(100000));
}

shared_ptr<RelearnRegularization>
DefaultDisambiguatorSettings::GetRelearnRegularizator()
{
    return shared_ptr<RelearnRegularization>(
        new RelearnRegularization(0.0001));
}

shared_ptr<MorphologicalDictionary>
DefaultDisambiguatorSettings::GetDictionary()
{
    return 0;
}

shared_ptr<DataFunctor> DefaultDisambiguatorSettings::GetTrainingFunctor()
{
    return std::make_shared<Tools::NotModFunctor<9> >();
}

shared_ptr<DataFunctor> DefaultDisambiguatorSettings::GetTestFunctor()
{
    return std::make_shared<Tools::ModFunctor<9> >();
}

Language DefaultDisambiguatorSettings::GetLanguage() const
{
    return language;
}

bool DefaultDisambiguatorSettings::IsModelFileBinary() const
{
    return true;
}

size_t DefaultDisambiguatorSettings::GetMaxNumberOfIterations() const
{
    return 10000;
}

size_t DefaultDisambiguatorSettings::GetPeriodOfTesting() const
{
    return 10;
}

shared_ptr<IDisambiguator>
DefaultDisambiguatorSettings::GetDisambiguator()
{
    return std::make_shared<Disambiguator>(
        modelPath
        , this->GetDictionary()
        , this->GetFeatureCalculator()
        , this->GetChainTransformer()
        , this->GetPossibleStateFinder()
        , this->GetLanguage()
        , this->IsModelFileBinary());
}

shared_ptr<IDisambiguator>
DefaultDisambiguatorSettings::ReloadDisambiguator()
{
    return GetDisambiguator();
}

}
