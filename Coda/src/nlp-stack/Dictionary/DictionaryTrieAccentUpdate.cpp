/**
 * DictionaryTrieAccent.cpp
 */

#include "DictionaryTrieAccentUpdate.h"
#include "Dictionary.h"

#define ACCENT_DICTIONARY_PATH "E:\\SharedFolder\\Accent.txt"

/**
 * Constructor of DictionaryTrieAccentUpdate
 */
DictionaryTrieAccentUpdate::DictionaryTrieAccentUpdate(Dictionary* _dic, string dictionaryFile) : DictionaryTrieAccent(_dic, dictionaryFile)
{
	initFeatureScore();
}

/**
 * Destructor of DictionaryTrieAccentUpdate
 */
DictionaryTrieAccentUpdate::~DictionaryTrieAccentUpdate(void)
{
}


/**
 * update stress position to dictionary
 */
void DictionaryTrieAccentUpdate::updateStressPos(int lemmaId, int pos, int stressPos)
{
	if ((int) stressPosList.size() < lemmaId + 1)
	{
		stressPosList.resize(lemmaId + 1, vector<int>());
	}
	if ((int) stressPosList.at(lemmaId).size() < pos + 1)
	{
		stressPosList.at(lemmaId).resize(pos + 1, 0);
	}
	stressPosList.at(lemmaId).at(pos) = stressPos;
}

/**
 * get all lemmaIds and link from them of minfos
 */
set<int> DictionaryTrieAccentUpdate::lemmaIdSet(vector<MorphologicalInfo> minfos)
{
	set<int> _lis = set<int>();
	for (int i = 0; i < (int) minfos.size(); ++i)
	{
		MorphologicalInfo minfo = minfos.at(i);
		_lis.insert(minfo.lemmaId);
		DictionaryNodeModel* _dictionaryNodeModel = getNodeModelByLemmaId(minfo.lemmaId);
		/*
		vector<DictionaryNodeModel*> _fromDictionaryNodeModels = _dictionaryNodeModel->getLinkFrom();
		for (int j = 0; j < _fromDictionaryNodeModels.size(); ++j)
		{
			_lis.insert(_fromDictionaryNodeModels.at(j)->getLemmaId());
		}
		*/
		DictionaryNodeModel* dictionaryNodeModelLink = _dictionaryNodeModel->getLink();
		if (dictionaryNodeModelLink)
		{
			_lis.insert(dictionaryNodeModelLink->getLemmaId());
		}
	}
	return _lis;
}

/**
 * intersection of 2 sets
 */
set<int> setIntersection(set<int> s1, set<int> s2)
{
	set<int> result = set<int>();
	for (set<int>::iterator iter=s1.begin(); iter != s1.end(); ++iter)
	{
		if (s2.count(*iter))
		{
			result.insert(*iter);
		}
	}
	return result;
}

/**
 * intersection of list of sets
 */
set<int> setIntersection(vector<set<int> > listOfSet)
{
	if (!listOfSet.size())
	{
		return set<int>();
	}
	set<int> result = listOfSet.at(0);
	for (int i = 1; i < (int) listOfSet.size(); ++i)
	{
		set<int> tmp = setIntersection(result, listOfSet.at(i));
		result = tmp;
	}
	return result;
}

vector<wordInfoStructure> DictionaryTrieAccentUpdate::getWordInfoStructure(wstring line)
{
	vector<wordInfoStructure> result = vector<wordInfoStructure>();
	int sharp_pos = line.find_first_of(L"#");
    if (sharp_pos == static_cast<int>(string::npos))
	{
		return result;
	}
	wstring str = line.substr(sharp_pos + 1);
	vector<wstring> wordsWithStress = dic->getDictionaryTools()->dictionarySplit(str, L",");

	for (int i_word = 0; i_word < (int) wordsWithStress.size(); ++i_word)
	{
		wstring wordStress = wordsWithStress.at(i_word);
		
		wcout << wordStress << endl;
		
		int stress_pos = wordStress.find_first_of(L"'");
		
		wcout << stress_pos << endl;
		
        if (stress_pos == static_cast<int>(string::npos))
		{
			continue;
		}

		wstring word = wordStress.substr(0, stress_pos) + wordStress.substr(stress_pos + 1);
        shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
        this->getDictionary()->getMorphologicalInfo(word, minfos, 2);
        if (!minfos->size())
		{
			//wcout << "###### ERROR 1 : WORD NOT FOUND : " << word << endl;
			continue;
		}
		/*
		wcout << word << " (" << stress_pos << ")";
		for (int i_minfo = 0; i_minfo < minfos.size(); ++i_minfo)
		{
			wcout << " - " << minfos.at(i_minfo).lemmaId;
		}
		*/
        set<int> lIdSet = lemmaIdSet(*minfos);
		/*
		wcout << " (All lemmas & links : ";
		for (set<int>::iterator iter = lIdSet.begin(); iter != lIdSet.end(); ++iter)
		{
			wcout << " - " << *iter;
		}
		wcout << ")";

		wcout << endl;
		*/
		wordInfoStructure wis;
		wis.word = word;
		wis.lemmaIdSet = lIdSet;
        wis.minfos = *minfos;
		wis.stressPos = stress_pos;
		result.push_back(wis);
	}
	return result;
}

/**
 * Get candidates for updating stress
 */
vector<wordStressInfoStructure> DictionaryTrieAccentUpdate::getWordStressInfoStructure(wstring line)
{
	vector<wordStressInfoStructure> result = vector<wordStressInfoStructure>();
	vector<wordInfoStructure> WISS = getWordInfoStructure(line);
	if (!WISS.size())
	{
		return result;
	}
	vector<set<int> > lemmaIdSets = vector<set<int> >();
	for (int i = 0; i < (int) WISS.size(); ++i)
	{
		lemmaIdSets.push_back(WISS.at(i).lemmaIdSet);
	}

	set<int> lemmaIdIntersection = setIntersection(lemmaIdSets);
	
	wcout << "Intersection : ";
	for (set<int>::iterator iter = lemmaIdIntersection.begin(); iter != lemmaIdIntersection.end(); ++iter)
	{
		wcout << " - " << *iter;
	}
	wcout << endl;
	wcout << "-------------" << endl;
	
	for (int iw = 0; iw < (int) WISS.size(); ++iw)
	{
		wordInfoStructure WS = WISS.at(iw);
		vector<MorphologicalInfo> minfos = WS.minfos;
		for (int im = 0; im < (int) minfos.size(); ++im)
		{
			int lemmaId = minfos.at(im).lemmaId;
			int linkLemmaId = -1;
			DictionaryNodeModel* _dictionaryNodeModel = getNodeModelByLemmaId(lemmaId);
			DictionaryNodeModel* dictionaryNodeModelLink = _dictionaryNodeModel->getLink();
			if (dictionaryNodeModelLink)
			{
				linkLemmaId = dictionaryNodeModelLink->getLemmaId();
			}
			if (WS.lemmaIdSet.size() == 1 || lemmaIdIntersection.count(lemmaId) || lemmaIdIntersection.count(linkLemmaId))
			{
				wordStressInfoStructure candidate;
				candidate.lemmaId = lemmaId;
				candidate.pos = minfos.at(im).pos;
				candidate.stressPos = WS.stressPos;
				result.push_back(candidate);
			}
		}
	}
	
	return result;
}

/**
 * update a line in accent dictionary file
 */
void DictionaryTrieAccentUpdate::updateStressFromLine(wstring line)
{
	vector<wordStressInfoStructure> wordStressStructure = getWordStressInfoStructure(line);
	for (int i = 0; i < (int) wordStressStructure.size(); ++i)
	{
		updateStressPos(wordStressStructure.at(i).lemmaId, wordStressStructure.at(i).pos, wordStressStructure.at(i).stressPos);
	}
}

/**
 * update dictionary file
 */
void DictionaryTrieAccentUpdate::updateStressFromFile(void)
{
	stressPosList = vector<vector<int> >(nodeModelMap.size() + 1, vector<int>());
	wifstream fin(ACCENT_DICTIONARY_PATH);
	//wifstream fin("E:\\SharedFolder\\AccentTest.txt");
#ifdef MSVC	
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	int lineCount = 0;
	while(getline(fin, line)) 
	{
		updateStressFromLine(line);
		lineCount++;

		if (!(lineCount % 1000))
		{
			wcout << lineCount << endl;
			//break;
		}
	}
	fin.close();
	wcout << "Done, line count = " << lineCount << endl;
}

