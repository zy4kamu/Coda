/**
 * SuffixModelTrieBinaryFileReader.h
 * Class SuffixModelTrieBinaryFileReader
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


//#pragma warning( disable: 4018 )
#ifndef _SUFFIXMODELTRIEBINARYFILEREADER_H_
#define _SUFFIXMODELTRIEBINARYFILEREADER_H_

#include "SuffixModelTrie.h"

using namespace std;

/**
 * class SuffixModelTrieBinaryFileReader for searching featureListId for suffix of a non-dictionary word
 * load from binary file
 */

class SuffixModelTrieBinaryFileReader : public SuffixModelTrie
{
public:
	SuffixModelTrieBinaryFileReader(Dictionary* _dic);
	~SuffixModelTrieBinaryFileReader(void);
	
	/**
	 * @brief load from binary file
	 */
	void loadFromBinaryFile(string _filePath);
protected:
	vector<SuffixModelNode*> SuffixModelNodeList;
};

#endif /* _SUFFIXMODELTRIEBINARYFILEREADER_H_ */
