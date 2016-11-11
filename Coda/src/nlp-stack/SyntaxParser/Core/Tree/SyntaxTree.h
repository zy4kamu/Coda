/** 
 * @file	SyntaxTree.h
 * @brief	header file for syntax tree container
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


#ifndef SYNTAX_TREE
#define SYNTAX_TREE

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cmath>
#include <set>

#include "SyntaxNode.h"
#include "FileReader.h"

using std::vector;
using std::string;
using std::wstring;
using std::shared_ptr;
using std::set;

namespace SyntaxParser
{

struct SyntaxNode;

/** 
 * @class	SyntaxTree
 * @brief	Data describing syntax tree structure
 */
class SyntaxTree
{
public:
    /**
     * @brief	Empty constructor
     */
    SyntaxTree();

    /**
     * @brief	Creates tree without edges (only nodes)
     * @param[in]	syntaxNodes - nodes of the tree
     */
    SyntaxTree(const vector<Disambiguation::DisambiguatedData>& syntaxNodes);

    SyntaxTree(int rootIndex, vector<SyntaxNode>&& nodes
        , wstring&& sentence);

    /**
     * @brief	Parses syntax tree from line
     * @param[in]	line - special string representation
     */
    SyntaxTree(const wstring& line);

    /**
     * @brief	Copy constructor
     * @param[in]	tree - syntax tree to copy
     */
    SyntaxTree(const SyntaxTree& tree);

    /**
     * @brief	Move constructor
     * @param[in]	tree - syntax tree to move
     */
    SyntaxTree(SyntaxTree&& tree);

    /**
     * @brief	Copy assignment operator
     * @param[in]	tree - syntax tree to move
     */
    SyntaxTree& operator = (const SyntaxTree& tree);

    /**
     * @brief	Move assignment operator
     * @param[in]	tree - syntax tree to move
     */
    SyntaxTree& operator = (SyntaxTree&& tree);

    /**
     * @brief	Creates empty tree with fixed number of nodes
     * @param[in]	tree - syntax tree to copy
     */
    SyntaxTree(int size);

    /**
     * @brief	Destructor
     */
    virtual ~SyntaxTree();

    /**
     * @brief	Converts tree to dot type string
     * @param[in]	createCap - special parameter used for printing several trees at once
     * @param[in]	nodesPrefix - special parameter used for printing several trees at once
     */
    wstring ToDotString(bool createCap = true
        , const wstring& nodesPrefix = L"") const;

    /**
     * @brief	Converts to special format string
     */
    wstring ToRecursiveString() const;

    /**
     * @brief	Converts to special format string
     */
    wstring ToReadableString(bool compressToOneString = false) const;

    /**
     * @brief	Enables to get syntax nodes
     */
    vector<SyntaxNode>& GetNodes();

    /**
     * @brief	Enables to get syntax nodes
     */
    const vector<SyntaxNode>& GetNodes() const;

    /**
     * @brief	Enables to get index of the root in the sentence
     */
    int GetRootIndex() const;

    /**
     * @brief	Returns number of nodes in the tree
     */
    int GetSize() const;

    /**
     * @brief	Sets edges; used in parsing tree
     * @param[in]	dependencies - i connects to dependencies[i][j]
     */
    void SetEdges(const vector<vector<int> >& dependencies);

    /**
     * @brief	Sets root index; used in parsing tree
     * @param[in]	rootIndex - index of the root in the sentence
     */
    void SetRoot(int rootIndex);

    /**
     * @brief	Returns indexes of the nodes that are in the correspondent subtree
     * @param[in]	index - index of the root of subtree
     * @param[out]	subTree - nodes in the subtree
     */
    void GetSubTree(int index, set<int>* subTree) const;

    /**
     * @brief	Enables to get parrent index
     * @param[in]	nodeIndex - index of the node
     */
    int GetParentIndex(int nodeIndex) const;

    /**
     * @brief	Returns sentence of the tree
     */
    const wstring& GetSentence() const;

    /**
     * @brief	Checks whether the tree is ok (for example during training we can use only that trees that have no punctuation)
     */
    virtual bool IsValid() const;

    void Correct();
protected:
    int rootIndex; /**< index of the root in the sentence*/
    vector<SyntaxNode> nodes; /**< syntax nodes in the order of their appearence in the sentence*/
    wstring sentence; /**< initial sentence*/
private:
    /**
     * @brief	Converts to special string format
     */
    wstring convertToRecursiveString(const SyntaxNode& node) const;

    /**
     * @brief	Depreceted
     */
    void readRecursiveTree(const wstring& line);

    void readTree(const wstring& line);
};

}

#endif // SYNTAX_TREE

