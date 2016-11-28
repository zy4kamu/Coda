/**
 * @file	SyntaxParser.h
 * @brief	header file for parser sentence into syntax tree using disambiguator and tokenizer
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


#ifndef DISAMBIGUATOR_SYNTAX_TREE_PARSER
#define DISAMBIGUATOR_SYNTAX_TREE_PARSER

#include <memory>
#include <string>

#include "IDisambiguator.h"
#include "Token.h"
#include "ITokenizer.h"
#include "DisambiguatedData.h"
#include "ConnectionBuilder.hpp"
#include "ISyntaxParser.h"

using std::wstring;
using std::shared_ptr;

using Disambiguation::IDisambiguator;
using Disambiguation::DisambiguatedData;
using Tokenization::Token;
using Tokenization::ITokenizer;

#define D_S_T_PARSER_TEMPLATE template< \
    class EdgeFCalculator \
    , class RootFCalculator \
    , class DependencySearcher \
    , class WeightCalculator \
    , class StringBatch \
    , class IntBatch>

#define D_S_T_PARSER_TEMPLATE1 SyntaxParser< \
    EdgeFCalculator \
    , RootFCalculator \
    , DependencySearcher \
    , WeightCalculator \
    , StringBatch \
    , IntBatch>

namespace SyntaxParser
{

/**
 * @class	SyntaxParser
 * @brief	Parses sentence into syntax tree using disambiguator and tokenizer
 */
D_S_T_PARSER_TEMPLATE
class SyntaxParser
    : public ConnectionBuilder
        <EdgeFCalculator
        , RootFCalculator
        , DependencySearcher
        , WeightCalculator
        , StringBatch
        , IntBatch>
    , public ISyntaxParser
{
    typedef ConnectionBuilder<
        EdgeFCalculator
        , RootFCalculator
        , DependencySearcher
        , WeightCalculator
        , StringBatch
        , IntBatch> Base;
public:
    /**
     * @brief constructor of interface
       @param[in] syntaxTreeConfigFile file containing model description
       @param[in] tokenizer tokenizes sentence
       @param[in] disambiguator morphology analyzer
     */
    SyntaxParser(
        const string& syntaxTreeConfigFile
        , Tools::Language language);

    /**
     * @brief destructor
     */
    virtual ~SyntaxParser(void) {};

    /**
     * @brief parses sentence
       @param[in] disambiguatedData input data
       @param[out] syntaxTree syntax tree for sentence
     */
    virtual SyntaxTree Parse(
        const vector<DisambiguatedData>& disambiguatedData);

    virtual void GetDependencyMatrix(
        const std::vector<DisambiguatedData>& disambiguatedData
        , vector<vector<double> >* edgeWeights
        , vector<double>* rootWeights);

    virtual Tools::Language GetLanguage() const;

    virtual void SetConnections(
        SyntaxTree* treeWithoutDependencies);
private:
    Tools::Language language;
};

D_S_T_PARSER_TEMPLATE
D_S_T_PARSER_TEMPLATE1::SyntaxParser(
    const string& syntaxTreeConfigFile
    , Tools::Language language)
    :  ConnectionBuilder<
        EdgeFCalculator
        , RootFCalculator
        , DependencySearcher
        , WeightCalculator
        , StringBatch
        , IntBatch>(syntaxTreeConfigFile)
    , language(language)
{
}

D_S_T_PARSER_TEMPLATE
SyntaxTree D_S_T_PARSER_TEMPLATE1::Parse(
    const vector<DisambiguatedData>& disambiguatedData)
{
    SyntaxTree tree(disambiguatedData);
    Base::SetConnections(&tree);
    return tree;
}

D_S_T_PARSER_TEMPLATE
void D_S_T_PARSER_TEMPLATE1::GetDependencyMatrix(
    const std::vector<Disambiguation::DisambiguatedData>& disambiguatedData
    , vector<vector<double> >* edgeWeights
        , vector<double>* rootWeights)
{
    SyntaxTree tree(disambiguatedData);
    Base::GetDependencyMatrix(&tree, edgeWeights, rootWeights);
}

D_S_T_PARSER_TEMPLATE
Tools::Language D_S_T_PARSER_TEMPLATE1::GetLanguage() const
{
    return language;
}

D_S_T_PARSER_TEMPLATE
void D_S_T_PARSER_TEMPLATE1::SetConnections(
    SyntaxTree* treeWithoutDependencies)
{
    Base::SetConnections(treeWithoutDependencies);
}

}

#endif // DISAMBIGUATOR_SYNTAX_TREE_PARSER
