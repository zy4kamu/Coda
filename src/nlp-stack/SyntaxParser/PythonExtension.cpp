#include "PythonExtension.h"
#include "Drawer.h"
#include "Disambiguator/PythonExtension.h"

extern "C"
{

namespace SyntaxParser
{

/* FUNCTIONS RELATED TO SYNTAX PARSER */

SyntaxTree currentTree;

void CreateSyntaxParser(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    Tools::PrepareConsole(language);
    shared_ptr<ISyntaxParser> parser = ISyntaxParser::GetSyntaxParser(language);
}

void SyntaxParse(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    Tools::PrepareConsole(language);
    shared_ptr<ISyntaxParser> parser = ISyntaxParser::GetSyntaxParser(language);
    currentTree = parser->Parse(Disambiguation::parsedDisambiguated);
}

int GetRootIndex()
{
    return currentTree.GetRootIndex();
}

const wchar_t* GetSentence()
{
    return currentTree.GetSentence().c_str();
}

int GetParentIndex(size_t tokenIndex)
{
    return currentTree.GetNodes()[tokenIndex].parentIndex;
}

void Draw(const char* outputFile, bool openPDF)
{
    Drawer drawer(outputFile);
    drawer.Draw(parsedTree, openPDF);
}

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

SyntaxTree parsedTree;

void CreateTreeFromParsedDisambiguated()
{
    parsedTree = SyntaxTree(Disambiguation::parsedDisambiguated);
}

void SetParent(int index, int parentIndex)
{
    vector<SyntaxNode>& nodes = parsedTree.GetNodes();
    nodes[index].parentIndex = parentIndex;
    if (parentIndex == -1) {
        parsedTree.SetRoot(index);
    } else {
        nodes[parentIndex].neighbours.push_back(index);
    }
}

}

}
