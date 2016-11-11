/**
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

#ifndef AHO_CORASICK
#define AHO_CORASICK

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

#include "Tools.h"

namespace Tools
{

using std::vector;
using std::pair;
using std::unordered_map;

/* DEFINITIONS */

template<class Char>
struct AhoCorasickNode;

template<class String>
class AhoCorasickParser
{
public:
    AhoCorasickParser(const vector<String>& input);
    ~AhoCorasickParser();
    vector<pair<size_t, size_t> > Match(const String& toMatch);
private:
    typedef typename String::value_type Char;
    typedef AhoCorasickNode<Char> Node;

    Node* addNode(Node* node, Char value);
    Node* makeTransition(Node* node, Char value);
    void getMatches(Node* node, size_t index
        , vector<pair<size_t, size_t> >* matched);

    Node* root;
    vector<size_t> lengths;
};

/* REALIZATION */

template<class Char>
struct AhoCorasickNode
{

    AhoCorasickNode();
    AhoCorasickNode(AhoCorasickNode<Char>* parent
        , Char value, AhoCorasickNode<Char>* root);
    ~AhoCorasickNode();

    unordered_map<char, AhoCorasickNode<Char>* > transitions;
    size_t matchIndex; // -1 if not terminal
    AhoCorasickNode<Char>* suffixReference;
    AhoCorasickNode<Char>* finalReference;
};

template<class Char>
AhoCorasickNode<Char>::AhoCorasickNode()
    : matchIndex(-1)
    , suffixReference(0)
    , finalReference(0)
{
}

template<class Char>
AhoCorasickNode<Char>::AhoCorasickNode(
    AhoCorasickNode<Char>* parent, Char value
    , AhoCorasickNode<Char>* root)
    : matchIndex(-1)
    , suffixReference(root)
    , finalReference(root)
{
    parent->transitions[value] = this;
    AhoCorasickNode* sufRef = parent->suffixReference;
    while (sufRef != 0)
    {
        auto found = sufRef->transitions.find(value);
        if (found != sufRef->transitions.end()) {
            suffixReference = found->second;
            if (suffixReference->matchIndex != size_t(-1)) {
                finalReference = suffixReference;
            } else {
                finalReference = suffixReference->finalReference;
            }
            return;
        } else {
            sufRef = sufRef->suffixReference;
        }
    }
}

template<class Char>
AhoCorasickNode<Char>::~AhoCorasickNode()
{
    for (auto trans : transitions)
    {
        delete trans.second;
    }
}

template<class String>
AhoCorasickParser<String>::AhoCorasickParser(
    const vector<String>& input)
    : lengths(input.size())
{
    std::transform(input.begin(), input.end(), lengths.begin()
        , [](const String& str) { return str.size(); });

    root = new Node;
    vector<Node*> nodes(input.size(), root);

    vector<size_t> range = Tools::Range<size_t>(0, input.size());
    std::unordered_set<size_t> inputIndexes(range.begin(), range.end());

    size_t position = 0;
    while (!inputIndexes.empty())
    {
        auto itr = inputIndexes.begin();
        while (itr != inputIndexes.end()) {
            size_t patternIndex = *itr;
            nodes[patternIndex] = addNode(nodes[patternIndex], input[patternIndex][position]);
            if (position + 1 == input[patternIndex].size()) {
                nodes[patternIndex]->matchIndex = patternIndex;
                inputIndexes.erase(itr++);
            } else {
                ++itr;
            }
        }
        ++position;
    }
}

template<class String>
AhoCorasickParser<String>::~AhoCorasickParser()
{
    delete root;
}

template<class String>
AhoCorasickNode<typename String::value_type>*
AhoCorasickParser<String>::addNode(
    AhoCorasickNode<typename String::value_type>* node
    , typename String::value_type value)
{
    auto found = node->transitions.find(value);
    if (found != node->transitions.end())
    {
        return found->second;
    }
    return new Node(node, value, root);
}

template<class String>
vector<pair<size_t, size_t> > AhoCorasickParser<String>::Match(
    const String& toMatch)
{
    vector<pair<size_t, size_t> > matched;
    Node* current = root;
    for (size_t i = 0; i < toMatch.size(); ++i)
    {
        current = makeTransition(current, toMatch[i]);
        getMatches(current, i, &matched);
    }
    return matched;
}

template<class String>
AhoCorasickNode<typename String::value_type>*
AhoCorasickParser<String>::makeTransition(
    AhoCorasickNode<typename String::value_type>* node, Char value)
{
    while (node != 0)
    {
        auto found = node->transitions.find(value);
        if (found != node->transitions.end()) {
            return found->second;
        } else {
            node = node->suffixReference;
        }
    }
    return root;
}

template<class String>
void AhoCorasickParser<String>::getMatches(
    AhoCorasickNode<typename String::value_type>* node
    , size_t index, vector<pair<size_t, size_t> >* matched)
{
    while (node != root)
    {
        if (node->matchIndex != size_t(-1))
        {
            matched->emplace_back(index - lengths[node->matchIndex] + 1, node->matchIndex);
        }
        node = node->finalReference;
    }
}

/* STRESS TESTS */

template<class String>
vector<pair<size_t, size_t> > StupidMatch(
    const vector<String>& input, const String& toMatch)
{
    vector<pair<size_t, size_t> > matched;
    for (size_t start = 0; start < toMatch.size(); ++start)
    {
        for (size_t strIndex = 0; strIndex < input.size(); ++strIndex)
        {
            bool got = true;
            for (size_t i = 0; i < input[strIndex].size(); ++i)
            {
                if (start + i >= toMatch.size())
                {
                    got = false;
                    break;
                }
                if (input[strIndex][i] != toMatch[start + i])
                {
                    got = false;
                    break;
                }
            }
            if (got)
            {
                matched.emplace_back(start, strIndex);
            }
        }
    }
    std::sort(matched.begin(), matched.end());
    return matched;
}

template<class String>
vector<String> GeneratePatterns()
{
    size_t letterNum = std::rand() % 4 + 1;
    vector<String> str(std::rand() % 5 + 1);
    for (size_t i = 0; i < str.size(); ++i)
    {
        str[i].resize(std::rand() % 3 + 1);
        for (size_t j = 0; j < str[i].size(); ++j)
        {
            str[i][j] = 'a' + std::rand() % letterNum;
        }
    }
    std::sort(str.begin(), str.end());
    str.erase(std::unique(str.begin(), str.end()), str.end());
    return str;
}

template<class String>
String GenerateString()
{
    size_t letterNum = std::rand() % 4 + 1;
    String str(std::rand() % 100, 'a');
    for (size_t i = 0; i < str.size(); ++i)
    {
        str[i] = 'a' + std::rand() % letterNum;
    }
    return str;
}

template<class String>
void TestAhoCorasick()
{
    size_t numTests = 100000;
    size_t passedTests = 0;
    for (size_t i = 0; i < numTests; ++i)
    {
        String str = GenerateString<String>();
        vector<String> patterns = GeneratePatterns<String>();
        vector<pair<size_t, size_t> > stupidMatch = StupidMatch(patterns, str);
        AhoCorasickParser<String> parser(patterns);
        vector<pair<size_t, size_t> > match = parser.Match(str);
        std::sort(match.begin(), match.end());
        if (stupidMatch == match)
        {
            ++passedTests;
        }
    }
    std::wcout << L"Aho-Corasick passed " << passedTests << L" tests of " << numTests << std::endl;
}


}

#endif // AHO_CORASICK
