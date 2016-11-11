#include "LanguageNotRegisteredException.h"

namespace SyntaxParser
{

LanguageNotRegisteredException::LanguageNotRegisteredException(Tools::Language language)
    : language(language)
{
    wstring message = L"You should register your language settings (template dependent): ";
    message += Tools::ConvertStringToWstring(Tools::LanguageToString(language));
    message += L" in ISyntaxParser::GetSyntaxParser(Language language)";
    std::wcout << message;
}

const char* LanguageNotRegisteredException::what() const noexcept
{
    string message = "You should register your language settings (template dependent): ";
    message += Tools::LanguageToString(language);
    message += " in ISyntaxParser::GetSyntaxParser(Language language)";
    return message.c_str();
}

}

