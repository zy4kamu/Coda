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
    DictionaryOps::CreateDictionary("RU");
    int nParses = DictionaryOps::GetGramInfo(L"закуплена", "RU");
    wcout << nParses;
    const wchar_t* features[8]{L"PRTS", L"femn", L"sing", L"perf", L"past", L"pssv"};
    DictionaryOps::SynthesizeTokenFromLemma(L"закупить", features, 6, "RU");

    DictionaryOps::GetParadigmForLemma(L"пригрозить", "RU");
	return 0;
}
