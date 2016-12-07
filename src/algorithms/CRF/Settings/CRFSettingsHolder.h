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

#ifndef CRF_SETTINGS_HOLDER
#define CRF_SETTINGS_HOLDER

#include "CRF.h"
#include "ICRFSettings.h"

namespace LinearCRF
{

class CRFSettingsHolder
{
public:
    CRFSettingsHolder(
        std::shared_ptr<ICRFSettings> settings);

    shared_ptr<IChainTransformer> GetChainTransformer();
    shared_ptr<CRF> GetModel();
    shared_ptr<IPossibleStateFinder> GetPossibleStateFinder();
    shared_ptr<IRegularization> GetRegularizator();

    const string& GetIntermidiateOutputFile() const;
    const string& GetModelFile() const;
    const string& GetTrainingSetFile() const;
    const string& GetTestSetFile() const;

    bool IsModelFileBinary() const;
    size_t GetMaxNumberOfIterations() const;
    size_t GetPeriodOfTesting() const;

    void ReloadModel();

protected:
    shared_ptr<IChainTransformer> chainTransformer;
    shared_ptr<CRF> model;
    shared_ptr<IPossibleStateFinder> possibleStateFinder;
    shared_ptr<IRegularization> regularizator;
    shared_ptr<DataFunctor> trainingFunctor;
    shared_ptr<DataFunctor> testFunctor;

    std::shared_ptr<ICRFSettings> settings;
};

}

#endif // CRF_SETTINGS_HOLDER


