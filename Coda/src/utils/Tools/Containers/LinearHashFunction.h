/** 
 * @file	LinearHashFunction.h
 * @brief	Calculates ax + b function
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

#ifndef LINEAR_HASH_FUNCTION
#define LINEAR_HASH_FUNCTION

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

const long long PRIME = 2000000011;
const long long UNDEFINED_NUMBER = PRIME;
const long long RANDMAX = 9000000000;
const int DEFAULT_HASH_SLOPE = 1;
const int DEFAULT_HASH_COERCIDENT = 0;

/** 
 * @class	LinearHashFunction
 * @brief	Calculates ax + b function
 */
struct LinearHashFunction
{
    int slope; /**< linear coefficient*/
    int intercept; /**< free factor*/

    /**
	 * @brief	Constructor
	 * @param[in]	possibleStateFinder - tool for finding it
	 */
    LinearHashFunction();

    /**
	 * @brief	Constructor
	 * @param[in]	slope - linear coefficient
     * @param[in]	intercept - free factor
	 */
    LinearHashFunction(int slope, int intercept);

    /**
	 * @brief	Calculates value
	 * @param[in]	input - value for which calculate hash function
	 */
    int operator ()(int input) const;
};

/**
* @brief	Creates random hash function
* @param[in]	generator - random number generator
*/
LinearHashFunction RandomizeHashFunction(
    std::default_random_engine& generator);

#endif
