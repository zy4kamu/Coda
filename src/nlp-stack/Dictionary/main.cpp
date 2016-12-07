/**
 * main.cpp
 */

#include "TestTools.h"

int main(int argc, char *argv[])
{
	start = clock();

#ifdef MSVC
	_setmode(_fileno(stdout), _O_U8TEXT);
#else
	// Linux
	setlocale(LC_ALL, "");
#endif

/*
    std::shared_ptr<MorphologicalDictionary> dict = shared_ptr<MorphologicalDictionary>(new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN));
    shared_ptr<vector<shared_ptr<Morphology> > > morphology = make_shared<vector<shared_ptr<Morphology> > >();
    dict->getMorphology(L"Иван", morphology);
	wstring str = L"\u0447\u0438\u0442\u0430\u0442\u044c";//here was cyrrilic symbols: читать
	//wcin >> str;
	wcout << str << endl;
*/	
	test();

	finish = clock();
	wcout << "====================================================" << endl;
    wcout << "Time = " << (finish - start) * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	wcout << "Done! " << endl;
#ifdef MSVC
	_getch();
#endif
	return 0;
}
