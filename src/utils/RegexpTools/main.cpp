#include "RegExp.h"
#include <iostream>

void init()
{
    std::wcout<<Tools::CheckStrOnRegExp(L"1203-21323-ere",L"\\d+-\\w{1,3}")<<std::endl;
}

int main()
{
    #ifdef MSVC
        system("chcp 1251");
        std::locale::global(std::locale("Russian"));
    #else
        setlocale(LC_ALL, "");
        std::locale::global(std::locale("ru_RU.UTF-8"));
	#endif

	init();
	return 0;
}
