#include <cmath>
#include <iostream>
#include "LBFGS.h"
#include "Tools.h"
#include "softmax-regression.h"

namespace softmax_regression
{

SoftmaxRegressionLearningFunction::SoftmaxRegressionLearningFunction(const std::shared_ptr<TrainingSet>& i_sample)
    : m_sample(i_sample)
    , m_Z(0)
{
	
	std::unordered_map<size_t,double> element;

	//for(size_t i = 0; i < m_sample->getSize(); i++)
	//{
	//	element = m_sample->getElement(i);
	//	size_t classIndex = m_sample->getLabelOfElement(i);
	//	
	//	for(auto featIter = element.begin(); featIter != element.end(); featIter++)
	//	{
	//		std::pair<size_t, size_t> keyPair(classIndex, featIter->first);

	//		if(m_featureClassPairToThetaIndex.find(keyPair) == m_featureClassPairToThetaIndex.end())
	//		{
	//			m_featureClassPairToThetaIndex[keyPair] = m_featureClassPairToThetaIndex.size();
	//		}
	//	}
	//}
	m_numberOfFeatures =  m_sample->getNumberOfFeatures();
	m_numberOfClasses =  m_sample->getNumberOfClasses();
	dimension = m_numberOfClasses * m_numberOfFeatures;
}

double SoftmaxRegressionLearningFunction::CalculateValue(const std::vector<double>& i_theta)
{
    double result = 0;
    int m = m_sample->getSize();

    for(int i = 0; i < m; ++i)
    {
        result -= logSoftmax_(i_theta, i);;
    }

    double norm = 0;

    for(int j = 0; j < dimension; j++)
    {
        norm += i_theta[j]*i_theta[j];
    }

    result += 0.0001*norm;

    return result;
}

void SoftmaxRegressionLearningFunction::CalculateGradient(
    const std::vector<double>& i_theta, std::vector<double>* gradient)
{
	cacheSums_(i_theta);
    int m = m_sample->getSize();
    std::unordered_map<size_t,double> element;

    for(int i = 0; i < m; ++i)
    {
        double fi = softmax_(i_theta, i);
        element = m_sample->getElement(i);
		size_t classIndex = m_sample->getLabelOfElement(i);
        for(auto iter = element.begin(); iter != element.end(); iter++)
        {
			size_t indexInTheta = classIndex * m_numberOfFeatures + iter->first;//m_featureClassPairToThetaIndex[std::pair<int, int>(classIndex, iter->first)];
            (*gradient)[indexInTheta] -= (iter->second)*fi*(1-fi);
            (*gradient)[indexInTheta] += 2*0.0001 * i_theta[
                    indexInTheta];
        }
    }
}

double SoftmaxRegressionLearningFunction::logSoftmax_(const std::vector<double>& i_theta, size_t elementIndex)
{
    double result = 0;
    double trueClassScore = 0;
    size_t classIndex = m_sample->getLabelOfElement(elementIndex);

    const std::unordered_map<size_t, double>& item = m_sample->getElement(elementIndex);

    for(auto iter = item.begin(); iter != item.end(); iter++)
    {
        size_t indexInTheta = classIndex * m_numberOfFeatures + iter->first;
        trueClassScore += (iter->second)*i_theta[indexInTheta];
    }

    result = trueClassScore - std::log(m_Z[elementIndex]);

    return result;
}

double SoftmaxRegressionLearningFunction::softmax_(const std::vector<double>& i_theta, size_t elementIndex)
{
    double result = 0;
    double trueClassScore = 0;
    size_t classIndex = m_sample->getLabelOfElement(elementIndex);

    const std::unordered_map<size_t, double>& item = m_sample->getElement(elementIndex);

    for(auto iter = item.begin(); iter != item.end(); iter++)
    {
        size_t indexInTheta = classIndex * m_numberOfFeatures + iter->first;
        trueClassScore += i_theta[indexInTheta]*(iter->second);
    }

    result = std::exp(trueClassScore) / m_Z[elementIndex];

    return result;
}

void SoftmaxRegressionLearningFunction::cacheSums_(const std::vector<double>& i_theta)
{
	m_Z = std::vector<double>(m_sample->getSize(), 0);

    for(size_t itemIndex = 0; itemIndex < m_sample->getSize(); itemIndex++)
    {
        double z = 0;
        std::unordered_map<size_t, double> item = m_sample->getElement(itemIndex);

        for(size_t classIndex = 0; classIndex < m_numberOfClasses; classIndex++)
        {
            double classScore = 0;

            for(auto iter = item.begin(); iter != item.end(); iter++)
            {
                size_t indexInTheta = classIndex * m_numberOfFeatures + iter->first;
                classScore += i_theta[indexInTheta]*(iter->second);
            }

            z += std::exp(classScore);
        }

        m_Z[itemIndex] = z;
    }
}

SoftmaxRegression::SoftmaxRegression(const std::shared_ptr<IModelReader>& modelSource)
{
    std::vector<std::wstring> features;
    modelSource->load(features,m_indexToClassLabel,m_theta);

    for(size_t i = 0; i < features.size(); i++)
    {
        m_featureNameToIndex[features[i]] = i;
    }

    for(size_t i = 0; i < m_indexToClassLabel.size(); i++)
    {
        m_classLabelToIndex[m_indexToClassLabel[i]] = i;
    }

    m_numberOfClasses = m_classLabelToIndex.size();

    m_numberOfFeatures = m_theta.size() / m_numberOfClasses;

    if(m_numberOfFeatures * m_numberOfClasses != m_theta.size())
    {
        throw EModelCorruption();
    }
}

wstring SoftmaxRegression::getClass(const std::map<std::wstring, double>& i_features) const
{
    double maxScore = 0;//std::numeric_limits<double>::infinity();
    size_t maxArg = -1;

    for(size_t c = 0; c < m_indexToClassLabel.size(); c++)
    {
        double score = getScore_(i_features,c);
        if(score > maxScore)
        {
            maxArg = c;
            maxScore = score;
        }
    }

    if(maxArg == size_t(-1))
    {
        std::wcout << L"ERROR: No class determined!" << std::endl;
        return L"ERROR(_NO_CLASS)";
    }
    return m_indexToClassLabel[maxArg];
}

double SoftmaxRegression::getScore_(const std::map<std::wstring, double>& i_features, size_t label) const
{
    double o_result = 0;

    for(auto iter = i_features.begin(); iter != i_features.end(); iter++)
    {
        if(m_featureNameToIndex.find(iter->first) == m_featureNameToIndex.end()) {
            continue;
        }
        size_t index = label * m_numberOfFeatures + m_featureNameToIndex.at(iter->first);
        o_result += m_theta.at(index) * iter->second;
    }

    return o_result;
}

} //namespace _softmax_regression
