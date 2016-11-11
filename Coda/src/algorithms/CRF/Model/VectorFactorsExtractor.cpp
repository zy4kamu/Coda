#include "VectorFactorsExtractor.h"

using namespace LinearCRF;

VectorFactorsExtractor::VectorFactorsExtractor(const CRF& i_crf, vector<double>* o_parameters)
{
    o_parameters->clear();

    const Factors& factors = i_crf.GetParameters();
    const vector<double>& initialWeights = factors.GetInitialWeights();
    const vector<double>& finalWeights = factors.GetFinalWeights();
    const vector<double>& defaultWeights = factors.GetDefaultWeights();

    numberOfLabels = i_crf.GetOutputAlphabet().Size();
    numberOfFeatures = i_crf.GetInputAlphabet().Size();
    numberOfCliques = i_crf.GetWeightsAlphabet().Size();

    // Insert intial, final and default weights
    dimension = 2 * numberOfLabels + numberOfCliques;
    o_parameters->resize(dimension);
    std::copy(initialWeights.begin(), initialWeights.end(),
        o_parameters->begin());
    std::copy(finalWeights.begin(), finalWeights.end(),
        o_parameters->begin() + numberOfLabels);
    std::copy(defaultWeights.begin(), defaultWeights.end(),
        o_parameters->begin() + 2 * numberOfLabels);

    cliqueFeatureConverter.resize(numberOfCliques);
    const vector<unordered_map<int, double> >& cliquesData = factors.GetWeights();
    // Iterate over cliques
    for (size_t cliqueIndex = 0; cliqueIndex < cliquesData.size();
        ++cliqueIndex)
    {
        unordered_map<int, int> featureToPositionInVector;
        const unordered_map<int, double>& currentCliquesData = cliquesData[cliqueIndex];
        for (auto iter = currentCliquesData.begin(); iter != currentCliquesData.end(); ++iter)
        {
            o_parameters->push_back(iter->second);
            featureToPositionInVector[iter->first] = dimension;
            ++dimension;
        }
        cliqueFeatureConverter[cliqueIndex] = std::move(featureToPositionInVector);
        printf("\rVectorFactorsExtractor evaluated %zu cliques", cliqueIndex);
    }
}

double VectorFactorsExtractor::GetInitialWeight(
    int labelIndex, const vector<double>& factorsVector)
{
    return factorsVector[labelIndex];
}


double VectorFactorsExtractor::GetFinalWeight(
    int labelIndex, const vector<double>& factorsVector)
{
    return factorsVector[numberOfLabels + labelIndex];
}

double VectorFactorsExtractor::GetDefaultWeight(
    int cliqueIndex, const vector<double>& factorsVector)
{
    return factorsVector[2 * numberOfLabels + cliqueIndex];
}

double VectorFactorsExtractor::GetFeatureWeight(
    int cliqueIndex, int featureIndex, 
    const vector<double>& factorsVector)
{
    return factorsVector[cliqueFeatureConverter[cliqueIndex][featureIndex]];
}

int VectorFactorsExtractor::GetInitialWeightIndex(int labelIndex)
{
    return labelIndex;
}


int VectorFactorsExtractor::GetFinalWeightIndex(int labelIndex)
{
    return numberOfLabels + labelIndex;
}

int VectorFactorsExtractor::GetDefaultWeightIndex(int cliqueIndex)
{
    return 2 * numberOfLabels + cliqueIndex;
}

int VectorFactorsExtractor::GetFeatureWeightIndex(
    int cliqueIndex, int featureIndex)
{
    return cliqueFeatureConverter[cliqueIndex][featureIndex];
}

bool VectorFactorsExtractor::ContainsFeature(
        int cliqueIndex, int featureIndex) const
{
    const unordered_map<int, int>& cliqueMap = cliqueFeatureConverter[cliqueIndex];
    return cliqueMap.find(featureIndex) != cliqueMap.end();
}

void VectorFactorsExtractor::Initialize(
        int _numberOfLabels, int _numberOfFeatures, 
        int _numberOfCliques,
        const unordered_map<int, unordered_set<int> >& cliquePossibleFeatures)
{
    numberOfLabels = _numberOfLabels;
    numberOfFeatures = _numberOfFeatures;
    numberOfCliques = _numberOfCliques;

    cliqueFeatureConverter.resize(numberOfCliques);
    dimension = 2 * numberOfLabels + numberOfCliques;
    // Iterate over cliques
    int enumerator = 0;
    for (auto cliqueIterator = cliquePossibleFeatures.begin(); 
        cliqueIterator != cliquePossibleFeatures.end();
        ++cliqueIterator)
    {
        unordered_map<int, int> featureToPositionInVector;
        const unordered_set<int>& possibleFeatures =  
            cliqueIterator->second;
        // Find positions in vector for features in clique
        for (auto featureIterator = possibleFeatures.begin();
            featureIterator != possibleFeatures.end();
            ++featureIterator)
        {
            featureToPositionInVector[*featureIterator] = 
                dimension;
            ++dimension;
        }
        cliqueFeatureConverter[cliqueIterator->first] =
            std::move(featureToPositionInVector);
        ++enumerator;
        printf("\rEvaluated %d cliques", enumerator);
    }
}

int VectorFactorsExtractor::GetDimension()
{
    return dimension;
}

shared_ptr<vector<double> >  VectorFactorsExtractor::GetInitialWeights(
    const vector<double>& factorsVector)
{
    return std::make_shared<vector<double> >(
        vector<double>(factorsVector.begin(), 
        factorsVector.begin() + numberOfLabels));
}

shared_ptr<vector<double> > VectorFactorsExtractor::GetFinalWeights(
    const vector<double>& factorsVector)
{
    return std::make_shared<vector<double> >(
        factorsVector.begin() + numberOfLabels, 
        factorsVector.begin() + 2 * numberOfLabels);
}

shared_ptr<vector<double> > VectorFactorsExtractor::GetDefaultWeights(
    const vector<double>& factorsVector)
{
    return std::make_shared<vector<double> >(
        factorsVector.begin() + 2 * numberOfLabels, 
        factorsVector.begin() + 2 * numberOfLabels + numberOfCliques);
}

unordered_map<int, double> VectorFactorsExtractor::GetCliqueWeights(
    int cliqueIndex,
    const vector<double>& factorsVector) const
{
    unordered_map<int, double> cliqueWeghts;
    const unordered_map<int, int>& cliqueMap = cliqueFeatureConverter[cliqueIndex];
    for (auto& item : cliqueMap)
    {
        cliqueWeghts[item.first] = factorsVector[item.second];
    }
    return cliqueWeghts;
}