vector<WordFormWithStress> DictionaryTrieAccentUpdate::getWordFormWithStress(wstring _word, bool _beginWithPo)
{
	// initialize empty list of WordFormWithStress
	vector<WordFormWithStress> result = vector<WordFormWithStress>();
	// begin from root
    DictionaryNode* current = root;
	// _beginWithPo = true    : begin from 1-st character (_word[2])
	// _beginWithPo = false   : begin from 3-st character (_word[0])
	int i_word = _beginWithPo ? 1 : -1;
	int w_len = _word.length();
	wstring _prefix = L"";
    while (i_word < w_len)
    {
		// get list of NodeModel of current node
        vector<DictionaryNodeModel*> * _nodeModels = current->getNodeModels();
		// for each NodeModel
        for (int i_model = 0; i_model < (int) _nodeModels->size(); ++i_model) {
            DictionaryNodeModel* _nodeModel = _nodeModels->at(i_model);
			// get modelId of NodeModel
			int _trieModelId = _nodeModel->getModelId();
			// get TrieModel by _trieModelId
			DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
			// continue if TrieModel not found
			if (!_trieModel) {
				continue;
			}

			// get list of TrieModelElement of TrieModel
            vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
			// continue if _trieModelElements is empty
            if (_trieModelElements->empty()) {
				continue;
			}
			// basic form
			wstring basicForm = L"";
			// get first TrieModelElement
            DictionaryTrieModelElement* _firstTrieModelElement = _trieModelElements->at(0);

			// get link of NodeModel
			DictionaryNodeModel* _linkNode = _nodeModel->getLink();
			// if NodeModel has a link
			if (_linkNode) {
				// get string from root to link node
				wstring _link_prefix = _linkNode->getStringFromRoot();
				// get link TrieModel
				DictionaryTrieModel *_linkTrieModel = getTrieModelByModelId(_linkNode->getModelId());
				// get first TrieModelElement
				DictionaryTrieModelElement* _firstLinkTrieModelElement = _linkTrieModel->getFirstDictionaryTrieModelElement();
				// get basic form
                basicForm = _link_prefix + *(_firstLinkTrieModelElement->getSuffix());
			} else { // node does not have a link
				// get basic form
                basicForm = _prefix + *(_firstTrieModelElement->getSuffix());
			}
			int matchCount = 0;
			// for each TrieModelElement, from the 2nd TrieModelElement
            for (int i_model_element = 1; i_model_element < (int) _trieModelElements->size(); ++i_model_element) {
                DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i_model_element);
				// continue if PO-attr of TrieModelElement not match with _beginWithPo
				if (_trieModelElement->isBeginWithPo() != _beginWithPo) {
					continue;
				}
				// get suffix of TrieModelElement
                wstring _suffix = *(_trieModelElement->getSuffix());
				// concat prefix and suffix
				wstring _prefix_suffix = _prefix + _suffix;
				// if match with _word
				if (_word.compare(_prefix_suffix) == 0) {
					// create new GrammInfo
					WordFormWithStress _wordFormWithStress;
					// set word for GrammInfo
					_wordFormWithStress.word = _word;
					// set id for GrammInfo
					_wordFormWithStress.lemmaId = _nodeModel->getLemmaId();
					// set initial form for _morphologicalInfo
					_wordFormWithStress.initial_form = basicForm;
					// set basicFeatureListId for _morphologicalInfo
					_wordFormWithStress.featureListId = _trieModelElement->getFeatureListId();
					// set basicFeatureListId for _morphologicalInfo
					_wordFormWithStress.basicFeatureListId = _firstTrieModelElement->getFeatureListId();
					// stressPos
					_wordFormWithStress.stressPos = (int) stressPosList.at(_wordFormWithStress.lemmaId).size() > i_model_element ? stressPosList.at(_wordFormWithStress.lemmaId).at(i_model_element) : -1;

					// add _grammInfo to result
					result.push_back(_wordFormWithStress);
					// count number of matches
					matchCount++;
				}
			}
			// if no match was found, consider the 1st TrieModelElement
			if (matchCount == 0) {
                DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(0);
				// continue if PO-attr of TrieModelElement not match with _beginWithPo
				if (_trieModelElement->isBeginWithPo() != _beginWithPo) {
					continue;
				}
				// get suffix of TrieModelElement
                wstring _suffix = *(_trieModelElement->getSuffix());
				// concat prefix and suffix
				wstring _prefix_suffix = _prefix + _suffix;
				// if match with _word
				if (_word.compare(_prefix_suffix) == 0) {
					// create new GrammInfo
					WordFormWithStress _wordFormWithStress;
					// set word for GrammInfo
					_wordFormWithStress.word = _word;
					// set id for GrammInfo
					_wordFormWithStress.lemmaId = _nodeModel->getLemmaId();
					// set initial form for GrammInfo
					_wordFormWithStress.initial_form = basicForm;
					// set basicFeatureListId for _morphologicalInfo
					_wordFormWithStress.basicFeatureListId = _trieModelElement->getFeatureListId();
					// features 
					_wordFormWithStress.featureListId = 0;
					// stressPos
					_wordFormWithStress.stressPos = (int) stressPosList.size() > _wordFormWithStress.lemmaId && stressPosList.at(_wordFormWithStress.lemmaId).size() > 0 ? stressPosList.at(_wordFormWithStress.lemmaId).at(0) : -1;

					// add _grammInfo to result
					result.push_back(_wordFormWithStress);
				}
			}
		}
		// increase i_word
		i_word++;
		// break if all character are processed
        if (i_word >= w_len) {
			break;
		}
		// update prefix
		// @todo
		_prefix = _word.substr(0, i_word + 1);
		// find child node by next character
        DictionaryNode* tmp = current->findChild(_word[i_word]);
		// break if child node not found
        if ( tmp == NULL ) {
            break;
		}
		// set current node to child node
        current = tmp;
    }
	return result;
}

vector<WordFormWithStress> DictionaryTrieAccentUpdate::getWordFormWithStress(wstring _word)
{
	vector<WordFormWithStress> result = getWordFormWithStress(_word, false);
	if (this->getDictionary()->getDictionaryTools()->stringBeginsWithPo(_word))
	{
		vector<WordFormWithStress> result_1 = getWordFormWithStress(_word, true);
		result.insert(result.end(), result_1.begin(), result_1.end());
	}
	return result;
}

wstring DictionaryTrieAccentUpdate::convertWordFormWithStressToWstring(WordFormWithStress wfws)
{
	wstring result = wfws.stressPos >= 1 && wfws.stressPos <= (int) wfws.word.length() ? wfws.word.substr(0, wfws.stressPos) + L"'" + wfws.word.substr(wfws.stressPos) : wfws.word;
	return result;
}

