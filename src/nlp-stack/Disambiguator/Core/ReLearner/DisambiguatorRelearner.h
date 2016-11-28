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

#ifndef DISAMBIGUATOR_RELEARNER
#define DISAMBIGUATOR_RELEARNER

#include "ChainCollectionReLearner.h"
#include "Controller.h"
#include "DisambiguatorSettingsHolder.h"
#include "ITokenizer.h"

namespace Disambiguation
{

using Tokenization::ITokenizer;

typedef LinearCRF::ChainCollectionLearner <
    Optimization::LBFGS,
    LinearCRF::ChainCollectionTester,
    LinearCRF::DONT_USE_UNPRESENTED_IN_CLIQUES_FEATURES
> Learner;

typedef LinearCRF::ChainCollectionReLearner<Learner> ReLearner;

class DisambiguatorRelearner
{
public:
    DisambiguatorRelearner(
        shared_ptr<DisambiguatorSettingsHolder> settings
        , shared_ptr<ITokenizer> tokenizer);

    void RelearnDialog();

    void RelearnFromFile(const string& fileToFit);
private:
    void relearnFromFileWithFeatures(
        const string& fileToFit);

    bool fillFileOrTrainOneDialog();

    void printDisambiguatedDataToConsole(
        const vector<DisambiguatedData>& disambiguated);

    void printRelearnDataToFileForCorrections(
        const vector<Token>& tokens
        , const vector<PredisambiguatedData>& predisambiguated
        , const vector<DisambiguatedData>& disambiguated);

    bool printRelearnDataToFillFile(
        const vector<Token>& tokens
        , const vector<DisambiguatedData>& disambiguated);

    bool runCorrectionDialog();

    void correctFillFile();

    bool relearnDialog();

    bool relearnDialogFromFile();

    bool moveRelearnedModel();

    void testCorrectedModelDialog();

    bool useRelearnedModelDialog();

    bool readRelearnSentence(wifstream& in, wstring* sentence, vector<wstring>* labels);

    shared_ptr<DisambiguatorSettingsHolder> settings;
    Language language;
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<IFeatureCalculator> featureCalculator;
    shared_ptr<ITokenizer> tokenizer;
    shared_ptr<IDisambiguator> disambiguator;
};

}

#endif // DISAMBIGUATOR_RELEARNER
