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

#ifndef DISAMBIGUATOR_SETTINGS_HOLDER
#define DISAMBIGUATOR_SETTINGS_HOLDER

#include "CRFSettingsHolder.h"
#include "IDisambiguatorSettings.h"

namespace Disambiguation
{

using LinearCRF::CRFSettingsHolder;

class DisambiguatorSettingsHolder : public CRFSettingsHolder
{
public:
    DisambiguatorSettingsHolder(
        std::shared_ptr<IDisambiguatorSettings> settings);

    shared_ptr<MorphologicalDictionary> GetDictionary();
    shared_ptr<IFeatureCalculator> GetFeatureCalculator();
    shared_ptr<DataFunctor> GetTrainingFunctor();
    shared_ptr<DataFunctor> GetTestFunctor();

    const string& GetTokenLabelSetFile() const;

    Language GetLanguage() const;

    shared_ptr<RelearnRegularization> GetRelearnRegularizator();
    const string& GetRelearnSetFile() const;
    const string& GetRelearnInputSetFile() const;

    shared_ptr<IDisambiguator> GetDisambiguator();
    shared_ptr<IDisambiguator> ReloadDisambiguator();

protected:
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<IFeatureCalculator> featureCalculator;
    shared_ptr<IDisambiguator> disambiguator;
    shared_ptr<RelearnRegularization> relearnRegularizator;
};

}

#endif // DISAMBIGUATOR_SETTINGS_HOLDER

