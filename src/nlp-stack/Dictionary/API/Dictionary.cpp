/**
 * Dictionary.cpp
 */

#include "Dictionary.h"
#include <algorithm>

/**
 * Constructor of interface
 */
DictionaryInterface* DictionaryInterface::CreateDictionary(const string & dictionaryFile, int _dicLevel)
{
    return new Dictionary(dictionaryFile, _dicLevel);
}

/**
 * Constructor of dictionary
 */
Dictionary::Dictionary(const string & dictionaryFile, int _dicLevel)
{
    pathToData = BIN_DIC_DATA_PATH;
	//wcout << "###### Create dictionary ... " << endl;
    dicLevel = _dicLevel;

    ignoreE = false;
    hyphenSplit = false;
    tools = new DictionaryTools(this);

    trie = new DictionaryTrieRebuild(this, dictionaryFile);

	suffixModelTrie = 0;
	if (dicLevel >= 1)
	{
		suffixModelTrie = new SuffixModelTrieBinaryFileReader(this);
	}

	nGram = 0;
	//nGramVer2 = 0;
	if (dicLevel >= 2)
	{
		nGram = new NGramTrieBinaryFileReader(this);
		//nGramVer2 = new NGramTrieBinaryFileCreator(this);
	}

	testMorphologicalAnalyze = 0;
	//testMorphologicalAnalyze = new TestMorphologicalAnalyze(this);
	
	//wcout << "       Done!" << endl;
	setMainWordFormFromLinkedLemmas(true);
    enableRules(false);
}

/**
 * Destructor of dictionary
 */
Dictionary::~Dictionary(void) 
{
	//wcout << "###### Delete dictionary : " << endl;
	// free memory
    if (trie)
    {
        delete trie;
        trie = NULL;
    }
    if (tools)
	{
		delete tools;
		tools = NULL;
	}
	if (nGram)
	{
		delete nGram;
		nGram = NULL;
	}
	if (suffixModelTrie)
	{
		delete suffixModelTrie;
		suffixModelTrie = NULL;
	}
	if (testMorphologicalAnalyze)
	{
		delete testMorphologicalAnalyze;
		testMorphologicalAnalyze = NULL;
	}
	//wcout << "       Done!" << endl;
}

/**
 * @brief get grammatical information of word
 * @param word : a word for which need grammatical information
 * @param level : level of analyze
 * @return a vector of struct GrammInfo
 */
void Dictionary::getGrammInfo(const wstring & word, shared_ptr<vector<GrammInfo> > result, int level)
{
    shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
    getMorphologicalInfo(word, minfos, level);
    for (int i = 0; i < (int) minfos->size(); ++i)
	{
        shared_ptr<GrammInfo> ginfo = std::make_shared<GrammInfo>();
        getDictionaryTrie()->convertMorphologicalInfoToGrammInfo(&minfos->at(i), ginfo);
        result->push_back(*ginfo);
	}
}

/**
 * @brief get morphological information of word
 * @param word : a word for which need grammatical information
 * @param level : level of analyze
 * @return a vector of struct MorphologicalInfo
 */
