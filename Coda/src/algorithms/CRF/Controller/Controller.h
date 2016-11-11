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

#ifndef CRF_CONTROLLER
#define CRF_CONTROLLER

#include "CRFApplier.h"
#include "CRFSettingsHolder.h"
#include "ICRFSettings.h"

namespace LinearCRF
{

class Controller
{
public:
   Controller(shared_ptr<ICRFSettings> settings);

   void Learn();

   void Test();

   void Truncate(
        const string& inputCRFfile
        , const string& outputCRFfile
        , double truncationLevel);

   void TranslateTextModelToBinary(
        const string& fileText
        , const string& fileBin);

   void TranslateBinaryModelToText(
        const string& fileBin
        , const string& fileText);

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

private:
   CRFSettingsHolder settingsHolder;
   CRFApplier applier;
};

}

#endif // CRF_CONTROLLER
