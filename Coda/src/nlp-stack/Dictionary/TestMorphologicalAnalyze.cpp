/**
 * TestMorphologicalAnalyze.cpp
 */

#include "TestMorphologicalAnalyze.h"
#include "Dictionary.h"

TestMorphologicalAnalyze::TestMorphologicalAnalyze(Dictionary* _dic)
{
	dic = _dic;
}

TestMorphologicalAnalyze::~TestMorphologicalAnalyze(void)
{
	dic = NULL;
}

int TestMorphologicalAnalyze::compareTwoVectorOfWstring(vector<wstring> w1, vector<wstring> w2)
{
	/*
	vector<wstring> difference;
	vector<wstring>::iterator iter;
	iter = set_difference(logFeatures.at(i).begin(), logFeatures.at(i).end(), analyzeFeatures.at(i).begin(), analyzeFeatures.at(i).end(), difference.begin());
	difference.resize(iter - difference.begin());
	*/
	if (w1.size() != w2.size())
	{
		return 0;
	}
	set<wstring> sw1 = set<wstring>();
	for (int i = 0; i < (int) w1.size(); ++i)
	{
		sw1.insert(w1.at(i));
	}
	for (int i = 0; i < (int) w2.size(); ++i)
	{
		if (!sw1.count(w2.at(i)))
		{
			return 0;
		}
	}
	return 1;
}

/**
 * testSentence
 * \u0441\u0443\u0434<NOUN inan masc sing nomn> \u043d\u0430\u0447\u0430\u043b<VERB perf tran masc sing past indc> \u043e\u0433\u043b\u0430\u0448\u0430\u0442\u044c<INFN impf tran> \u043f\u0440\u0438\u0433\u043e\u0432\u043e\u0440<NOUN inan masc sing nomn> \u043f\u043e<PREP> \u0434\u0435\u043b\u0443<NOUN inan neut sing datv> \u0445\u0440\u043e\u043c\u043e\u0439<ADJF Subx Qual masc sing nomn> \u043b\u043e\u0448\u0430\u0434\u0438<NOUN anim femn plur nomn>//here was cyrrilic symbols: суд,начал,оглашать,приговор,по,делу,хромой,лошади
 */
void TestMorphologicalAnalyze::testSentence(wstring _line)
{
	vector<vector<wstring> > logFeatures = vector<vector<wstring> >();
	vector<vector<wstring> > analyzeFeatures = vector<vector<wstring> >();
	wstring _sentence = L"";
	wstring _featureStr = L"";
	bool _readingFeatures = false;
	for (int i = 0; i < (int) _line.length(); ++i)
	{
		if (_line.at(i) == L'<')
		{
			_readingFeatures = true;
		}
		else if (_line.at(i) == L'>')
		{
			_readingFeatures = false;
			vector<wstring> featureList = dic->getDictionaryTools()->dictionarySplit(_featureStr, WORD_CHARACTERS, false);
			logFeatures.push_back(featureList);
			_featureStr = L"";
		}
		else
		{
			if (_readingFeatures)
			{
				_featureStr.push_back(_line.at(i));
			}
			else
			{
				_sentence.push_back(_line.at(i));
			}
		}
	}
	vector<wstring> _words = dic->getDictionaryTools()->dictionarySplit(_sentence, WORD_CHARACTERS, false);
	NGramMorphologicalAnalyze _analyze = dic->getNGramMorphologicalAnalyze(_sentence);
	for (int i = 0; i < (int) _analyze.mAnalyze.size(); ++i)
	{
		MorphologicalInfo _minfo = _analyze.mAnalyze.at(i).minfos.at(_analyze.mAnalyze.at(i).bestVariant);
		vector<wstring> _featureIds = dic->getDictionaryTrie()->convertMorphologicalInfoToFeatureIds(_minfo);
		analyzeFeatures.push_back(_featureIds);
	}
	int _n = _words.size();
	if (_n != (int) analyzeFeatures.size() || _n != (int) logFeatures.size())
	{
		wcout << "###### Sizes not match!!!" << endl;
		return;
	}
	totalWords += _n;
	for (int i = 0; i < _n; ++i)
	{
		wcout << _words.at(i) << " <";
		for (int j = 0; j < (int) logFeatures.at(i).size(); ++j)
		{
			wcout << logFeatures.at(i).at(j) << " ";
		}
		wcout << "| ";
		for (int j = 0; j < (int) analyzeFeatures.at(i).size(); ++j)
		{
			wcout << analyzeFeatures.at(i).at(j) << " ";
		}

		int _match = compareTwoVectorOfWstring(logFeatures.at(i), analyzeFeatures.at(i));
		totalWordsMatch += _match;
		wcout << "> [" << _match << "]" << endl;
	}
}

void TestMorphologicalAnalyze::testFile(string file_path)
{
	// open file
	wifstream fin(file_path.c_str());
	// set encoding to UTF-8
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	while(getline(fin, line)) 
	{
		testSentence(line);
	}
	// close file
	fin.close();
}

void TestMorphologicalAnalyze::testFolder(string _path, string _file_type)
{
#ifdef MSVC
	wcout << "@TestMorphologicalAnalyze::testFolder BEGIN ..." << endl;
	string path_file_type = _path + _file_type;
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file_type.c_str(), &fd);
    // no files found
	if (h == INVALID_HANDLE_VALUE)
    {
        return;
    }
	int count = 0;
    while(true)
    {
		string file_path = _path + fd.cFileName;
		testFile(file_path);

		count++;
		wcout << count << endl;

        if(FindNextFile(h, &fd) == FALSE)
		{
            break;
		}
    }
	wcout << "@TestMorphologicalAnalyze::testFolder DONE" << endl;
	wcout << "---------------------------------------------------------" << endl;
#endif
}

void TestMorphologicalAnalyze::test(void)
{
	totalWords = 0;
	totalWordsMatch = 0;
	testFile("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\log.txt");
	wcout << "---------------------------------------------------------" << endl;
	wcout << "totalWords = " << totalWords << endl;
	wcout << "totalWordsMatch = " << totalWordsMatch << endl;
	wcout << "---------------------------------------------------------" << endl;
}