//vector<MorphologicalInfo> Dictionary::getMorphologicalInfo(wstring word, int level)
void Dictionary::getMorphologicalInfo(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result, int level)
{
    wstring _lower_case_word = tools->toLowerCase(word);
    //_word = tools->deleteSpaces(_word);
    //vector<MorphologicalInfo> result = trie->getMorphologicalInfoList(_word);
    trie->getMorphologicalInfoList(_lower_case_word, result);
    //wcout << "result->size() = " << result->size() << endl;

// Always try fixing \u0435/\u0451 to find more variants//here was cyrrilic symbols: е,ё
/*	
	// if word found, return result
	if (result.size() || level <= 1)
	{
		for (int i = 0; i < (int) result.size(); ++i)
		{
			result.at(i).additionalInfo = L"dictionary";
		}
        goto ___RETURN;
	}
*/	
    /*
	if (result.size())
	{
		for (int i = 0; i < (int) result.size(); ++i)
		{
			result.at(i).additionalInfo = L"dictionary";
		}
	}
    */

	// \u0435/\u0451 & hyphens processing//here was cyrrilic symbols: е,ё
	// Try with \u0435/\u0451//here was cyrrilic symbols: е,ё
    if ((ignoreE) && (_lower_case_word.find(L"\u0435") >= 0 || _lower_case_word.find(L"\u0451") >= 0))//here was cyrrilic symbols: е,ё
	{
        //vector<MorphologicalInfo> result_e = getMorphologicalInfoIgnoreE(_word);
        shared_ptr<vector<MorphologicalInfo> > result_e = std::make_shared<vector<MorphologicalInfo> >();
        getMorphologicalInfoIgnoreE(_lower_case_word, result_e);
        if (result_e->size())
		{
            /*
			for (int i = 0; i < (int) result_e.size(); ++i)
			{
				result_e.at(i).additionalInfo = L"\u0435/\u0451";//here was cyrrilic symbols: е,ё
			}
            */
            result->insert(result->end(), result_e->begin(), result_e->end());
		}
	}

	// Try with hyphens
//    if ((level >= 3) && (hyphenSplit) && (_lower_case_word.find(L"-") != string::npos))
//	{
//        //vector<MorphologicalInfo> result_h = getMorphologicalInfoHyphenSplit(_word);
//        shared_ptr<vector<MorphologicalInfo> > result_h = std::make_shared<vector<MorphologicalInfo> >();
//        getMorphologicalInfoHyphenSplit(_lower_case_word, result_h);
//        if (result_h->size())
//		{
//			for (int i = 0; i < (int) result_h.size(); ++i)
//			{
//				result_h.at(i).additionalInfo = L"hyphen";
//			}
//            result->insert(result->end(), result_h->begin(), result_h->end());
//		}
//	}
	// non-dictionary word
	// try variants
    /*
    if (level >= 4)
    {
        vector<MorphologicalInfo> result_v = getPrediction(_word);
        if (result_v.size())
        {
            for (int i = 0; i < (int) result_v.size(); ++i)
            {
                result_v.at(i).additionalInfo = L"misspell";
            }
            result.insert(result.end(), result_v.begin(), result_v.end());
        }
    }
    */
	// neologism
    if (level >= 5)
    {
        shared_ptr<vector<MorphologicalInfo> > result_n = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalPrediction(word, result_n);
        //vector<MorphologicalInfo> result_n = getSuffixModelTrie()->getMorphologicalPredictionBySuffix(_word);
        if (result_n->size())
        {
//            for (int i = 0; i < (int) result_n.size(); ++i)
//            {
//                result_n.at(i).additionalInfo = L"neologism";
//            }
            result->insert(result->end(), result_n->begin(), result_n->end());
        }
    }
    if (this->getDictionaryTrie()->ruleIsEnable())
    {
        //vector<MorphologicalInfo> result_r = trie->getMorphologicalInfoListByRules(_word);
        shared_ptr<vector<MorphologicalInfo> > result_r = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalInfoListByRules(_lower_case_word, result_r);
        if (result_r->size())
        {
            result->insert(result->end(), result_r->begin(), result_r->end());
        }
    }
}

/**
 * @brief Try to fix ONE misspell, then find morphological information of fixed word
 * @param word : word to be fixed
 * 
 */
void Dictionary::getPrediction(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result)
{
    //wstring _word = tools->toLowerCase(word);
    //_word = tools->deleteSpaces(_word);
    vector<wstring> modifications = tools->getModifications(word);
    //vector<MorphologicalInfo> result = vector<MorphologicalInfo>();
    result->clear();
    for (int i = 0; i < (int) modifications.size(); ++i)
	{
		wstring str = modifications.at(i);
        shared_ptr<vector<MorphologicalInfo> > tmp = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalInfoList(str, tmp);
        result->insert(result->end(), tmp->begin(), tmp->end());
	}
    //return result;
}

/**
 * @brief morphological analyze ignoring \u0435/\u0451//here was cyrrilic symbols: е,ё
 * @param word : word to be analyzed
 * 
 */
void Dictionary::getMorphologicalInfoIgnoreE(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result)
{
    //vector<MorphologicalInfo> result = vector<MorphologicalInfo>();
    result->clear();
	vector<wstring> modifications = tools->getAllEeVariant(word);
	for (int i = 0; i < (int) modifications.size(); ++i)
	{
		wstring str = modifications.at(i);
        shared_ptr<vector<MorphologicalInfo> > tmp = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalInfoList(str, tmp);
        result->insert(result->end(), tmp->begin(), tmp->end());
	}
    //return result;
}

/**
 * @brief split multi-word and run analyze 
 * 
 * 
 */
void Dictionary::getMorphologicalInfoHyphenSplit(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result)
{
	vector<wstring> variants = tools->getAllWordVariant(word, ignoreE, hyphenSplit);
    result->clear();
	for (int i = 0; i < (int) variants.size(); ++i)
	{
		wstring str = variants.at(i);
        shared_ptr<vector<MorphologicalInfo> > tmp = std::make_shared<vector<MorphologicalInfo> >();
        trie->getMorphologicalInfoList(str, tmp);
        result->insert(result->end(), tmp->begin(), tmp->end());
	}
    //return result;
}

/**
 * @brief analyze each word in a sentence 
 * 
 */
