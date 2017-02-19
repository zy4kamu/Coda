/**
 * main.cpp
 */

#include "TestTools.h"
#include "PythonExtension.h"

int main(int argc, char *argv[])
{
//	start = clock();
    setlocale(LC_ALL, "");
//	test();

//	finish = clock();
//	wcout << "====================================================" << endl;
//    wcout << "Time = " << (finish - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
//	wcout << "Done! " << endl;
    DictionaryOps::createDictionary("RU");
    int nParses = DictionaryOps::getGramInfo(L"россия", "RU");
    wcout << nParses;
    const wchar_t* features[8]{L"VERB", L"perf", L"sing", L"masc", L"past", L"indc", L"intr"};
    DictionaryOps::synthesizeTokenFromLemma(L"пригрозить", features, 7, "RU");

    DictionaryOps::getParadigmForLemma(L"пригрозить", "RU");
	return 0;
}
