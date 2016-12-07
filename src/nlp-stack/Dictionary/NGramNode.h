/**
 * NGramNode.h
 * Class NGramNode
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
#ifndef _NGRAMNODE_H_
#define _NGRAMNODE_H_

#include "DictionaryDataTypes.h"

using namespace std;

/**
 * class NGramNode :
 */
class NGramNode 
{
public:
	NGramNode(void);
	NGramNode(int _basicFeatureListId, int _featureListId);
	~NGramNode(void);
	/**
	 * @brief get feature list ID
	 */
	int getFeatureListId(void) { return featureListId;}
	/**
	 * @brief get basic feature list ID
	 */
	int getBasicFeatureListId(void) { return basicFeatureListId;}
	/**
	 * @brief set feature list ID
	 */
	void setFeatureListId(int _basicFeatureListId, int _featureListId) { basicFeatureListId = _basicFeatureListId; featureListId = _featureListId;}
	/**
	 * @brief get all children nodes
	 */
	vector<NGramNode*> getChildren(void) { return children;}
	/**
	 * @brief find a child by feature list ID
	 */
	NGramNode* findChild(int _basicFeatureListId, int _featureListId);
	/**
	 * @brief add a child node
	 */
	void addChild(NGramNode* child) { children.push_back(child);}
	/**
	 * @brief get N-Gram count
	 */
	int getNGramCount(void) { return NGramCount;}
	/**
	 * @brief set N-Gram count
	 */
	void setNGramCount(int _NGramCount) { NGramCount = _NGramCount;}
	/**
	 * @brief increase N-Gram count
	 */
	void increaseNGramCount(void) { NGramCount++;}
private:
	int featureListId;
	int basicFeatureListId;
	int NGramCount;
	vector<NGramNode*> children;
};
#endif /* _NGRAMNODE_H_ */
