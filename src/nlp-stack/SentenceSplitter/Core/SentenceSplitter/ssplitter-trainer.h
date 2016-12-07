/**
 * @file    ssplitter-trainer.h
 * @brief	header file with class implementing sentence splitter training environment
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


#ifndef SSPLITTERTRAINER_H
#define SSPLITTERTRAINER_H

#include <string>
#include "logistic-regression.h"

//typedef std::tuple<std::vector<std::wstring>, int> dataEntry;

typedef std::tuple<std::vector<std::wstring>, std::unordered_map<std::wstring, double>, int> dataEntry;

namespace _sentence_splitter
{

/**
 * @class	TestData
 * @brief	struct for storing and printing test data
 */
struct TestData
{
    float total;    /**< total number of test examples (true positive + true negative + false positive + false negative)*/
    float success;  /**< correctly classified (true positive + true negative) */
    float positive; /**< classified as positive (true positive + false positive)*/
    float tru_positive; /**< examples from positive class, classified correctly (true positive) */
    float tru;  /**< belonging to positive class (true positive + false negative)*/

    /**
     * @brief print test results
     */
    void printResult()
    {
        std::cout << "Total: " << int(total) << std::endl << "True positive: " << int(tru_positive) << std::endl
                  << "True(data): " << int(tru) << std::endl << "Positive(classified): " << int(positive) << std::endl;

        double accuracy = success / total;
        double precision = tru_positive / positive;
        double recall = tru_positive / tru;

        std::cout << "Precision: " << precision << std::endl << "Recall: " << recall << std::endl << "Accuracy: " << accuracy << std::endl;
    }

    /**
     * @brief Constructor. Initializes all data with zeroes.
     */
    TestData() : total(0), success(0), positive(0), tru_positive(0), tru(0) {};

};

/**
 * @class	Trainer
 * @brief	Sentence splitter trainer. Input file should contain sentences delimited by linebreak (sentence per line)
 */
class Trainer
{
public:
    /**
     * @brief Constructor
     * @param[in] i_left left context width
     * @param[in] i_right right context width
     */
    Trainer(int i_left, int i_right) : m_model(new _classifiers::BinaryFeaturesReader() ), extractor(i_left, i_right) {};

    /**
     * @brief load data in format 'sentence per line', prepare train set, then train
     * @param[in] i_data path to train file
     * @param[in] i_modelPath path to model
     */
    void train(const std::string i_data, const std::string i_validPath, const std::string i_modelPath, double spaceImportanceParam);

    /**
     * @brief load data in format 'sentence per line', prepare test set, then test
     * @param[in] i_data path to test file
     * @param[in] i_modelPath path to model
     */
    void test(const std::string i_data, const std::string i_modelPath, bool with_spaces);

    /**
     * @brief open log for saving features extracted during training/testing
     * @param[in] i_logPath path to log file
     */
    void openLog(std::string i_logPath);

private:
    std::shared_ptr<_classifiers::BinaryFeaturesReader> m_model;
    std::shared_ptr<_classifiers::LogisticRegressionModel> m_testModel;
    FeatureExtractor extractor;
    TestData m_testResults;

    //void loadData(const std::string i_data, bool forTrain);
    void loadData(const std::string i_data,
                         void(_sentence_splitter::Trainer::* processText)(const std::wstring&, const vector<int>&, int, double),
                         bool with_spaces,
                         double spaceImportanceParam);
    void processTextForTest(const std::wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double weight = 1.);
    void processTextForTrain(const std::wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double weight);
    void processTextForSemiTrain(const std::wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, double weight = 1.);
    void getLabeledData(const std::wstring& i_line_to_split, const vector<int>& i_endPositions, int i_ignoreLength, bool i_sure, vector<dataEntry>& result);
    bool getTextBlock(std::wifstream& i_in, const int i_blockSize, const std::wstring& i_leftContext,
                               std::wstring& o_text, vector<int>& o_endPositions, std::wstring delimiter);

    std::wofstream featureLogger;
};

} //_sentence_splitter

#endif // SSPLITTERTRAINER_H
