/** 
 * @file	ViterbiNode.h
 * @brief	header file for structure representing Viterbi node (used in Viterbi algorithm for CRF-model)
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

#ifndef VITERBI_NODE
#define VITERBI_NODE

#include "AbstractViterbiNode.h"

namespace LinearCRF
{

/** 
 * @struct	ViterbiNode
 * @brief	Represents Viterbi node (used in Viterbi algorithm for CRF-model)
 */
struct ViterbiNode : public AbstractViterbiNode
{
public:
    ViterbiNode();

    virtual void Initialize(int layer, int stateIndex);

    virtual void Initialize(int layer, int stateIndex, double initialLogWeight);

    /**
     * @brief	Destructor
     */
    ~ViterbiNode(void);

    virtual void AddTransition(
        AbstractViterbiNode* fromNode,
        double weight);

    virtual double GetWeight();
    
    double weight; /**< value of the node*/
    ViterbiNode* bestPredecessor; /**< best predecessor (used in order to determine the best sequence)*/
};

};

#endif // VITERBI_NODE

