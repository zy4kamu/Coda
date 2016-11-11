/** 
 * @file	VectorFactorsExtractor.h
 * @brief	Given vector that describes all parameters of the CRF 
 *          this class enables to extract features from it
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


#ifndef VECTOR_FACTORS_ECTRACTOR
#define VECTOR_FACTORS_ECTRACTOR

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "CRF.h"

using std::vector;
using std::shared_ptr;
using std::unordered_map;
using std::unordered_set;

const double IMPOSSIBLE_WEIGHT = -1000000;

namespace LinearCRF
{

/** 
 * @class	VectorFactorsExtractor
 * @brief	given vector that describes all parameters of the CRF 
 *          this class enables to extract features from it
 *          Vector should contain parameters in the following format:
 *          initialWeights, finalWeights, defaultWeights, features in cliques
 */
class VectorFactorsExtractor
{
public:
    /**
     * @brief	Constructor
     */
    VectorFactorsExtractor()
    {}

    /**
    * @brief	Constructor that represents CRF as a single vector
    * @param[in]	i_crf - CRF-model
    * @param[out]	o_parameters - number of features in CRF-model
    */
    VectorFactorsExtractor(const CRF& i_crf, vector<double>* o_parameters);

     /**
     * @brief	Creates correspondence of parameters and its labels
     * @param[in]	numberOfLabels - number of labels in CRF-model
     * @param[in]	numberOfFeatures - number of features in CRF-model
     * @param[in]	numberOfCliques - number of cliques in CRF-model
     * @param[in]	cliquePossibleFeatures - possible features for each clique
     */
    void Initialize(
        int numberOfLabels, int numberOfFeatures, 
        int numberOfCliques, 
        const unordered_map<int, unordered_set<int> >& cliquePossibleFeatures);

    /**
     * @brief	Destructor
     */
    ~VectorFactorsExtractor(void){}

    /**
	 * @brief	Extracts initial weights from vector
	 * @param[in]	labelIndex - index in outputAlphabet of label of initial node
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    double GetInitialWeight(int labelIndex, const vector<double>& factorsVector);

    /**
	 * @brief	Extracts final weights from vector
	 * @param[in]	labelIndex - index in outputAlphabet of label of last node
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    double GetFinalWeight(int labelIndex, const vector<double>& factorsVector);

    /**
	 * @brief	Extracts free factors of the clique weights
	 * @param[in]	cliqueIndex - index in weightAlphabet of clique
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    double GetDefaultWeight(int cliqueIndex, const vector<double>& factorsVector);

    /**
	 * @brief	Extracts features weight in clique
	 * @param[in]	cliqueIndex - index in weightAlphabet of clique
     * @param[in]	featureIndex - index in inputAlphabet of feature
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    double GetFeatureWeight(int cliqueIndex, int featureIndex,
        const vector<double>& factorsVector);

    /**
	 * @brief	Extracts initial weights postition from vector
	 * @param[in]	labelIndex - index in outputAlphabet of label of initial node
	 */
    int GetInitialWeightIndex(int labelIndex);

    /**
	 * @brief	Extracts final weights postition from vector
	 * @param[in]	labelIndex - index in outputAlphabet of label of last node
	 */
    int GetFinalWeightIndex(int labelIndex);

    /**
	 * @brief	Extracts free factors postition of the clique weights
	 * @param[in]	cliqueIndex - index in weightAlphabet of clique
	 */
    int GetDefaultWeightIndex(int cliqueIndex);

    /**
	 * @brief	Extracts features weight postition in clique
	 * @param[in]	cliqueIndex - index in weightAlphabet of clique
     * @param[in]	featureIndex - index in inputAlphabet of feature
	 */
    int GetFeatureWeightIndex(int cliqueIndex, int featureIndex);

    /**
	 * @brief	Returns dimension of the model
	 */
    int GetDimension();

    /**
	 * @brief	Returns all initial weights
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    shared_ptr<vector<double> > GetInitialWeights(
        const vector<double>& factorsVector);
    
    /**
	 * @brief	Returns all final weights
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    shared_ptr<vector<double> > GetFinalWeights(
        const vector<double>& factorsVector);

    /**
	 * @brief	Returns all default cliques weights
     * @param[in]	factorsVector - vector containing CRF parameters
	 */
    shared_ptr<vector<double> > GetDefaultWeights(
        const vector<double>& factorsVector);

    /**
     * @brief   Returns parameters of the clique
     * @param[in]	cliqueIndex - index of the clique
     * @param[in]	factorsVector - vector containing CRF parameters
     */
    unordered_map<int, double> GetCliqueWeights(int cliqueIndex,
        const vector<double>& factorsVector) const;

    /**
     * @brief   Verifies if feature is presentesd in clique
     * @param[in]	cliqueIndex - index of the clique
      * @param[in]	featureIndex - index of the feature
     */
    bool ContainsFeature(
        int cliqueIndex, int featureIndex) const;

private:
    int dimension; /**< total number of parameters of the model*/
    int numberOfLabels; /**< number of labels in CRF-model*/
    int numberOfFeatures; /**< number of features in CRF-model*/
    int numberOfCliques; /**< number of cliques in CRF-model*/
    vector<unordered_map<int, int> > cliqueFeatureConverter; /**< for each pair (clique, feature) finds index of its weight in vector*/
};

}

#endif

