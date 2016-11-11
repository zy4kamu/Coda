/** 
 * @file    main-data-types.h
 * @brief	header file with declaration of data types in statcalc namespace
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


#ifndef _MAIN_DATA_TYPES_
#define _MAIN_DATA_TYPES_

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "category-processor.h"
#include "eigen-data-types.h"

using std::string;
using std::wstring;
using std::unordered_map;
using std::vector;
using std::pair;

namespace statcalc
{

//typedef WikiCategoryFullLabel Category;
typedef GoogleAppsLabel Category;
//typedef PatentLabel Category;
//typedef PatentLabelVec Category;

/**
*   Structure for storing word and list documents in which this word appears
*   pair<unsigned int, unsigned int> store id of document and number of occurrences this word
*/
typedef eigentools::SparseMat TDMatrix;

/**
*   Structure for mapping name of document into id
*/
typedef unordered_map<string,unsigned int> DocID;

/**
*   Structure for mapping word into id
*/
typedef unordered_map<wstring,int> Vocabulary;


/**
*   Structure for mapping document id into type of domain
*/
typedef unordered_map<unsigned int,Category> IDCategory;

/**
*   Structure storing map of term and its double valued statistic
*/
typedef eigentools::DenseVec WordDoubleStat;

/**
*   Structure for storing term and its integer statistics
*/
typedef Eigen::Matrix<int,Eigen::Dynamic,1> WordIntStat;

typedef vector<vector<vector<int> > > DocPlainByWords;

typedef unordered_map<unsigned int, DocPlainByWords> DocCollectionPlainIdx;

typedef vector<Eigen::Triplet<int> > DocCollectionBOW;

class StatCalc;
class DocumentProcessor;
}
#endif //_MAIN_DATA_TYPES_
