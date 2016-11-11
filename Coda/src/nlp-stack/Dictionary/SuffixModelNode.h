/**
 * SuffixModelNode.h
 * Class SuffixModelNode
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
#ifndef _SUFFIXMODELNODE_H_
#define _SUFFIXMODELNODE_H_

#include "DictionaryDataTypes.h"

using namespace std;

class SuffixModelNode
{
public:
	SuffixModelNode(wchar_t _character);
	~SuffixModelNode(void);

	/**
	 * @brief get character of node
	 */
	wchar_t getCharacter(void) { return character;}
	/**
	 * @brief update featureId
	 */
	void updateFeature(int _basicFeatureListId, int _featureListId);
	/**
	 * @brief add a child node
	 */
	void addChildNode(SuffixModelNode* _childNode) { childrenNodes.push_back(_childNode);}
	/**
	 * @brief find child node by character
	 */
	SuffixModelNode* findChildNode(wchar_t _character);
	/**
	 * @brief get all children nodes
	 */
	vector<SuffixModelNode*> getChildrenNode() { return childrenNodes;}
	/**
	 * @brief get feature-frequency map
	 */
	map<int, int> getFeatureFrequencyMap() {return featureFrequencyMap;}
	/**
	 * @brief set feature-frequency map
	 */
	void setFeatureFrequencyMap(map<int, int> _featureFrequencyMap) {featureFrequencyMap = _featureFrequencyMap;}
protected:
	wchar_t character;
	// map<featureId, frequency> - number of times featureId occurs at that node
	// featureId = basicFeatureListId * 1000 + featureListId
	map<int, int> featureFrequencyMap;
	vector<SuffixModelNode*> childrenNodes;
};

#endif /* _SUFFIXMODELNODE_H_ */
