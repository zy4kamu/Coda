#include "SyntaxParser.hpp"

extern "C"
{

namespace SyntaxParser
{

void CreateSyntaxParser(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    Tools::PrepareConsole(language);
    shared_ptr<ISyntaxParser> parser = ISyntaxParser::GetSyntaxParser(language);
}

}

}
