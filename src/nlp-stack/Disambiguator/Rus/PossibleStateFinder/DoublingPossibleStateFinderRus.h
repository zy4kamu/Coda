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

#ifndef DOUBLING_POSSIBLE_STATE_FINDER_RUS
#define DOUBLING_POSSIBLE_STATE_FINDER_RUS

#include <string>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "IPossibleStateFinder.h"

using std::string;
using std::wstring;
using std::unordered_set;
using std::unordered_map;
using std::set;

namespace Disambiguation
{

using LinearCRF::Chain;

class DoublingPossibleStateFinderRus : public LinearCRF::IPossibleStateFinder
{
public:
    /**
     * @brief	Constructor
     * @param[in]	exceptionalTokensFile - file with exceptional tokens
     * @param[in]	tagsFile - file with all possible states
     */
    DoublingPossibleStateFinderRus(
        const string& exceptionalTokensFile
        , const string& tagsFile);

    /**
     * @brief	Finds possible states in chain
     * @param[out]	chain - chain to process
     */
    void FindPossibleStates(Chain* chain);
private:
    unordered_set<wstring> allTags; /**< contains full tags in output alphabet*/
    unordered_map<wstring, vector<wstring> > fullTagsForExceptionTokens; /**< contains translations for exceptional tokens*/

    bool evaluateExceptionalTokens(
        const wstring& currentFeature
        , vector<wstring>* currentPossibleStates) const;

    void evaluateEvenNode(
        const wstring& currentFeature, const wstring& currentToken
        , set<wstring>* currentPossibleStatesSet) const;

    void evaluateOddNode(
        const vector<wstring>& neighbourStates
        , vector<wstring>* statesToCalculate) const;

    void evaluateUnrealizedNode(int indexInChain
        , vector<wstring>* currentPossibleStates) const;

    wstring correctFeatureToLabel(const wstring& feature) const;
};

}

#endif // DOUBLING_POSSIBLE_STATE_FINDER_RUS
