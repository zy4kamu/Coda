/**
 * NGramTrieBuild.h
 * Class NGramTrieBuild
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
#ifndef _NGRAMTRIEBUILD_H_
#define _NGRAMTRIEBUILD_H_

#include "NGramTrie.h"

/**
 * class NGramTrieBuild :
 */
class NGramTrieBuild : public NGramTrie
{
public:
	NGramTrieBuild(Dictionary* _dic);
	~NGramTrieBuild(void);

	/**
	 * @brief update a sentence
	 */
	void updateSentence(wstring _sentence);
	/**
	 * @brief update a file
	 */
	void updateFromStandardFile(string _filePath);
	/**
	 * @brief update a folder
	 */
	void updateFromFolder(string _path, string _file_type);
};
#endif /* _NGRAMTRIEBUILD_H_ */
