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

#ifndef VITERBI_NBEST_NODE
#define VITERBI_NBEST_NODE

#include <cstdio>
#include <vector>
#include <set>

#include "ViterbiPathItem.h"
#include "ZViterbiNode.h"

using std::vector;
using std::set;

namespace LinearCRF
{

struct ViterbiNBestNode : public ZViterbiNode
{
public:
    static size_t MAX_NUMBER_IN_ONE_NODE;

    ViterbiNBestNode();

    virtual void Initialize(int layer, int stateIndex);

    virtual void Initialize(int layer, int stateIndex, double initialLogWeight);

    virtual void AddTransition(
        AbstractViterbiNode* fromNode,
        double weight);

    virtual double GetWeight();

    ~ViterbiNBestNode(void);

set<ViterbiPathItem> pathItems;

private:
    void tryInsert(
        const ViterbiPathItem& pretendent);
};

};

#endif // VITERBI_NBEST_NODE