void DictionaryTrieAccentUpdate::printAllModelToFile(string filePath)
{
	wofstream fout(filePath.c_str());
#ifdef MSVC
	fout.imbue(locale(fout.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fout.imbue(std::locale("ru_RU.UTF-8"));
	fout.imbue(std::locale("en_US.UTF-8"));
#endif
	fout << stressPosList.size() << endl;
	for (int i = 0; i < (int) stressPosList.size(); ++i)
	{
		fout << i << "#";
		for (int j = 0; j < (int) stressPosList.at(i).size(); ++j)
		{
			fout << " " << stressPosList.at(i).at(j);
		}
		fout << endl;
	}
	fout.close();
}

wstring WordWithStress(wstring word, int stressPos)
{
	wstring result = stressPos >= 1 && stressPos <= (int) word.length() ? word.substr(0, stressPos) + L"'" + word.substr(stressPos) : word;
	return result;
}

void DictionaryTrieAccentUpdate::updateStressPos_Ver1(wstring word, int lemmaId, int pos, int stressPos, bool writeToLog)
{
	if ((int) stressPosList.size() < lemmaId + 1)
	{
		stressPosList.resize(lemmaId + 1, vector<int>());
	}
	if ((int) stressPosList.at(lemmaId).size() < pos + 1)
	{
		stressPosList.at(lemmaId).resize(pos + 1, 0);
	}

	if (stressPosList.at(lemmaId).at(pos) <= 0)
	{
		stressPosList.at(lemmaId).at(pos) = stressPos;
		//wcout << word << " " << stressPos << endl;
	}

	/*
	int currentStressPos = stressPosList.at(lemmaId).at(pos);
	if (currentStressPos && (currentStressPos != stressPos))
	{
		// duplication - write to log
		

		flog << word << " : " << WordWithStress(word, currentStressPos) << " - " << WordWithStress(word, stressPos) << endl;
		dupCount++;
	}
	else
	{
		stressPosList.at(lemmaId).at(pos) = stressPos;
	}
	*/
	if (writeToLog)
	{
		if (!stressPosLog.count(word))
		{
			stressPosLog[word] = set<int>();
		}
		stressPosLog.at(word).insert(stressPos);

		if (!lemmaIdLog.count(word))
		{
			lemmaIdLog[word] = set<int>();
		}
		lemmaIdLog.at(word).insert(lemmaId);
	}
}

void DictionaryTrieAccentUpdate::updateStressFromLine_Ver1(wstring line)
{
	int sharp_pos = line.find_first_of(L"#");
    if (sharp_pos == static_cast<int>(string::npos))
	{
		return;
	}
	wstring str = line.substr(sharp_pos + 1);
	vector<wstring> wordsWithStress = dic->getDictionaryTools()->dictionarySplit(str, L",");

	for (int i_word = 0; i_word < (int) wordsWithStress.size(); ++i_word)
	{
		wstring wordStress = wordsWithStress.at(i_word);
		int stress_pos = wordStress.find_first_of(L"'");
        if (stress_pos == static_cast<int>(string::npos))
		{
			continue;
		}

		wstring word = wordStress.substr(0, stress_pos) + wordStress.substr(stress_pos + 1);
        shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
        this->getDictionary()->getMorphologicalInfo(word, minfos, 2);
        for (int i = 0; i < (int) minfos->size(); ++i)
		{
            MorphologicalInfo minfo = minfos->at(i);
			updateStressPos_Ver1(word, minfo.lemmaId, minfo.pos, stress_pos);
		}
	}
}

void DictionaryTrieAccentUpdate::updateStressFromFile_Ver1(void)
{
	// map<word, set<stressPos>> stressPosLog
	stressPosLog = map<wstring, set<int> >();
	// map<word, set<lemmaId>> stressPosLog
	lemmaIdLog = map<wstring, set<int> >();

	dupCount = 0;
	
	flog.open("AccentLog.txt");
#ifdef MSVC
	flog.imbue(locale(flog.getloc(), new codecvt_utf8<wchar_t>));
#else
	//flog.imbue(std::locale("ru_RU.UTF-8")); // test linux-style -> not working
	flog.imbue(std::locale("en_US.UTF-8")); // test linux-style -> not working
#endif

	stressPosList = vector<vector<int> >(nodeModelMap.size() + 1, vector<int>());
	wifstream fin(ACCENT_DICTIONARY_PATH);
	//wifstream fin("E:\\SharedFolder\\AccentTest_1.txt");
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8")); // test linux-style -> not working
	fin.imbue(std::locale("en_US.UTF-8")); // test linux-style -> not working
#endif
	wstring line;
	int lineCount = 0;
	while(getline(fin, line)) 
	{
		updateStressFromLine_Ver1(line); // Ver_1
		lineCount++;

		if (!(lineCount % 1000))
		{
			wcout << lineCount << " - " << dupCount << endl;
			//break;
		}
	}
	fin.close();
	wcout << "Done, line count = " << lineCount << endl;

	flog << dupCount << endl;
	flog.close();
}

void DictionaryTrieAccentUpdate::saveLogToFile_Ver1(void)
{
	flog.open("StressAndLemmaLog.txt");
#ifdef MSVC
	flog.imbue(locale(flog.getloc(), new codecvt_utf8<wchar_t>));
#else
	//flog.imbue(std::locale("ru_RU.UTF-8"));
	flog.imbue(std::locale("en_US.UTF-8"));
#endif
	int count = 0;
	for (map<wstring, set<int> >::iterator iter = stressPosLog.begin(); iter != stressPosLog.end(); ++iter)
	{
		set<int> stressPosSet = iter->second;
		if ((int) stressPosSet.size() > 1)
		{
			wstring word = iter->first;
			set<int> lemmaIdSet = lemmaIdLog.at(word);
			flog << word << " : ";
			for (set<int>::iterator it = lemmaIdSet.begin(); it != lemmaIdSet.end(); ++it)
			{
				flog << *it << " ";
			}
			flog << "# ";
			for (set<int>::iterator it = stressPosSet.begin(); it != stressPosSet.end(); ++it)
			{
				flog << *it << " ";
			}
			flog << endl;
			count++;
		}
	}
	flog << count << endl;
	flog.close();
}

void DictionaryTrieAccentUpdate::testRusCorporaWord(wstring word)
{
	int stress_pos = word.find_first_of(L"`"); // ` <> '
    if (stress_pos == static_cast<int>(string::npos))
	{
		return;
	}
	// to lower case
	wstring _word = this->getDictionary()->getDictionaryTools()->toLowerCase(word);
	// remove stress from word
	wstring word_without_stress = _word.substr(0, stress_pos) + _word.substr(stress_pos + 1);
	// if word_without_stress is in log -> return
	if (rusCorporaLog.count(_word))
	{
		return;
	}
	// in RusCopora stress is placed in front of character
	stress_pos++;
	// get all stress variants
	vector<WordFormWithStress> WFWS = getWordFormWithStress(word_without_stress);
	// check if have variant coincides with rusCorpora
	int count = 0;
	for (int i = 0; i < (int) WFWS.size(); ++i)
	{
		if (WFWS.at(i).stressPos == stress_pos)
		{
			count++;
			break;
		}
	}
	// if no meet found, add to log
	if (!count)
	{
		RusCorporaLogElement element;
		element.rusCorporaStressPos = stress_pos;
		element.dictionaryStressPos = vector<int>();

		for (int i = 0; i < (int) WFWS.size(); ++i)
		{
			if (WFWS.at(i).stressPos > 0)
			{
				element.dictionaryStressPos.push_back(WFWS.at(i).stressPos);
			}
		}
		rusCorporaLog[_word] = element;

		// 2013-06-17 additional UPDATE
		if (word_without_stress.find_first_of(L"`") == string::npos)
		{
            shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
            this->getDictionary()->getMorphologicalInfo(word_without_stress, minfos, 2);
            for (int i = 0; i < (int) minfos->size(); ++i)
			{
                MorphologicalInfo minfo = minfos->at(i);
				updateStressPos_Ver1(word_without_stress, minfo.lemmaId, minfo.pos, stress_pos, false);
			}
		}
	}
}

void DictionaryTrieAccentUpdate::testRusCorporaLine(wstring line)
{
	vector<wstring> words = this->getDictionary()->getDictionaryTools()->dictionarySplit(line, L" ");
	for (int i = 0; i < (int) words.size(); ++i)
	{
		testRusCorporaWord(words.at(i));
	}
}

void DictionaryTrieAccentUpdate::testRusCorporaFile(string filePath)
{
	wifstream fin(filePath.c_str());
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	while(getline(fin, line)) 
	{
		testRusCorporaLine(line);
	}
	fin.close();
}

void DictionaryTrieAccentUpdate::testRusCorporaFolder(string path, string fileType)
{
#ifdef MSVC
	wcout << "@Test RusCorpora : RusCorpora  BEGIN ..." << endl;
	rusCorporaLog = map<wstring, RusCorporaLogElement>();
	string path_file_type = path + fileType;
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file_type.c_str(), &fd);
	if (h == INVALID_HANDLE_VALUE)
    {
        return;
    }
	int fileCount = 0;
    while(true)
    {
		string file_path = path + fd.cFileName;
		testRusCorporaFile(file_path);
		fileCount++;
		wcout << fileCount << endl;
        if(FindNextFile(h, &fd) == FALSE)
		{
            break;
		}
    }
	wcout << "@TestMorphologicalAnalyze::testFolder. DONE. File count  = " << fileCount << endl;
#endif
}

void DictionaryTrieAccentUpdate::testRusCorporaWriteLogToFile(string filePath)
{
	wcout << "@Test RusCorpora : write log to file ...";
	flog.open(filePath.c_str());
#ifdef MSVC
	flog.imbue(locale(flog.getloc(), new codecvt_utf8<wchar_t>));
#else
	//flog.imbue(std::locale("ru_RU.UTF-8"));
	flog.imbue(std::locale("en_US.UTF-8"));
#endif
	flog << rusCorporaLog.size() << endl;
	for (map<wstring, RusCorporaLogElement>::iterator iter = rusCorporaLog.begin(); iter != rusCorporaLog.end(); ++iter)
	{
		wstring word = iter->first;
		RusCorporaLogElement element = iter->second;
		flog << word << " @ " << element.rusCorporaStressPos << " # ";
		for (int j = 0; j < (int) element.dictionaryStressPos.size(); ++j)
		{
			flog << element.dictionaryStressPos.at(j) << " ";
		}
		flog << endl;
	}
	flog.close();
	wcout << " DONE." << endl;
}

void DictionaryTrieAccentUpdate::readRusCorporaFeatureMap(string filePath)
{
	wcout << "###### Read RusCorpora feature IDs file : " << endl;
	
	RusCorporaFeatureId = map<wstring, int>();
	IdRusCorporaFeature = map<int, wstring>();

	wifstream fin(filePath.c_str());
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	int featureId = 0;
	getline(fin, line); // 1-st line unbihavior
	while(getline(fin, line)) 
	{
		featureId++;
		if (line.size() > 0 && line != L"-")
		{
			RusCorporaFeatureId[line] = featureId;
			//wcout << line << " " << featureId << " " << idFeatureMap.at(featureId) << endl;
		}
	}
	fin.close();
	// additions
	RusCorporaFeatureId[L"ADV-PRO"] = 12;
	RusCorporaFeatureId[L"brev"] = 4;
	RusCorporaFeatureId[L"plen"] = 3;

	for (map<wstring, int>::iterator iter = RusCorporaFeatureId.begin(); iter != RusCorporaFeatureId.end(); ++iter)
	{
		IdRusCorporaFeature[iter->second] = iter->first;
	}

	wcout << "       Done." << endl;
}

void DictionaryTrieAccentUpdate::readAccentLogFile(string filePath)
{
	wcout << "###### readAccentLogFile ... " << endl;
	wifstream fin(filePath.c_str());
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	_stressAndLemmaLog = map<wstring, wstring>();
	getline(fin, line); // 1-st line unbihavior
	int count = 0;
	while(getline(fin, line)) 
	{
		if (line.size() > 1)
		{
			int spacePos = line.find_first_of(L" ");
            if (spacePos != static_cast<int>(string::npos))
			{
				wstring word = line.substr(0, spacePos);
				_stressAndLemmaLog[word] = line;
				//wcout << word << "-" << line << endl;
				count++;
				/*
				if (count >= 10)
				{
					break;
				}
				*/
			}
		}
	}
	fin.close();
	wcout << "       Done. Count = " << count << ". Map size = " << _stressAndLemmaLog.size() << endl;
}

void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_word_updateAccent_2013_07_01(wstring word, int stressPos, vector<int> featureIdList)
{
    shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
    this->getMorphologicalInfoList(word, minfos);
	double maxScore = 0;
	int maxScoreIndex = -1;
    for (int i = 0; i < (int) minfos->size(); ++i)
	{
        MorphologicalInfo minfo = minfos->at(i);
		vector<int> openCorporaFeatureIdList = vector<int>();
		if (minfo.basicFeatureListId > 0 && featureListMap.count(minfo.basicFeatureListId) > 0)
		{
			vector<int> _featuresList = featureListMap.at(minfo.basicFeatureListId);
			openCorporaFeatureIdList.insert(openCorporaFeatureIdList.end(), _featuresList.begin(), _featuresList.end());
		}
		if (minfo.featureListId > 0 && featureListMap.count(minfo.featureListId) > 0)
		{
			vector<int> _featuresList = featureListMap.at(minfo.featureListId);
			openCorporaFeatureIdList.insert(openCorporaFeatureIdList.end(), _featuresList.begin(), _featuresList.end());
		}
		double matchScore = featureListMatchScore(featureIdList, openCorporaFeatureIdList);
		if (matchScore > maxScore)
		{
			maxScore = matchScore;
			maxScoreIndex = i;
		}
	}
	// write to log
	// wstring wordWithStress = stressPos >= 1 && stressPos <= word.length() ? word.substr(0, stressPos) + L"'" + word.substr(stressPos) : word; 
	flog << word << " [" << stressPos << "] :";
	for (int i = 0; i < (int) featureIdList.size(); ++i)
	{
		if (idFeatureMap.count(featureIdList.at(i)))
		{
			flog << " " << idFeatureMap.at(featureIdList.at(i));
		}
	}

	if (maxScore > 0.1)
	{
        MorphologicalInfo minfo = minfos->at(maxScoreIndex);
		updateStressPos(minfo.lemmaId, minfo.pos, stressPos);

		vector<int> openCorporaFeatureIdList = vector<int>();
		if (minfo.basicFeatureListId > 0 && featureListMap.count(minfo.basicFeatureListId) > 0)
		{
			vector<int> _featuresList = featureListMap.at(minfo.basicFeatureListId);
			openCorporaFeatureIdList.insert(openCorporaFeatureIdList.end(), _featuresList.begin(), _featuresList.end());
		}
		if (minfo.featureListId > 0 && featureListMap.count(minfo.featureListId) > 0)
		{
			vector<int> _featuresList = featureListMap.at(minfo.featureListId);
			openCorporaFeatureIdList.insert(openCorporaFeatureIdList.end(), _featuresList.begin(), _featuresList.end());
		}

		flog << " (";
	
		for (int i = 0; i < (int) openCorporaFeatureIdList.size(); ++i)
		{
			if (idFeatureMap.count(openCorporaFeatureIdList.at(i)))
			{
				flog << " " << idFeatureMap.at(openCorporaFeatureIdList.at(i));
			}
		}

		flog << " ) >> ok " << maxScore << endl;
		analyzeRusCorporaVer2LogCount++;
	}
	else
	{
		flog << " >> ## " << maxScore << endl;
	}
}

void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_word_updateAccent(wstring word, int stressPos, vector<int> featureIdList)
{
	if (!_stressAndLemmaLog.count(word))
	{
		return;
	}
	analyzeRusCorporaVer2LogCount++;
	flog << _stressAndLemmaLog.at(word) << " >>> " << word << " (" << stressPos << ")";
	for (int i = 0; i < (int) featureIdList.size(); ++i)
	{
		int featureId = featureIdList.at(i);
		wstring RusCorporaFeature = featureId > 0 && IdRusCorporaFeature.count(featureId) ? IdRusCorporaFeature.at(featureId) : L"?";
		wstring OpenCorporaFeature = featureId > 0 && idFeatureMap.count(featureId) ? idFeatureMap.at(featureId) : L"?";
		flog << " + " << featureId << " (" << RusCorporaFeature << " | " << OpenCorporaFeature << ")";
	}
	flog << endl;
}

void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_word(wstring word_with_attrs)
{
	// find start of attributes <
	int attr_start_pos = word_with_attrs.find_first_of(L"<");
	// find end of attributes <
	int attr_end_pos = word_with_attrs.find_first_of(L">");
	// get word (with stress)
	wstring word_with_stress = word_with_attrs.substr(0, attr_start_pos);
	// get attributes
	wstring attrs = word_with_attrs.substr(attr_start_pos + 1, attr_end_pos - attr_start_pos - 1);
	// get attributes list
	vector<wstring> attrsList = this->getDictionary()->getDictionaryTools()->dictionarySplit(attrs, L" ,=");
	// find stress pos
	int stress_pos = word_with_stress.find_first_of(L"`"); // ` != '
	// if word does not contain stress -> return
    if (stress_pos == static_cast<int>(string::npos))
	{
		return;
	}
	// to lower case
	wstring _word_with_stress = this->getDictionary()->getDictionaryTools()->toLowerCase(word_with_stress);
	// remove stress from word
	wstring word = _word_with_stress.substr(0, stress_pos) + _word_with_stress.substr(stress_pos + 1);
	// in RusCopora stress is placed in front of character
	stress_pos++;
	vector<int> featureIdList = vector<int>();
	
	for (int i = 0; i < (int) attrsList.size(); ++i)
	{
		int featureId = RusCorporaFeatureId.count(attrsList.at(i)) ? RusCorporaFeatureId.at(attrsList.at(i)) : 0;
		featureIdList.push_back(featureId);
	}

	//analyzeRusCorporaVer2_word_updateAccent(word, stress_pos, featureIdList);
	// 2013-07-01 update from RusCorpora
	analyzeRusCorporaVer2_word_updateAccent_2013_07_01(word, stress_pos, featureIdList);
}
	
void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_line(wstring line)
{
	vector<wstring> word_with_attrs_list = this->getDictionary()->getDictionaryTools()->dictionarySplit(line, L" ");
	for (int i = 0; i < (int) word_with_attrs_list.size(); ++i)
	{
		analyzeRusCorporaVer2_word(word_with_attrs_list.at(i));
	}
}
	
void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_file(string filePath)
{
	wifstream fin(filePath.c_str());
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	int lineCount = 0;
	while(getline(fin, line)) 
	{
		analyzeRusCorporaVer2_line(line);
		lineCount++;
		/*
		if (lineCount >= 6)
		{
			break;
		}
		*/
	}
	fin.close();
	//wcout << "      $$$$$$ File analyze done" << endl << endl;
}
	
void DictionaryTrieAccentUpdate::analyzeRusCorporaVer2_folder(string path, string fileType)
{
#ifdef MSVC
	wcout << endl << "@@@@@@ analyzeRusCorporaVer2_folder  BEGIN ..." << endl << endl;

	flog.open("2013_07_01_UpdateRusCorporaLogVer2.txt");
#ifdef MSVC
	flog.imbue(locale(flog.getloc(), new codecvt_utf8<wchar_t>));
#else
	//flog.imbue(std::locale("ru_RU.UTF-8"));
	flog.imbue(std::locale("en_US.UTF-8"));
#endif
	flog << "2013.07.01 : update from ruscorpora" << endl;

	analyzeRusCorporaVer2LogCount = 0;
	string path_file_type = path + fileType;
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file_type.c_str(), &fd);
	if (h == INVALID_HANDLE_VALUE)
    {
        return;
    }
	int fileCount = 0;
    while(true)
    {
		string fileName = fd.cFileName;
		wstring wFileName;
		wFileName.assign(fileName.begin(), fileName.end());
		wcout << "      $$$$$$ File #" << fileCount << " : " << wFileName << endl;
		string file_path = path + fd.cFileName;
		analyzeRusCorporaVer2_file(file_path);
		fileCount++;
		//break;
        if(FindNextFile(h, &fd) == FALSE)
		{
            break;
		}
    }
	flog << analyzeRusCorporaVer2LogCount << endl;
	flog.close();
	wcout << endl <<  "@@@@@@ analyzeRusCorporaVer2_folder. DONE. File count  = " << fileCount << ". Log count = " << analyzeRusCorporaVer2LogCount << endl << endl;
#endif
}

