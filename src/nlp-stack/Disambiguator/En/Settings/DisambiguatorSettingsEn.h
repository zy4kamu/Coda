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

#ifndef DISAMBIGUATOR_SETTINGS_EN
#define DISAMBIGUATOR_SETTINGS_EN

#include "IDisambiguatorSettings.h"

namespace Disambiguation
{

class DisambiguatorSettingsEn : public IDisambiguatorSettings
{
public:
    typedef Tools::LessFunctor<38220> TrainingFunctor;
    typedef Tools::MoreFunctor<43714> TestFunctor;

    DisambiguatorSettingsEn();

    virtual shared_ptr<IChainTransformer> GetChainTransformer();
    virtual shared_ptr<IPossibleStateFinder> GetPossibleStateFinder();
    virtual shared_ptr<IRegularization> GetRegularizator();
    virtual shared_ptr<RelearnRegularization> GetRelearnRegularizator();
    virtual shared_ptr<MorphologicalDictionary> GetDictionary();
    virtual shared_ptr<IFeatureCalculator> GetFeatureCalculator();

    virtual shared_ptr<DataFunctor> GetTrainingFunctor();
    virtual shared_ptr<DataFunctor> GetTestFunctor();

    virtual const string& GetIntermidiateOutputFile() const {return intermidiateOutputFile;}
    virtual size_t GetMaxNumberOfIterations() const {return maxNumberOfIterations;}
    virtual const string& GetModelFile() const {return modelFile;}
    virtual size_t GetPeriodOfTesting() const {return periodOfTesting;}
    virtual const string& GetTrainingSetFile() const {return trainingSetFile;}
    virtual const string& GetTestSetFile() const {return testSetFile;}
    virtual const string& GetTokenLabelSetFile() const {return tokenLabelSetFile;}
    virtual const string& GetRelearnSetFile() const {return relearnSetFile;}
    virtual const string& GetRelearnInputSetFile() const {return relearnInputSetFile;}
    virtual bool IsModelFileBinary() const {return isModelFileBinary;}

    virtual Language GetLanguage() const {return language;}

    virtual shared_ptr<IDisambiguator> GetDisambiguator();
    virtual shared_ptr<IDisambiguator> ReloadDisambiguator();

private:
    shared_ptr<IChainTransformer> chainTransformer;
    shared_ptr<IPossibleStateFinder> possibleStateFinder;
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<IFeatureCalculator> featureCalculator;

    static const string intermidiateOutputFile;
    static const size_t maxNumberOfIterations;
    static const string modelFile;
    static const size_t periodOfTesting;
    static const string trainingSetFile;
    static const string testSetFile;
    static const string tokenLabelSetFile;
    static const string relearnSetFile;
    static const string relearnInputSetFile;
    static const bool isModelFileBinary;

    static const string tagsFile;
    static const string tokenFrequencyFile;

    Language language;

 };

}


#endif // DISAMBIGUATOR_SETTINGS_EN
