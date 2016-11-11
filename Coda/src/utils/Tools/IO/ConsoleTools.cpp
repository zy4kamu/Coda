#include "ConsoleTools.h"
#include "Strings/StringTools.h"

#ifdef MSVC
    #include <fcntl.h>
    #include "io.h"
#endif

#include <iostream>
using std::wcout;

namespace Tools
{

void AddToConsole( wstring string)
{

#ifdef MSVC

    const wchar_t badChar = L'?';

    if( !wcout.good() )
    {
        wcout.clear();
        wcout << L"Stream was broken!\r\n";
    }
    if( _setmode( _fileno( stdout ), _O_WTEXT ) == NOT_FOUND )
    {
        wcout << L"Can't set console mode!\r\n";
    }
    bool isGood = true;
    for( unsigned int i = 0; i < string.length(); i++ )
    {
        wcout << string[i];
        if( !wcout.good() )
        {
            wcout.clear();
            wcout << badChar;
            isGood = false;
        }
    }
    if( !isGood )
    {
        wcout << L"\r\nIt was bad characters!\r\n";
    }

#else
    std::wcout<<string;
#endif
}

string ExecuteShellScript(const string& script)
{
#ifndef MSVC
    FILE* pipe = popen(script.c_str(), "r");
    if (!pipe)
    {
        return "ERROR";
    }
    char buffer[128];
    string result = "";
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
        {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
#else
	return string();
#endif
}

}
