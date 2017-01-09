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
    int nParses = DictionaryOps::getGramInfo(L"Россия", "RU");
    wcout << nParses;
	return 0;
}
