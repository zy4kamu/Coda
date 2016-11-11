/** 
 * @file    libsvm-wrapper.h
 * @brief	header file for class-wrapper for main libsvm functions
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


#ifndef _LIBSVM_WRAPPER_
#define _LIBSVM_WRAPPER_

#include <string>
#include <memory>
#include <utility>
#include <vector>

using std::pair;
using std::string;
using std::shared_ptr;
using std::vector;

#include "svm.h"

namespace _svm
{
/**
 * @class	SVM
 * @brief	Class-wrapper for libsvm main functions
 */
class SVM
{
public:
    /**
     * @brief SVM is default constructor for svm class
     */
    SVM();

    /**
     * @brief loadModel is function for loading svm model from file.
     * To train model use train utility from original libsvm project
     * @param[in] i_filename is name of file with svm model
     */
    void loadModel(const string& i_filename);

    /**
     * @brief svm_type is getter for m_svm_type (code of type of svm)
     * @return m_svm_type member
     */
    int svm_type(void){return m_svm_type;}

    /**
     * @brief predict_probability is getter for m_predict_probability class member (0 is model without ability to return
     * probabilities, otherwise - with ability)
     * @return m_predict_probability
     */
    int predict_probability(void) { return m_predict_probability; }

    /**
     * @brief nr_class is getter for m_nr_class member (number of classes in svm model)
     * @return m_nr_class
     */
    int nr_class(void) { return m_nr_class; }

    /**
     * @brief labels is getter for m_labels member (vector with labels of classes in array of probabilities, when
     * m_predict_probability is not 0)
     * @return m_labels
     */
    vector<int> labels(void);

    /**
     * @overload predictInstanse(shared_ptr<svm_node> i_x)
     * @param i_x is smart pointer to instance (array of structures svm_node)
     * @param o_prob_estimates is array for storing probability estimations
     * @return prediction
     */
    double predictInstanse(shared_ptr<svm_node> i_x, vector<double>& o_prob_estimates);

    /**
     * @overload predictInstanse(shared_ptr<svm_node> i_x)
     * @param i_x is smart pointer to instance (array of structures svm_node)
     * @param o_prob_estimates is vector for storing probability estimations
     * @return prediction
     */
    double predictInstanse(shared_ptr<svm_node> i_x, shared_ptr<double> o_prob_estimates);

    /**
     * @brief predictInstanse is function for prediction for one instance i_x
     * @param i_x is smart pointer to instance (array of structures svm_node)
     * @return prediction
     */
    double predictInstanse(shared_ptr<svm_node> i_x);

    /**
     * @brief convertInstance is function for converting vector of indexes and values of features to "shared_ptred" form
     * @param i_x is vector of pairs: index of feature and its value
     * @return shared_ptr of array of svm_node structures
     */
    shared_ptr<svm_node> convertInstance(vector<pair<int, double> > &i_x);

    /**
     * @brief predictFile is function for processing file with instances.
     * Format for each line:
     * target_label feature_index:feature_value ... feature_index:feature_value
     * for example:
     * 1 2:32 4:13 14:0.23 21:-0.23
     * NOTES:
     * features should be placed in ascend order
     * decimal delimiter equal decimal delimiter in C locale(by default symbol of dot: ".")
     * @param i_ifile is name of file for reading data
     * @param i_ofile is name of file for writing results
     * @return accuracy of prediction for file
     */
    double predictFile(const string& i_ifile, const string& i_ofile);

private:
    shared_ptr<struct svm_model> m_model;
    vector<int> m_labels;

    int m_predict_probability;
    int m_svm_type;
    int m_nr_class;
};

}
#endif //_LIBSVM_WRAPPER_
