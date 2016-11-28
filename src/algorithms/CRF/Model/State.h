/** 
 * @file	States.h
 * @brief	header file of the class containing state of the CRF-model
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


#ifndef STATE
#define STATE

#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::wstring;
using std::shared_ptr;

namespace LinearCRF
{

class CRF;

/** 
 * @class	States
 * @brief	Represents state of the CRF-model
 */
struct State
{
    /**
     * @brief	Constructor. Empty constructor.
     * @param[in]	_crf - pointer to the CRF-model containg this state
     */
    State(CRF* _crf);

	/**
     * @brief	Constructor. Fills CRF-state content
     * @param[in]	_name - name of the state (must belong to outputAlphabet in CRF-model)
     * @param[in]	_index - index of the name in the outputAlphabet
	 * @param[in]	_destinations - indexes of the states available from the state
	 * @param[in]	_weightsIndexes - indexes of cliques corresponding current state. _weightsIndexes->at(i) contains indexes of cliques coressponding i-th transition from current state
	 * @param[in]	_destinationNames - names of the states available from the state
	 * @param[in]	_crf - pointer to the CRF-model containg this state
     */
	State(const wstring& _name, int _index, 
		  shared_ptr<vector<int>> _destinations, 
		  shared_ptr<vector<vector<int>>> _weightsIndexes,
		  shared_ptr<vector<wstring>> _destinationNames, CRF* _crf);

	/**
     * @brief	Destructor
     */
	~State(void);
	
	/**
     * @brief	Returns the number of destinations available from the state
     */
	int GetNumDestinations() const;

	/**
     * @brief	Returns the index if the state
     */
    int GetIndex() const;

	/**
     * @brief	Returns the name of the state
     */
	wstring GetName() const;

	/**
     * @brief	Returns the indexes of the destinations available from the current state
     */
	const vector<int>& GetDestinations() const;

	/**
     * @brief	Returns the indexes of cliques corresponding current state. i-th component contains indexes of cliques coressponding i-th transition from current state
     */
	const vector<vector<int>>& GetWeightsIndexes() const;

	/**
     * @brief	Sets weightsIndexes
	 * @param[in]	data - weightIndexes to set
     */
	void SetWeightIndexes(shared_ptr<vector<vector<int>>> data);

	CRF* crf; /**< pointer to the CRF model to which the state belongs*/
	int index; /**< index in the state in the CRF outputAlphabet*/
	wstring name; /**< name of the state*/
	shared_ptr<vector<int>> destinations; /**< indexes of the states available from the state*/
	shared_ptr<vector<vector<int>>> weightsIndexes; /**< indexes of cliques corresponding current state. weightsIndexes->at(i) contains indexes of cliques coressponding i-th transition from current state*/
	shared_ptr<vector<wstring>> destinationNames; /**<  names of the states available from the state*/
};

};

#endif // STATE

