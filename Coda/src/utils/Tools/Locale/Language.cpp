#include "Language.h"
#include "Exceptions.h"

namespace Tools
{

std::string LanguageToString(Language language)
{
    switch (language)
    {
    case Language::RU:
        return "RU";
        break;
    case Language::EN:
        return "EN";
        break;
    case Language::EN_FAST:
        return "EN_FAST";
        break;
    default:
        LanguageNotFoundException exception;
        throw exception;
    }
}

std::string LanguageToLocaleString(Language language)
{
    switch (language)
    {
    case Language::RU:
        #ifdef MSVC
            return "Russian";
        #else
            return "ru_RU.UTF-8";
        #endif
        break;
    case Language::EN:
        return "en_US.UTF-8";
        break;
    case Language::EN_FAST:
        return "en_US.UTF-8";
        break;
    default:
        throw LanguageNotFoundException();
    }
}

}
