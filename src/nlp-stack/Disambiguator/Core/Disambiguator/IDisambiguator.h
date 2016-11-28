/** 
 * @file	DisambiguatorInterface.h
 * @brief	header file for an interface of the instrument enabling to disambiguate input sentences and texts
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


#ifndef DISAMBIGUATOR_INTERFACE
#define DISAMBIGUATOR_INTERFACE

#include <vector>
#include <string>
#include <memory>

#include "DisambiguatedData.h"
#include "Token.h"
#include "Tools.h"

using std::vector;
using std::wstring;
using std::string;
using std::shared_ptr;

using Tokenization::Token;

namespace Disambiguation
{

/** 
 * @class	DisambiguatorInterface
 * @brief	Interface of the instrument enabling to disambiguate input sentences and texts
 */
class IDisambiguator
{
public:
    static shared_ptr<IDisambiguator> GetDisambiguator(Tools::Language language);

	/**
	 * @brief destructor of interface
	 */
    virtual ~IDisambiguator() {};

    /**
     * @brief	Disambiguates input sentence
     * @param[in]	tokens - input sentence
	 * @param[out]	data - disambiguated sequence
     */
    virtual vector<DisambiguatedData> Disambiguate(
        const vector<Token>& tokens) = 0;

    /**
     * @brief	Disambiguates input sentence
     * @param[in]	tokens - input sentence
     * @param[in]	numberOfHypothesis - number of hypothesis
     * @param[out]	hypothesisDistibution - distributions by hypothesis
     * @param[out]	data - disambiguated sequence
     */
    virtual vector<vector<DisambiguatedData> > Disambiguate(
        const vector<Token>& tokens
        , int numberOfHypothesis
        , vector<double>* hypothesisDistibution) = 0;

    /**
     * @brief	Disambiguates input sentence
     * @param[in]	tokens - input sentence
     * @param[in]	percentage - sum of probabilities of paths must exceed this value
     * @param[out]	hypothesisDistibution - distributions by hypothesis
     * @param[out]	data - disambiguated sequence
     */
    virtual vector<vector<DisambiguatedData> > Disambiguate(
        const vector<Token>& tokens
        , double percentage
        , size_t maxNumberOfPaths
        , vector<double>* hypothesisDistibution) = 0;

    virtual vector<wstring> Lemmatize(const vector<Token>& tokens) = 0;

    virtual const Tools::Alphabet& OutputAlphabet() const = 0;

    virtual Tools::Language GetLanguage() const = 0;

    virtual double GetPartitionFunction(const vector<Token>& tokens) = 0;
};

}

#endif // DISAMBIGUATOR_INTERFACE
