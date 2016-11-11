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

#ifndef DISAMBIGUATOR_SETTINGS_DEFAULT
#define DISAMBIGUATOR_SETTINGS_DEFAULT

#include "IDisambiguatorSettings.h"

namespace Disambiguation
{

class DefaultDisambiguatorSettings : public virtual IDisambiguatorSettings
{
public:
    DefaultDisambiguatorSettings(Tools::Language language);

    virtual shared_ptr<IChainTransformer> GetChainTransformer();
    virtual shared_ptr<IPossibleStateFinder> GetPossibleStateFinder();
    virtual shared_ptr<IRegularization> GetRegularizator();
    virtual shared_ptr<RelearnRegularization> GetRelearnRegularizator();
    virtual shared_ptr<MorphologicalDictionary> GetDictionary();

    virtual shared_ptr<DataFunctor> GetTrainingFunctor();
    virtual shared_ptr<DataFunctor> GetTestFunctor();

    virtual const string& GetIntermidiateOutputFile() const;
    virtual size_t GetMaxNumberOfIterations() const;
    virtual const string& GetModelFile() const;
    virtual size_t GetPeriodOfTesting() const;
    virtual const string& GetTrainingSetFile() const;
    virtual const string& GetTestSetFile() const;
    virtual const string& GetTokenLabelSetFile() const;
    virtual const string& GetRelearnSetFile() const;
    virtual const string& GetRelearnInputSetFile() const;

    virtual Language GetLanguage() const;
    virtual bool IsModelFileBinary() const;

    virtual shared_ptr<IDisambiguator> GetDisambiguator();
    virtual shared_ptr<IDisambiguator> ReloadDisambiguator();

protected:
    const Tools::Language language;

    shared_ptr<IChainTransformer> chainTransformer;
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<IFeatureCalculator> featureCalculator;
    shared_ptr<IPossibleStateFinder> possibleStateFinder;

    const string intermediateOutputPath;
    const string modelPath;
    const string relearnInputSetPath;
    const string relearnSetPath;
    const string testSetPath;
    const string tokenLabelSetPath;
    const string trainingSetPath;
};

}

#endif // DISAMBIGUATOR_SETTINGS_DEFAULT

