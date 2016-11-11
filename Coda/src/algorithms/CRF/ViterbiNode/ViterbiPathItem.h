/** 
 * @file	ViterbiPathItem.h
 * @brief	Path item contained in viterbi node
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


#ifndef VITERBI_PATH_ITEM
#define VITERBI_PATH_ITEM

#include <set>
using std::set;

namespace LinearCRF
{

struct ViterbiNBestNode;
struct ViterbiPathItem;

typedef const ViterbiPathItem* ViterbiPathPointer;

/** 
 * @struct	ViterbiPathItem
 * @brief	Path item contained in viterbi node
 */
struct ViterbiPathItem
{
public:
    /**
     * @brief	Constructor
     * @param[in]	bestWeight - best weight of path
     * @param[in]	bestPredecessor - best predecessor
     * @param[in]	indexInPredecessor - index of path in best predecessor
     */
    ViterbiPathItem(
        double bestWeight, 
        ViterbiNBestNode* currentNode, 
        ViterbiPathPointer previousItem)
        : bestWeight(bestWeight)
        , currentNode(currentNode)
        , previousItem(previousItem)
    {
    }

    double bestWeight; /**< best weight of path*/
    ViterbiNBestNode* currentNode; /**< node containing this item*/
    ViterbiPathPointer previousItem; /**< previous item in the whole path*/

    friend bool operator < (const ViterbiPathItem& first,
        const ViterbiPathItem& second)
    {
        return first.bestWeight < second.bestWeight;
    }
};

};


#endif // VITERBI_PATH_ITEM
