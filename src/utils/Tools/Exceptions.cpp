#include "Exceptions.h"
#include <iostream>

namespace Tools
{

LanguageNotFoundException::LanguageNotFoundException()
    : std::runtime_error(
          "You should register your language settings (utils/Tools/Locale/Language):\n"
          "1. Add it to enum Language\n"
          "2. Register it in LanguageToString function\n"
          "3. Register it in LanguageToLocaleString function")
{
    std::wcout << L"You should register your language settings (utils/Tools/Locale/Language):\n";
    std::wcout << L"1. Add it to enum Language\n";
    std::wcout << L"2. Register it in LanguageToString function\n";
    std::wcout << L"3. Register it in LanguageToLocaleString function";
}

ProcessClientConnectionException::ProcessClientConnectionException()
    : std::runtime_error("Couldn't receive message from server")
{
    std::wcout << L"Couldn't receive message from server" << std::endl;
}

ProcessClientCreationException::ProcessClientCreationException()
    : std::runtime_error("Couldn't create connection with server")
{
    std::wcout << L"Couldn't create connection with server" << std::endl;
}

ProcessServerConnectionException::ProcessServerConnectionException()
    : std::runtime_error("Error in connection with client")
{
    std::wcout << L"Error in connection with client" << std::endl;
}

ProcessServerCreationException::ProcessServerCreationException()
    : std::runtime_error("Couldn't create socket for communication of two processes")
{
    std::wcout << L"Couldn't create socket for communication of two processes" << std::endl;
}

}

