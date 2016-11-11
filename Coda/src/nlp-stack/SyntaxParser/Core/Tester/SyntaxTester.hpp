/** 
 * @file	SyntaxTester.h
 * @brief	header file for accuracy tester of syntax parsing (parses file and prints statistics)
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


#ifndef SYNTAXTREE_TESTER
#define SYNTAXTREE_TESTER

#include <string>
#include <memory>

#include "Tools.h"
#include "SyntaxTree.h"

using std::string;
using std::shared_ptr;

#define SYNTAXTREE_TESTER_TEMPLATE template < \
    class Parser \
    , class MorphologyRecoverer >

#define SYNTAXTREE_TESTER_TEMPLATE1 SyntaxTester < \
    Parser \
    , MorphologyRecoverer >


namespace SyntaxParser
{

static const double EPS = 1e-5;

/** 
* @class	SyntaxTester
* @brief	Reads syntax trees from file, calculates and prints statistics of its evaluation
*/
SYNTAXTREE_TESTER_TEMPLATE
class SyntaxTester
{
public:
    typedef Parser ParserType;

    /**
     * @brief	Calculates number of coincied edges between two syntax trees
     * @param[in]	first - first syntax tree
     * @param[in]	second - second syntax tree
     */
    static int CalculateNumberOfCoincidence(
        const SyntaxTree& first
        , const SyntaxTree& second
        , unordered_map<wstring, std::pair<double, double> >* posStatistics);

    /**
     * @brief	Constructor
     * @param[in]	parser - recovers connections in syntax tree
     */
    SyntaxTester(shared_ptr<Parser> parser, Tools::Language language);

    /**
     * @brief	Destructor
     */
    ~SyntaxTester(void);

    /**
     * @brief	tests from file
     * @param[in]	file - input file
     */
    void Test(const string& file);
private:
    MorphologyRecoverer morphologyRecoverer; /**< gets morphology from real syntax tree*/
    shared_ptr<Parser> parser; /**< recovers connections in syntax tree*/

    /**
     * @brief	Enables to check if sentence is OK (we use only OK sentence for sentence)
     * @param[in]	tree - syntax tree to chaeck
     * @param[out]	result - true if tree is OK, false otherwise
     */
    bool isNormalSentence(const SyntaxTree& tree);
};

SYNTAXTREE_TESTER_TEMPLATE
SYNTAXTREE_TESTER_TEMPLATE1::SyntaxTester(
    shared_ptr<Parser> parser, Tools::Language language) :
    morphologyRecoverer(language), parser(parser)
{
}


SYNTAXTREE_TESTER_TEMPLATE
SYNTAXTREE_TESTER_TEMPLATE1::~SyntaxTester(void)
{
}

SYNTAXTREE_TESTER_TEMPLATE
bool SYNTAXTREE_TESTER_TEMPLATE1::isNormalSentence(const SyntaxTree& tree)
{
//    const vector<SyntaxNode>& nodes = tree.GetNodes();
//    for (size_t it = 0; it < nodes.size(); ++it)
//    {
//        if (Contains(nodes[it].token, L"-") ||
//            Contains(nodes[it].token, L"(") ||
//            Contains(nodes[it].token, L"\""))
//        {
//            return false;
//        }
//    }
    return true;
}

SYNTAXTREE_TESTER_TEMPLATE
void SYNTAXTREE_TESTER_TEMPLATE1::Test(const string& file)
{
    unordered_map<wstring, std::pair<double, double> > posStatistics;
    int totalNumCoincidence = 0;
    int totalNumTokens = 0;
    int totalNumCoincidence_perfectSearch = 0;
    wifstream in(file);
    wstring line;
    int sum = 0;
    while(getline(in, line))
    {
        ++sum;
        SyntaxTree realTree(line);
        if (!isNormalSentence(realTree))
        {
            continue;
        }
        vector<Disambiguation::DisambiguatedData> disambiguated
            = morphologyRecoverer.GetMorphology(realTree);
        SyntaxTree predictedTree = parser->Parse(disambiguated);
        if (predictedTree.GetSize() != realTree.GetSize()) continue;
        int numCoincidence = CalculateNumberOfCoincidence(realTree, predictedTree, &posStatistics);
        totalNumCoincidence += numCoincidence;
        totalNumTokens += realTree.GetSize();
        totalNumCoincidence_perfectSearch += 
            (parser->CalculateWeight(realTree) > parser->CalculateWeight(predictedTree) + EPS) ?
            realTree.GetSize() : numCoincidence;
        if (sum % 100 == 0)
        {
            std::wcout << L"Coincidens = " << totalNumCoincidence
                  << L"; Cheat coincidens = " << totalNumCoincidence_perfectSearch 
                  << L"; Total = " << totalNumTokens 
                  << L"; Ratio = " << static_cast<double>(totalNumCoincidence) / 
                      static_cast<double>(totalNumTokens)
                  << L"; Cheat ratio = " << static_cast<double>(totalNumCoincidence_perfectSearch) / 
                      static_cast<double>(totalNumTokens)
                  << std::endl;
        }
    }
    std::wcout
          << L"Coincidens = " << totalNumCoincidence
          << L"; Total = " << totalNumTokens 
          << L"; Ratio = " << static_cast<double>(totalNumCoincidence) / 
              static_cast<double>(totalNumTokens)
          << std::endl << std::endl;

    std::wcout << L"POS mistakes distribution: " << std::endl;
    std::wcout << L"\tPOS\tAccuracy\tMistake" << std::endl;
    for (auto& item : posStatistics)
    {
        std::wcout
            << L"\t" << item.first
            << L"\t" << 100.0 - 100.0 * item.second.second / item.second.first << L"%"
            << L"\t" << 100.0 * item.second.second / totalNumTokens << L"%" << std::endl;
    }
    in.close();
}

SYNTAXTREE_TESTER_TEMPLATE
int SYNTAXTREE_TESTER_TEMPLATE1::CalculateNumberOfCoincidence(
    const SyntaxTree& first, const SyntaxTree& second
    , unordered_map<wstring, std::pair<double, double> >* posStatistics)
{
    int numCoincidence = 0;
    for (int nodeIndex = 0; nodeIndex < first.GetSize(); ++nodeIndex)
    {
        wstring pos = Tools::Split(first.GetNodes()[nodeIndex].label, L"@")[0];
        auto found = posStatistics->find(pos);
        if (found == posStatistics->end()) {
            (*posStatistics)[pos] = std::pair<double, double>(1, 0);
            found = posStatistics->find(pos);
        } else {
            ++(found->second.first);
        }
        if (first.GetNodes()[nodeIndex].parentIndex == second.GetNodes()[nodeIndex].parentIndex) {
            ++numCoincidence;
        } else {
            ++(found->second.second);
        }
    }
    return numCoincidence;
}

}

#endif // SYNTAXTREE_TESTER

