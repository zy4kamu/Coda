/** 
 * @file	CRFTextReader.h
 * @brief	header file of the CRF-reader in text format
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


#ifndef CRFTEXTREADER
#define CRFTEXTREADER

#include "CRF.h"
#include "State.h"
#include "Tools.h"

using std::locale;

namespace LinearCRF
{

class CRF;
class State;

/** 
 * @class	CRFTextReader
 * @brief	CRF-reader in the text format
 */
class CRFTextReader
{
public:
	/**
	 * @brief	Constructor of CRFTextReader
	 * @param[in]	file - name of file
	 */
	CRFTextReader(const string& file);

	/**
	 * @brief	Destructor of CRFTextReader
	 */
	~CRFTextReader(void);

	/**
	 * @brief	Reads the file and fills content of the CRF-class
	 * @param[in]	crf - the model which parameters to be filled
	 */
	void Read(CRF& crf) const;

private:
	string file;  /**<name of file to download*/
	shared_ptr<Alphabet> inputAlphabet; /**<input alphabet (feature's names) of CRF-model*/
	shared_ptr<Alphabet> outputAlphabet; /**<output alphabet (names of hidden states) of CRF-model*/
	shared_ptr<Alphabet> weightAlphabet; /**<weight alphabet (names of cliques) of CRF-model*/
    shared_ptr<vector<double> > initialWeights; /**<initial weights of CRF-model (log-probability to start from corrsponding state)*/
    shared_ptr<vector<double> > finalWeights; /**<final weights of CRF-model (log-probability to finish at corrsponding state)*/
    shared_ptr<vector<double> > defaultWeights; /**<default weights of the cliques in CRF-model*/
    shared_ptr<vector<State> > states; /**<states of CRF-model*/
    shared_ptr<vector<unordered_map<int, double> > > weights; /**<parameters of cliques (weights) in CRF-model*/
	wifstream in; /**<stream from where the model is read*/

	/**
	 * @brief	Reads states of CRF-model and fills the corresponding fields
	 * @param[in]	in - stream from which the model to be read
	 * @param[in]	crf - the model which content to be filled
	 */
	void readStates(wifstream& in, CRF& crf) const;

	/**
	 * @brief	Reads parameters of the cliques in crf-model
	 * @param[in]	in - stream from which the model to be read
	 */
	void readWeights(wifstream& in) const;

	/**
	 * @brief	Substitute parameters read from file into crf-model
	 * @param[in]	crf - the model which content to be filled
	 */
	void fillContent(CRF& crf) const;
};

}

#endif // CRFTEXTREADER

