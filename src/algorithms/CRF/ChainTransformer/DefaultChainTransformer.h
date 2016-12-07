/** 
 * @file	DefaultChainTransformer.h
 * @brief	header file of the class that enables to transform one chain to another chain
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


#ifndef DEFAULT_CHAIN_TRANSFORMER
#define DEFAULT_CHAIN_TRANSFORMER

#include "IChainTransformer.h"

namespace LinearCRF
{

/** 
 * @class	DefaultChainTransformer
 * @brief	Simple pype. Just returns the same chain
 */
class DefaultChainTransformer : public IChainTransformer
{
public:
    /**
	 * @brief	Constructor
	 */
    DefaultChainTransformer() {}

    /**
	 * @brief	Destructor
	 */
    ~DefaultChainTransformer() {}

    /**
	 * @brief	Just returns the same ChainTransformer. Attention 'move semantic' is used. 
     *          Input chain is not valid anymore.
	 * @param[in]	chain - input chain
     * @param[out]	result - transformed chain
	 */
    Chain ForwardTransform(Chain& chain) const
    {
        Chain copy = std::move(chain);
        return copy;
    }

    /**
	 * @brief	Implements backward transformation function
	 * @param[in]	transformedPredictedStates - predicted states for transformed chain
     * @param[out]	result - predicted states for initial chain
	 */
    vector<int> BackwardTransform(
        const vector<int>& transformedPredictedStates) const
    {
        return transformedPredictedStates;
    }

    /**
     * @brief	Abstract method: should transfrom predicted weights of transformed chain
     *          into predicted weights of initial chain
     * @param[in]	transformedPredictedStates - predicted states for transformed chain
     * @param[out]	result - predicted weights for initial chain
     */
    vector<double> BackwardTransform(
        const vector<double>& transformedPredictedStates) const
    {
        return transformedPredictedStates;
    }
};

};

#endif // DEFAULT_CHAIN_TRANSFORMER
