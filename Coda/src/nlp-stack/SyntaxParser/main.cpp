#include "Controller.hpp"
#include "SyntaxSettings.h"
#include "SyntaxSettingsEn.h"
#include "SyntaxSettingsRus.h"

using SyntaxParser::Controller;
using SyntaxParser::SyntaxSettingsRus;

using namespace SyntaxParser;

void DealWithEnglishPunctuation(const string& fileIn, const string& fileTo);

template<class Settings>
void Run()
{
    Controller<Settings> controller;
    while (true)
    {
        wcout << L"Input input: learn, cut, prepare, test_on_train, test_on_test, test_on_trees or real_test: ";
        wstring input;
        getline(std::wcin, input);
        if (input == L"learn") {
            controller.Learn();
        } else if (input == L"test_on_train") {
            controller.Test(Settings::SYNTAX_TRAINING_SET_PATH);
        } else if (input == L"test_on_test") {
            controller.Test(Settings::SYNTAX_TEST_SET_PATH);
        } else if (input == L"test_on_trees") {
            controller.TestOnTrees();
        } else if (input == L"real_test") {
            controller.Test();
        } else if (input == L"prepare") {
            controller.Prepare();
        } else if (input == L"cut") {
            controller.Cut();
        } else {
            wcout << "Wrong input" << std::endl;
        }
    }
}

int main()
{
    Tools::PrepareConsole();
    std::wcout << L"Input language: ru, en" << std::endl;
    wstring language;
    std::getline(std::wcin, language);
    if (language == L"ru") {
        Run<SyntaxSettingsRus>();
    } else if (language == L"en") {
        Run<SyntaxSettingsEn>();
    } else {
        std::wcout << L"wrong language..." << std::endl;
    }
}

/********************* TO DELETE ***************************/

void Translate(const string& fileIn, const string& fileTo)
{
    wifstream in(fileIn);
    wofstream out(fileTo);
    wstring line;
    while (getline(in, line))
    {
        if (line.size() > 0) {
            SyntaxTree tree(line);
            out << tree.ToReadableString(true) << std::endl;
        } else {
            out << std::endl;
        }
    }
    in.close();
    out.close();
    std::wcout << L"Ready!!!";
}

bool ShouldDrop(const SyntaxNode& node)
{
    return node.label == L"``"
        || node.label == L"''"
        || node.label == L"."
        || node.label == L"-LRB-"
        || node.label == L":"
        || node.label == L"COMMA"
        || node.label == L"-RRB-";
}

SyntaxTree CorrectEnglishTree(const SyntaxTree& tree)
{
    vector<SyntaxNode> oldNodes = tree.GetNodes();
    vector<SyntaxNode> newNodes;
    vector<int> newToOld;
    map<int, int> oldToNew;
    vector<wstring> firstOrderPunct;
    for (size_t i = 0; i < oldNodes.size(); ++i)
    {
        if (!ShouldDrop(oldNodes[i]))
        {
            oldToNew[i] = newToOld.size();
            newToOld.push_back(i);
            newNodes.push_back(oldNodes[i]);
            newNodes.back().index = newNodes.size();
        } else {
            if (newNodes.size() > 0) {
                newNodes.back().punctuation.push_back(oldNodes[i].content);
            } else {
                firstOrderPunct.push_back(oldNodes[i].content);
            }
        }
    }
    for (wstring str : firstOrderPunct)
    {
       newNodes[0].punctuation.push_back(str);
    }
    for (size_t i = 0; i < newNodes.size(); ++i)
    {
        SyntaxNode& node = newNodes[i];
        if (node.parentIndex != -1)
        {
            node.parentIndex = oldToNew[node.parentIndex];
        }
        vector<int>& neighbours = node.neighbours;
        for (size_t j = 0; j < neighbours.size(); ++j)
        {
            neighbours[j] = oldToNew[neighbours[j]];
        }
    }
    wstring sentence = tree.GetSentence();
    int rootIndex = oldToNew[tree.GetRootIndex()];
    return SyntaxTree(rootIndex, std::move(newNodes), std::move(sentence));
}


void DealWithEnglishPunctuation(const string& fileIn, const string& fileTo)
{
    wifstream in(fileIn);
    wofstream out(fileTo);
    wstring line;
    while (getline(in, line))
    {
        if (Tools::StartsWith(line, L"0	-1	``	``	``~1	2	Spain	spain	NNP~2	0	is	be	VBZ~3"))
        {
            std::wcout << L"";
        }
        SyntaxTree tree(line);
        tree = CorrectEnglishTree(line);
        out << tree.ToReadableString(true) << std::endl;
    }
    in.close();
    out.close();
    std::wcout << L"Ready!!!";
}
