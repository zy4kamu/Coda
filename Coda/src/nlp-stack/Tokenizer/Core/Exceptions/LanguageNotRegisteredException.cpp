#include "LanguageNotRegisteredException.h"

namespace Tokenization
{

LanguageNotRegisteredException::LanguageNotRegisteredException(Tools::Language language)
    : language(language)
{
}

const char* LanguageNotRegisteredException::what() const NOEXCEPT
{
    string message = "You should register your language settings (TokenizerCreator.cpp): ";
    message += Tools::LanguageToString(language);
    message += " in TokenizerCreator::TokenizerCreator()";
    return message.c_str();
}

}

