/**
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

#ifndef SOFTMAXREGRESSION_H
#define SOFTMAXREGRESSION_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "IOptimizable.h"
#include "sample.h"

namespace softmax_regression
{

struct IntIntPairHash
{
    size_t operator()(std::pair<size_t, size_t> x) const throw()
    {
         size_t sum = x.first + x.second;
         size_t h = sum * (sum + 1);
         h >>= 1;
         h += x.second;
         return h;
    }
};

/**
 * @class	SoftmaxRegressionLearningFunction
 * @brief	provides interface for LBFGs optimization based training
 */
class SoftmaxRegressionLearningFunction : public Optimization::IOptimizable
{
public:
    /**
     * @brief constructor
     * @param[in] i_model shared pointer to an object for reading training sample
     */
    SoftmaxRegressionLearningFunction(const std::shared_ptr<TrainingSet>& i_sample);

    /**
     * @brief calculates value of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @return value of the target function (sigmoid) at the point
     */
    double CalculateValue(const std::vector<double>& i_point);
    /**
     * @brief calculates gradient of the target function (sigmoid) at the point
     * @param[in] i_point values of features at the point
     * @param[out] o_gradient value of the gradient function (sigmoid) at the point
     */
    void CalculateGradient(
        const std::vector<double>& i_point, std::vector<double>* o_gradient);
    /**
     * @brief returns number of features
     * @return number of features
     */
    inline int GetDimension() const { return dimension; }
private:
    void cacheSums_(const std::vector<double>& i_theta);

    size_t m_numberOfClasses;
    double logSoftmax_(const std::vector<double>& i_theta, size_t elementIndex);
    double softmax_(const std::vector<double>& i_theta, size_t elementIndex);
    std::shared_ptr<TrainingSet> m_sample;
    std::vector<double> m_Z;
	size_t m_numberOfFeatures;
};

class SoftmaxRegression
{
public:
    SoftmaxRegression(const std::shared_ptr<IModelReader>& modelSource);
    wstring getClass(const std::map<std::wstring, double>& i_features) const;
private:
    double getScore_(const std::map<std::wstring, double>& i_features, size_t label) const;
    std::vector<double> m_theta;
    size_t m_numberOfFeatures;
    size_t m_numberOfClasses;
    std::map<std::wstring, size_t> m_featureNameToIndex;
    std::map<std::wstring, int> m_classLabelToIndex;
    std::vector<std::wstring> m_indexToClassLabel;
};

class EModelCorruption
{
public:
    EModelCorruption() { std::wcout << L"ERROR: softmax regression model is corrupted" << std::endl; }
};

} //namespace _softmax_regression

#endif // SOFTMAXREGRESSION_H
