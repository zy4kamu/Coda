/**
 * @file	DisambiguatorRus.h
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


#ifndef MORPHOLOGY_DISAMBIGUATOR_RUS
#define MORPHOLOGY_DISAMBIGUATOR_RUS

#include "Disambiguator.h"

namespace Disambiguation
{

/**
 * @class	DisambiguatorRus
 * @brief	Enables to disambiguate input sentences and texts
 */
class DisambiguatorRus : public Disambiguator
{
public:
    DisambiguatorRus(
        const string& crfFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , bool isBinary
        , bool useCyrillic = true
        , bool useTransitivity = true);

    DisambiguatorRus(
        shared_ptr<CRF> model
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , bool useCyrillic = true
        , bool useTransitivity = true);

    virtual Tools::Language GetLanguage() const;

protected:
    virtual void applyPostprocessRules(
        wstring* label
        , const GrammInfo& gramInfo);

private:
    bool useCyrillic;
    bool useTransitivity;
};

}

#endif // MORPHOLOGY_DISAMBIGUATOR_RUS

