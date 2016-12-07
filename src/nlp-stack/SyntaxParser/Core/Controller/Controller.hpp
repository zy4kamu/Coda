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

#ifndef SYNTAX_CONTROLLER
#define SYNTAX_CONTROLLER

#include "Drawer.h"
#include "IDisambiguator.h"
#include "ISyntaxParser.h"
#include "ITokenizer.h"
#include "SyntaxCreator.h"
#include "SyntaxModel.h"
#include "SyntaxSettings.h"

namespace SyntaxParser
{

using Disambiguation::IDisambiguator;
using Tokenization::ITokenizer;

template<class _Settings>
class Controller
{
public:
    typedef SyntaxSettings<_Settings> Settings;
    typedef typename Settings::Learner Learner;
    typedef typename Settings::Parser Parser;
    typedef typename Settings::TesterOnPerfectMorphology Tester;

    Controller();

    void Learn();

    void Test();

    void TestOnTrees();

    void Test(const string& file);

    void Cut();

    void Prepare();

    Tools::Language GetLanguage();
private:
    Drawer drawer;
    shared_ptr<ITokenizer> tokenizer;
    shared_ptr<IDisambiguator> disambiguator;
    shared_ptr<ISyntaxParser> parser;
};

template<class _Settings>
Controller<_Settings>::Controller()
{
}

template<class _Settings>
void Controller<_Settings>::Learn()
{
    Learner pool(
        Settings::GetTrainingSetPath()
        , Settings::GetTestSetPath()
        , Settings::GetModelPath()
        , this->GetLanguage());
    pool.Learn(1750, 30, 1000000);
}

template<class _Settings>
void Controller<_Settings>::Test()
{
    tokenizer = ITokenizer::GetTokenizer(this->GetLanguage());
    disambiguator = IDisambiguator::GetDisambiguator(this->GetLanguage());
    parser = ISyntaxParser::GetSyntaxParser(this->GetLanguage());
    clock_t t0, t1;
    wstring sentence;
    // Run dialog
    while(true)
    {
        wcout << "Input sentence:" << endl;
        getline(std::wcin, sentence);
        if (sentence == L"exit") break;
        wcout << endl;
        wcout << sentence << endl;
        t0 = clock();
        vector<Token> tokens = tokenizer->Tokenize(sentence);
        vector<DisambiguatedData> disambiguated = disambiguator->Disambiguate(tokens);
        SyntaxTree tree = parser->Parse(disambiguated);
        t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl << endl;
        drawer.Draw(tree);
    }
}

template<class _Settings>
void Controller<_Settings>::TestOnTrees()
{
    parser = ISyntaxParser::GetSyntaxParser(this->GetLanguage());
    wstring treeString;
    clock_t t0, t1;
    while(true)
    {
        wcout << "Input sentence:" << endl;
        getline(std::wcin, treeString);
        if (treeString == L"exit") break;
        SyntaxTree realTree(treeString);
        SyntaxTree predictedTree(realTree);
        t0 = clock();
        parser->SetConnections(&predictedTree);
        t1 = clock();
        wcout << L"time in seconds: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl << endl;
        drawer.Draw(realTree, predictedTree);
    }
}

template<class _Settings>
void Controller<_Settings>::Test(const string& file)
{
    shared_ptr<Parser> parser = std::make_shared<Parser>(
        Settings::GetModelPath()
        , this->GetLanguage());
    Tester tester(parser, this->GetLanguage());
    clock_t t0 = clock();
    tester.Test(file);
    clock_t t1 = clock();
    wcout << L"time in seconds: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl << endl;
}

template<class _Settings>
void Controller<_Settings>::Cut()
{
    SyntaxModel model;
    model.ReadFromText("/data/Coda/config/SyntaxParser/Config_Linux/RU/Model_Output1.txt");
    model.Cut(0.05);
    model.WriteToText("/data/Coda/config/SyntaxParser/Config_Linux/RU/Model_Output.txt");
}

template<class _Settings>
void Controller<_Settings>::Prepare()
{
    if (_Settings::dataPreparer != 0)
    {
        _Settings::dataPreparer->Prepare();
    }
}

template<class _Settings>
Tools::Language Controller<_Settings>::GetLanguage()
{
    return Settings::GetLanguage();
}


}

#endif // SYNTAX_CONTROLLER
