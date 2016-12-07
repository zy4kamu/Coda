/**
 * NGramTrieBuild.cpp
 */

#include "NGramTrieBuild.h"
#include "Dictionary.h"

NGramTrieBuild::NGramTrieBuild(Dictionary* _dic) : NGramTrie(_dic)
{
}

NGramTrieBuild::~NGramTrieBuild(void)
{
	dic = NULL;
}

void NGramTrieBuild::updateSentence(wstring _sentence)
{
	//wcout << "@UPDATE " << _sentence << endl;
	vector<wstring> tokens = dic->getDictionaryTools()->dictionarySplit(_sentence, WORD_DELIMITER);

	vector<MorphologicalInfo> _morphologicalInfoList = sentenceToMorphologicalInfo(_sentence);
	//wcout << "UPDATE : nGramInfoList.size() = " << _morphologicalInfoList.size() << endl;
	if ((int) _morphologicalInfoList.size() == 0)
	{
		return;
	}
	for (int i = -N + 1; i < (int) _morphologicalInfoList.size(); ++i)
	{
		vector<MorphologicalInfo> tmp = vector<MorphologicalInfo>();
		for (int j = 0; j < N; ++j)
		{
			int k = i + j;
			if (k < 0 || k >= (int) _morphologicalInfoList.size())
			{
				MorphologicalInfo _info;
				_info.featureListId = 0;
				tmp.push_back(_info);
			}
			else
			{
				tmp.push_back(_morphologicalInfoList.at(k));
			}
		}
		updateMorphologicalInfo(tmp);
		/*
		for (int j = 0; j < N; ++j)
		{
			int k = i + j;
			if (k >= 0 && k < (int) tokens.size())
			{
				wcout << tokens.at(k) << "(" << tmp.at(j).featureListId << ")" <<" ";
			}
		}
		wcout << endl;
		*/
	}
}

/**
 * Update N-Gram from standard file
 * @TODO :
 * standard file :
 * - lines are separated by .;!?\n
 * - words are separated by spaces, ',' 
 */
void NGramTrieBuild::updateFromStandardFile(string _filePath)
{
	// open file
	wifstream fin(_filePath.c_str());
	// set encoding to UTF-8
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	getline(fin, line);
	while(getline(fin, line))
	{
		line = dic->getDictionaryTools()->refineString(line);
		vector<wstring> sentences = dic->getDictionaryTools()->dictionarySplit(line, SENTENCE_DELIMITER);
		for (int i = 0; i < (int) sentences.size(); ++i)
		{
			updateSentence(sentences.at(i));
		}
	}
	// close file
	fin.close();
}

void NGramTrieBuild::updateFromFolder(string _path, string _file_type)
{
#ifdef MSVC
	wcout << "@updateFromFolder BEGIN ..." << endl;
	string path_file_type = _path + _file_type;
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file_type.c_str(), &fd);
 
    if(h == INVALID_HANDLE_VALUE)
    {
        return; // no files found
    }
	int count = 0;
    while(true)
    {
		string file_path = _path + fd.cFileName;
		this->updateFromStandardFile(file_path);

		count++;
		wcout << count << endl;

        if(FindNextFile(h, &fd) == FALSE)
		{
            break;
		}
    }
	wcout << "@updateFromFolder DONE" << endl;
	wcout << "---------------------------------------------------------" << endl;
#endif
}

