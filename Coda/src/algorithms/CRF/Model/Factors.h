/** 
 * @file	Factors.h
 * @brief	header file of the class containing general settings of the CRF-model
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


#ifndef FACTORS
#define FACTORS

#include "Tools.h"

namespace LinearCRF
{

using Tools::Alphabet;

/** 
 * @class	Factors
 * @brief	contains general settings of the CRF-model
 */
class Factors
{
public:
	/**
     * @brief	Constructor. Initalizing shared_ptr on weights
     */
	Factors(void);

	/**
     * @brief	Destructor
     */
	~Factors(void);

	/**
	 * @brief	Sets weight alphabet
	 * @param[in]	_weightAlphabet - weight alphabet
	 */
	void SetWeightAlphabet(shared_ptr<Alphabet> _weightAlphabet);

	/**
	 * @brief	Sets default weights (free factors of the clique weight)
	 * @param[in]	_defaultWeights - default weights
	 */
    void SetDefaultWeights(shared_ptr<vector<double> > _defaultWeights);

	/**
	 * @brief	Sets initial weights (weight of the initial state)
	 * @param[in]	_initialWeights - initial weights
	 */
    void SetInitialWeights(shared_ptr<vector<double> > _initialWeights);

	/**
	 * @brief	Sets final weights (weight of the last state)
	 * @param[in]	_finalWeights - final weights
	 */
    void SetFinalWeights(shared_ptr<vector<double> > _finalWeights);

	/**
	 * @brief	Sets weights (main parameters) of the cliques
	 * @param[in]	_weights - parameters of the cliques
	 */
    void SetWeights(shared_ptr<vector<unordered_map<int, double> > > _weights);

	/**
	 * @brief	Returns weight alphabet
	 */
	const Alphabet& GetWeightAlphabet() const;

    /**
	 * @brief	Returns weight alphabet
	 */
	shared_ptr<const Alphabet> GetWeightAlphabetPtr() const;

    /**
     * @brief	Returns weight alphabet
     */
    shared_ptr<Alphabet> GetWeightAlphabetPtr();

	/**
	 * @brief	Returns weights of the cliques
	 */
    const vector<unordered_map<int, double> >& GetWeights() const;

	/**
	 * @brief	Returns default weights (free factors of the clique weight)
	 */
	const vector<double>& GetDefaultWeights() const;

	/**
	 * @brief	Returns initial weights (weights of the initial states)
	 */
	const vector<double>& GetInitialWeights() const;

    /**
     * @brief	Returns final weights (weights of the final states)
     */
    const vector<double>& GetFinalWeights() const;

	/**
	 * @brief	Returns final weights (weights of the final states)
	 */
    shared_ptr<vector<double> > GetFinalWeightsPtr();

    /**
     * @brief	Returns weights of the cliques
     */
    shared_ptr<vector<unordered_map<int, double> > > GetWeightsPtr();

    /**
     * @brief	Returns default weights (free factors of the clique weight)
     */
    shared_ptr<vector<double> > GetDefaultWeightsPtr();

    /**
     * @brief	Returns initial weights (weights of the initial states)
     */
    shared_ptr<vector<double> > GetInitialWeightsPtr();

     /**
     * @brief	Removes all features with weight smaller than trancationLevel
     * @param[in]	truncationLevel - Removes all features with weight smaller than trancationLevel
     */
    void Truncate(double truncationLevel);
private:
	shared_ptr<Alphabet> weightAlphabet; /**< quick converter between names of the cliques and their indexes*/
    shared_ptr<vector<unordered_map<int, double> > > weights; /**< parameters of the cliques; weights->at(i) contains coefficients of the features in the i-th clique*/
    shared_ptr<vector<double> > defaultWeights;  /**< free factors of the weights of the cliques*/
    shared_ptr<vector<double> > initialWeights;  /**< initial weights of the states*/
    shared_ptr<vector<double> > finalWeights; /**< final weights of the states*/
};

};

#endif // FACTORS

