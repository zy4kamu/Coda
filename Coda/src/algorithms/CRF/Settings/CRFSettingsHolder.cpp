#include "CRFSettingsHolder.h"

namespace LinearCRF
{

CRFSettingsHolder::CRFSettingsHolder(
    std::shared_ptr<ICRFSettings> settings)
    : settings(settings)
{
}

shared_ptr<IChainTransformer>
CRFSettingsHolder::GetChainTransformer()
{
    if (chainTransformer == 0)
    {
        chainTransformer = settings->GetChainTransformer();
    }
    return chainTransformer;
}

shared_ptr<IPossibleStateFinder>
CRFSettingsHolder::GetPossibleStateFinder()
{
    if (possibleStateFinder == 0)
    {
        possibleStateFinder = settings->GetPossibleStateFinder();
    }
    return possibleStateFinder;
}


shared_ptr<IRegularization>
CRFSettingsHolder::GetRegularizator()
{
    if (regularizator == 0)
    {
        regularizator = settings->GetRegularizator();
    }
    return regularizator;
}

shared_ptr<CRF> CRFSettingsHolder::GetModel()
{
    if (model == 0 && Tools::FileExists(this->GetModelFile()))
    {
        model = std::make_shared<CRF>(
            this->GetModelFile()
            , this->IsModelFileBinary());
    }
    return model;
}

const string& CRFSettingsHolder::GetIntermidiateOutputFile() const
{
    return settings->GetIntermidiateOutputFile();
}

const string& CRFSettingsHolder::GetModelFile() const
{
    return settings->GetModelFile();
}

const string& CRFSettingsHolder::GetTrainingSetFile() const
{
    return settings->GetTrainingSetFile();
}

const string& CRFSettingsHolder::GetTestSetFile() const
{
    return settings->GetTestSetFile();
}

bool CRFSettingsHolder::IsModelFileBinary() const
{
    return settings->IsModelFileBinary();
}

size_t CRFSettingsHolder::GetMaxNumberOfIterations() const
{
    return settings->GetMaxNumberOfIterations();
}

size_t CRFSettingsHolder::GetPeriodOfTesting() const
{
    return settings->GetPeriodOfTesting();
}

}


