/** 
 * @file	SparseVector.h
 * @brief	header file of the class containing sequence of labeled doubles 
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


#ifndef SPARSE_VECTOR
#define SPARSE_VECTOR

#include <vector>
#include <memory>
#include <map>

using std::vector;
using std::shared_ptr;
using std::map;

namespace Tools
{

/** 
 * @class	SparseVector
 * @brief	contains sequence of labeled doubles 
 */
class SparseVector
{
public:
    /**
     * @brief	Constructor
     */
    SparseVector();

    /**
     * @brief	Constructor
     * @param[in]	data - map that contains indexes and values
     */
    SparseVector(const map<int, double>& data);

	/**
     * @brief	Constructor
	 * @param[in]	_indexes - labels of the SparseVector (_indexes must be sorted)
	 * @param[in]	_valued - data
     */
	SparseVector(shared_ptr<vector<int>> _indexes, shared_ptr<vector<double>> _values);

    /**
     * @brief	Copy constructor
     * @param[in]	data - sparse vector to copy
     */
    SparseVector(const SparseVector& data);

    /**
     * @brief	Move constructor
     * @param[in]	data - sparse vector to move
     */
    SparseVector(SparseVector&& data);

    /**
     * @brief	Assignment operator
     * @param[in]	data - sparse vector to copy
     */
    SparseVector& operator = (const SparseVector& data);

    /**
     * @brief	Assignment operator
     * @param[in]	data - sparse vector to move
     */
    SparseVector& operator = (SparseVector&& data);

	/**
     * @brief	Destructor
     */
	~SparseVector(void);

	/**
     * @brief	Returns the number of elements in SparseVector
     */
	int Size() const;

	/**
     * @brief	Returns value with corresponding index 
	 * @param[in]	index - index of the value
     */
	double GetValue(int index) const;

	/**
     * @brief	Returns the label with the corresponding index
	 * @param[in]	index - index
     */
	double GetIndex(int index) const;

	/**
     * @brief	Returns the index of the corresponding label if indexes contains it and return -1 otherwise
	 * @param[in]	index - index
     */
	int Contains(int index) const;

	/**
     * @brief	Returns the sequence of labels
     */
	const vector<int>& GetIndexes() const;

	/**
     * @brief	Returns the sequence of values
     */
	const vector<double>& GetValues() const;

     /**
     * @brief	Removes all features with weight smaller than trancationLevel
     * @param[in]	truncationLevel - Removes all entries with weight smaller than trancationLevel
     */
    void Truncate(double truncationLevel);

    /**
    * @brief	Converts sparse vector to map
    */
    map<int, double> ConvertToMap();
private:
	shared_ptr<vector<int> > indexes; /**< labels*/
	shared_ptr<vector<double> > values; /**< data*/
};

}

#endif // SPARSE_VECTOR

