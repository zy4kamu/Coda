/** 
 * @file	SyntaxNode.h
 * @brief	header file for data contained in one node of syntax tree
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


#ifndef SYNTAX_NODE
#define SYNTAX_NODE

#include <vector>
#include <string>
#include <set>

#include "DisambiguatedData.h"
#include "SyntaxTree.h"
#include "Tools.h"

using std::vector;
using std::wstring;
using std::set;

namespace SyntaxParser
{

/** 
 * @class	SyntaxNode
 * @brief	Data contained in one node of syntax tree
 */
struct SyntaxNode : public Disambiguation::DisambiguatedData
{
    int index; /**< index of the token in the sentence (first token has index 0)*/
    int parentIndex; /**< index of the parent node*/
    vector<int> neighbours; /**< index of childs of the syntax node*/
    vector<wstring> splittedLabel; /**< splitted full label into small parts*/

    /**
     * @brief	Empty constructor
     */
    SyntaxNode(void) {};

    /**
     * @brief	Constructor of node drom morphology data only
     * @param[in]	data - morphology data
     */
    SyntaxNode(const Disambiguation::DisambiguatedData& data) 
        : Disambiguation::DisambiguatedData(data)
        , index(-1)
        , parentIndex(-1)
        , neighbours(vector<int>())
    {
        splittedLabel = Tools::Split(data.label, L"@");
    }

    /**
     * @brief	Copy constructor
     * @param[in]	token - token (word)
     * @param[in]	lemma - initial form of the token
     * @param[in]	label - morphology label of the token
     * @param[in]	weight - weight assigned to the label by the classifier
     */
    SyntaxNode(const SyntaxNode& node) 
        : Disambiguation::DisambiguatedData(node)
        , index(node.index)
        , parentIndex(node.parentIndex)
        , neighbours(node.neighbours)
        , splittedLabel(node.splittedLabel)
    {}

    /**
     * @brief	Assignment operator
     * @param[in]	node - data to copy
     */
    SyntaxNode& operator=(const SyntaxNode& node)
    {
        lemma = node.lemma;
        label = node.label;
        weight = node.weight;
        content = node.content;
        punctuation = node.punctuation;
        splittedLabel = node.splittedLabel;
        return *this;
    }

    /**
     * @brief	Move assignment operator
     * @param[in]	node - data to move
     */
    SyntaxNode& operator=(SyntaxNode&& node)
    {
        lemma = std::move(node.lemma);
        label = std::move(node.label);
        weight = std::move(node.weight);
        content = std::move(node.content);
        punctuation = std::move(node.punctuation);
        splittedLabel = std::move(node.splittedLabel);
        lemmaId = node.lemmaId;
        return *this;
    }
};

};

#endif // SYNTAX_NODE

