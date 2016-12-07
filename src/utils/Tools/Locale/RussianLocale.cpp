#include "RussianLocale.h"

#include <iostream>

using std::string;



namespace Tools
{

RussianLocale::RussianLocale()
{
    externalLocale = string( setlocale( LC_ALL, NULL ) );
#ifdef MSVC
    char* currentLocale = setlocale( LC_ALL, "rus" );
#else
    char* currentLocale = setlocale( LC_ALL, "");
#endif
    if( !currentLocale )
    {
        std::wcout << L"Can't set locale!" << std::endl;
        return;
    }
}

RussianLocale::~RussianLocale()
{
    if( !setlocale( LC_ALL, externalLocale.c_str() ) )
    {
        //std::wcout << L"Can't turn back locale!" << std::endl;
        return;
    }
}

}
