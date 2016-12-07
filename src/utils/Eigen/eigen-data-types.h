/**
 * @file	eigen-data-types.h
 * @brief	header file containing main data types
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


#ifndef EIGEN_DT
#define EIGEN_DT

#include "Eigen/Dense"
#include "Eigen/Sparse"

namespace eigentools
{

typedef double ScalarType;
typedef Eigen::SparseMatrix<ScalarType> SparseMat;
typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> DenseMat;
typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> DenseVec;
typedef Eigen::Array<ScalarType, Eigen::Dynamic, Eigen::Dynamic> ArrayXX;
typedef Eigen::Array<ScalarType, Eigen::Dynamic, 1> ArrayX;

}

#endif // EIGEN_DT
