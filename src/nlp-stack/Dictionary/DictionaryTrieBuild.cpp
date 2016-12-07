/**
 * DictionaryTrieBuild.cpp
 */

#include "DictionaryTrieBuild.h"
#include "Dictionary.h"

#define DIC_FILE_PATH "C:\\Python33\\_Ha\\2013_03_11_DictionaryVer5\\2013_03_12_dicRootModel_Ver5.txt"
#define LINK_FILE_PATH "C:\\Python33\\_Ha\\2013_02_07_ImproveDictionary\\2013_02_08_links.txt"

/**
 * Constructor of DictionaryTrieBuild
 */
DictionaryTrieBuild::DictionaryTrieBuild(Dictionary* _dic) : DictionaryTrie(_dic)
{
    //this->readFromTextFiles(DIC_FILE_PATH, LINK_FILE_PATH);
}

/**
 * Destructor of DictionaryTrieBuild
 */
DictionaryTrieBuild::~DictionaryTrieBuild(void) 
{
	dic = NULL;
}

/**
 * Read models, LPMs and links from text files
 */
void DictionaryTrieBuild::readFromTextFiles(string _modelLpmFilePath, string _linkFilePath) 
{
	root = new DictionaryNode();
	numberOfNodes = 1;
	readModelsAndLPMsFromFile(_modelLpmFilePath);
	readLinksFromFile(_linkFilePath);
}

/**
 * Read models and LPMs from file
 */
void DictionaryTrieBuild::readModelsAndLPMsFromFile(string _filePath) 
{
	//wcout << "Read models ... " << endl;
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

	// read number of features
	getline(fin, line);
	// read features
	while(getline(fin, line)) 
	{
		// stop if reach end of index
		if (line.length() >= 2 && line.at(0) == L'#' && line.at(1) == L'-') 
		{
			break;
		}
		// process line to model
		//wcout << line << endl;
		addFeature(line);
	}

	// read number of featureLists
	getline(fin, line);
	// read features
	while(getline(fin, line)) 
	{
		// stop if reach end of index
		if (line.length() >= 2 && line.at(0) == L'#' && line.at(1) == L'-') 
		{
			break;
		}
		// process line to featureList
		//wcout << line << endl;
		addFeatureList(line);
	}

	// read number of models
	getline(fin, line);
	// get numerical value of number of models
	//numberOfModels = _wtoi(line.c_str());

	//wcout << "Read model model ... " << endl;
	// read models
	while(getline(fin, line)) 
	{
		// stop if reach end of index
		if (line.length() >= 2 && line.at(0) == L'#' && line.at(1) == L'-') 
		{
			break;
		}
		// process line to model
		//wcout << line << endl;
		addModel(line);
	}
	// read max lemma index
	getline(fin, line);
	// @todo
	//maxLemmaId = _wtoi(line.c_str());

	// read LPMs and model index
	//wcout << "Read models LPM... " << endl;
	while(getline(fin, line)) 
	{
		// process line to LPM
		addLPM(line);
	}
	// close file
	fin.close();
	//wcout << "Read models & LPMs done! " << endl;
}

/**
 * Read links from file
 */
void DictionaryTrieBuild::readLinksFromFile(string _filePath) 
{
	//wcout << "Read links ... ";
	// open file
	wifstream fin(_filePath.c_str());
	// set endoding to UTF-8
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	wstring line;
	// read links
	while(getline(fin, line)) 
	{
		// must have length >= 3, minimum 1#2
		if (line.length() >= 3) 
		{
			addLink(line);
		}
	}
	fin.close();
	//wcout << "done!" << endl;
}

/* 
 * add a line (rule)
 * eg : @PRTS@perf@past@pssv&@masc@sing&\u0430@femn@sing&\u043e@neut@sing&\u044b@plur#20//here was cyrrilic symbols: а,о,ы
 *      \u0430@NOUN@anim@femn@Sgtm@Surn&\u0430@sing@nomn&\u043e\u0439@sing@ablt&\u043e\u0439@sing@datv&\u043e\u0439@sing@gent&\u043e\u0439@sing@loct&\u0443@sing@accs#21//here was cyrrilic symbols: а,а,ой,ой,ой,ой,у
 */
void DictionaryTrieBuild::addModel(wstring _line) 
{
	// find position of # in _str, which separates suffixes-attributes and modelIndex
	int sharpPos = _line.find(L'#');
	// suffixes + attributes is before #
	wstring sufixesAttrs = _line.substr(0, sharpPos);
	// modelIndex part is after #
	wstring modelIndexStr = _line.substr(sharpPos + 1);
	// get numeric value of modelIndex
	//int modelIndex = _wtoi(modelIndexStr.c_str());
    int modelIndex = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(modelIndexStr).c_str());
	// continue if modelIndex is invalid
	if (modelIndex <= 0) 
	{
		return;
	}
	// create a TrieModel from string sufixesAttrs
	DictionaryTrieModel* trieModel = getTrieModelFromString(sufixesAttrs);

	// map modelIndex to trieModel
	mapModelIndexToTrieModel(modelIndex, trieModel);
}

/* 
 * add a LPM + modelIndex
 * eg : #11195&1
 *      #500&2
 *      \u0451\u0436\u0438\u043a#38&3//here was cyrrilic symbols: ёжик
 */
