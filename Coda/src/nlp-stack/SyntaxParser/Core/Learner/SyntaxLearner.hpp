/** 
 * @file	SyntaxLearner.h
 * @brief	header file for optimizable container of syntax trees
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


#ifndef SYNTAX_TREE_LEARNER
#define SYNTAX_TREE_LEARNER

#include <omp.h>

#include "FileReader.h"
#include "IOptimizable.h"
#include "ICallable.h"
#include "SimpleSyntaxTreeOptimizable.hpp"
#include "UblasException.h"

#define SYNTAX_TREE_LEARNER_TEMPLATE template < \
    class TreeOptimizable \
    , class Optimizer \
    , class Tester>

#define SYNTAX_TREE_LEARNER_TEMPLATE1 SyntaxLearner< \
    TreeOptimizable, Optimizer, Tester>

namespace SyntaxParser
{

const int NUMBER_OF_THREADS = omp_get_num_procs();

/** 
* @class	SyntaxLearner
* @brief	Optimizes weights for container of syntax trees
 */
SYNTAX_TREE_LEARNER_TEMPLATE
class SyntaxLearner
    : public Optimization::IOptimizable
    , public Optimization::ICallable
{
public:
    /**
     * @brief	Constructor
     * @param[in]	trainFile - each line of this file contains one syntax tree
     * @param[in]	validFile - each line of this file contains one syntax tree
     * @param[in]	modelFile - file with model
     */
    SyntaxLearner(
        const string& trainFile
        , const string& validFile
        , const string& modelFile
        , Tools::Language language);

    /**
     * @brief	Runs optimization procedure to find opitmal weights
     * @param[in]	numberOfIterations - max number of iterations of the algorithm
     * @param[in]	periodOfTesting - period of iterations when preliminary tester starts evaluation
     * @param[in]	periodOfPriniting - period of iterations when the model is saved to auxiliary file
     */
    void Learn(int numberOfIterations, int periodOfTesting
               , int periodOfPriniting);

    /**
     * @brief	Prints found model to file
     * @param[in]	file - output file
     */
    void PrintToFile(const string& file);

    /**
     * @brief	Cacluates log probability of the model
     * @param[in]	point - current parameters of the model
     */
    double CalculateValue(const vector<double>& point);

    /**
     * @brief	Cacluates gradient of log probability of the model
     * @param[in]	point - current parameters of the model
     * @param[in]	gradient - gradient to update
     */
    void CalculateGradient(const vector<double>& point
        , vector<double>* gradient);

    /**
     * @brief	Evaluates auxiliary results
     * @param[in]	currentIteration - iteration of gradient calculations
     * @param[in]	_parameters - current parameters of the model
     * @param[in]	dimension - size of parameters
     */
    bool Call(int currentIteration
        , const double* _parameters
        , int dimension);

private:
    /**
     * @brief	Enables to skip some sentences during training
     * @param[in]	tree - syntax tree to check for correctness
     */
    bool isNormalSentence(const wstring& tree);

    shared_ptr<Tools::Alphabet> alphabet; /**< translates string features to indexes*/
    vector<TreeOptimizable> trees; /**< set of syntax tree for optimization*/
    vector<double> parameters; /**< parameters of the model*/
    double sigmaFactor; /**< regularization factor*/
    string validFile; /**< file with data to test */
    string modelFile; /**< if some exception happens during training, the model is printed to this file, and program terminates*/
    int periodOfTesting; /**< period of iterations when preliminary tester starts evaluation*/
    int periodOfPrinting; /**< period of iterations when the model is saved to auxiliary file*/
    Tools::Language language;
};

SYNTAX_TREE_LEARNER_TEMPLATE
bool SYNTAX_TREE_LEARNER_TEMPLATE1::isNormalSentence(const wstring& line)
{
    SyntaxTree tree(line);
//    if (tree.GetSize() > 20) {
//        std::wcout << L"Bad sentence: " << tree.GetSize() << std::endl;
//        return false;
//    }
    return true;
}

