#include "DisambiguatorSettingsRus.h"

#include "ConjunctionsChainTransformer.h"
#include "DisambiguatorRus.h"
#include "DoublingChainTransformer.h"
#include "DoublingPossibleStateFinderRus.h"
#include "FeatureCalculatorRus.h"
#include "FullMorphologyChainTransformer.h"
#include "FullMorphologyPossibleStateFinder.h"
#include "L2Regularization.h"

using LinearCRF::ConjunctionsChainTransformer;
using LinearCRF::DoublingChainTransformer;
using LinearCRF::L2Regularization;

namespace Disambiguation
{

DisambiguatorSettingsRus::DisambiguatorSettingsRus()
    : DefaultDisambiguatorSettings(Tools::Language::RU)
{
}

shared_ptr<IChainTransformer>
DisambiguatorSettingsRus::GetChainTransformer()
{
    if (chainTransformer == 0)
    {
        vector<shared_ptr<IChainTransformer> > chainTransformers;

        vector<int> conjunctions;
        conjunctions.push_back(-2);
        conjunctions.push_back(-1);
        conjunctions.push_back(1);
        conjunctions.push_back(2);
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
DisambiguatorSettingsRus::GetPossibleStateFinder()
{
    if (possibleStateFinder == 0)
    {
        possibleStateFinder =
            std::make_shared<DoublingPossibleStateFinderRus>(
                string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/FileTranslationsForFeatureCalculator.txt"
                , string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Possible_labels.txt");
    }
    return possibleStateFinder;
}

shared_ptr<MorphologicalDictionary>
DisambiguatorSettingsRus::GetDictionary()
{
    if (dictionary == 0)
    {
        dictionary = std::make_shared<MorphologicalDictionary>(BIN_DIC_FILE_PATH_RUSSIAN);
        dictionary->setPrediction(true);
    }
    return dictionary;
}

shared_ptr<OpenCorporaToSynagrusConverter>
DisambiguatorSettingsRus::GetOpenCorporaToSynTagRusConverter()
{
    if (converter == 0) {
        converter = shared_ptr<OpenCorporaToSynagrusConverter>(
            new OpenCorporaToSynagrusConverter(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
            + "RU/Translation of OpenCorpora properties to Synatgrus.txt"));
    }
    return converter;
}

shared_ptr<IFeatureCalculator>
DisambiguatorSettingsRus::GetSimpleFeatureCalculator()
{
    return std::make_shared<SimpleFeatureCalculatorRus>(
        string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Token_LabelFrequencyDistribution.txt"
        , string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/UntranslatedTokens.txt"
        , string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Abbreviatures.txt"
        , this->GetDictionary()
        , this->GetOpenCorporaToSynTagRusConverter());
}

shared_ptr<IFeatureCalculator>
DisambiguatorSettingsRus::GetFeatureCalculator()
{
    if (featureCalculator == 0) {
        featureCalculator
            = std::make_shared<FeatureCalculatorRus>(
                string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Token_LabelFrequencyDistribution.txt"
                , string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/UntranslatedTokens.txt"
                , string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Abbreviatures.txt"
                , this->GetDictionary()
                , this->GetOpenCorporaToSynTagRusConverter());
    }
    return featureCalculator;
}

shared_ptr<DataFunctor> DisambiguatorSettingsRus::GetTrainingFunctor()
{
    return std::make_shared<Tools::NotModFunctor<9> >();
}

shared_ptr<DataFunctor> DisambiguatorSettingsRus::GetTestFunctor()
{
    return std::make_shared<Tools::ModFunctor<9> >();
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsRus::GetDisambiguator()
{
    return std::make_shared<DisambiguatorRus>(
        modelPath
        , this->GetDictionary()
        , this->GetFeatureCalculator()
        , this->GetChainTransformer()
        , this->GetPossibleStateFinder()
        , this->IsModelFileBinary());
}

}
