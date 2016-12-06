#include "SyntaxParser.hpp"
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

}

}
