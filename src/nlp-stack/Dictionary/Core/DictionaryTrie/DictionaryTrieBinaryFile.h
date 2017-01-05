/**
 * DictionaryTrieBinaryFileCreator.h
 * Class DictionaryTrieBinaryFileCreator
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
#ifndef _DICTIONARYTRIEBINARYFILE_H_
#define _DICTIONARYTRIEBINARYFILE_H_

#include "DictionaryTrieBuild.h"

/**
 * class DictionaryTrieBinaryFileCreator :
 * write to binary file
 */
class DictionaryTrieBinaryFileCreator : public DictionaryTrieBuild 
{
public:
    DictionaryTrieBinaryFileCreator(Dictionary* _dic);
	~DictionaryTrieBinaryFileCreator(void);

    /**
     * @brief build buffer before write it to file
     */
    void buildBuffer(void);
    /**
     * @brief build buffer from text file
     */
    void buildBufferFromTextFile(string _filePath);
    /**
	 * @brief add all NodeVer3 to buffer
	 */
	void addAllDictionaryNodeToBuffer(void);
	/**
	 * @brief add all NodeModel to buffer
	 */
	void addAllDictionaryNodeModelToBuffer(void);
	/**
	 * @brief read links from files
	 */
	void readLinksFromFileToBuffer(string _filePath);
	/**
	 * @brief add all TrieModel to buffer
	 */
	void addAllDictionaryTrieModelToBuffer(void);
	/**
	 * @brief add featureListMap to buffer
	 */
	void addFeatureListMapToBuffer(void);
	/**
	 * @brief add featureMap to buffer
	 */
	void addFeatureMapToBuffer(void);
	/**
	 * @brief add a link by lemmaIds
	 */
	void addLinkByLemmaIdsToBuffer(int fromLemmaId, int toLemmaId);
	/**
	 * @brief process a line to link
	 */
	void addLinkToBuffer(wstring line);
	/**
	 * @brief write models, nodes of trie, links to binary file
	 */
	void writeToBinaryFile(string _filePath);
	/**
	 * @brief add a vector<char> to buffer
	 */
    void writeToBuffer(vector<unsigned char> charVector);
	/**
	 * @brief convert NodeVer3 and its parentId to vector<char>
	 */
    vector<unsigned char> binaryDictionaryNode(DictionaryNode * _node, int _parentId);
	/**
	 * @brief convert NodeModel and its nodeId to vector<char>
	 */
    vector<unsigned char> binaryDictionaryNodeModel(DictionaryNodeModel * _nodeModel, int _nodeId);
protected:
	// buffer
    unsigned char *buffer;
	// size of buffer
	int bufferSize;
	// links count
	int countLink;
};

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

#endif /* _DICTIONARYTRIEBINARYFILE_H_ */