void DictionaryTrieBuild::addLPM(wstring _line) 
{
	int andPos = _line.find(L'&');
	int sharpPos = _line.find(L'#');
	// get LPM
	wstring lpm = _line.substr(0, sharpPos);
	// get model index
	wstring strModelIndex = _line.substr(sharpPos + 1, andPos - sharpPos - 1);
	// get lemma index
	wstring strLemmaIndex = _line.substr(andPos + 1);
	// get numerical value of modelIndex
	//int modelIndex = _wtoi(strModelIndex.c_str());
    int modelIndex = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(strModelIndex).c_str());
	// get numerical value of lemmaIndex
	//int lemmaIndex = _wtoi(strLemmaIndex.c_str());
    int lemmaIndex = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(strLemmaIndex).c_str());
	// stop if modelIndex or lemmaIndex is invalid

	//@todo : modelIndex > maxModelIndex
	if (modelIndex < 0 || lemmaIndex < 0) 
	{
		return;
	}
	addLPMModel(lpm, modelIndex, lemmaIndex);
}

/* 
 * add a link between 2 lemmas
 * eg : 1#2
 */
void DictionaryTrieBuild::addLink(wstring _line) 
{
	numberOfLinks++;
	// find first occurrence of '#'
	int sharpPos = _line.find(L"#");
	if (sharpPos >= 0) 
	{
		// convert 2 strings to int
		wstring fromLemmaIndexStr = _line.substr(0, sharpPos);
		//int _fromLemmaId = _wtoi(fromLemmaIndexStr.c_str());
        int _fromLemmaId = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(fromLemmaIndexStr).c_str());
		wstring toLemmaIndexStr = _line.substr(sharpPos + 1);
		//int _toLemmaId = _wtoi(toLemmaIndexStr.c_str());
        int _toLemmaId = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(toLemmaIndexStr).c_str());
		addLinkByLemmaIds(_fromLemmaId, _toLemmaId);
	}
}

/* 
 * add a feature
 * eg : 
 */
void DictionaryTrieBuild::addFeature(wstring _line) 
{
	// find first occurrence of '@'
	int _pos = _line.find(L"@");
	if (_pos >= 0) 
	{
		// convert 2 strings to int
		wstring _str1 = _line.substr(0, _pos);
		//int _featureId = _wtoi(_str1.c_str());
        int _featureId = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(_str1).c_str());
		wstring _str2 = _line.substr(_pos + 1);
		if (featureMap.count(_featureId) == 0)
		{
			featureMap.insert(pair<int, wstring>(_featureId, _str2));
		}
	}
}

/* 
 * add a featureList
 * eg : 
 */
void DictionaryTrieBuild::addFeatureList(wstring _line) 
{
	// find first occurrence of '#'
	int _pos = _line.find(L"%");
	if (_pos >= 0) 
	{
		// convert 2 strings to int
		wstring _str1 = _line.substr(0, _pos);
		wstring _str2 = _line.substr(_pos + 1);
		//int _featureListId = _wtoi(_str2.c_str());
        int _featureListId = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(_str2).c_str());
		if (featureListMap.count(_featureListId) == 0)
		{
			vector<wstring> toks = dic->getDictionaryTools()->dictionarySplit(_str1, L"@");
			vector<int> tmp = vector<int>();
			for (int i = 0; i < (int) toks.size(); ++i)
			{
                tmp.push_back(atoi(this->getDictionary()->getDictionaryTools()->wstring2string(toks.at(i)).c_str()));
			}
			featureListMap.insert(pair<int, vector<int> >(_featureListId, tmp));
		}
	}
}

/**
 * get TrieModel from string
 * return NULL if string can not be converted to TrieModel
 */
DictionaryTrieModel* DictionaryTrieBuild::getTrieModelFromString(wstring _str) 
{
	// return NULL if _str is empty
	if (_str.length() == 0) 
	{
		return NULL;
	}
	// create a TrieModel
	DictionaryTrieModel *trieModel = new DictionaryTrieModel();

	// tokenize suffix_attrs and add to model list
	// [suffix_attr]s are separated by '&'
	vector<wstring> toks = dic->getDictionaryTools()->dictionarySplit(_str, L"&");
	for (int i_tok = 0; i_tok < (int) toks.size(); ++ i_tok) 
	{
		wstring tok = toks.at(i_tok);
		DictionaryTrieModelElement* trieModelElement = getTrieModelElementFromString(tok);
		// if trieModelElement is valid than add it to trieModel
		if (trieModelElement != NULL && trieModelElement->isValid()) {
			trieModel->addDictionaryTrieModelElement(trieModelElement);
		}
	}
	return trieModel;
}

/**
 * get TrieModelElement from string
 * return NULL if string can not be converted to TrieModelElement
 */
DictionaryTrieModelElement* DictionaryTrieBuild::getTrieModelElementFromString(wstring _str) 
{
	// return NULL if _str is empty
	if (_str.length() == 0) 
	{
		return NULL;
	}
	// find position of '@' - \u0430@29//here was cyrrilic symbols: а
	int atPos = _str.find(L'@');
	// return NULL if not found '@'
	if (atPos < 0) 
	{
		return NULL;
	}
	// suffix is the part of string before first '@'
	wstring suffix = _str.substr(0, atPos);
	// attributes are the remaining
	wstring attrs = _str.substr(atPos + 1);

	DictionaryTrieModelElement* trieModelElement = new DictionaryTrieModelElement();
	// if suffix begins with '*' - word begins with PO
	if (suffix.length() > 0 && suffix.at(0) == L'*') 
	{
		trieModelElement->setBeginWithPo(true);
		suffix = suffix.substr(1);
	}
	// set suffix to trieModelElement
	trieModelElement->setSuffix(suffix);
	// convert attrs to int
	//int _featureListId = _wtoi(attrs.c_str());
    int _featureListId = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(attrs).c_str());
	// set featureListId for trieModelElement
	trieModelElement->setFeatureListId(_featureListId);
	return trieModelElement;
}

