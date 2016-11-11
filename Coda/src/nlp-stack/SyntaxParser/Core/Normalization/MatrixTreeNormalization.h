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

#ifndef MATRIX_TREE_NORMALIZATION
#define MATRIX_TREE_NORMALIZATION

#include <cstdlib>
#include <vector>
#include <utility>

#include "Math/MatrixTools.h"
#include "DisambiguatedData.h"

using std::pair;
using std::vector;
using Disambiguation::DisambiguatedData;

namespace SyntaxParser
{

class MatrixTreeNormalization
{
public:
   void Normalize(
       vector<vector<double> >* edgeLogWeights,
       vector<double>* rootLogWeights);
private:

   void normalizeRootWeights(
       vector<double>* rootLogWeights);

   void normalizeEdgeWeights(
       vector<vector<double> >* edgeWeights,
       double regularizationFactor);

   void getProbabilities(
       vector<vector<double> >* expEdgeWeights,
       vector<double>* expRootWeights);

   matrix<double> getMatrix(
       const vector<vector<double> >& edgeExpWeights,
       const vector<double>& rootExpWeights);

};

}

#endif // MATRIX_TREE_NORMALIZATION
