/**
 * @file	ConnectionBuilder.h
 * @brief	header file for connecter in syntax tree
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


#ifndef CONNECTION_BUILDER
#define CONNECTION_BUILDER

#include <limits>
#include <memory>
#include <string>

#include "MatrixTreeNormalization.h"
#include "SyntaxTree.h"

using std::wstring;
using std::string;
using std::shared_ptr;

#define S_T_PARSER_TEMPLATE template< \
    class EdgeFCalculator \
    , class RootFCalculator \
    , class DependencySearcher \
    , class WeightCalculator \
    , class StringBatch \
    , class IntBatch>

#define S_T_PARSER_TEMPLATE1 ConnectionBuilder< \
    EdgeFCalculator \
    , RootFCalculator \
    , DependencySearcher \
    , WeightCalculator \
    , StringBatch \
    , IntBatch>

namespace SyntaxParser
{

/** 
 * @class	ConnectionBuilder
 * @brief	Creates connections between nodes in syntax tree
 */
S_T_PARSER_TEMPLATE
class ConnectionBuilder
{
public:
    typedef IntBatch IntBatchType;

    /**
	 * @brief constructor
	   @param[in] syntaxTreeConfigFile file containing model description
	 */
    ConnectionBuilder(const string& syntaxTreeConfigFile);

    /**
	 * @brief destructor
	 */
    virtual ~ConnectionBuilder(void) {};

    /**
	 * @brief sets connections in the tree
       @param[in] treeWithoutDependencies tree to update
	 */
    void SetConnections(SyntaxTree* treeWithoutDependencies);

    /**
	 * @brief calculates weight of created syntax tree
       @param[in] tree syntax tree to process
	 */
    double CalculateWeight(const SyntaxTree& tree);

    void GetDependencyMatrix(
        SyntaxTree* treeWithoutDependencies
        , vector<vector<double> >* edgeWeights
        , vector<double>* rootWeights);
protected:
    /**
	 * @brief calculates features in integer representation for syntax tree
       @param[in] tree syntax tree to process
       @param[out] features indexes of features to calculate
	 */
    void calculateEdgeFeatures(const SyntaxTree& tree
        , vector<vector<IntBatchType> >* features);

    /**
	 * @brief calculates root features in integer representation for syntax tree
       @param[in] tree syntax tree to process
       @param[out] features features for the root
	 */
    vector<IntBatchType> calculateRootFeatures(
        const SyntaxTree& tree);

    /**
	 * @brief calculates weights of roots
       @param[in] rootFeatures indexes of the features
       @param[out] weights weights of the roots
	 */
    vector<double> calculateRootWeights(
        const vector<IntBatchType>& rootFeatures);

    /**
	 * @brief calculates weights of edges
       @param[in] edgeFeatures indexes of the features
       @param[out] weights weights of the edges
	 */
    vector<vector<double> > calculateEdgeWeights(
        const vector<vector<IntBatchType> >& edgeFeatures);
private:
    shared_ptr<Tools::Alphabet> alphabet; /**< translater of string feature to its index*/ 
    vector<double> parameters; /**< parameters of the model*/ 
    shared_ptr<EdgeFCalculator> featureCalculator; /**< calculates features for each edge in string representation*/
    RootFCalculator rootFeatureCalculator; /**< calculates features for roots pretendents in string representation*/
    DependencySearcher dependencySearcher; /**< finds optimal spanning tree in weighted graph*/ 
    WeightCalculator weightCalculator; /**< finds weight of the edge*/
};

S_T_PARSER_TEMPLATE
S_T_PARSER_TEMPLATE1::ConnectionBuilder(
    const string& syntaxTreeConfigFile) 
    : alphabet(new Tools::Alphabet())
    , featureCalculator(std::make_shared<EdgeFCalculator>())
{
    wifstream in(syntaxTreeConfigFile);
    wstring str;
	getline(in, str);
    int size = stoi(str);
    parameters.resize(size + 1);

    vector<wstring> splitted;
	for (int it = 0; it < size; ++it)
	{
		getline(in, str);
        splitted = Tools::Split(str, L"\t");
        alphabet->LookUpIndex_Adding(splitted[0]);
        parameters[it] = stod(splitted[1]);
	}
    alphabet->LookUpIndex_Adding(Tools::Alphabet::UNBELIEVABLE_ELEMENT);
    parameters.back() = -std::numeric_limits<double>::max();
    in.close();
}

