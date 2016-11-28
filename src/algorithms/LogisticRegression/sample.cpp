#include "sample.h"
#include <math.h>
#include <algorithm>

using std::vector;

namespace softmax_regression
{

TrainingSet::TrainingSet(const std::shared_ptr<ISampleReader>& i_sampleReader)
{
    i_sampleReader->load(m_indexToFeatureName, m_featureNameToIndex, m_trainingSet, m_classLabelToIndex, m_trainLabels);
    fillLabels();
}

size_t TrainingSet::getSize() const
{
    return m_trainingSet.size();
}

size_t TrainingSet::getNumberOfFeatures() const
{
    return m_indexToFeatureName.size();
}

const std::unordered_map<size_t,double>& TrainingSet::getElement(size_t i_elementIndex) const
{
    return m_trainingSet[i_elementIndex];
}

size_t TrainingSet::getLabelOfElement(size_t i_elementIndex) const
{
    return m_trainLabels[i_elementIndex];
}

size_t TrainingSet::getNumberOfClasses() const
{
	return m_classLabelToIndex.size();
}

void TrainingSet::fillLabels()
{
    m_classIndexToLabel.resize(m_classLabelToIndex.size());

    for(auto it = m_classLabelToIndex.begin(); it != m_classLabelToIndex.end(); it++ ) {
        m_classIndexToLabel[(size_t)it->second] = it->first;
    }
}

std::map<std::wstring,double> TrainingSet::getFeaturesOfElement(size_t i_elementIndex) const
{
    const std::unordered_map<size_t,double>& element = m_trainingSet[i_elementIndex];
    std::map<std::wstring,double> features;
    for( auto it = element.begin(); it != element.end(); it++) {
        features[m_indexToFeatureName[it->first] ] = it->second;
    }
    return features;
}

std::wstring TrainingSet::getStringLabelOfElement(size_t i_elementIndex) const
{
    return m_classIndexToLabel[ m_trainLabels[i_elementIndex] ];
}

} //namespace _softmax_regression
