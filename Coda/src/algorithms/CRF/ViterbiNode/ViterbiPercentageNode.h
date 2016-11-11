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

#ifndef VITERBI_PERCENTAGE_NODE
#define VITERBI_PERCENTAGE_NODE

#include <cstdio>
#include <vector>
#include <set>

#include "ZViterbiNode.h"

using std::vector;
using std::set;

namespace LinearCRF
{

struct ViterbiPercentagePathItem;
struct ViterbiPercentageNode;
typedef const ViterbiPercentagePathItem* ViterbiPercentagePathPointer;

struct ViterbiPercentagePathItem
{
public:
    ViterbiPercentagePathItem();

    ViterbiPercentagePathItem(
        double bestWeight,
        ViterbiPercentageNode* currentNode,
        ViterbiPercentagePathPointer previousItem);

    friend bool operator < (const ViterbiPercentagePathItem& first,
        const ViterbiPercentagePathItem& second);

    friend bool operator > (const ViterbiPercentagePathItem& first,
        const ViterbiPercentagePathItem& second);

    double bestWeight;
    double expWeight;
    ViterbiPercentageNode* currentNode;
    ViterbiPercentagePathPointer previousItem;
};

struct ViterbiPercentageNode : public ZViterbiNode
{
public:
    static double VITERBI_PERCENTAGE_VALUE;
    static size_t MAX_NUMBER_OF_PATHS;

    ViterbiPercentageNode();

    virtual void Initialize(int layer, int stateIndex);

    virtual void Initialize(int layer, int stateIndex, double initialLogWeight);

    virtual void AddTransition(
        AbstractViterbiNode* fromNode,
        double weight);

    virtual double GetWeight();

    ~ViterbiPercentageNode(void);

set<ViterbiPercentagePathItem> pathItems;

private:
    double currentSum;
};

};

#endif // VITERBI_PERCENTAGE_NODE


