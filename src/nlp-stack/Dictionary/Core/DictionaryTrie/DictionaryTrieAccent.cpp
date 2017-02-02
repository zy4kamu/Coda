/**
 * DictionaryTrieAccent.cpp
 */

#include "DictionaryTrieAccent.h"
#include "Dictionary.h"

/**
 * Constructor of DictionaryTrieAccent
 */
DictionaryTrieAccent::DictionaryTrieAccent(Dictionary* _dic, string dictionaryFile) : DictionaryTrieWordFormByFeatures(_dic, dictionaryFile)
{
/*
	try
	{
		readAllModelFromFile(STRESS_POS_FILE_PATH);
	}
	catch (int e)
	{
		if (e == FILE_NOT_FOUND_ERROR_CODE)
		{
			wcout << "Error : File not found : " << STRESS_POS_FILE_PATH << endl;
			getch();
			exit(0);
		}
	}
	saveStressInFormationToBinaryFile("Accent.bin");
*/

	try
	{
		loadStressInFormationFromBinaryFile(dic->getPathToData() + ACCENT_BINARY_FILE_PATH);
	}
	catch (int e)
	{
		if (e == FILE_NOT_FOUND_ERROR_CODE)
		{
			wcout << "Error : File not found : " << ACCENT_BINARY_FILE_PATH << endl;
#ifdef MSVC			
			_getch();
#endif
			exit(0);
		}
	}

}

/**
 * Destructor of DictionaryTrieAccent
 */
DictionaryTrieAccent::~DictionaryTrieAccent(void)
{
	stressPosList.clear();
}

void DictionaryTrieAccent::readAllModelFromFile(string filePath)
{
	/*
	wstring wFilePath;
	wFilePath.assign(filePath.begin(), filePath.end());
	wcout << "readAllModelFromFile :  " << wFilePath << endl;
	*/
	stressPosList = vector<vector<int> >();
	stressModelId = vector<int>();
	
	stressModel = vector<vector<int> >(5410, vector<int>());
	//stressPos.push_back(vector<int>());

	map<vector<int>, int> posMap = map<vector<int>, int>();

	int lineCount = 0;
	int nonEmptySetCount = 0;
	int posSetId = 0;

	wifstream fin(filePath.c_str());
#ifdef MSVC
	fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fin.imbue(std::locale("ru_RU.UTF-8"));
	fin.imbue(std::locale("en_US.UTF-8"));
#endif
	if (fin.is_open())
	{
		wstring line;
		getline(fin, line);
		while(getline(fin, line)) 
		{
			int sharpPos = line.find(L'#');
			wstring posStr = line.substr(sharpPos + 1);
			vector<wstring> pStr = this->getDictionary()->getDictionaryTools()->dictionarySplit(posStr, L"1234567890", false);
			vector<int> pos = vector<int>();
			for (int i = 0; i < (int) pStr.size(); ++i)
			{
				//int posInt = _wtoi(pStr.at(i).c_str());
                int posInt = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(pStr.at(i)).c_str());
				pos.push_back(posInt);
			}
			// add to dictionary
			stressPosList.push_back(pos);
			/*
			if (lineCount <= 100)
			{
				wcout << lineCount << " - " << pos.size() << "#";
				for (int i = 0; i < pos.size(); ++i)
				{
					wcout << " " << pos.at(i);
				}
				wcout << endl;
			}
			*/
			
			if (pos.size() > 0)
			{
				//int pos_0 = pos.at(0);
				//for (int i = 0; i < pos.size(); ++i)
				//{
				//	pos.at(i) -= pos_0;
				//}
				if (!posMap.count(pos))
				{
					posSetId++;
					posMap[pos] = posSetId;
					stressModel[posSetId] = pos;
					stressModelId.push_back(posSetId);
				}
				else
				{
					stressModelId.push_back(posMap.at(pos));
				}
				nonEmptySetCount++;
			}
			else
			{
				stressModelId.push_back(0);
			}
			
			lineCount++;
		}
		fin.close();
	}
	else
	{
		throw FILE_NOT_FOUND_ERROR_CODE;
	}
	
	// write stress model to file, begin
	wofstream fout("2013_07_03_StressPosSet.txt");
#ifdef MSVC	
	fout.imbue(locale(fout.getloc(), new codecvt_utf8<wchar_t>));
#else
	//fout.imbue(std::locale("ru_RU.UTF-8"));
	fout.imbue(std::locale("en_US.UTF-8"));
#endif
	fout << posMap.size() << endl;
	int totalPos = 0;
	for (map<vector<int>, int>::iterator iter = posMap.begin(); iter != posMap.end(); ++iter)
	{
		vector<int> pos = iter->first;
		for (int j = 0; j < (int) pos.size(); ++j)
		{
			fout << pos.at(j) << " ";
		}
		fout << endl;
		totalPos += pos.size();
	}
	fout.close();
	// write stress model to file, done
	wcout << "Done, s.size = " << posMap.size() << ". totalPos = " << totalPos << ". nonEmptySetCount = " << nonEmptySetCount << endl;

}

