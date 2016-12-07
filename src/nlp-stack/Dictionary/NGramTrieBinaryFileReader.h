/**
 * NGramTrieBinaryFileReader.h
 * Class NGramTrieBinaryFileReader
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


//#pragma warning( disable: 4018 4244)
#ifndef _NGRAMTRIEBINARYFILEREADER_H_
#define _NGRAMTRIEBINARYFILEREADER_H_

#include "NGramTrie.h"

/**
 * class NGramTrieBinaryFileReader :
 * load N-Gram from binary file
 */
class NGramTrieBinaryFileReader : public NGramTrie
{
public:
	NGramTrieBinaryFileReader(Dictionary* _dic);
	~NGramTrieBinaryFileReader(void);

	/**
	 * @brief load n-gram data from binary file
	 */
	void loadFromBinaryFile(string _filePath);
protected:
	// vector of NGramNode
	vector<NGramNode*> NGramNodeList;
};
#endif /* _NGRAMTRIEBINARYFILEREADER_H_ */
