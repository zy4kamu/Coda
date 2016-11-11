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

#ifndef TREE_TYPE_IDENTIFIER
#define TREE_TYPE_IDENTIFIER

#include "FunctionalSyntaxTreeOptimizable.hpp"
#include "MatrixSyntaxTreeOptimizable.hpp"
#include "SimpleSyntaxTreeOptimizable.hpp"

namespace SyntaxParser
{

enum TreeOptimizableType
{
    FULL,
    FUNCTIONAL,
    MATRIX
};

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile
    , size_t Type>
struct TreeTypeIdentifier;

/****** FULL *********/

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>
struct TreeTypeIdentifier<
    EdgeFeatureCalculator
    , EdgeWeightCalculator
    , RootFeatureCalculator
    , RootWeightCalculator
    , saveToFile
    , TreeOptimizableType::FULL>
{
    typedef SimpleSyntaxTreeOptimizable<
        EdgeFeatureCalculator
        , EdgeWeightCalculator
        , RootFeatureCalculator
        , RootWeightCalculator
        , saveToFile> TreeOptimizable;
};

/****** FUNCTIONAL *********/

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>
struct TreeTypeIdentifier<
    EdgeFeatureCalculator
    , EdgeWeightCalculator
    , RootFeatureCalculator
    , RootWeightCalculator
    , saveToFile
    , TreeOptimizableType::FUNCTIONAL>
{
    typedef FunctionalSyntaxTreeOptimizable<
        EdgeFeatureCalculator
        , EdgeWeightCalculator
        , RootFeatureCalculator
        , RootWeightCalculator
        , saveToFile> TreeOptimizable;
};

/****** MATRIX *********/

template < \
    class EdgeFeatureCalculator \
    , class EdgeWeightCalculator \
    , class RootFeatureCalculator \
    , class RootWeightCalculator \
    , bool saveToFile>
struct TreeTypeIdentifier<
    EdgeFeatureCalculator
    , EdgeWeightCalculator
    , RootFeatureCalculator
    , RootWeightCalculator
    , saveToFile
    , TreeOptimizableType::MATRIX>
{
    typedef MatrixSyntaxTreeOptimizable<
        EdgeFeatureCalculator
        , EdgeWeightCalculator
        , RootFeatureCalculator
        , RootWeightCalculator
        , saveToFile> TreeOptimizable;
};



}

#endif // TREE_TYPE_IDENTIFIER
