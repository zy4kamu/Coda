/** 
 * @file	ChuLiuEdmondDependencySearcher.h
 * @brief	header file for Chu Liu Edmond dependency parsing algorithm
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


#ifndef CHU_LIU_EDMOND_DEPENDENCY_PARSER
#define CHU_LIU_EDMOND_DEPENDENCY_PARSER

#include <vector>
#include <set>

#include "SyntaxTree.h"

using std::vector;
using std::set;

namespace SyntaxParser
{

/** 
* @class	ChuLiuEdmondDependencySearcher
* @brief	Chu Liu Edmond dependency parsing algorithm
*/
class ChuLiuEdmondDependencySearcher
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
    vector<int> heads; /**< contains head for each node in the graph. First element is root, recursive added nodes are included here also.*/ 
    vector<vector<double> > weights; /**< contains weights of the edges. First index is modifier, second index is head, root has index 0, recursive added nodes are included here also*/ 
    set<int> currentNodes; /**< contains nodes of the graph on each step (on the in-recursion stage cycles in this set are exchanged to new nodes, on out-recursion stage cycles appears again).*/ 

    /**
	 * @brief initializes heads, weights, currentNodes structures
       @param[in] rootWeights weights of each root pretendent
       @param[in] edgeWeights weights of each edge in the graph
       @param[in] rootIndex root of the tree
	 */
    void initialize(
        const vector<double>& rootWeights
        , const vector<vector<double> >& edgeWeights
        , size_t rootIndex);
    
    /**
	 * @brief initializes currentNodes (0, 1, 2, ..., size + 1)
       @param[in] size - number of nodes in syntax tree
	 */
    void initializeCurrentNodes(size_t size);

    /**
	 * @brief initializes weights structure
       @param[in] rootWeights weights of each root pretendent
       @param[in] edgeWeights weights of each edge in the graph
       @param[in] rootIndex root of the tree
	 */
    void intializeWeights(
        const vector<double>& rootWeights
        , const vector<vector<double> >& edgeWeights
        , size_t rootIndex);

    /**
	 * @brief initializes heads structure. Each node has head with maximum weight of possible heads
	 */
    void initializeHeads();

    /**
	 * @brief verifies if there is a cycle in the graph. Returns false if there is no cycle in the graph and true otherwise
       @param[out] cycle nodes in the cycle in the opposite direction of arrows 
	 */
    bool findCycle(vector<int>* cycle);

    /**
	 * @brief runs recursive algorithm procedure (finds cycle, exchanges it with new one node, recalculates weights and runs the procedure again)
	 */
    void process();

    /**
	 * @brief concatenates nodes in the cycle into one node and recalculates weights (heads are not updated here)
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
	 */
    void contract(const vector<int>& cycle);

    /**
	 * @brief updates currentNodes structure: removes nodes in the cycle and adds new node
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
	 */
    void contractCurrentNodes(
        const vector<int>& cycle);

    /**
	 * @brief updates weights structure: recalculates weights for edges with head in the cycle
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
	 */
    void contractEdgesWithHeadInCycle(
        const vector<int>& cycle);

    /**
	 * @brief updates weights structure: recalculates weights for edges with modifer in the cycle
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
	 */
    void contractEdgesWithModifierInCycle(
        const vector<int>& cycle);

    /**
	 * @brief updates heads in the graph after appearence of new cycle node
	 */
    void updateHeads(
        const vector<int>& cycle
        , int cycleNodeIndex);

    /**
	 * @brief turns recursion back: creates cycle again from one node
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
       @param[in] cycleNodeIndex index of the cycle node
	 */
    void decodeTree(
        const vector<int>& cycle
        , int cycleNodeIndex);

     /**
	 * @brief turns recursion back: updates current nodes structure
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
       @param[in] cycleNodeIndex index of the cycle node
	 */
    void decodeCurrentNodes(
        const vector<int>& cycle
        , int cycleNodeIndex);

    /**
	 * @brief turns recursion back: finds node of the cycle which is a head of the cycle in the resulting graph
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
       @param[in] cycleNodeIndex index of the cycle node
	 */
    void decodeMainNodeInCycle(
        const vector<int>& cycle
        , int cycleNodeIndex);

    /**
	 * @brief turns recursion back: updates edges of the graph (that are not in cycle)
       @param[in] cycle nodes in the cycle in the opposite direction of arrows 
       @param[in] cycleNodeIndex index of the cycle node
	 */
    void decodeHeadsNotInCycle(
        const vector<int>& cycle
        , int cycleNodeIndex);

    /**
	 * @brief constructs tree in appropriate form (root is not in the heads again)
       @param[in] numberOfNodes number of nodes in the resulting syntax tree
	 */
    vector<int> constructTree(size_t numberOfNodes);

    /**
     * @brief sets dependencies of the syntax tree
       @param[in] treeHeads heads for each node in the tree
       @param[out] tree syntax tree to update
	 */
    void setTreeDependencies(
        const vector<int>& treeHeads
        , SyntaxTree* tree);

    /**
     * @brief calculates weight of tree
       @param[in] treeHeads heads for each node in the tree
       @param[out] tree syntax tree to update
     */
    double calculateWeight(
        const vector<int>& treeHeads
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights);

    /**
     * @brief visualization for debug
     */
    void visualize() const;
};

}

#endif // CHU_LIU_EDMOND_DEPENDENCY_PARSER

