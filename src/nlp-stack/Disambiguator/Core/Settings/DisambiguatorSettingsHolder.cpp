#include "DisambiguatorSettingsHolder.h"

namespace Disambiguation
{



DisambiguatorSettingsHolder::DisambiguatorSettingsHolder(
    std::shared_ptr<IDisambiguatorSettings> settings)
    : CRFSettingsHolder(settings)
{
}

shared_ptr<MorphologicalDictionary>
DisambiguatorSettingsHolder::GetDictionary()
{
    if (dictionary == 0)
    {
        dictionary
            = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetDictionary();
    }
    return dictionary;
}


shared_ptr<IFeatureCalculator>
DisambiguatorSettingsHolder::GetFeatureCalculator()
{
    if (featureCalculator == 0)
    {
        featureCalculator
            = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetFeatureCalculator();
    }
    return featureCalculator;
}

shared_ptr<DataFunctor> DisambiguatorSettingsHolder::GetTrainingFunctor()
{
    if (trainingFunctor == 0)
    {
        trainingFunctor
            = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetTrainingFunctor();
    }
    return trainingFunctor;
}

shared_ptr<DataFunctor> DisambiguatorSettingsHolder::GetTestFunctor()
{
    if (testFunctor == 0)
    {
        testFunctor
            = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetTestFunctor();
    }
    return testFunctor;
}

const string& DisambiguatorSettingsHolder::GetTokenLabelSetFile() const
{
    return std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetTokenLabelSetFile();
}

Language DisambiguatorSettingsHolder::GetLanguage() const
{
    return std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetLanguage();
}

shared_ptr<RelearnRegularization>
DisambiguatorSettingsHolder::GetRelearnRegularizator()
{
    if (relearnRegularizator == 0)
    {
        relearnRegularizator =  std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetRelearnRegularizator();
    }
    return relearnRegularizator;
}

const string&
DisambiguatorSettingsHolder::GetRelearnSetFile() const
{
    return  std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetRelearnSetFile();
}

const string&
DisambiguatorSettingsHolder::GetRelearnInputSetFile() const
{
    return  std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetRelearnInputSetFile();
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsHolder::GetDisambiguator()
{
    if (disambiguator == 0)
    {
        disambiguator
            = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->GetDisambiguator();
    }
    return disambiguator;
}

shared_ptr<IDisambiguator>
DisambiguatorSettingsHolder::ReloadDisambiguator()
{
    disambiguator
        = std::dynamic_pointer_cast<IDisambiguatorSettings>(settings)->ReloadDisambiguator();
    return disambiguator;
}


}

