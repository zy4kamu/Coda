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

#ifndef SYNTAX_SETTINGS_RU
#define SYNTAX_SETTINGS_RU

#include "ChuLiuEdmondDependencySearcher.h"
#include "EdgeFeatureCalculatorRus.hpp"
#include "EisnerDependencySearcher.h"
#include "FunctionalSyntaxTreeOptimizable.hpp"
#include "GreedyDependencySearcher.h"
#include "LBFGS.h"
#include "MatrixSyntaxTreeOptimizable.hpp"
#include "NodeFeatureCalculatorRus.h"
#include "RootFeatureCalculatorRus.hpp"
#include "SimpleSyntaxTreeOptimizable.hpp"
#include "SyntaxDataPreparerRus.h"
#include "TreeTypeIdentifier.h"
#include "WeightCalculator_Exponent.hpp"

namespace SyntaxParser
{

struct SyntaxSettingsRus
{
    static const bool areFeaturesDiscrete = true;
    static const TreeOptimizableType treeType = TreeOptimizableType::FUNCTIONAL;

    typedef WeightCalculator_Exponent<true> WeightCalculator;
    typedef RootFeatureCalculatorRus<true> RootCalculator;
    typedef EdgeFeatureCalculatorRus<true, NodeFeatureCalculatorRus> EdgeCalculator;
    typedef EisnerDependencySearcher DependencySearcher;
    typedef Optimization::LBFGS Optimizer;

    static const Tools::Language language;
    static const string SYNTAX_MODEL_OUTPUT_PATH;
    static const string SYNTAX_TEST_SET_PATH;
    static const string SYNTAX_TRAINING_SET_PATH;

    static const shared_ptr<SyntaxDataPreparer> dataPreparer;

    static const bool saveToFile = false;
};

}

#endif // SYNTAX_SETTINGS_RU
