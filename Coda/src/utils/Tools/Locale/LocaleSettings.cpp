#include "LocaleSettings.hpp"

namespace Tools
{

void PrepareConsole(Tools::Language language)
{
    const std::string& localeString = Tools::LanguageToLocaleString(language);
    #ifdef MSVC
        system("chcp 1251");
//        std::locale::global(std::locale(localeString.c_str()));
		std::locale::global(std::locale(std::locale("Russian_Russia.1251"), "C", std::locale::numeric));
//		std::locale::global(std::locale(std::locale("C")));
    #else
        setlocale(LC_ALL, "");
        std::locale locale(localeString.c_str());
        locale = std::locale(locale, new no_separator<wchar_t>);
        locale = std::locale(locale, new no_separator<char>);
        std::locale::global(locale);
    #endif
}

}
