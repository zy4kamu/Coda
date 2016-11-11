#include "LanguageNotRegisteredException.h"

namespace _sentence_splitter
{

LanguageNotRegisteredException::LanguageNotRegisteredException(Tools::Language language)
    : language(language)
{
}

const char* LanguageNotRegisteredException::what() const NOEXCEPT
{
    string message = "You should register your language settings (SSPlitterCreator.cpp): ";
    message += Tools::LanguageToString(language);
    message += " in SSPlitterCreator::SSPlitterCreator()";
    return message.c_str();
}

}

