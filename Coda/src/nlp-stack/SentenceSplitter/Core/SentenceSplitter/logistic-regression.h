/**
 * @file    logistic-regression.h
 * @brief	header file with class implementing sentence splitting algorithm
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


#ifndef LOGISTICREGRESSION_H
#define LOGISTICREGRESSION_H

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include "ssplitter-feature-extractor.h"
#include "IOptimizable.h"
#include <iostream>

namespace _classifiers
{

class LogisticRegressionModelTrainer;

/**
 * @class	BinaryFeaturesReader
 * @brief	Helper class for logistic regression model training/test
 */
class BinaryFeaturesReader
{
public:
    /**
     * @brief loads sample from a file. Format is feature1\tfeature2\t...featureN\t {0|1}
     * @param[in] i_fileName path to the file with sample
     */
    void load(const char * i_fileName);
    /**
     * @brief adds one data entry (one line from model file)
     * @param[in] features list of features
     * @param[in] label class label
     */
    void addDataEntry(const std::vector<std::wstring>& features, const std::unordered_map<std::wstring, double>& double_features, int label, double weight = 1.);
    /**
     * @brief returns names of features of a model
     * @return vector of wstring's - names of features
     */
    std::vector<std::wstring> getFeatureNames() const;
    /**
     * @brief returns number of features
     * @return number of features
     */
    inline int getDimension() const { return m_index_to_feature_name.size(); }
    friend class LogisticRegressionModelTrainer;
    friend class EMLogisticRegressionTrainer;

private:
    std::map<std::wstring, int> m_feature_name_to_index;
    std::vector<std::wstring> m_index_to_feature_name;
    std::vector<std::unordered_map<int,double>> m_training_set;
    std::vector<int> m_train_labels;
 // for weighted training
    std::vector<double> m_weights;
};

/**
 * @class	LogisticRegressionModelTrainer
 * @brief	provides interface for LBFGs optimization based training
 */
class LogisticRegressionModelTrainer : public Optimization::IOptimizable
{
public:
    /**
     * @brief constructor
     * @param[in] i_model shared pointer to an object for reading training sample
     */
    LogisticRegressionModelTrainer(const std::shared_ptr<BinaryFeaturesReader>& i_model) : m_model(i_model) {dimension = m_model->m_index_to_feature_name.size();}
    /**
     * @brief calculates value of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @return value of the target function (sigmoid) at the point
     */
    virtual double CalculateValue(const std::vector<double>& i_point);
    /**
     * @brief calculates gradient of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @param[out] o_gradient value of the gradient function (sigmoid) at the point
     */
    virtual void CalculateGradient(
        const std::vector<double>& i_point, std::vector<double>* o_gradient);
    /**
     * @brief returns number of features
     * @return number of features
     */
    inline int GetDimension() const { return dimension; }
protected:
    double sigmoid_(const std::vector<double>& i_theta, const std::unordered_map<int,double>& i_item);
    std::shared_ptr<BinaryFeaturesReader> m_model;

};

/**
 * @class	LogisticRegressionModelTrainer
 * @brief	provides interface for LBFGs optimization based training
 */
class EMLogisticRegressionTrainer : public LogisticRegressionModelTrainer
{
public:
    /**
     * @brief constructor
     * @param[in] i_model shared pointer to an object for reading training sample
     */
    EMLogisticRegressionTrainer(const std::shared_ptr<BinaryFeaturesReader>& i_model) :
        LogisticRegressionModelTrainer( i_model ),
        expectationTheta(i_model->getDimension(),0) {};
    /**
     * @brief calculates value of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @return value of the target function (sigmoid) at the point
     * @remark for feature without labels calculatets soft count
     */
    virtual double CalculateValue(const std::vector<double>& i_point);
    /**
     * @brief calculates gradient of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @param[out] o_gradient value of the gradient function (sigmoid) at the point
     * @remark for feature without labes calculatets soft count
     */
    virtual void CalculateGradient(
        const std::vector<double>& i_point, std::vector<double>* o_gradient);

    /**
     * @brief set expectation theta value
     * @param[in] theta
     */
    void SetExpectation(const std::vector<double>& i_theta) { expectationTheta = i_theta; }
    /**
     * @brief get expectation theta value
     */
    const std::vector<double>& GetExpectation() const { return expectationTheta; }

private:
    // store theta from previous iteration
    vector<double> expectationTheta;

    double likelihood(const std::vector<double>& i_theta, const std::unordered_map<int, double>& i_item, const int label);
    void gradient(const std::vector<double>& i_theta, const std::unordered_map<int, double>& i_item, const int label, std::vector<double>* o_gradient);
};

/**
 * @class	LogisticRegressionModel
 * @brief	logistic regression classifier
 */
class LogisticRegressionModel
{
public:
    /**
     * @brief constructor. Reads the model from file.
     * @param[in] i_file_name path to the model file
     */
    LogisticRegressionModel(const std::string& i_file_name);
    /**
     * @brief constructor. Inits classifier from features and weights.
     * @param[in] i_features feature names
     * @param[in] i_theta feature weights
     */
    LogisticRegressionModel(const std::vector<std::wstring>& i_features, const std::vector<double>& i_theta);
    /**
     * @brief saves model in a file.
     * @param[in] i_file_name path to the model file
     */
    void save(const std::string& i_file_name) const;
    /**
     * @brief returns 1 or 0 - number of one of two classes
     * @param[in] i_features input features
     * @returns 1 or 0 for class
     */
    int getClass(const std::vector<std::wstring>& i_features) const;
private:
    std::map<std::wstring, int> m_feature_name_to_index;
    std::vector<std::wstring> m_index_to_feature_name;
    std::vector<double> m_theta;
};

/**
 * @class	LogisticRegressionManager
 * @brief	Wrap around training and test routunes
 */
class LogisticRegressionManager
{
public:
    void train(const std::string& i_path_to_training_set, const std::string& i_path_to_logistic_regression_model);
    void train(std::shared_ptr<_classifiers::BinaryFeaturesReader>& i_lrr, const std::string& i_path_to_logistic_regression_model);
    void trainEM(std::shared_ptr<_classifiers::BinaryFeaturesReader>& i_lrr, const std::string& i_path_to_logistic_regression_model);
    void test(const std::string& i_path_to_test_set, const std::string& i_path_to_logistic_regression_model);
};

class ENoSuchFile
{
public:
    ENoSuchFile(const std::string& i_fileName) { std::cout << "No such file: " << i_fileName << std::endl;}
};

} //namespace _classifiers

#endif // LOGISTICREGRESSION_H
