/**
 * TestNGram.cpp
 */

#include "TestNGram.h"

TestNGram::TestNGram(void) : Dictionary()
{
}

TestNGram::~TestNGram(void)
{
}

/**
 * tokenize _sentence to words
 * morphologicaly analyze each word
 * for each word, analyze it as neologism
 * find best N-gram variant
 * if best variant match dictionary-based morphological analyze -> ok
 */
void TestNGram::testSentence(wstring _sentence)
{
	// tokenize sentence to words
	vector<wstring> words = getDictionaryTools()->dictionarySplit(_sentence, WORD_DELIMITER);
	// number of words
	int n = (int) words.size();
	// update totalWords
	totalWords += n;
	// init vector of morphological info lists
	vector<vector<MorphologicalInfo> > minfos = vector<vector<MorphologicalInfo> >();
	// morphological analysis for each word
	for (int i = 0; i < n; ++i)
	{
		// morphological analysis for words[i]
        shared_ptr<vector<MorphologicalInfo> > minfo = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalInfoList(words.at(i), minfo);
		// if no MorphologicalInfo found
        if (minfo->size() == 0)
		{
			// create MorphologicalInfo - NULL 
			MorphologicalInfo info;
			info.featureListId = 0;
			// then add to minfo
            minfo->push_back(info);
		}
        minfos.push_back(*minfo);
	}

	// TEST
	int _N = nGram->getN();
	// test each word
	for (int i = 0; i < n; ++i)
	{
		// if have no variants then continue
		if (minfos.at(i).size() == 1 && minfos.at(i).at(0).featureListId == 0)
		{
			continue;
		}
		// check if all variants have the same lemmaId
		int commonLemmaId = 1;
		for (int j = 0; j < (int) minfos.at(i).size() - 1; ++j)
		{
			if (minfos.at(i).at(j).lemmaId != minfos.at(i).at(j + 1).lemmaId)
			{
				commonLemmaId = 0;
				break;
			}
		}
		// update totalWordsWithCommonLemmaId
		totalWordsWithCommonLemmaId += commonLemmaId;
		// if variants have different lemmaId then continue
		if (!commonLemmaId)
		{
			continue;
		}
		// init empty vector of lists of MorphologicalInfo
		vector<vector<MorphologicalInfo> > tmp = vector<vector<MorphologicalInfo> >();
		// get N word with word[i] at center
		for (int j = -_N + 1; j <= _N -1; ++j)
		{
			int k = i + j;
			if (k < 0 || k >= n)
			{
				MorphologicalInfo _info;
				_info.featureListId = 0;
				vector<MorphologicalInfo> _info_vector = vector<MorphologicalInfo>();
				_info_vector.push_back(_info);
				tmp.push_back(_info_vector);
			}
			else
			{
				tmp.push_back(minfos.at(k));
			}

		}
		// get prediction as neologism for word i
        shared_ptr<vector<MorphologicalInfo> > neologism_info = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalPrediction(words.at(i), neologism_info);
		// if have no neologism analyze then continue
        if (neologism_info->size() == 0)
		{
			continue;
		}
		// init max_n_gram
		int max_n_gram = -1;
		int j_max = -1;
		// for each analyze as neologism
        for (int j = 0; j < (int) neologism_info->size(); ++j)
		{
			vector<MorphologicalInfo> _info_vector = vector<MorphologicalInfo>();
            _info_vector.push_back(neologism_info->at(j));
			tmp.at(_N) = _info_vector;
			// generate combinations :
			vector<int> tmpIndex = vector<int>(2 * _N - 1, 0);
			while (true)
			{
				// get combination of MorphologicalInfo
				vector<MorphologicalInfo> _tmpMinfo = vector<MorphologicalInfo>();
				for(int k = 0; k < _N; ++k)
				{
					_tmpMinfo.push_back(tmp.at(k).at(tmpIndex.at(k)));
				}
				// get NGram value for the combination
				int tmpNGramValue = this->getNGramTrie()->morphologicalNGramValue(_tmpMinfo);
				// compare tmpNGramValue to maxNGram
				if (tmpNGramValue > max_n_gram)
				{
					max_n_gram = tmpNGramValue;
					j_max = j;
				}
				// change combination
				int y = _N - 1;
				int z = 1;
				while (y >= 0 && z == 1)
				{
					if (tmpIndex.at(y) + z < (int) tmp.at(y).size())
					{
						tmpIndex.at(y) += z;
						z = 0;
					}
					else
					{
						tmpIndex.at(y) = 0;
						z = 1;
					}
					y--;
				}
				if (y < 0 && z == 1)
				{
					break;
				}
			}
			// int _current_n_gram = nGram->morphologicalNGramValue(tmp, false);
		}
        int featureListIdJMax = neologism_info->at(j_max).featureListId;
		int featureListIdMatch = 0;
		for (int j = 0; j < (int) minfos.at(i).size(); ++j)
		{
			if (minfos.at(i).at(j).featureListId == featureListIdJMax)
			{
				featureListIdMatch = 1;
				break;
			}
		}
		totalWordsMatch += featureListIdMatch;
	}
}

void TestNGram::testFile(string file_path)
{
	// open file
	wifstream fin(file_path.c_str());
	// set encoding to UTF-8
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	getline(fin, line);
	while(getline(fin, line)) {
		line = tools->refineString(line);
		vector<wstring> sentences = tools->dictionarySplit(line, SENTENCE_DELIMITER);
		for (int i = 0; i < (int) sentences.size(); ++i)
		{
			testSentence(sentences.at(i));
		}
	}
	// close file
	fin.close();
}

void TestNGram::testFolder(string _path, string _file_type)
{
#ifdef MSVC
	//wcout << "@TestNGram::testFolder BEGIN ..." << endl;
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
	wcout << "@TestNGram::testFolder DONE" << endl;
	wcout << "---------------------------------------------------------" << endl;
#endif
}

void TestNGram::test(void)
{
	totalWords = 0;
	totalWordsWithCommonLemmaId = 0;
	totalWordsMatch = 0;
	testFile("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_03_18_OpenCorporaCorpus\\text\\data_0.txt");
	//testFolder("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_03_18_OpenCorporaCorpus\\text\\", "*.txt");
	wcout << "totalWords = " << totalWords << endl;
	wcout << "totalWordsWithCommonLemmaId = " << totalWordsWithCommonLemmaId << endl;
	wcout << "totalWordsMatch = " << totalWordsMatch << endl;
	wcout << "---------------------------------------------------------" << endl;
}