vector<MorphologicalAnalyze> Dictionary::getMorphologicalAnalyze(const wstring & _sentence)
{
	// tokenize sentence to words
	vector<wstring> words = getDictionaryTools()->dictionarySplit(_sentence, WORD_DELIMITER);
	// number of words
	int n = (int) words.size();
	// init morphological info list
	vector<vector<MorphologicalInfo> > minfos = vector<vector<MorphologicalInfo> >();
	vector<int> minfoSize = vector<int>();

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
        minfoSize.push_back(minfo->size());
	}
	//init maxNGram;
	int maxNGram = -1;
	vector<int> maxNGramIndex;
	// init zero-array length n
	vector<int> tmpIndex(n, 0);
	while (1)
	{
		// get combination of MorphologicalInfo
		vector<MorphologicalInfo> _tmpMinfo = vector<MorphologicalInfo>();
		for(int i = 0; i < n; ++i)
		{
			_tmpMinfo.push_back(minfos.at(i).at(tmpIndex.at(i)));
		}
		// get NGram value for the combination
		int tmpNGramValue = this->getNGramTrie()->morphologicalNGramValue(_tmpMinfo);
		// compare tmpNGramValue to maxNGram
		if (tmpNGramValue > maxNGram)
		{
			maxNGram = tmpNGramValue;
			maxNGramIndex = tmpIndex;
		}

		// change combination
		int i = n - 1;
		int z = 1;
		while (i >= 0 && z == 1)
		{
			if (tmpIndex.at(i) + z < minfoSize.at(i))
			{
				tmpIndex.at(i) += z;
				z = 0;
			}
			else
			{
				tmpIndex.at(i) = 0;
				z = 1;
			}
			i--;
		}
		if (i < 0 && z == 1)
		{
			break;
		}
	}
	// get result
	//wcout << "maxNGram = " << maxNGram << endl;
	vector<MorphologicalAnalyze> result = vector<MorphologicalAnalyze>();
	for (int i = 0; i < n; ++i)
	{
		MorphologicalAnalyze mAnalyze;
		mAnalyze.minfos = minfos.at(i);
		mAnalyze.bestVariant = maxNGramIndex.at(i);
		result.push_back(mAnalyze);
	}
	return result;
}

/**
 * getNGramMorphologicalAnalyze : 
 * 
 */
NGramMorphologicalAnalyze Dictionary::getNGramMorphologicalAnalyze(const wstring & sentence)
{
    wstring _sentence = tools->toLowerCase(sentence);
	// tokenize sentence to words
	vector<wstring> words = getDictionaryTools()->dictionarySplit(_sentence, WORD_CHARACTERS, false);
	// number of words
	int n = (int) words.size();
	// init morphological info list
	vector<vector<MorphologicalInfo> > minfos = vector<vector<MorphologicalInfo> >();
	vector<int> minfoSize = vector<int>();

	// morphological analysis for each word
	for (int i = 0; i < n; ++i)
	{
		// morphological analysis for words[i]
        shared_ptr<vector<MorphologicalInfo> > minfo = std::make_shared<vector<MorphologicalInfo> >();
        getMorphologicalInfo(words.at(i), minfo);
		// if no MorphologicalInfo found -> create NULL MorphologicalInfo for N-Gram
        if (minfo->size() == 0)
		{
			// create MorphologicalInfo - NULL 
			MorphologicalInfo info;
			info.basicFeatureListId = 0;
			info.featureListId = 0;
			// then add to minfo
            minfo->push_back(info);
		}
        minfos.push_back(*minfo);
        minfoSize.push_back(minfo->size());
	}
	//init maxNGram;
	int maxNGram = -1;
	vector<int> maxNGramIndex;
	/*
	// init zero-array length n
	vector<int> tmpIndex(n, 0);
	while (1)
	{
		// get combination of MorphologicalInfo
		vector<MorphologicalInfo> _tmpMinfo = vector<MorphologicalInfo>();
		for(int i = 0; i < n; ++i)
		{
			_tmpMinfo.push_back(minfos.at(i).at(tmpIndex.at(i)));
		}
		// get NGram value for the combination
		int tmpNGramValue = this->getNGramTrie()->morphologicalNGramValue(_tmpMinfo);
		// compare tmpNGramValue to maxNGram
		if (tmpNGramValue > maxNGram)
		{
			maxNGram = tmpNGramValue;
			maxNGramIndex = tmpIndex;
		}

		// change combination
		int i = n - 1;
		int z = 1;
		while (i >= 0 && z == 1)
		{
			if (tmpIndex.at(i) + z < minfoSize.at(i))
			{
				tmpIndex.at(i) += z;
				z = 0;
			}
			else
			{
				tmpIndex.at(i) = 0;
				z = 1;
			}
			i--;
		}
		if (i < 0 && z == 1)
		{
			break;
		}
	}
	*/

	// faster
	maxNGram = 0;
	for (int i = 0; i < n; ++i)
	{
		vector<MorphologicalInfo> tmp = vector<MorphologicalInfo>();
		for (int j = i - N_GRAM + 1; j < i; ++j)
		{
			if (j < 0)
			{
				MorphologicalInfo _minfo;
				_minfo.basicFeatureListId = 0;
				_minfo.featureListId = 0;
				tmp.push_back(_minfo);
			}
			else
			{
				tmp.push_back(minfos.at(j).at(maxNGramIndex.at(j)));
			}
		}
		int localMaxNGram = -1;
		int localMaxNGramIndex = 0;
		for (int j = 0; j < (int) minfos.at(i).size(); ++j)
		{
			tmp.push_back(minfos.at(i).at(j));
			int tmpNGramValue = this->getNGramTrie()->getNGrammCount(tmp);
			if (tmpNGramValue > localMaxNGram)
			{
				localMaxNGram = tmpNGramValue;
				localMaxNGramIndex = j;
			}
			tmp.pop_back();
		}
		maxNGram += localMaxNGram;
		maxNGramIndex.push_back(localMaxNGramIndex);
	}

	// get result
	//wcout << "maxNGram = " << maxNGram << endl;
	vector<MorphologicalAnalyze> result = vector<MorphologicalAnalyze>();
	for (int i = 0; i < n; ++i)
	{
		MorphologicalAnalyze mAnalyze;
		mAnalyze.word = words.at(i);
		mAnalyze.minfos = minfos.at(i);
		mAnalyze.bestVariant = maxNGramIndex.at(i);
		mAnalyze.currentVariant = mAnalyze.bestVariant;
		result.push_back(mAnalyze);
	}
	NGramMorphologicalAnalyze nGramResult;
	nGramResult.maxNGram = maxNGram;
	nGramResult.mAnalyze = result;
	return nGramResult;
}

