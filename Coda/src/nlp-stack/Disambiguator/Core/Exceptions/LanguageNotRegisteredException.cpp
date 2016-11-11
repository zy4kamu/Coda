#include "LanguageNotRegisteredException.h"

namespace Disambiguation
{

LanguageNotRegisteredException::LanguageNotRegisteredException(Tools::Language language)
    : language(language)
{
    wstring message = L"You should register your language settings (IDiasmbiguatorSettings): ";
    message += Tools::ConvertStringToWstring(Tools::LanguageToString(language));
    message += L" in DisambiguatorCreator::DisambiguatorCreator()";
    std::wcout << message;
}

const char* LanguageNotRegisteredException::what() const NOEXCEPT 
{
    string message = "You should register your language settings (IDiasmbiguatorSettings): ";
    message += Tools::LanguageToString(language);
    message += " in DisambiguatorCreator::DisambiguatorCreator()";
    return message.c_str();
}

}
