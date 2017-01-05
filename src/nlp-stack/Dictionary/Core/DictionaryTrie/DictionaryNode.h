/**
 * DictionaryNode.h
 * Class DictionaryNode
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
#ifndef _DICTIONARYNODE_H_
#define _DICTIONARYNODE_H_

#include "DictionaryNodeModel.h"

using namespace std;

/**
 * class DictionaryNode :
 */
class DictionaryNode
{
public:
    DictionaryNode(void);
	~DictionaryNode(void);
	/**
	 * @brief return character of this node
	 */
    wchar_t getCharacter(void) { return character; }
	/**
	 * @brief set character for this node
	 */
    void setCharacter(wchar_t _character) { character = _character;}
	/**
	 * @brief return number of NodeModel
	 */
	int getNumberOfModels(void) { return nodeModels.size(); }
	/**
	 * @brief add NodeModel
	 */
	void addNodeModel(DictionaryNodeModel* _nodeModel) { nodeModels.push_back(_nodeModel); _nodeModel->setNode(this); }
	/**
	 * @brief return list of NodeModels
	 */
    vector<DictionaryNodeModel*> * getNodeModels(void) { return &nodeModels;}
	/**
	 * @brief add a child node
	 */
	void addChild(DictionaryNode* child) { childrenNodes.push_back(child); child->setParentNode(this); }
	/**
	 * @brief return list of children nodes
	 */
    std::vector<DictionaryNode*> getChildren(void) { return childrenNodes; }
	/**
	 * @brief return child with _charcter
	 */
    DictionaryNode* findChild(wchar_t _character);
	/**
	 * @brief get parent node
	 */
	DictionaryNode* getParentNode(void) { return parentNode;}
	/**
	 * @brief set parent node
	 */
	void setParentNode(DictionaryNode* _parentNode) { parentNode = _parentNode;}
	/**
	 * @brief get prefix-string from root to this node
	 */
	wstring getStringFromRoot(void);
private:
	// character : character of node
    wchar_t character;
	// childrenNodes : children nodes
    vector<DictionaryNode*> childrenNodes;
	// parentNode
	DictionaryNode* parentNode;
	// model list
    vector<DictionaryNodeModel*> nodeModels;
};
#endif /* _DICTIONARYNODE_H_ */