vector<WordInfo> DictionaryTrieAccent::getWordInfo(wstring word, bool _beginWithPo)
{
	// initialize empty list of WordInfo
	vector<WordInfo> result = vector<WordInfo>();
	// begin from root
    DictionaryNode* current = root;
	// _beginWithPo = true    : begin from 1-st character (_word[2])
	// _beginWithPo = false   : begin from 3-st character (_word[0])
	int i_word = _beginWithPo ? 1 : -1;
	int w_len = word.length();
	wstring _prefix = L"";
    while (i_word < w_len)
    {
		// get list of NodeModel of current node
        vector<DictionaryNodeModel*> * _nodeModels = current->getNodeModels();
		// for each NodeModel
        for (int i_model = 0; i_model < (int)_nodeModels->size(); ++i_model) {
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
            vector<DictionaryTrieModelElement*> * _trieModelElements
                    = _trieModel->getDictionaryTrieModelElements();
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
				if (word.compare(_prefix_suffix) == 0) {
					// create new GrammInfo
					WordInfo _wordInfo;
					// set word for GrammInfo
					_wordInfo.word = word;
					// set id for GrammInfo
					_wordInfo.lemmaId = _nodeModel->getLemmaId();
					// basicFeatureListId
					int basicFeatureListId = _firstTrieModelElement->getFeatureListId();
					// convert to OpenCorpora features
					vector<wstring> openCorporaFeatures = convertFeatureListIdToFeatures(basicFeatureListId);
					// featureListId
					int featureListId = _trieModelElement->getFeatureListId();
					// convert to OpenCorpora features
					vector<wstring> _features = convertFeatureListIdToFeatures(featureListId);
					openCorporaFeatures.insert(openCorporaFeatures.end(), _features.begin(), _features.end());
					_wordInfo.openCorporaFeatures = openCorporaFeatures;
					// stressPos
					
					//int stressPos = stressPosList.at(_wordInfo.lemmaId).size() > i_model_element ? stressPosList.at(_wordInfo.lemmaId).at(i_model_element) : 0;
					vector<int> _stressPosModel = stressModel.at(stressModelId.at(_wordInfo.lemmaId));
					int stressPos = (int) _stressPosModel.size() > i_model_element ? _stressPosModel.at(i_model_element) : 0;
					
					// add stress to word
					wstring _wordWithStress = stressPos >= 1 && stressPos <= (int) word.length() ? word.substr(0, stressPos) + L"1" + word.substr(stressPos) : word;
					_wordInfo.wordWithStress = _wordWithStress;
					// add _grammInfo to result
					result.push_back(_wordInfo);
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
				if (word.compare(_prefix_suffix) == 0) {
					// create new GrammInfo
					WordInfo _wordInfo;
					// set word for GrammInfo
					_wordInfo.word = word;
					// set lemmaId
					_wordInfo.lemmaId = _nodeModel->getLemmaId();
					// basicFeatureListId
					int basicFeatureListId = _trieModelElement->getFeatureListId();
					// convert to OpenCorpora features
					vector<wstring> openCorporaFeatures = convertFeatureListIdToFeatures(basicFeatureListId);
					_wordInfo.openCorporaFeatures = openCorporaFeatures;
					// featuresListId 
//					int featureListId = 0;
					// stressPos
					int stressPos = (int) stressPosList.size() > _wordInfo.lemmaId && stressPosList.at(_wordInfo.lemmaId).size() > 0 ? stressPosList.at(_wordInfo.lemmaId).at(0) : 0;
					wstring _wordWithStress = stressPos >= 1 && stressPos <= (int) word.length() ? word.substr(0, stressPos) + L"1" + word.substr(stressPos) : word;
					_wordInfo.wordWithStress = _wordWithStress;
					// add _grammInfo to result
					result.push_back(_wordInfo);
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
		_prefix = word.substr(0, i_word + 1);
		// find child node by next character
        DictionaryNode* tmp = current->findChild(word[i_word]);
		// break if child node not found
        if ( tmp == NULL ) {
            break;
		}
		// set current node to child node
        current = tmp;
    }
	return result;
}

vector<WordInfo> DictionaryTrieAccent::getWordInfo(wstring word)
{
	vector<WordInfo> result = getWordInfo(word, false);
	if (this->getDictionary()->getDictionaryTools()->stringBeginsWithPo(word))
	{
		vector<WordInfo> result_1 = getWordInfo(word, true);
		result.insert(result.end(), result_1.begin(), result_1.end());
	}
	return result;
}

void DictionaryTrieAccent::writeToBuffer(vector<unsigned char> charVector)
{
	for (int i = 0; i < (int) charVector.size(); ++i)
	{
		buffer[bufferSize] = charVector.at(i);
		bufferSize++;
	}
}

void DictionaryTrieAccent::saveStressInFormationToBinaryFile(string filePath)
{
	wcout << "###### saveStressInFormationToBinaryFile : Begin ... " << endl;
	// init buffer
    buffer = new unsigned char[1000000];
	// init buffer size
	bufferSize = 0;

	//================ STRESS MODELS =========================================================================================

	// number of stress models
	numberOfStressModels = 5408;
	// convert number of stress models to vector<char>
    vector<unsigned char> binNumberOfStressModels = getDictionary()->getDictionaryTools()->intToCharVector2(numberOfStressModels);
	// write to buffer
	writeToBuffer(binNumberOfStressModels);
	// for each stress model
	for (int i = 1; i <= numberOfStressModels; ++i)
	{
		vector<int> stress_model = stressModel.at(i);
		// size of stress model
		int stress_model_size = stress_model.size();
		// convert size of stress model to vector<char>
        vector<unsigned char> binStressModelSize = getDictionary()->getDictionaryTools()->intToCharVector1(stress_model_size);
		// write to buffer
		writeToBuffer(binStressModelSize);
		// for each element
		for (int j = 0; j < stress_model_size; ++j)
		{
			// stress pos
			int p = stress_model.at(j);
			// convert stress pos to vector<char>
            vector<unsigned char> binStressPos = getDictionary()->getDictionaryTools()->intToCharVector1(p);
			// write to buffer
			writeToBuffer(binStressPos);
		}
	}

	//================ STRESS MODEL Ids =========================================================================================

	// size of stressModelId
	int stressModelIdSize = stressModelId.size();
	// convert size of stressModelId to vector<char>
    vector<unsigned char> binStressModelIdSize = getDictionary()->getDictionaryTools()->intToCharVector3(stressModelIdSize);
	// write to buffer
	writeToBuffer(binStressModelIdSize);
	// for each stress model id
	for (int i = 0; i < stressModelIdSize; ++i)
	{
		// stress model id
		int stress_model_id = stressModelId.at(i);
		// convert stress model id to vector<char>
        vector<unsigned char> binStressModelId = getDictionary()->getDictionaryTools()->intToCharVector2(stress_model_id);
		// write to buffer
		writeToBuffer(binStressModelId);
	}
	// write to file
	ofstream f(filePath.c_str(), ios::out|ios::binary);
    f.write((char *)&buffer[0], bufferSize);
	f.close();
	wcout << "###### saveStressInFormationToBinaryFile : Done!" << endl;
}

void DictionaryTrieAccent::loadStressInFormationFromBinaryFile(string filePath)
{
	// init variables
	stressModel = vector<vector<int> >();
	stressModelId = vector<int>();
	// open binary file
	ifstream f(filePath.c_str(), ios::in|ios::binary|ios::ate);
	char* buffer;
	if (f.is_open())
	{
		// get size of file
		int size = (int) f.tellg();
		// jump to begin of file
		f.seekg(0, ios::beg);
		// allocate buffer
		buffer = new char[size];
		// read file
		f.read(buffer, size);
		// close file
		f.close();
		// set offset begin of buffer
		int offset = 0;

		//================ STRESS MODELS =========================================================================================

		// numberOfStressModels = 2 bytes
		int numberOfStressModels = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
		offset += 2;
		// stressModel[0]
		stressModel.push_back(vector<int>());
		// for each stress model
		for (int i = 0; i < numberOfStressModels; ++i)
		{
			// init stress model
			vector<int> stress_model = vector<int>();
			// size of stress model = 1 byte
			int stress_model_size = (unsigned char) buffer[offset];
			offset++;
			// for each element
			for (int j = 0; j < stress_model_size; ++j)
			{
				// stress pos = 1 byte
				int p = (unsigned char) buffer[offset];
				offset++;
				// add stress pos to stress model
				stress_model.push_back(p);
			}
			// add stress_model to stressModel
			stressModel.push_back(stress_model);
		}

		//================ STRESS MODEL Ids =========================================================================================

		// size of stressModelId = 3 bytes
		int stressModelIdSize = (unsigned char) buffer[offset] * 65536 + (unsigned char) buffer[offset + 1] * 256 + (unsigned char) buffer[offset + 2];
		offset += 3;
		// for each stress model id
		for (int i = 0; i < stressModelIdSize; ++i)
		{
			// stress model id = 2 bytes
			int stress_model_id = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
			offset += 2;
			// add to stressModelId
			stressModelId.push_back(stress_model_id);
		}
        delete[] buffer;
	}
	else 
	{
		throw FILE_NOT_FOUND_ERROR_CODE;
	}
}