S_T_PARSER_TEMPLATE
void S_T_PARSER_TEMPLATE1::calculateEdgeFeatures(
    const SyntaxTree& tree, vector<vector<IntBatchType> >* features)
{
    // Calculate features in string representation
    vector<vector<StringBatch> > featuresString;
    featureCalculator->CalculateFeatures(tree, &featuresString);
    
    // Calculate features in int representation
    features->resize(featuresString.size());
    for (size_t it = 0; it < features->size(); ++it)
    {
        (*features)[it].resize(featuresString[it].size());
        for (size_t jt = 0; jt < featuresString[it].size(); ++jt)
        {
            (*features)[it][jt] = Translate(featuresString[it][jt], alphabet, false);
        }
    }
}

S_T_PARSER_TEMPLATE
vector<typename S_T_PARSER_TEMPLATE1::IntBatchType> S_T_PARSER_TEMPLATE1::calculateRootFeatures(
    const SyntaxTree& tree)
{
    // Calculate string root features
    vector<StringBatch> rootFeatures
        = rootFeatureCalculator.Calculate(tree);
    vector<IntBatchType> rootFeatureIndexes(rootFeatures.size());
    // Iterate over roots
    for (size_t rootIndex = 0; rootIndex < rootFeatures.size(); ++rootIndex)
    {
        rootFeatureIndexes[rootIndex] = Translate(
            rootFeatures[rootIndex], alphabet, false);
    }
    return rootFeatureIndexes;
}

S_T_PARSER_TEMPLATE
vector<double> S_T_PARSER_TEMPLATE1::calculateRootWeights(
    const vector<IntBatchType>& rootFeatures)
{
    vector<double> rootWeights(rootFeatures.size());
    for (size_t rootIndex = 0; rootIndex < rootFeatures.size(); ++rootIndex)
    {
        rootWeights[rootIndex] = weightCalculator.CalculateLogWeight(
            rootFeatures[rootIndex], parameters);
    }
    return rootWeights;
}

S_T_PARSER_TEMPLATE
vector<vector<double> > S_T_PARSER_TEMPLATE1::calculateEdgeWeights(
    const vector<vector<IntBatchType> >& features)
{
    size_t size = features.size();
    vector<vector<double> > edgeWeights(size);
    for (size_t it = 0; it < size; ++it)
    {
        edgeWeights[it].resize(size);
        for (size_t jt = 0; jt < size; ++jt)
        {
            if (it == jt)
            {
                continue;
            }
            edgeWeights[it][jt] = weightCalculator.CalculateLogWeight(
                features[it][jt], parameters);
        }
    }
    return edgeWeights;
}

S_T_PARSER_TEMPLATE
void S_T_PARSER_TEMPLATE1::SetConnections(
    SyntaxTree* treeWithoutDependencies)
{     
    // Calculate edge weights
    vector<vector<IntBatchType> > edgeFeatures;
    calculateEdgeFeatures(*treeWithoutDependencies, &edgeFeatures);
    vector<vector<double> > edgeWeights = calculateEdgeWeights(edgeFeatures);

    // Calculate root weights
    vector<IntBatchType> rootFeatures = calculateRootFeatures(
        *treeWithoutDependencies);
    vector<double> rootWeights = calculateRootWeights(rootFeatures);
    
    // Find edges of the tree
    dependencySearcher.FindDependencies(
        treeWithoutDependencies
        , edgeWeights, rootWeights);
}

S_T_PARSER_TEMPLATE
void S_T_PARSER_TEMPLATE1::GetDependencyMatrix(
    SyntaxTree* treeWithoutDependencies
    , vector<vector<double> >* edgeWeights
    , vector<double>* rootWeights)
{
    // Calculate edge weights
    vector<vector<IntBatchType> > edgeFeatures;
    calculateEdgeFeatures(*treeWithoutDependencies, &edgeFeatures);
    *edgeWeights = calculateEdgeWeights(edgeFeatures);

    // Calculate root weights
    vector<IntBatchType> rootFeatures = calculateRootFeatures(
        *treeWithoutDependencies);
    *rootWeights = calculateRootWeights(rootFeatures);
}

S_T_PARSER_TEMPLATE
double S_T_PARSER_TEMPLATE1::CalculateWeight(
    const SyntaxTree& tree)
{   
    // Calculate features in int representation
    vector<vector<IntBatchType> > features;
    calculateEdgeFeatures(tree, &features);

    // Calculate root features in int representation
    vector<IntBatchType> rootFeatures
        = calculateRootFeatures(tree);

    double weight = weightCalculator.CalculateLogWeight(
        rootFeatures[tree.GetRootIndex()], parameters);
    const vector<SyntaxNode>& nodes = tree.GetNodes();
    for (int it = 0; it < tree.GetSize(); ++it)
    {
        for (size_t jt = 0; jt < nodes[it].neighbours.size(); ++jt)
        {
            int left = it;
            int right = nodes[it].neighbours[jt];
            weight += weightCalculator.CalculateLogWeight(
                features[left][right], parameters);
        }
    }
    return weight;
}

};

#endif // CONNECTION_BUILDER
