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

#ifndef NODE_FEATURE_CALCULATOR_EN
#define NODE_FEATURE_CALCULATOR_EN

#include <set>
#include <memory>

#include "SyntaxTree.h"

using std::unordered_set;
using std::shared_ptr;

namespace SyntaxParser
{

class NodeFeatureCalculatorEn
{
public:
    NodeFeatureCalculatorEn();

    vector<vector<wstring> > Calculate(const SyntaxTree& tree);
private:

    vector<vector<wstring> > calculateOneTokenFeatures(
        const vector<SyntaxNode>& nodes);

    void applyConjunctions(
        vector<vector<wstring> >* features
        , int conjunctions);

    shared_ptr<unordered_set<wstring> > exceptionTokens;
};

}

#endif // NODE_FEATURE_CALCULATOR_EN


