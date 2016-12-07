/** 
 * @file	Chain.h
 * @brief	header file of the class that represents data for CRF-learning (one sentence) 
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


#ifndef CHAIN
#define CHAIN

#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::wstring;
using std::wifstream;

namespace LinearCRF
{

/** 
 * @class	Chain
 * @brief	Describes one sentence for CRF-learning
 */
class Chain
{
public:
    
    /**
	 * @brief	Empty constructor
	 */
    Chain();

    /**
	 * @brief	Copy constructor
	 */
    Chain(const Chain& chain);

    /**
	 * @brief	R-value copy constructor
	 */
    Chain(const Chain&& chain);

    /**
	 * @brief	Reads chain from file
	 * @param[in]	stream - stream from which we read data
	 */
    Chain(wifstream& stream);

    /**
	 * @brief	Constructor
     * @param[in]	tokens - tokens
	 * @param[in]	features - features
     * @param[in]	labels - labels
     * @param[in]	possibleStates - possible states
	 */
    Chain(vector<wstring>& tokens
        , vector<vector<wstring> >& features 
        , vector<wstring>& labels
        , vector<vector<wstring> >& possibleStates);

     /**
	 * @brief	Constructor
     * @param[in]	tokens - tokens
	 * @param[in]	features - features
     * @param[in]	labels - labels
     * @param[in]	possibleStates - possible states
	 */
    Chain(vector<wstring>&& tokens
        , vector<vector<wstring> >&& features 
        , vector<wstring>&& labels
        , vector<vector<wstring> >&& possibleStates);

    /**
	 * @brief	Destructor
	 */
    ~Chain(void);

    /**
     * @brief   Returns reference to labels
     */
    const vector<wstring>& GetTokens() const;

    /**
	 * @brief   Returns reference to labels
	 */
    const vector<wstring>& GetLabels() const;

    /**
	 * @brief   Returns reference to features
	 */
    const vector<vector<wstring> >& GetFeatures() const;

    /**
	 * @brief   Returns possible states
	 */
    const vector<vector<wstring> >& GetPossibleStates() const;

    /**
	 * @brief	Enables to add new features
	 * @param[in]	nodeIndex - index of the node where we want to add feature
     * @param[in]	feature - name of the feature that we add
	 */
    void AddFeature(int nodeIndex, const wstring& feature);

    /**
	 * @brief   Returns size of the chain
	 */
    const size_t GetSize() const;

    /**
	 * @brief	Reads chain from file
	 * @param[in]	stream - stream from which we read data
     * @param[in]	chain - chain to read
	 */
    friend wifstream& operator >>(wifstream& stream, Chain& chain);

    /**
	 * @brief	Copy assigment
     * @param[in]	chain - chain to copy
	 */
    Chain& operator = (const Chain& chain);

    /**
	 * @brief	Move assigment
     * @param[in]	chain - chain to move
	 */
    Chain& operator = (Chain&& chain);

    /**
	 * @brief	Sets possible states
     * @param[in]	possibleStates - possible states to copy
	 */
    void SetPossibleStates(
        const vector<vector<wstring> >& possibleStates);

    /**
	 * @brief	Sets possible states
     * @param[in]	possibleStates - possible states to move
	 */
    void SetPossibleStates(
        vector<vector<wstring> >&& possibleStates);
private:
    vector<wstring> labels; /**< contains hidden labels for each node of the chain (should be defined only during training)*/
    vector<vector<wstring> > features; /**< contains observable features for each node of the chain*/
    vector<wstring> tokens; /**< contains tokens corresponding to each node of the chain*/
    vector<vector<wstring> > possibleStates; /**< contains possible labels for each node of the chain*/
};

};

#endif // CHAIN

