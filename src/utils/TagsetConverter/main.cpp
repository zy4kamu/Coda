/**
 * main.cpp
 */

#include "PythonExtension.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    TagsetConverting::createConverter();
    const wchar_t* markers[7] {L"NOUN", L"inan", L"femn", L"sing" ,L"Sgtm", L"gent", L"Geox"};
    auto nVariants = TagsetConverting::convert(markers, 7);
    return 0;
}
