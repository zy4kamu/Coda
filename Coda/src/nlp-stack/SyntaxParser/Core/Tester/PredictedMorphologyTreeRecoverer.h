/**
 * @file	PredictedMorphologyTreeRecoverer.h
 * @brief	header file for class that predicts syntax tree NOT using labels contained in its nodes 
 *          (they should be calculated in tree connectionBuilder)
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


#ifndef DISAMBIGUATOR_TREE_PREDICTOR
#define DISAMBIGUATOR_TREE_PREDICTOR

#include "IDisambiguator.h"
#include "SyntaxTree.h"
#include <memory>

namespace SyntaxParser
{

using std::shared_ptr;
using Disambiguation::IDisambiguator;
using Disambiguation::DisambiguatedData;

class PredictedMorphologyTreeRecoverer
{
public:
    PredictedMorphologyTreeRecoverer(Tools::Language language);

    /**
     * @brief	First transfers real tree to string and then parses it
     * @param[in]	realTree - syntax tree to process
	 * @param[out]	predictedSyntaxTree - recovered tree
     */
    SyntaxTree PredictTree(const SyntaxTree& realTree);

    vector<DisambiguatedData> GetMorphology(const SyntaxTree& realTree);
private:
    shared_ptr<IDisambiguator> disambiguator;
};

}

#endif // DISAMBIGUATOR_TREE_PREDICTOR
