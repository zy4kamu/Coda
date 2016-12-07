/** 
 * @file	CRFBinaryReader.h
 * @brief	header file of the CRF-reader from binary file
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


#ifndef CRFBINARYREADER
#define CRFBINARYREADER

#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "CRF.h"
#include "State.h"

using std::wifstream;
using std::ifstream;
using std::wstring;
using std::shared_ptr;
using std::vector;

namespace LinearCRF
{

/**
 * @class	CRFBinaryReader
 * @brief	Class for reading CRF-model from binary file
 */
class CRFBinaryReader
{

public:
	/**
	 * @brief	Constructor of CRFBinaryReader
	 * @param[in]	file - name of file
	 */
	CRFBinaryReader(const string& file);

	/**
	 * @brief	Destructor of CRFBinaryReader
	 */
	~CRFBinaryReader(void);

	/**
	 * @brief	Reads the file and fills content of the CRF-class
	 * @param[in]	crf - the model which parameters to be filled
	 */
	void Read(CRF& crf);

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
	void readStates(ifstream& in, CRF& crf);

	/**
	 * @brief	Reads parameters of the cliques in crf-model
	 * @param[in]	in - stream from which the model to be read
	 */
	void readWeights(ifstream& in);

	/**
	 * @brief	Substitute parameters read from file into crf-model
	 * @param[in]	crf - the model which content to be filled
	 */
	void fillContent(CRF& crf) const;
};

};

#endif // CRFBINARYREADER

