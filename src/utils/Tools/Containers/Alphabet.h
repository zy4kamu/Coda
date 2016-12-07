/** 
 * @file	Alphabet.h
 * @brief	header file foalphabet structure for CRF-model
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


#ifndef ALPHABET
#define ALPHABET

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include "IO/BinaryIO.h"

using std::unordered_map;
using std::vector;
using std::wstring;
using std::ifstream;
using std::wifstream;

namespace Tools
{

/**
 * @class	Alphabet
 * @brief	Class for quick search of feature by index and index by feature
 */
class Alphabet
{
public:
    static const wstring UNBELIEVABLE_ELEMENT;
	/**
	 * @brief	Constructor of alphabet
	 */
	Alphabet(void);

    /**
     * @brief	Copy constructor
     */
    Alphabet(const Alphabet& alphabet);

	/**
	 * @brief	Reads alphabet from input stream
	 * @param[in]	in - input stream
	 */
	Alphabet(ifstream& in);

	/**
	 * @brief	Reads alphabet from input stream
	 * @param[in]	in - input stream
	 */
	Alphabet(wifstream& in);

	/**
	 * @brief	Destructor of alphabet
	 */
	~Alphabet(void);

	/**
	 * @brief	Reads alphabet from input stream
	 * @param[in]	in - input stream
	 */
	void Read(ifstream& in);

	/**
	 * @brief	Reads alphabet from input stream
	 * @param[in]	in - input stream
	 */
	void Read(wifstream& in);

	/**
	 * @brief	Looks up for index of feature in alphabet and if it's not presented there pushes this feature into alphabet and returns its index
	 * @param[in]	data - name of feature
	 */
	int LookUpIndex_Adding(const wstring& data);

	/**
	 * @brief	Looks up for index of feature in alphabet and if it's not presented there returns -1
	 * @param[in]	data - name of feature
	 */
	int LookUpIndex_NoAdding(const wstring& data) const;

	/**
	 * @brief	Looks up for feature in alphabet by its index
	 * @param[in]	index - index of the feature
	 */
	wstring LookUpObject(int index) const;

	/**
	 * @brief	Checks whether alphabet contains the feature
	 * @param[in]	feature - name of the feature
	 */
	bool Contains(const wstring& feature) const;

	/**
	 * @brief	Copies one alphabet to another
	 * @param[in]	alphabet - alphabet to copy
	 */
	Alphabet& operator=(const Alphabet& alphabet);

	/**
	 * @brief	Returns number of features in the alphabet
	 */
	int Size() const;

	/**
	 * @brief	Prints alphabet into stream
	 * @param[in]	out - output stream
	 */
    void Print(std::ofstream& out) const;


    const vector<wstring>& GetData() const;

    void Clear();

    size_t GetByteSize() const;
private:
	unordered_map<wstring, int> _map; /**< quick converter from feature to index*/
	vector<wstring> _vec; /**< quick converter from index to feature*/
};

}

#endif // ALPHABET