SYNTAX_TREE_LEARNER_TEMPLATE
SYNTAX_TREE_LEARNER_TEMPLATE1::SyntaxLearner(
    const string& trainFile
    , const string& validFile
    , const string& modelFile
    , Tools::Language language) :
    alphabet(new Tools::Alphabet())
    , sigmaFactor(1000000000) //250000???
    , validFile(validFile)
    , modelFile(modelFile)
    , language(language)
{
    clock_t startTime = clock();
    clock_t endTime;
    // Creating edge feature calculator
    typedef typename TreeOptimizable::EdgeFeatureCalculatorType EdgeFeatureCalculatorType;
    shared_ptr<EdgeFeatureCalculatorType> edgeFeatureCalculator
        = std::make_shared<EdgeFeatureCalculatorType>();
    // Read trees in string representation
    vector<wstring> lines;
    FileManager::Read(trainFile, &lines);
    trees.reserve(lines.size());
    // Create splittings into threads
    vector<shared_ptr<Tools::Alphabet> > splittedThreadsAlphabets(NUMBER_OF_THREADS);
    for (int it = 0; it < NUMBER_OF_THREADS; ++it)
    {
        splittedThreadsAlphabets[it] = std::make_shared<Tools::Alphabet>(
            Tools::Alphabet());
    }
    vector<vector<TreeOptimizable> > splittedThreadsTrees(NUMBER_OF_THREADS);
    // Read trees
    #pragma omp parallel for
    for (int indexInDataSet = 0; indexInDataSet < static_cast<int>(lines.size())
        ; ++indexInDataSet)
    {
        int threadId = omp_get_thread_num();
        try
        {
            if (!isNormalSentence(lines[indexInDataSet]))
            {
                continue;
            }
            splittedThreadsTrees[threadId].emplace_back(
                indexInDataSet
                , lines[indexInDataSet]
                 , splittedThreadsAlphabets[threadId]
                 , edgeFeatureCalculator);
            splittedThreadsTrees[threadId].back().Save();
            splittedThreadsTrees[threadId].back().Clear();
            if (splittedThreadsTrees[threadId].size() % 100 == 0)
            {
                std::wcout << threadId << L": Parsed "
                      << splittedThreadsTrees[threadId].size() 
                      << L" trees; "
                      << L"Alphabet size = "
                      << splittedThreadsAlphabets[threadId]->Size()
                      << L"; Alphabet bytes = "
                      << splittedThreadsAlphabets[threadId]->GetByteSize()
                      << std::endl;
            }
        }
        catch (...)
        {
           std::wcout << lines[indexInDataSet] << std::endl;
        }
    }
    endTime = clock();
    std::wcout << L"Starting merging into one forest " <<
        (double)(endTime - startTime) / CLOCKS_PER_SEC << L" seconds" << std::endl;
    // Merge threads into one
    for (int threadIndex = 0; threadIndex < NUMBER_OF_THREADS; ++threadIndex)
    {
        // Merge alphabet
        for (int alphabetIndex = 0; alphabetIndex < splittedThreadsAlphabets[threadIndex]->Size();
            ++alphabetIndex)
        {
            alphabet->LookUpIndex_Adding(
                splittedThreadsAlphabets[threadIndex]->LookUpObject(alphabetIndex));
        }
        // Merge trees
        for(size_t treeIndex = 0; treeIndex < splittedThreadsTrees[threadIndex].size();
            ++treeIndex)
        {
            trees.push_back(std::move(splittedThreadsTrees[threadIndex][treeIndex]));
        }
    }
    endTime = clock();
    std::wcout << L"Setting common alphabet for all trees " <<
        (double)(endTime - startTime) / CLOCKS_PER_SEC << L" seconds" << std::endl;
    // Set merged alphabet for each tree
    #pragma omp parallel for
    for (int treeIndex = 0; treeIndex < int(trees.size()); ++treeIndex)
    {
        trees[treeIndex].Upload();
        trees[treeIndex].SetAlphabet(alphabet);
        trees[treeIndex].Save();
        trees[treeIndex].Clear();
    }
    dimension = alphabet->Size();
    endTime = clock();
    std::wcout << L"Disamension: " << dimension << std::endl;
    std::wcout << L"Have download: " << trees.size() << L" trees for " <<
        (double)(endTime - startTime) / CLOCKS_PER_SEC << L" seconds" << std::endl;
}

SYNTAX_TREE_LEARNER_TEMPLATE
double SYNTAX_TREE_LEARNER_TEMPLATE1::CalculateValue(
    const vector<double>& point)
{
    // Create threads
    vector<double> threadSplittedValues(NUMBER_OF_THREADS, 0);
    // Calculate value
    #pragma omp parallel for
    for (int it = 0; it < int(trees.size()); ++it)
    {
        try
        {
            trees[it].Upload();
            threadSplittedValues[omp_get_thread_num()] +=
                trees[it].CalculateValue(point);
            trees[it].Clear();
        }
        catch(...)
        {
            std::wcout << L"Exception during learning..." << std::endl;
            PrintToFile(modelFile);
            exit (0);
        }
    }
    // Merge into one value
    double sum = 0;
    for (size_t it = 0; it < threadSplittedValues.size(); ++it)
    {
        sum += threadSplittedValues[it];
    }
    // Calculate normFactor
    double normFactor = 0;
    for (size_t it = 0; it < point.size(); ++it) // parameters -> point ???
    {
        normFactor += point[it] * point[it];
    }
    // Return value
    return sum + double(trees.size()) * normFactor / sigmaFactor;
}