void DictionaryTrieAccentUpdate::initFeatureScore()
{
	featureScore = map<int, double>();
	featureScore[1] = 0.3; //	1	POST	\u0427\u0420	\u0447\u0430\u0441\u0442\u044c \u0440\u0435\u0447\u0438//here was cyrrilic symbols: ЧР,часть,речи
	featureScore[2] = 1.0; //	2	NOUN	\u0421\u0423\u0429	\u0438\u043c\u044f \u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: СУЩ,имя,существительное
	featureScore[3] = 1.0; //	3	ADJF	\u041f\u0420\u0418\u041b	\u0438\u043c\u044f \u043f\u0440\u0438\u043b\u0430\u0433\u0430\u0442\u0435\u043b\u044c\u043d\u043e\u0435 (\u043f\u043e\u043b\u043d\u043e\u0435)//here was cyrrilic symbols: ПРИЛ,имя,прилагательное,полное
	featureScore[4] = 1.0; //	4	ADJS	\u041a\u0420_\u041f\u0420\u0418\u041b	\u0438\u043c\u044f \u043f\u0440\u0438\u043b\u0430\u0433\u0430\u0442\u0435\u043b\u044c\u043d\u043e\u0435 (\u043a\u0440\u0430\u0442\u043a\u043e\u0435)//here was cyrrilic symbols: КР,ПРИЛ,имя,прилагательное,краткое
	featureScore[5] = 1.0; //	5	COMP	\u041a\u041e\u041c\u041f	\u043a\u043e\u043c\u043f\u0430\u0440\u0430\u0442\u0438\u0432//here was cyrrilic symbols: КОМП,компаратив
	featureScore[6] = 1.0; //	6	VERB	\u0413\u041b	\u0433\u043b\u0430\u0433\u043e\u043b (\u043b\u0438\u0447\u043d\u0430\u044f \u0444\u043e\u0440\u043c\u0430)//here was cyrrilic symbols: ГЛ,глагол,личная,форма
	featureScore[7] = 1.0; //	7	INFN	\u0418\u041d\u0424	\u0433\u043b\u0430\u0433\u043e\u043b (\u0438\u043d\u0444\u0438\u043d\u0438\u0442\u0438\u0432)//here was cyrrilic symbols: ИНФ,глагол,инфинитив
	featureScore[8] = 1.0; //	8	PRTF	\u041f\u0420\u0418\u0427	\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435 (\u043f\u043e\u043b\u043d\u043e\u0435)//here was cyrrilic symbols: ПРИЧ,причастие,полное
	featureScore[9] = 1.0; //	10	PRTS	\u041a\u0420_\u041f\u0420\u0418\u0427	\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435 (\u043a\u0440\u0430\u0442\u043a\u043e\u0435)//here was cyrrilic symbols: КР,ПРИЧ,причастие,краткое
	featureScore[10] = 0.3; //	11	GRND	\u0414\u0415\u0415\u041f\u0420	\u0434\u0435\u0435\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435//here was cyrrilic symbols: ДЕЕПР,деепричастие
	featureScore[11] = 0.3; //	12	NUMR	\u0427\u0418\u0421\u041b	\u0447\u0438\u0441\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: ЧИСЛ,числительное
	featureScore[12] = 0.3; //	13	ADVB	\u041d	\u043d\u0430\u0440\u0435\u0447\u0438\u0435//here was cyrrilic symbols: Н,наречие
	featureScore[13] = 0.3; //	14	NPRO	\u041c\u0421	\u043c\u0435\u0441\u0442\u043e\u0438\u043c\u0435\u043d\u0438\u0435-\u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: МС,местоимение,существительное
	featureScore[14] = 0.3; //	15	PRED	\u041f\u0420\u0415\u0414\u041a	\u043f\u0440\u0435\u0434\u0438\u043a\u0430\u0442\u0438\u0432//here was cyrrilic symbols: ПРЕДК,предикатив
	featureScore[15] = 0.3; //	16	PREP	\u041f\u0420	\u043f\u0440\u0435\u0434\u043b\u043e\u0433//here was cyrrilic symbols: ПР,предлог
	featureScore[16] = 0.3; //	17	CONJ	\u0421\u041e\u042e\u0417	\u0441\u043e\u044e\u0437//here was cyrrilic symbols: СОЮЗ,союз
	featureScore[17] = 0.3; //	18	PRCL	\u0427\u0410\u0421\u0422	\u0447\u0430\u0441\u0442\u0438\u0446\u0430//here was cyrrilic symbols: ЧАСТ,частица
	featureScore[18] = 0.3; //	19	INTJ	\u041c\u0415\u0416\u0414	\u043c\u0435\u0436\u0434\u043e\u043c\u0435\u0442\u0438\u0435//here was cyrrilic symbols: МЕЖД,междометие
	featureScore[19] = 0.3; //	21	ANim	\u041e\u0434-\u043d\u0435\u043e\u0434	\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c / \u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0441\u0442\u044c \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d\u0430//here was cyrrilic symbols: Од,неод,одушевлённость,одушевлённость,не,выражена
	featureScore[20] = 0.3; //	22	anim	\u043e\u0434	\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0435//here was cyrrilic symbols: од,одушевлённое
	featureScore[21] = 0.3; //	23	inan	\u043d\u0435\u043e\u0434	\u043d\u0435\u043e\u0434\u0443\u0448\u0435\u0432\u043b\u0451\u043d\u043d\u043e\u0435//here was cyrrilic symbols: неод,неодушевлённое
	featureScore[22] = 0.3; //	24	GNdr	\u0445\u0440	\u0440\u043e\u0434 / \u0440\u043e\u0434 \u043d\u0435 \u0432\u044b\u0440\u0430\u0436\u0435\u043d//here was cyrrilic symbols: хр,род,род,не,выражен
	featureScore[23] = 0.3; //	25	masc	\u043c\u0440	\u043c\u0443\u0436\u0441\u043a\u043e\u0439 \u0440\u043e\u0434//here was cyrrilic symbols: мр,мужской,род
	featureScore[24] = 0.3; //	26	femn	\u0436\u0440	\u0436\u0435\u043d\u0441\u043a\u0438\u0439 \u0440\u043e\u0434//here was cyrrilic symbols: жр,женский,род
	featureScore[25] = 0.3; //	27	neut	\u0441\u0440	\u0441\u0440\u0435\u0434\u043d\u0438\u0439 \u0440\u043e\u0434//here was cyrrilic symbols: ср,средний,род
	featureScore[26] = 0.3; //	28	Ms-f	\u043e\u0440	\u043e\u0431\u0449\u0438\u0439 \u0440\u043e\u0434//here was cyrrilic symbols: ор,общий,род
	featureScore[27] = 0.3; //	29	NMbr	\u0427\u0438\u0441\u043b\u043e	\u0447\u0438\u0441\u043b\u043e//here was cyrrilic symbols: Число,число
	featureScore[28] = 0.3; //	30	sing	\u0435\u0434	\u0435\u0434\u0438\u043d\u0441\u0442\u0432\u0435\u043d\u043d\u043e\u0435 \u0447\u0438\u0441\u043b\u043e//here was cyrrilic symbols: ед,единственное,число
	featureScore[29] = 0.3; //	31	plur	\u043c\u043d	\u043c\u043d\u043e\u0436\u0435\u0441\u0442\u0432\u0435\u043d\u043d\u043e\u0435 \u0447\u0438\u0441\u043b\u043e//here was cyrrilic symbols: мн,множественное,число
	featureScore[30] = 0.3; //	32	Sgtm	sg	singularia tantum
	featureScore[31] = 0.3; //	33	Pltm	pl	pluralia tantum
	featureScore[32] = 0.3; //	36	Fixd	0	\u043d\u0435\u0438\u0437\u043c\u0435\u043d\u044f\u0435\u043c\u043e\u0435//here was cyrrilic symbols: неизменяемое
	featureScore[33] = 0.3; //	37	CAse	\u041f\u0430\u0434\u0435\u0436	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u043f\u0430\u0434\u0435\u0436\u0430//here was cyrrilic symbols: Падеж,категория,падежа
	featureScore[34] = 1.0; //	38	nomn	\u0438\u043c	\u0438\u043c\u0435\u043d\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: им,именительный,падеж
	featureScore[35] = 1.0; //	39	gent	\u0440\u0434	\u0440\u043e\u0434\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: рд,родительный,падеж
	featureScore[36] = 1.0; //	40	datv	\u0434\u0442	\u0434\u0430\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: дт,дательный,падеж
	featureScore[37] = 1.0; //	41	accs	\u0432\u043d	\u0432\u0438\u043d\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: вн,винительный,падеж
	featureScore[38] = 1.0; //	42	ablt	\u0442\u0432	\u0442\u0432\u043e\u0440\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: тв,творительный,падеж
	featureScore[39] = 1.0; //	43	loct	\u043f\u0440	\u043f\u0440\u0435\u0434\u043b\u043e\u0436\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: пр,предложный,падеж
	featureScore[40] = 1.0; //	44	voct	\u0437\u0432	\u0437\u0432\u0430\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: зв,звательный,падеж
	featureScore[41] = 0.3; //	45	gen1	\u0440\u04341	\u043f\u0435\u0440\u0432\u044b\u0439 \u0440\u043e\u0434\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: рд,первый,родительный,падеж
	featureScore[42] = 0.3; //	46	gen2	\u0440\u04342	\u0432\u0442\u043e\u0440\u043e\u0439 \u0440\u043e\u0434\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 (\u0447\u0430\u0441\u0442\u0438\u0447\u043d\u044b\u0439) \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: рд,второй,родительный,частичный,падеж
	featureScore[43] = 0.3; //	47	acc2	\u0432\u043d2	\u0432\u0442\u043e\u0440\u043e\u0439 \u0432\u0438\u043d\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: вн,второй,винительный,падеж
	featureScore[44] = 0.3; //	48	loc1	\u043f\u04401	\u043f\u0435\u0440\u0432\u044b\u0439 \u043f\u0440\u0435\u0434\u043b\u043e\u0436\u043d\u044b\u0439 \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: пр,первый,предложный,падеж
	featureScore[45] = 0.3; //	49	loc2	\u043f\u04402	\u0432\u0442\u043e\u0440\u043e\u0439 \u043f\u0440\u0435\u0434\u043b\u043e\u0436\u043d\u044b\u0439 (\u043c\u0435\u0441\u0442\u043d\u044b\u0439) \u043f\u0430\u0434\u0435\u0436//here was cyrrilic symbols: пр,второй,предложный,местный,падеж
	featureScore[46] = 0.3; //	50	Abbr	\u0430\u0431\u0431\u0440	\u0430\u0431\u0431\u0440\u0435\u0432\u0438\u0430\u0442\u0443\u0440\u0430//here was cyrrilic symbols: аббр,аббревиатура
	featureScore[47] = 0.3; //	51	Name	\u0438\u043c\u044f	\u0438\u043c\u044f//here was cyrrilic symbols: имя,имя
	featureScore[48] = 0.3; //	52	Surn	\u0444\u0430\u043c	\u0444\u0430\u043c\u0438\u043b\u0438\u044f//here was cyrrilic symbols: фам,фамилия
	featureScore[49] = 0.3; //	53	Patr	\u043e\u0442\u0447	\u043e\u0442\u0447\u0435\u0441\u0442\u0432\u043e//here was cyrrilic symbols: отч,отчество
	featureScore[50] = 0.3; //	54	Geox	\u0433\u0435\u043e	\u0442\u043e\u043f\u043e\u043d\u0438\u043c//here was cyrrilic symbols: гео,топоним
	featureScore[51] = 0.3; //	55	Orgn	\u043e\u0440\u0433	\u043e\u0440\u0433\u0430\u043d\u0438\u0437\u0430\u0446\u0438\u044f//here was cyrrilic symbols: орг,организация
	featureScore[52] = 0.3; //	56	Trad	tm	\u0442\u043e\u0440\u0433\u043e\u0432\u0430\u044f \u043c\u0430\u0440\u043a\u0430//here was cyrrilic symbols: торговая,марка
	featureScore[53] = 0.3; //	57	Subx	\u0441\u0443\u0431\u0441\u0442?	\u0432\u043e\u0437\u043c\u043e\u0436\u043d\u0430 \u0441\u0443\u0431\u0441\u0442\u0430\u043d\u0442\u0438\u0432\u0430\u0446\u0438\u044f//here was cyrrilic symbols: субст,возможна,субстантивация
	featureScore[54] = 0.3; //	58	Supr	\u043f\u0440\u0435\u0432\u043e\u0441\u0445	\u043f\u0440\u0435\u0432\u043e\u0441\u0445\u043e\u0434\u043d\u0430\u044f \u0441\u0442\u0435\u043f\u0435\u043d\u044c//here was cyrrilic symbols: превосх,превосходная,степень
	featureScore[55] = 0.3; //	59	Qual	\u043a\u0430\u0447	\u043a\u0430\u0447\u0435\u0441\u0442\u0432\u0435\u043d\u043d\u043e\u0435//here was cyrrilic symbols: кач,качественное
	featureScore[56] = 0.3; //	60	Apro	\u043c\u0435\u0441\u0442-\u043f	\u043c\u0435\u0441\u0442\u043e\u0438\u043c\u0435\u043d\u043d\u043e\u0435//here was cyrrilic symbols: мест,п,местоименное
	featureScore[57] = 0.3; //	61	Anum	\u0447\u0438\u0441\u043b-\u043f	\u043f\u043e\u0440\u044f\u0434\u043a\u043e\u0432\u043e\u0435//here was cyrrilic symbols: числ,п,порядковое
	featureScore[58] = 0.3; //	62	Poss	\u043f\u0440\u0438\u0442\u044f\u0436	\u043f\u0440\u0438\u0442\u044f\u0436\u0430\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: притяж,притяжательное
	featureScore[59] = 0.3; //	63	V-ey	*\u0435\u044e	\u0444\u043e\u0440\u043c\u0430 \u043d\u0430 -\u0435\u044e//here was cyrrilic symbols: ею,форма,на,ею
	featureScore[60] = 0.3; //	64	V-oy	*\u043e\u044e	\u0444\u043e\u0440\u043c\u0430 \u043d\u0430 -\u043e\u044e//here was cyrrilic symbols: ою,форма,на,ою
	featureScore[61] = 0.3; //	65	Cmp2	\u0441\u0440\u0430\u0432\u043d2	\u0441\u0440\u0430\u0432\u043d\u0438\u0442\u0435\u043b\u044c\u043d\u0430\u044f \u0441\u0442\u0435\u043f\u0435\u043d\u044c \u043d\u0430 \u043f\u043e-//here was cyrrilic symbols: сравн,сравнительная,степень,на,по
	featureScore[62] = 0.3; //	66	V-ej	*\u0435\u0439	\u0444\u043e\u0440\u043c\u0430 \u043a\u043e\u043c\u043f\u0430\u0440\u0430\u0442\u0438\u0432\u0430 \u043d\u0430 -\u0435\u0439//here was cyrrilic symbols: ей,форма,компаратива,на,ей
	featureScore[63] = 0.3; //	67	ASpc	\u0412\u0438\u0434	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u0432\u0438\u0434\u0430//here was cyrrilic symbols: Вид,категория,вида
	featureScore[64] = 0.3; //	68	perf	\u0441\u043e\u0432	\u0441\u043e\u0432\u0435\u0440\u0448\u0435\u043d\u043d\u044b\u0439 \u0432\u0438\u0434//here was cyrrilic symbols: сов,совершенный,вид
	featureScore[65] = 0.3; //	69	impf	\u043d\u0435\u0441\u043e\u0432	\u043d\u0435\u0441\u043e\u0432\u0435\u0440\u0448\u0435\u043d\u043d\u044b\u0439 \u0432\u0438\u0434//here was cyrrilic symbols: несов,несовершенный,вид
	featureScore[66] = 0.3; //	70	TRns	\u041f\u0435\u0440\u0435\u0445	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u043e\u0441\u0442\u0438//here was cyrrilic symbols: Перех,категория,переходности
	featureScore[67] = 0.3; //	71	tran	\u043f\u0435\u0440\u0435\u0445	\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439//here was cyrrilic symbols: перех,переходный
	featureScore[68] = 0.3; //	72	intr	\u043d\u0435\u043f\u0435\u0440\u0435\u0445	\u043d\u0435\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439//here was cyrrilic symbols: неперех,непереходный
	featureScore[69] = 0.3; //	73	Impe	\u0431\u0435\u0437\u043b	\u0431\u0435\u0437\u043b\u0438\u0447\u043d\u044b\u0439//here was cyrrilic symbols: безл,безличный
	featureScore[70] = 0.3; //	74	Uimp	\u0431\u0435\u0437\u043b-\u0443	\u0431\u0435\u0437\u043b\u0438\u0447\u043d\u043e\u0435 \u0443\u043f\u043e\u0442\u0440\u0435\u0431\u043b\u0435\u043d\u0438\u0435//here was cyrrilic symbols: безл,у,безличное,употребление
	featureScore[71] = 0.3; //	75	Mult	\u043c\u043d\u043e\u0433	\u043c\u043d\u043e\u0433\u043e\u043a\u0440\u0430\u0442\u043d\u044b\u0439//here was cyrrilic symbols: мног,многократный
	featureScore[72] = 0.3; //	76	Refl	\u0432\u043e\u0437\u0432\u0440	\u0432\u043e\u0437\u0432\u0440\u0430\u0442\u043d\u044b\u0439//here was cyrrilic symbols: возвр,возвратный
	featureScore[73] = 0.3; //	77	PErs	\u041b\u0438\u0446\u043e	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u043b\u0438\u0446\u0430//here was cyrrilic symbols: Лицо,категория,лица
	featureScore[74] = 0.3; //	78	1per	1\u043b	1 \u043b\u0438\u0446\u043e//here was cyrrilic symbols: л,лицо
	featureScore[75] = 0.3; //	79	2per	2\u043b	2 \u043b\u0438\u0446\u043e//here was cyrrilic symbols: л,лицо
	featureScore[76] = 0.3; //	80	3per	3\u043b	3 \u043b\u0438\u0446\u043e//here was cyrrilic symbols: л,лицо
	featureScore[77] = 0.3; //	81	TEns	\u0412\u0440\u0435\u043c\u044f	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u0432\u0440\u0435\u043c\u0435\u043d\u0438//here was cyrrilic symbols: Время,категория,времени
	featureScore[78] = 0.3; //	82	pres	\u043d\u0430\u0441\u0442	\u043d\u0430\u0441\u0442\u043e\u044f\u0449\u0435\u0435 \u0432\u0440\u0435\u043c\u044f//here was cyrrilic symbols: наст,настоящее,время
	featureScore[79] = 0.3; //	83	past	\u043f\u0440\u043e\u0448	\u043f\u0440\u043e\u0448\u0435\u0434\u0448\u0435\u0435 \u0432\u0440\u0435\u043c\u044f//here was cyrrilic symbols: прош,прошедшее,время
	featureScore[80] = 0.3; //	84	futr	\u0431\u0443\u0434	\u0431\u0443\u0434\u0443\u0449\u0435\u0435 \u0432\u0440\u0435\u043c\u044f//here was cyrrilic symbols: буд,будущее,время
	featureScore[81] = 0.3; //	85	MOod	\u041d\u0430\u043a\u043b	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u043d\u0430\u043a\u043b\u043e\u043d\u0435\u043d\u0438\u044f//here was cyrrilic symbols: Накл,категория,наклонения
	featureScore[82] = 0.3; //	86	indc	\u0438\u0437\u044a\u044f\u0432	\u0438\u0437\u044a\u044f\u0432\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435 \u043d\u0430\u043a\u043b\u043e\u043d\u0435\u043d\u0438\u0435//here was cyrrilic symbols: изъяв,изъявительное,наклонение
	featureScore[83] = 0.3; //	87	impr	\u043f\u043e\u0432\u0435\u043b	\u043f\u043e\u0432\u0435\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435 \u043d\u0430\u043a\u043b\u043e\u043d\u0435\u043d\u0438\u0435//here was cyrrilic symbols: повел,повелительное,наклонение
	featureScore[84] = 0.3; //	88	INvl	\u0421\u043e\u0432\u043c	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u0441\u043e\u0432\u043c\u0435\u0441\u0442\u043d\u043e\u0441\u0442\u0438//here was cyrrilic symbols: Совм,категория,совместности
	featureScore[85] = 0.3; //	89	incl	\u0432\u043a\u043b	\u0433\u043e\u0432\u043e\u0440\u044f\u0449\u0438\u0439 \u0432\u043a\u043b\u044e\u0447\u0451\u043d (\u0438\u0434\u0435\u043c, \u0438\u0434\u0435\u043c\u0442\u0435)//here was cyrrilic symbols: вкл,говорящий,включён,идем,идемте
	featureScore[86] = 0.3; //	90	excl	\u0432\u044b\u043a\u043b	\u0433\u043e\u0432\u043e\u0440\u044f\u0449\u0438\u0439 \u043d\u0435 \u0432\u043a\u043b\u044e\u0447\u0451\u043d \u0432 \u0434\u0435\u0439\u0441\u0442\u0432\u0438\u0435 (\u0438\u0434\u0438, \u0438\u0434\u0438\u0442\u0435)//here was cyrrilic symbols: выкл,говорящий,не,включён,в,действие,иди,идите
	featureScore[87] = 0.3; //	91	VOic	\u0417\u0430\u043b\u043e\u0433	\u043a\u0430\u0442\u0435\u0433\u043e\u0440\u0438\u044f \u0437\u0430\u043b\u043e\u0433\u0430//here was cyrrilic symbols: Залог,категория,залога
	featureScore[88] = 0.3; //	92	actv	\u0434\u0435\u0439\u0441\u0442\u0432	\u0434\u0435\u0439\u0441\u0442\u0432\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u0437\u0430\u043b\u043e\u0433//here was cyrrilic symbols: действ,действительный,залог
	featureScore[89] = 0.3; //	93	pssv	\u0441\u0442\u0440\u0430\u0434	\u0441\u0442\u0440\u0430\u0434\u0430\u0442\u0435\u043b\u044c\u043d\u044b\u0439 \u0437\u0430\u043b\u043e\u0433//here was cyrrilic symbols: страд,страдательный,залог
	featureScore[90] = 0.3; //	94	Infr	\u0440\u0430\u0437\u0433	\u0440\u0430\u0437\u0433\u043e\u0432\u043e\u0440\u043d\u043e\u0435//here was cyrrilic symbols: разг,разговорное
	featureScore[91] = 0.3; //	95	Slng	\u0436\u0430\u0440\u0433	\u0436\u0430\u0440\u0433\u043e\u043d\u043d\u043e\u0435//here was cyrrilic symbols: жарг,жаргонное
	featureScore[92] = 0.3; //	96	Arch	\u0430\u0440\u0445	\u0443\u0441\u0442\u0430\u0440\u0435\u0432\u0448\u0435\u0435//here was cyrrilic symbols: арх,устаревшее
	featureScore[93] = 0.3; //	97	Litr	\u043b\u0438\u0442	\u043b\u0438\u0442\u0435\u0440\u0430\u0442\u0443\u0440\u043d\u044b\u0439 \u0432\u0430\u0440\u0438\u0430\u043d\u0442//here was cyrrilic symbols: лит,литературный,вариант
	featureScore[94] = 0.3; //	98	Erro	\u043e\u043f\u0435\u0447	\u043e\u043f\u0435\u0447\u0430\u0442\u043a\u0430//here was cyrrilic symbols: опеч,опечатка
	featureScore[95] = 0.3; //	99	Dist	\u0438\u0441\u043a\u0430\u0436	\u0438\u0441\u043a\u0430\u0436\u0435\u043d\u0438\u0435//here was cyrrilic symbols: искаж,искажение
	featureScore[96] = 0.3; //	100	Ques	\u0432\u043e\u043f\u0440	\u0432\u043e\u043f\u0440\u043e\u0441\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: вопр,вопросительное
	featureScore[97] = 0.3; //	101	Dmns	\u0443\u043a\u0430\u0437	\u0443\u043a\u0430\u0437\u0430\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: указ,указательное
	featureScore[98] = 0.3; //	103	Prnt	\u0432\u0432\u043e\u0434\u043d	\u0432\u0432\u043e\u0434\u043d\u043e\u0435 \u0441\u043b\u043e\u0432\u043e//here was cyrrilic symbols: вводн,вводное,слово
	featureScore[99] = 0.3; //	104	V-be	*\u044c\u0435	\u0444\u043e\u0440\u043c\u0430 \u043d\u0430 -\u044c\u0435//here was cyrrilic symbols: ье,форма,на,ье
	featureScore[100] = 0.3; //	105	V-en	*\u0435\u043d\u0435\u043d	\u0444\u043e\u0440\u043c\u0430 \u043d\u0430 -\u0435\u043d\u0435\u043d//here was cyrrilic symbols: енен,форма,на,енен
	featureScore[101] = 0.3; //	106	V-ie	*\u0438\u0435	\u043e\u0442\u0447\u0435\u0441\u0442\u0432\u043e \u0447\u0435\u0440\u0435\u0437 -\u0438\u0435-//here was cyrrilic symbols: ие,отчество,через,ие
	featureScore[102] = 0.3; //	107	V-bi	*\u044c\u0438	\u0444\u043e\u0440\u043c\u0430 \u043d\u0430 -\u044c\u0438//here was cyrrilic symbols: ьи,форма,на,ьи
	featureScore[103] = 0.3; //	108	Fimp	*\u043d\u0435\u0441\u043e\u0432	\u0434\u0435\u0435\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435 \u043e\u0442 \u0433\u043b\u0430\u0433\u043e\u043b\u0430 \u043d\u0435\u0441\u043e\u0432\u0435\u0440\u0448\u0435\u043d\u043d\u043e\u0433\u043e \u0432\u0438\u0434\u0430//here was cyrrilic symbols: несов,деепричастие,от,глагола,несовершенного,вида
	featureScore[104] = 0.3; //	109	Prdx	\u043f\u0440\u0435\u0434\u043a?	\u043c\u043e\u0436\u0435\u0442 \u0432\u044b\u0441\u0442\u0443\u043f\u0430\u0442\u044c \u0432 \u0440\u043e\u043b\u0438 \u043f\u0440\u0435\u0434\u0438\u043a\u0430\u0442\u0438\u0432\u0430//here was cyrrilic symbols: предк,может,выступать,в,роли,предикатива
	featureScore[105] = 0.3; //	110	Coun	\u0441\u0447\u0435\u0442\u043d	\u0441\u0447\u0451\u0442\u043d\u0430\u044f \u0444\u043e\u0440\u043c\u0430//here was cyrrilic symbols: счетн,счётная,форма
	featureScore[106] = 0.3; //	111	Coll	\u0441\u043e\u0431\u0438\u0440	\u0441\u043e\u0431\u0438\u0440\u0430\u0442\u0435\u043b\u044c\u043d\u043e\u0435 \u0447\u0438\u0441\u043b\u0438\u0442\u0435\u043b\u044c\u043d\u043e\u0435//here was cyrrilic symbols: собир,собирательное,числительное
	featureScore[107] = 0.3; //	112	V-sh	*\u0448\u0438	\u0434\u0435\u0435\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435 \u043d\u0430 -\u0448\u0438//here was cyrrilic symbols: ши,деепричастие,на,ши
	featureScore[108] = 0.3; //	113	Af-p	*\u043f\u0440\u0435\u0434\u043b	\u0444\u043e\u0440\u043c\u0430 \u043f\u043e\u0441\u043b\u0435 \u043f\u0440\u0435\u0434\u043b\u043e\u0433\u0430//here was cyrrilic symbols: предл,форма,после,предлога
	featureScore[109] = 0.3; //	114	Inmx	\u043d\u0435/\u043e\u0434\u0443\u0448?	\u043c\u043e\u0436\u0435\u0442 \u0438\u0441\u043f\u043e\u043b\u044c\u0437\u043e\u0432\u0430\u0442\u044c\u0441\u044f \u043a\u0430\u043a \u043e\u0434\u0443\u0448. / \u043d\u0435\u043e\u0434\u0443\u0448.//here was cyrrilic symbols: не,одуш,может,использоваться,как,одуш,неодуш
	featureScore[110] = 0.3; //	115	Vpre	\u0432_\u043f\u0440\u0435\u0434\u043b	\u0412\u0430\u0440\u0438\u0430\u043d\u0442 \u043f\u0440\u0435\u0434\u043b\u043e\u0433\u0430 ( \u0441\u043e, \u043f\u043e\u0434\u043e, ...)//here was cyrrilic symbols: в,предл,Вариант,предлога,со,подо
}

