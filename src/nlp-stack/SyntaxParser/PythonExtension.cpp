#include "SyntaxParser.hpp"
#include "Drawer.h"
#include "Disambiguator/PythonExtension.h"

extern "C"
{

namespace SyntaxParser
{

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
    drawer.Draw(currentTree, openPDF);
}

}

}
