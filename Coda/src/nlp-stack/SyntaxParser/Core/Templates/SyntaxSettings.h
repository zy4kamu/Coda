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

#ifndef SYNTAX_SETTINGS
#define SYNTAX_SETTINGS

#include "BatchTypeIdentifier.h"
#include "PerfectMorphologyTreeRecoverer.h"
#include "PredictedMorphologyTreeRecoverer.h"
#include "SyntaxDataPreparer.h"
#include "SyntaxLearner.hpp"
#include "SyntaxParser.hpp"
#include "SyntaxTester.hpp"
#include "TreeTypeIdentifier.h"

namespace SyntaxParser
{

template<class Settings>
struct SyntaxSettings
{
    typedef Settings SettingsType;

    typedef typename Settings::WeightCalculator WeightCalculator;
    typedef typename Settings::RootCalculator RootCalculator;
    typedef typename Settings::EdgeCalculator EdgeCalculator;


    typedef typename BatchTypeIdentifier<Settings::areFeaturesDiscrete>::StringBatchType StringBatch;
    typedef typename BatchTypeIdentifier<Settings::areFeaturesDiscrete>::IntBatchType IntBatch;

    typedef SyntaxParser<
        EdgeCalculator
        , RootCalculator
        , typename Settings::DependencySearcher
        , WeightCalculator
        , StringBatch
        , IntBatch> Parser;

    typedef typename TreeTypeIdentifier<
        EdgeCalculator
        , WeightCalculator
        , RootCalculator
        , WeightCalculator
        , Settings::saveToFile
        , Settings::treeType>::TreeOptimizable TreeOptimizable;

    typedef SyntaxTester<
        Parser
        , PerfectMorphologyTreeRecoverer> Tester;

    typedef SyntaxTester<
        Parser
        , PerfectMorphologyTreeRecoverer> TesterOnPerfectMorphology;

    typedef SyntaxTester<
        Parser
        , PredictedMorphologyTreeRecoverer> TesterOnPredictedMorphology;

    typedef SyntaxLearner<
        TreeOptimizable
        , typename Settings::Optimizer
        , Tester> Learner;

    static Tools::Language GetLanguage()
    {
        return Settings::language;
    }

    static const string& GetModelPath()
    {
        return Settings::SYNTAX_MODEL_OUTPUT_PATH;
    }

    static const string& GetTestSetPath()
    {
        return Settings::SYNTAX_TEST_SET_PATH;
    }

    static const string& GetTrainingSetPath()
    {
        return Settings::SYNTAX_TRAINING_SET_PATH;
    }
};

}

#endif // SYNTAX_SETTINGS
