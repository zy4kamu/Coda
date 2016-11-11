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

#ifndef CRF_APPLIER
#define CRF_APPLIER

#include "CRF.h"
#include "IChainTransformer.h"
#include "IPossibleStateFinder.h"

namespace LinearCRF
{

class CRFApplier
{
public:
    CRFApplier(
        const string& crfFile
       , shared_ptr<IChainTransformer> chainTransformer
       , shared_ptr<IPossibleStateFinder> possibleStateFinder
       , bool isCRFfileBinary);

    CRFApplier(
        shared_ptr<CRF> crf
       , shared_ptr<IChainTransformer> chainTransformer
       , shared_ptr<IPossibleStateFinder> possibleStateFinder);

    void Apply(Chain& chain
        , vector<wstring>* bestSequence
        , vector<double>* bestSequenceWeights);

    void Apply(Chain& chain
        , size_t numberOfHypothesis
        , vector<vector<wstring> >* bestSequences
        , vector<vector<double> >* bestSequenceWeights
        , vector<double>* hypothesisDistribution);

    void Apply(Chain& chain
        , double percentage
        , size_t maxNumberOfPaths
        , vector<vector<wstring> >* bestSequences
        , vector<vector<double> >* bestSequenceWeights
        , vector<double>* hypothesisDistribution);

    double GetPartFunction(Chain& chain);
protected:
    vector<vector<int> > getFeatureIndexes(
        const Chain& chain);

    vector<int> getLabelIndexes(
        const Chain& chain);

    vector<vector<int> > getPossibleStates(
        const Chain& chain);

    shared_ptr<IChainTransformer> chainTransformer;
    shared_ptr<IPossibleStateFinder> possibleStateFinder;
    shared_ptr<CRF> crf;
    shared_ptr<Alphabet> inputAlphabet;
    shared_ptr<Alphabet> outputAlphabet;
};

}

#endif // CRF_APPLIER
