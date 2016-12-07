/** 
 * @file	EisnerDependencySearcher.h
 * @brief	header file for Eisner projective trees dependency parsing algorithm
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


#ifndef EISNER_DEPENDENCY_SEARCHER
#define EISNER_DEPENDENCY_SEARCHER

#include <mutex>
#include <thread>
#include <vector>
#include <set>


#include "SyntaxTree.h"

using std::vector;
using std::set;

namespace SyntaxParser
{

/** 
* @class	EisnerDependencySearcher
* @brief	Eisner projective tree dependency parsing algorithm
*/
class EisnerDependencySearcher
{
public:
    /**
	 * @brief find best connections in graph
	   @param[out] tree syntax tree where to create connections
       @param[in] edgeWeights weights of each edge in the graph
       @param[in] rootWeights weights of each root pretendent
	 */
    void FindDependencies(
        SyntaxTree* tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights);
private:
    vector<double> rootWeights; /**< vector of weights for roots in tree*/
    vector<vector<double> > edgeWeights; /**< matrix of the edge weights in the graph. first index corresponds to head, second to modifier*/
    vector<vector<double> > fullSpan; /**< weights of the FULL subgraphs. first index is root index, second is the farthest node*/ 
    vector<vector<double> > partialSpan; /**< weights of the PARTIAL subgraphs. first index is root index, second is the farthest node*/ 
    vector<vector<int> > fullSplitting; /**< FULL subgraphs are splitted by these nodes into PARTIAL and FULL components*/ 
    vector<vector<int> > partialSplitting; /**< PARTIAL subgraphs are splitted by these nodes into two FULL components*/ 
    std::recursive_mutex m_lock;
    
    /**
	 * @brief clears all structures before proceeding with the new graph
	 */
    void clear();

    /**
	 * @brief resizes spans before running main algorithm
       @param[in] size number of nodes in the tree + 1 (for root)
	 */
    void initializeSpans(size_t size);

    /**
	 * @brief runs dynamic programming algorithm that calculates spannings and splittings
	 */
    void process();

    /**
	 * @brief part of dynamic programming algorithm that fills FULL structures (here root is on the left part of the segment)
       @param[in] leftNode left node of the segment to process
       @param[in] rightNode right node of the segment to process
	 */
    void evaluateLeftFullSpan(
        size_t leftNode, size_t rightNode);

    /**
	 * @brief part of dynamic programming algorithm that fills FULL structures (here root is on the right part of the segment)
       @param[in] leftNode left node of the segment to process
       @param[in] rightNode right node of the segment to process
	 */
    void evaluateRightFullSpan(
        size_t leftNode, size_t rightNode);

    /**
	 * @brief part of dynamic programming algorithm that fills PARTIAL structures (here root is on the left part of the segment)
       @param[in] leftNode left node of the segment to process
       @param[in] rightNode right node of the segment to process
	 */
    void evaluateLeftPartialSpan(
        size_t leftNode, size_t rightNode);

    /**
	 * @brief part of dynamic programming algorithm that fills PARTIAL structures (here root is on the right part of the segment)
       @param[in] leftNode left node of the segment to process
       @param[in] rightNode right node of the segment to process
	 */
    void evaluateRightPartialSpan(
        size_t leftNode, size_t rightNode);

    /**
     * @brief finds root with optimal weight
     */
    size_t findRoot();

    /**
	 * @brief recursive proccedure for finding best projective spanning tree using calculated FULL and PARTIAL structures.
              Returns heads for each node in the tree (root has -1 head).
        @param[in] rootIndex index of the root node
	 */
    vector<int> decodeTree(size_t rootIndex);

     /**
	 * @brief recursive proccedure for finding best projective spanning tree using calculated FULL and PARTIAL structures
       @param[in] leftIndex left node of the segment to process
       @param[in] rightIndex right node of the segment to process
       @param[in] fullOrPartial true if segment is FULL, false otherwise.
       @param[in] leftOrRight true if root is in leftIndex, false otherwise
       @param[out] heads vector of heads to update
	 */
    void decodeTree(size_t leftIndex
        , size_t rightIndex, bool fullOrPartial
        , bool leftOrRight, vector<int>* heads);

    /**
	 * @brief sets dependencies of the syntax treee
       @param[in] heads heads for each node in the tree
       @param[out] tree syntax tree to update
	 */
    void setTreeDependencies(
        const vector<int>& heads
        , SyntaxTree* tree);
};

}

#endif // EISNER_DEPENDENCY_SEARCHER

