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

#ifndef SOFTMAXREGRESSIONTRAINER_H
#define SOFTMAXREGRESSIONTRAINER_H

#include <memory>
#include <climits>
#include "sample.h"

using std::string;

namespace softmax_regression
{

/**
 * @class	SoftmaxRegressionTrainerInterface
 * @brief   Interface for launch train, test, prepare
 */


class ISoftmaxRegressionTrainer
{
public:
    /**
     * @brief virtual destructor
     */
    virtual ~ISoftmaxRegressionTrainer() {};

    /**
     * @brief feature selection
     * @param[in] number of features to leave
     */
    virtual void SelectFeatures(int number) = 0;

    /**
     * @brief train model
     */
    virtual void Train() = 0;

    /**
     * @brief test model and output results to std output
     */
    virtual void Validate(string validPath) = 0;

};

struct TrainingParams {

    double pmiThreshold;
    int maxFeaturesCount;

    TrainingParams() : pmiThreshold( 0.3 ), maxFeaturesCount(INT_MAX) {};
};

/**
 * @class	SoftmaxRegressionTrainer
 * @brief	Launch train, test, prepare
 */
class SoftmaxRegressionTrainer : public ISoftmaxRegressionTrainer
{
public:
    /**
     * @brief constructor
     * @param[in] trainPath training set path
     * @param[in] testPath test set path
     */
    SoftmaxRegressionTrainer(string trainPath, string modelPath, TrainingParams params = TrainingParams() );

    /**
     * @brief feature selection
     * @param[in] number of features to leave
     */
   void SelectFeatures(int number);

   /**
   * @brief train model
   */
   void Train();

    /**
     * @brief test model and output results to std output
     */
    void Validate(string validPath);

private:
    std::shared_ptr<TrainingSet> m_trainingSet;
    std::string m_modelPath;
    TrainingParams m_params;
};

} //namespace softmax_regression

#endif // SOFTMAXREGRESSIONTRAINER_H