double DictionaryTrieAccentUpdate::getFeatureScore(int featureId)
{
	if (featureScore.count(featureId))
	{
		return featureScore.at(featureId);
	}
	else
	{
		return 0;
	}
}

double DictionaryTrieAccentUpdate::featureListMatchScore(vector<int> featureIdList_1, vector<int> featureIdList_2)
{
	// init score
	double score = 0;
	// coefficient of score
	double plus_coefficient  = 1.0;
	double minus_coefficient  = 0.2;
	// get the union set of the two lists
	set<int> featureIdSet_1 = set<int>();
	for (int i = 0; i < (int) featureIdList_1.size(); ++i)
	{
		featureIdSet_1.insert(featureIdList_1.at(i));
	}
	set<int> intersection_features = set<int>();
	for (int i = 0; i < (int) featureIdList_2.size(); ++i)
	{
		if (featureIdSet_1.count(featureIdList_2.at(i)))
		{
			intersection_features.insert(featureIdList_2.at(i));
		}
	}
	// calculate score
	int f_count = 0;
	for (int i = 0; i < (int) featureIdList_1.size(); ++i)
	{
		if (intersection_features.count(featureIdList_1.at(i)))
		{
			score += plus_coefficient * getFeatureScore(featureIdList_1.at(i));
		}
		else
		{
			score -= minus_coefficient * getFeatureScore(featureIdList_1.at(i));
		}
		f_count++;
	}
	for (int i = 0; i < (int) featureIdList_2.size(); ++i)
	{
		if (!intersection_features.count(featureIdList_2.at(i)))
		{
			score -= minus_coefficient * getFeatureScore(featureIdList_2.at(i));
			f_count++;
		}
	}
	score /= f_count;
	return score;
}