/**
 * @brief Get word info
 */
vector<WordInfo> Dictionary::getWordInfo(const wstring & word)
{
	wstring _word = tools->toLowerCase(word);
	_word = tools->deleteSpaces(_word);
	vector<WordInfo> result = trie->getWordInfo(_word);
	return result;
}

/**
 * @brief Get list of MorphologicalInfo by a given lemmaId
 */
void Dictionary::getMorphologicalInfoOfLemmaId(int lemmaId, shared_ptr<vector<MorphologicalInfo> > result)
{
    trie->getMorphologicalInfoListOfLemmaId(lemmaId, result);
}

/**
 * @brief Get list of GrammInfo by a given lemmaId
 */
void Dictionary::getGrammInfoOfLemmaId(int lemmaId, shared_ptr<vector<GrammInfo> > result)
{
    shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
    getMorphologicalInfoOfLemmaId(lemmaId, minfos);
    for (vector<MorphologicalInfo>::iterator iter = minfos->begin(); iter != minfos->end(); ++iter)
    {
        shared_ptr<GrammInfo> ginfo = std::make_shared<GrammInfo>();
        getDictionaryTrie()->convertMorphologicalInfoToGrammInfo(&(*iter), ginfo);
        result->push_back(*ginfo);
    }
}

/**
 * @brief get lemma of lemmaId
 * @param [in] lemmaId : lemmaId that need morphological analyze
 * @return a vector of struct GrammInfo
 */
wstring Dictionary::getLemmaOfLemmaId(int lemmaId)
{
    wstring result = L"#LemmaNotFoundError";
    shared_ptr<vector<MorphologicalInfo> >minfos = std::make_shared<vector<MorphologicalInfo> >();
    trie->getMorphologicalInfoListOfLemmaId(lemmaId, minfos);
    if (minfos->size())
    {
        result = minfos->at(0).initial_form;
    }
    return result;
}

bool compare_by_lemma_id(const WordInfo& first, const WordInfo& second)
{
    return first.lemmaId < second.lemmaId;
}

void Dictionary::getParadigmsForLemma(const wstring lemma, vector<vector<GrammInfo>>& result)
{
    result.clear();

    vector<WordInfo> variants = getWordInfo(lemma);
    std::sort(variants.begin(), variants.end(), &compare_by_lemma_id);
    int prevLemmaId = -1;
    for(auto iter = variants.begin(); iter != variants.end(); iter++) {
        if(iter->lemmaId == prevLemmaId) {
            continue;
        }
        prevLemmaId = iter->lemmaId;
        shared_ptr<vector<GrammInfo>> paradigm = shared_ptr<vector<GrammInfo>>(new vector<GrammInfo>);
        getGrammInfoOfLemmaId(iter->lemmaId, paradigm);
        if(!paradigm->empty() && (*paradigm)[0].initial_form.compare(lemma) == 0) {
            result.push_back(*paradigm.get());
        }
    }
}

