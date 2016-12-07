/**
 * DictionaryTrieBinaryFileReader.h
 * Class DictionaryTrieBinaryFileReader
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


//#pragma warning( disable: 4018 4244 )
#ifndef _DICTIONARYTRIEBINARYFILEREADER_H_
#define _DICTIONARYTRIEBINARYFILEREADER_H_

#include "DictionaryTrieBinaryFileCreator.h"

/**
 * Class DictionaryTrieBinaryFileReader :
 * Read from binary file
 */
class DictionaryTrieBinaryFileReader : public DictionaryTrieBinaryFileCreator
{
public:
    DictionaryTrieBinaryFileReader(Dictionary* _dic, string dictionaryFile = BIN_DIC_FILE_PATH_RUSSIAN);
	~DictionaryTrieBinaryFileReader(void);
	/**
	 * @brief load models, nodes of trie, links, featureLists and features from binary file
	 */
	void loadFromBinaryFile(string _filePath);
protected:
	// vector of DictionaryNode
	vector<DictionaryNode*> DictionaryNodeList;
};

#endif /* _DICTIONARYTRIEBINARYFILEREADER_H_ */
