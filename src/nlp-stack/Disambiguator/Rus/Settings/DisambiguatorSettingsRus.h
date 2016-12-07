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

#ifndef DISAMBIGUATOR_SETTINGS_RUS
#define DISAMBIGUATOR_SETTINGS_RUS

#include "DefaultDisambiguatorSettings.h"
#include "OpenCorporaToSynagrusConverter.h"

using TagsetConverting::OpenCorporaToSynagrusConverter;

namespace Disambiguation
{

class DisambiguatorSettingsRus : public virtual DefaultDisambiguatorSettings
{
public:
    typedef Tools::NotModFunctor<9> TrainingFunctor;
    typedef Tools::ModFunctor<9> TestFunctor;

    DisambiguatorSettingsRus();

    virtual shared_ptr<IChainTransformer> GetChainTransformer();
    virtual shared_ptr<IPossibleStateFinder> GetPossibleStateFinder();
    virtual shared_ptr<MorphologicalDictionary> GetDictionary();
    virtual shared_ptr<IFeatureCalculator> GetFeatureCalculator();

    virtual shared_ptr<DataFunctor> GetTrainingFunctor();
    virtual shared_ptr<DataFunctor> GetTestFunctor();

    shared_ptr<IFeatureCalculator> GetSimpleFeatureCalculator();
    shared_ptr<OpenCorporaToSynagrusConverter> GetOpenCorporaToSynTagRusConverter();

    virtual shared_ptr<IDisambiguator> GetDisambiguator();
private:
    shared_ptr<OpenCorporaToSynagrusConverter> converter;
};

}

#endif // DISAMBIGUATOR_SETTINGS_RUS
