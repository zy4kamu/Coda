/** 
 * @file	GreedyDependencySearcher.h
 * @brief	header file for finder optimal spanning tree of weighted full-connected graph
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


#ifndef GREEDY_DEPENDENCY_SEARCHER
#define GREEDY_DEPENDENCY_SEARCHER

#include "SyntaxTree.h"

namespace SyntaxParser
{

/** 
 * @class	DisambiguatorSyntaxTreeParser
 * @brief	Finds optimal spanning tree of weighted full-connected graph
 */
class GreedyDependencySearcher
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
    /**
	 * @brief find best connections for one root
	   @param[in] tree syntax tree where to create connections
       @param[in] edgeWeights weights of each edge in the graph
       @param[in] rootWeights weights of each root pretendent
       @param[in] rootIndex index of the root
       @param[out] dependencies edges of the best graph
       @param[out] weight weight of the found tree
	 */
    double findBestConnections(
        const SyntaxTree& tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights
        ,int rootIndex
        , vector<vector<int> >* dependencies);


    /**
	 * @brief some postprocessing improving found tree
	   @param[in] tree syntax tree where to create connections
       @param[in] edgeWeights weights of each edge in the graph
       @param[in] rootWeights weights of each root pretendent
       @param[in] rootIndex index of the root
       @param[out] dependencies edges of the best graph
       @param[out] sumWeight weight of the found tree
	 */
    void postProcess(
        const SyntaxTree& tree
        , vector<vector<double> >& edgeWeights
        , vector<double>& rootWeights
        , int rootIndex
        , vector<vector<int> >* dependencies
        , double* sumWeight);
};

};

#endif // GREEDY_DEPENDENCY_SEARCHER
