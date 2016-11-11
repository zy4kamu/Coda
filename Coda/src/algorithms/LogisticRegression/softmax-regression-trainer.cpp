#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "softmax-regression-trainer.h"
#include "Tools.h"
#include "softmax-regression.h"
#include "IOptimizable.h"
#include "LBFGS.h"
#include "features-writer.h"
#include "features-filter.h"

namespace softmax_regression
{
    SoftmaxRegressionTrainer::SoftmaxRegressionTrainer(string trainPath, string modelPath, TrainingParams params)
        : m_modelPath(modelPath)
        , m_params(params)
    {
        std::shared_ptr<softmax_regression::ISampleReader> sampleReader = std::shared_ptr<softmax_regression::ISampleReader>(new softmax_regression::TextSampleReader(trainPath));

        m_trainingSet = std::make_shared<softmax_regression::TrainingSet>(softmax_regression::TrainingSet(sampleReader));

        std::shared_ptr<IFeaturesFilter> filter = std::shared_ptr<softmax_regression::IFeaturesFilter>(new softmax_regression::FeaturesFilter(INT_MIN, params.maxFeaturesCount));
        filter->Filter(m_trainingSet);
    }

    void SoftmaxRegressionTrainer::SelectFeatures(int number)
    {
 //        m_trainingSet->FilterFeaturesByPMI(number);
    }

    void SoftmaxRegressionTrainer::Train()
    {
        softmax_regression::SoftmaxRegressionLearningFunction optimizableFunction(m_trainingSet);

        Optimization::LBFGS lbfgs;

        std::vector<double> zeroInit(optimizableFunction.GetDimension(), 0);
        std::vector<double> theta = zeroInit;
        lbfgs.Optimize(optimizableFunction, NULL, zeroInit, &theta, 150000);

        std::shared_ptr<softmax_regression::IModelWriter> writer(std::shared_ptr<softmax_regression::IModelWriter>(new softmax_regression::BinaryModelWriter()));
        writer->save(m_trainingSet->getFeatures(), m_trainingSet->getLabels(), theta, std::string(m_modelPath));
    }

    void SoftmaxRegressionTrainer::Validate(string validPath)
    {
        std::shared_ptr<softmax_regression::ISampleReader> sampleReader = std::shared_ptr<softmax_regression::ISampleReader>(new softmax_regression::TextSampleReader(validPath));
        std::shared_ptr<softmax_regression::TrainingSet> testSet = std::make_shared<softmax_regression::TrainingSet>(sampleReader);

        std::shared_ptr<softmax_regression::IModelReader> modelReader = std::shared_ptr<softmax_regression::IModelReader>(new softmax_regression::ModelBinaryReader(m_modelPath));
        softmax_regression::SoftmaxRegression classifier(modelReader);

        int right_counter = 0;
        int total_counter = 0;
        for(size_t i = 0; i < testSet->getNumberOfElements(); i++) {
             std::map<std::wstring,double> element = testSet->getFeaturesOfElement(i);
             std::wstring label = testSet->getStringLabelOfElement(i);
             wstring predicted = classifier.getClass(element);
             total_counter++;
             if(label.compare(predicted) == 0) {
                 right_counter++;
             } else {
                 for(auto it = element.begin(); it != element.end(); it++) {
                     wcout << it->first << L" ";
                 }
                 wcout << label << endl;
             }
        }
        wcout << "Right: " << right_counter << " : " << total_counter << endl;

    }

} //namespace _softmax_regression
