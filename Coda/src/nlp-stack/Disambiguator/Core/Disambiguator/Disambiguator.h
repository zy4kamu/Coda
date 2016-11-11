/** 
 * @file	Disambiguator.h
 * @brief	header file for instrument enabling to disambiguate input sentences and texts
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


#ifndef MORPHOLOGY_DISAMBIGUATOR
#define MORPHOLOGY_DISAMBIGUATOR

#include "CRFApplier.h"
#include "DisambiguatedData.h"
#include "IDisambiguator.h"
#include "IFeatureCalculator.h"
#include "MorphologicalDictionary.h"
#include "SyntagrusToRusCorporaConverter.h"

namespace Disambiguation
{

using TagsetConverting::SyntagrusToRusCorporaConverter;
using LinearCRF::CRF;
using LinearCRF::CRFApplier;
using LinearCRF::IChainTransformer;
using LinearCRF::IPossibleStateFinder;
using LinearCRF::Chain;

/** 
 * @class	Disambiguator
 * @brief	Enables to disambiguate input sentences and texts
 */
class Disambiguator : public CRFApplier, public IDisambiguator
{
public:
    Disambiguator(
        const string& crfFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , Tools::Language language
        , bool isCRFfileBinary);

    Disambiguator(
        shared_ptr<CRF> model
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , Tools::Language language);

    /**
     * @brief	Disambiguates input sentence
     * @param[in]	tokens - input sentence
	 * @param[out]	data - disambiguated sequence
     */
    virtual vector<DisambiguatedData> Disambiguate(
        const vector<Token>& tokens);

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
        , vector<double>* hypothesisDistibution);

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
        , vector<double>* hypothesisDistibution);

    virtual const Tools::Alphabet& OutputAlphabet() const;

    virtual ~Disambiguator(void);

    virtual Tools::Language GetLanguage() const;

    virtual vector<wstring> Lemmatize(const vector<Token>& tokens);

    virtual double GetPartitionFunction(const vector<Token>& tokens);
protected:
    virtual void applyPostprocessRules(
        wstring* label
        , shared_ptr<Morphology> gramInfo) const;

    vector<DisambiguatedData> disambiguate(
        const vector<PredisambiguatedData>& predisambiguated);

    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<IFeatureCalculator> featureCalculator;
    Tools::Language language;
private:
    shared_ptr<Morphology> getBestGrammInfo(
        const PredisambiguatedData& predisambiguatedData
        , const wstring& label);
};

}

#endif // MORPHOLOGY_DISAMBIGUATOR