SYNTAX_TREE_LEARNER_TEMPLATE
void SYNTAX_TREE_LEARNER_TEMPLATE1::CalculateGradient(
    const vector<double>& point, vector<double>* gradient)
{
    bool areAllTreesValid = true;
    // Create threads
    vector<vector<double>> threadSplittedGradient(NUMBER_OF_THREADS);
    for (int it = 0; it < NUMBER_OF_THREADS; ++it)
    {
        threadSplittedGradient[it].resize(gradient->size());
    }
    // Calculate gradients
    #pragma omp parallel for
    for (int it = 0; it < int(trees.size()); ++it)
    {
        trees[it].Upload();
        trees[it].CalculateGradient(
            point, &threadSplittedGradient[omp_get_thread_num()]);
        if (!trees[it].IsValid())
        {
            areAllTreesValid = false;
        }
        trees[it].Clear();
    }
    // Terminate if there were problems while computing gradient
    if (!areAllTreesValid)
    {
        std::wcout << L"Printing model to file since there are errors in calculating gradient" << std::endl;
        parameters = point;
        PrintToFile(modelFile);
        exit(0);
    }
    // Merge into one gradient
    for (int threadIndex = 0; threadIndex < NUMBER_OF_THREADS; ++threadIndex)
    {
        for (size_t it = 0; it < gradient->size(); ++it)
        {
            (*gradient)[it] += threadSplittedGradient[threadIndex][it];
        }
    }
    // Adding regularization
    for (size_t it = 0; it < point.size(); ++it)
    {
        (*gradient)[it] += 2.0 * point[it] / sigmaFactor;
    }
}

SYNTAX_TREE_LEARNER_TEMPLATE
void SYNTAX_TREE_LEARNER_TEMPLATE1::Learn(int numberOfIterations
    , int _periodOfTesting
    , int _periodOfPriniting)
{
    periodOfTesting = _periodOfTesting;
    periodOfPrinting = _periodOfPriniting;
    clock_t startTime = clock();
    std::wcout << L"Starting Syntax Tree Learning" << std::endl;
    vector<double> initialPoint(dimension, 0);
    Optimizer optimizer;
    optimizer.Optimize(*this, this, initialPoint, &parameters, numberOfIterations);
    clock_t endTime = clock();
    PrintToFile(modelFile);
    std::wcout << L"Have learned for " <<
        (double)(endTime - startTime) / CLOCKS_PER_SEC << L" seconds" << std::endl;
}

SYNTAX_TREE_LEARNER_TEMPLATE
void SYNTAX_TREE_LEARNER_TEMPLATE1::PrintToFile(
    const string& file)
{
    wofstream out(file);
    out << Tools::ConvertIntToWstring(dimension) << std::endl;
    for (int it = 0; it < dimension; ++it)
    {
        out << alphabet->LookUpObject(it) << L"\t" << parameters[it] << std::endl;
    }
    out.close();
}

SYNTAX_TREE_LEARNER_TEMPLATE
bool SYNTAX_TREE_LEARNER_TEMPLATE1::Call(
        int currentIteration
        , const double* _parameters
        , int dimension)
{
    std::wcout << L"Syntax learner event handler starts..." << std::endl;
    bool shouldPrint = (currentIteration > 0) && (currentIteration % periodOfPrinting == 0);
    bool shouldTest = (currentIteration > 0) && (currentIteration % periodOfTesting == 0);
    if (shouldPrint || shouldTest)
    {
        parameters.resize(dimension);
        for (int index = 0; index < dimension; ++index)
        {
            parameters[index] = _parameters[index];
        }
        PrintToFile(modelFile);
    }
    if (shouldTest)
    {
        shared_ptr<typename Tester::ParserType> parser(
            new typename Tester::ParserType(modelFile, language));
        Tester tester(parser, language);
        tester.Test(validFile);
    }
    return false;
}

}

#endif // SYNTAX_TREE_LEARNER
