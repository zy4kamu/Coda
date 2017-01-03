/**
 * SuffixModelTrieBuild.cpp
 */

#include "SuffixModelTrieBuild.h"
#include "Dictionary.h"

#define CORPUS_PATH "E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_03_18_OpenCorporaCorpus\\text\\"

SuffixModelTrieBuild::SuffixModelTrieBuild(Dictionary* _dic) : SuffixModelTrie(_dic)
{
	updateFromDictionaryTrie(dic->getDictionaryTrie());
	updateFromDirectory(CORPUS_PATH, "*.txt");
}

SuffixModelTrieBuild::~SuffixModelTrieBuild(void)
{
}

/**
 * addSuffixFromDictionaryNode
 */
void SuffixModelTrieBuild::updateFromDictionaryNode(DictionaryNode* _dictionaryNode)
{
	wstring _prefix = _dictionaryNode->getStringFromRoot();
	// get list of NodeModel of node
    vector<DictionaryNodeModel*> * _nodeModels = _dictionaryNode->getNodeModels();
	// for each NodeModel
    for (int i_model = 0; i_model < (int) _nodeModels->size(); ++i_model) {
        DictionaryNodeModel* _nodeModel = _nodeModels->at(i_model);
		// get modelId of NodeModel
		int _trieModelId = _nodeModel->getModelId();
		// get TrieModel by _trieModelId
		DictionaryTrieModel *_trieModel = dic->getDictionaryTrie()->getTrieModelByModelId(_trieModelId);
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
		// get first TrieModelElement
        DictionaryTrieModelElement* _firstTrieModelElement = _trieModelElements->at(0);

		// for each TrieModelElement, from the 2nd TrieModelElement
        for (int i_model_element = 0; i_model_element < (int) _trieModelElements->size(); ++i_model_element) {
            DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i_model_element);
			// get suffix of TrieModelElement
            wstring _suffix = *(_trieModelElement->getSuffix());
			// concat prefix and suffix
			wstring _word = _prefix + _suffix;
			// if _beginWithPo
			if (_trieModelElement->isBeginWithPo()) {
				_word = L"\u043f\u043e" + _word;//here was cyrrilic symbols: по
			}
			// get only suffix of length less than MAX_SUFFIX_MODEL_TRIE_DEPTH
			if ((int) _word.length() > MAX_SUFFIX_MODEL_TRIE_DEPTH)
			{
				_word = _word.substr(_word.length() - MAX_SUFFIX_MODEL_TRIE_DEPTH);
			}
			// get featureListId
			int _featureListId = _trieModelElement->getFeatureListId();
			// get basicFeatureListId
			int _basicFeatureListId = i_model_element > 0 ? _firstTrieModelElement->getFeatureListId() : 0;
			// update suffixModelTrie
			updateSuffix(_word, _basicFeatureListId, _featureListId);
		}
	}
}

/**
 * build SuffixModelTrie
 */
void SuffixModelTrieBuild::updateFromDictionaryTrie(DictionaryTrieWordFormByFeatures* _dictionaryTrie)
{
	wcout << "-----------------------------------------------" << endl;
	wcout << "SuffixModelTrieBuild::updateFromDictionaryTrie ... ";
	// init an empty queue of DictionaryNode
	queue<DictionaryNode*> nodeQueue = queue<DictionaryNode*>();
	// add root to queue
	nodeQueue.push(_dictionaryTrie->getRoot());
	while (!nodeQueue.empty())
	{
		// pop out a node
		DictionaryNode* currentNode = nodeQueue.front();
		nodeQueue.pop();
		updateFromDictionaryNode(currentNode);

		vector<DictionaryNode*> childrenNodes = currentNode->getChildren();
		for (int i = 0; i < (int) childrenNodes.size(); ++i)
		{
			DictionaryNode* childNode = childrenNodes.at(i);
			nodeQueue.push(childNode);
		}
	}
	wcout << "done!" << endl;
}

void SuffixModelTrieBuild::updateFromSentence(wstring _sentence)
{
	vector<wstring> words = dic->getDictionaryTools()->dictionarySplit(_sentence, WORD_CHARACTERS, false);
	for (int i = 0; i < (int) words.size(); ++i)
	{
        shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
        dic->getMorphologicalInfo(words.at(i), minfos, 3); // not considering non-dictionary words
		wstring _suffix = (int) words.at(i).length() > MAX_SUFFIX_MODEL_TRIE_DEPTH ? words.at(i).substr(words.at(i).length() - MAX_SUFFIX_MODEL_TRIE_DEPTH) : words.at(i);
        updateSuffix(_suffix, *minfos);
	}
}

void SuffixModelTrieBuild::updateFromFile(string _filePath)
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
	while(getline(fin, line)) {
		line = dic->getDictionaryTools()->refineString(line);
		vector<wstring> sentences = dic->getDictionaryTools()->dictionarySplit(line, SENTENCE_DELIMITER);
		for (int i = 0; i < (int) sentences.size(); ++i)
		{
			updateFromSentence(sentences.at(i));
		}
	}
	// close file
	fin.close();
}

void SuffixModelTrieBuild::updateFromDirectory(string _path, string _file_type)
{
#ifdef MSVC
	wcout << "@SuffixModelTrieBuild::updateFromDirectory BEGIN ..." << endl;
	string path_file_type = _path + _file_type;
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file_type.c_str(), &fd);
 
    if (h == INVALID_HANDLE_VALUE)
    {
		wcout << " ... path not found" << endl;
        return; // no files found
    }
	int count = 0;
    while(true)
    {
		string file_path = _path + fd.cFileName;
		this->updateFromFile(file_path);

		count++;
		wcout << "#" << count << endl;

        if(FindNextFile(h, &fd) == FALSE)
		{
            break;
		}
    }
	wcout << "@updateFromDirectory DONE" << endl;
	wcout << "---------------------------------------------------------" << endl;
#endif
}


