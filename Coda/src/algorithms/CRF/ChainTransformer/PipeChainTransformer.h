/**
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

#ifndef PIPE_CHAIN_TRANSFORMER
#define PIPE_CHAIN_TRANSFORMER

/** 
 * @file	PipeChainTransformer.h
 * @brief	header file for chain transformer that 
 *          concatenates transformesr into one pipe
 */

#include <memory>

#include "IChainTransformer.h"

using std::shared_ptr;

namespace LinearCRF
{


/** 
 * @class	PipeChainTransformer
 * @brief	concatenates transformesr into one pipe
 */
template <class TransformerIterator = vector<shared_ptr<IChainTransformer> >::iterator>
class PipeChainTransformer : public IChainTransformer
{
public:
    /**
	 * @brief	Constuctor
	 * @param[in]	begin - iterator to the first transformer
     * @param[out]	end - iterator to the element after last transformer
	 */
    PipeChainTransformer(TransformerIterator begin, TransformerIterator end);

    /**
	 * @brief	Implements transformation function
	 * @param[in]	chain -  chain to convert
	 */
    Chain ForwardTransform(Chain& chain) const;

    /**
	 * @brief	Implements backward transformation function
	 * @param[in]	transformedPredictedStates - predicted states for transformed chain
     * @param[out]	result - predicted states for initial chain
	 */
    vector<int> BackwardTransform(
        const vector<int>& transformedPredictedStates) const;

    /**
     * @brief	Abstract method: should transfrom predicted weights of transformed chain
     *          into predicted weights of initial chain
     * @param[in]	transformedPredictedStates - predicted states for transformed chain
     * @param[out]	result - predicted weights for initial chain
     */
    vector<double> BackwardTransform(
        const vector<double>& transformedPredictedStates) const;
private:
    vector<shared_ptr<IChainTransformer> > transformers;
};

template <class TransformerIterator>
PipeChainTransformer<TransformerIterator>::PipeChainTransformer(
    TransformerIterator begin, TransformerIterator end) 
    : transformers(begin, end)
{
}

template <class TransformerIterator>
Chain PipeChainTransformer<TransformerIterator>::ForwardTransform (
    Chain& chain) const
{
    Chain transformed = std::move(chain);
    for (auto iter = transformers.begin()
        ; iter != transformers.end(); ++iter)
    {
        transformed = (*iter)->ForwardTransform(transformed);
    }
    return transformed;
}

template <class TransformerIterator>
vector<int> PipeChainTransformer<TransformerIterator>::BackwardTransform(
        const vector<int>& transformedPredictedStates) const
{
    vector<int> predictedStates = transformedPredictedStates;
    for (auto iter = transformers.rbegin()
        ; iter != transformers.rend(); ++iter)
    {
        predictedStates = (*iter)->BackwardTransform(predictedStates);
    }
    return predictedStates;
}

template <class TransformerIterator>
vector<double> PipeChainTransformer<TransformerIterator>::BackwardTransform(
        const vector<double>& transformedPredictedStates) const
{
    vector<double> predictedStates = transformedPredictedStates;
    for (auto iter = transformers.rbegin()
        ; iter != transformers.rend(); ++iter)
    {
        predictedStates = (*iter)->BackwardTransform(predictedStates);
    }
    return predictedStates;
}

};

#endif // PIPE_CHAIN_TRANSFORMER
