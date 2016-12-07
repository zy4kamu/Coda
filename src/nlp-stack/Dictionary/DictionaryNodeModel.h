/**
 * DictionaryNodeModel.h
 * Class DictionaryNodeModel
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
#ifndef _DICTIONARYNODEMODEL_H_
#define _DICTIONARYNODEMODEL_H_

#include "DictionaryDataTypes.h"

using namespace std;

class DictionaryNode;

class DictionaryNodeModel
{
public:
	DictionaryNodeModel(int _modelId, int _lemmaId);
	~DictionaryNodeModel(void);
	/**
	 * @brief return id of model (models are saved in TrieVer3)
	 */
	int getModelId(void) { return modelId;}
	/**
	 * @brief set id for model
	 */
	void setModelId(int _modelId) { modelId = _modelId;}
	/**
	 * @brief return id of lemma (id of lemma in OpenCorpora)
	 */
	int getLemmaId(void) { return lemmaId;}
	/**
	 * @brief set lemma id for model
	 */
	void setLemmaId(int _lemmaId) { lemmaId = _lemmaId;}
	/**
	 * @brief return DictionaryNodeModel that this one links to
	 */
	inline DictionaryNodeModel* getLink(void) { return linkNode;}
	/**
	 * @brief set link to other DictionaryNodeModels
	 */
	void setLink(DictionaryNodeModel* _linkNode) { linkNode = _linkNode; _linkNode->addLinkFrom(this);}
	/**
	 * @brief set link from other DictionaryNodeModels
	 */
	void addLinkFrom(DictionaryNodeModel* _linkNode) { linkFrom.push_back(_linkNode);}
	/**
	 * @brief get links from this node model
	 */
	vector<DictionaryNodeModel*> getLinkFrom(void) { return linkFrom;}
	/**
	 * @brief return NodeVer3 of this model
	 */
	DictionaryNode* getNode() { return node;}
	/**
	 * @brief set NodeVer3 for this model
	 */
	void setNode(DictionaryNode* _node) { node = _node;}
	/**
	 * @brief get prefix-string from root to this DictionaryNodeModel's node
	 */
	wstring getStringFromRoot(void);

private:
	// id of model
	int modelId;
	// id of lemma
	int lemmaId;
	// node of this model
	DictionaryNode* node;
	// link to this model
	DictionaryNodeModel* linkNode;
	// 2013-04-11 link from this model -> for finding word form by basic form and features
	vector<DictionaryNodeModel*> linkFrom;
};

#endif /* _DICTIONARYNODEMODEL_H_ */
