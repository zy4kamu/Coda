#include "PythonFloatSizeConnecter.h"
#include "Strings/StringConvertions.h"

namespace Tools
{

string PythonFloatSizeConnecter::Get(int connectionIndex)
{
    char[256] bufferSizeString;
    memset(bufferSizeString, 0, 256);
    read(connectionIndex, &bufferSizeString, sizeof(bufferSizeString));
    size_t bufferSize = static_cast<size_t>(std::stoi(bufferSizeString));

    char message[bufferSize + 1];
    memset(message, 0, bufferSize + 1);
    read(connectionIndex, message, bufferSize);
    return string(message);
}

void PythonFloatSizeConnecter::Send(const string& responce, int connectionIndex)
{
    string size = Tools::ToString(responce.size());
    write(connectionIndex, size.c_str(), 256);
    write(connectionIndex, responce.c_str(), responce.size());
}

}
