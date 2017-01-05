/**
 * DictionaryTrieBinaryFileCreator.cpp
 */

#include "DictionaryTrieBinaryFile.h"
#include "Dictionary.h"

#define LINK_FILE_PATH "2015_03_23_links.txt"
#define MAX_BUFFER_SIZE 15000000

/**
 * Constructor of DictionaryTrieBinaryFileCreator
 */
DictionaryTrieBinaryFileCreator::DictionaryTrieBinaryFileCreator(Dictionary* _dic) : DictionaryTrieBuild(_dic)
{
}

/**
 * Destructor of DictionaryTrieBinaryFileCreator
 */
DictionaryTrieBinaryFileCreator::~DictionaryTrieBinaryFileCreator(void) 
{
	dic = NULL;
}

void DictionaryTrieBinaryFileCreator::writeToBinaryFile(string _filePath) 
{
	wcout << "Write Trie to binary file ..." << endl;
	ofstream f(_filePath.c_str(), ios::out|ios::binary);
    f.write((char *)&buffer[0], bufferSize);
	f.close();
	wcout << "Write Trie to binary file done!" << endl;
}

vector<unsigned char> DictionaryTrieBinaryFileCreator::binaryDictionaryNode(DictionaryNode* _node, int _parentId)
{
    vector<unsigned char> _result;
	_result.push_back(getDictionary()->getDictionaryTools()->wcharToChar(_node->getCharacter()));
    _result.push_back((unsigned char) (_parentId / 65536));
	_parentId %= 65536;
    _result.push_back((unsigned char) (_parentId / 256));
    _result.push_back((unsigned char) (_parentId % 256));
	
	return _result;
}

vector<unsigned char> DictionaryTrieBinaryFileCreator::binaryDictionaryNodeModel(DictionaryNodeModel * _nodeModel, int _nodeId)
{
    vector<unsigned char> _result;
	int _modelId = _nodeModel->getModelId();
    _result.push_back((unsigned char) (_modelId / 256));
    _result.push_back((unsigned char) (_modelId % 256));

	int _lemmaId = _nodeModel->getLemmaId();
    _result.push_back((unsigned char) (_lemmaId / 65536));
	_lemmaId %= 65536;
    _result.push_back((unsigned char) (_lemmaId / 256));
    _result.push_back((unsigned char) (_lemmaId % 256));

    _result.push_back((unsigned char) (_nodeId / 65536));
	_nodeId %= 65536;
    _result.push_back((unsigned char) (_nodeId / 256));
    _result.push_back((unsigned char) (_nodeId % 256));
	
	return _result;
}

void DictionaryTrieBinaryFileCreator::writeToBuffer(vector<unsigned char> charVector)
{
	for (int i = 0; i < (int) charVector.size(); ++i)
	{
		buffer[bufferSize] = charVector.at(i);
		bufferSize++;
	}
}

void DictionaryTrieBinaryFileCreator::addAllDictionaryNodeToBuffer(void)
{
	wcout << "Add all DictionaryNode to buffer ..." << endl;
    wcout << "numberOfNodes = " << numberOfNodes << endl;
	// write number of nodes
    vector<unsigned char> binaryNumberOfNodes = getDictionary()->getDictionaryTools()->intToCharVector3(numberOfNodes);
	writeToBuffer(binaryNumberOfNodes);

	queue<DictionaryNode*> nodeQueue = queue<DictionaryNode*>();
	
	nodeQueue.push(root);
	int currentNodeId = 0;
	
    vector<unsigned char> _binNode = binaryDictionaryNode(root, -1);
	// write root
	writeToBuffer(_binNode);
	
	while (!nodeQueue.empty())
	{
		DictionaryNode* currentNode = nodeQueue.front();
		nodeQueue.pop();
		vector<DictionaryNode*> childrenNodes = currentNode->getChildren();
		for (int i = 0; i < (int) childrenNodes.size(); ++i)
		{
			DictionaryNode* childNode = childrenNodes.at(i);
			nodeQueue.push(childNode);

			_binNode = binaryDictionaryNode(childNode, currentNodeId);
			writeToBuffer(_binNode);
		}
		currentNodeId++;
	}
	wcout << "Add DictionaryNode done! currentNodeId = " << currentNodeId << endl;
}

void DictionaryTrieBinaryFileCreator::addAllDictionaryNodeModelToBuffer(void)
{
	wcout << "Add all DictionaryNodeModel to buffer ... nodeModelMap.size() = " << nodeModelMap.size() << endl;
	wcout << "......................................... numberOfNodeModels  = " << numberOfNodeModels << endl;
	// write number of nodes
    vector<unsigned char> binaryNumberOfNodeModels = getDictionary()->getDictionaryTools()->intToCharVector3((int) nodeModelMap.size());
	writeToBuffer(binaryNumberOfNodeModels);
	
	queue<DictionaryNode*> nodeQueue = queue<DictionaryNode*>();
	
	nodeQueue.push(root);
	int currentNodeId = 0;
	int nodeModelCount = 0;
	while (!nodeQueue.empty())
	{
		DictionaryNode* currentNode = nodeQueue.front();
		nodeQueue.pop();

        vector<DictionaryNodeModel*> * _nodeModels = currentNode->getNodeModels();
        for (int i = 0; i < (int) _nodeModels->size(); ++i)
		{
            DictionaryNodeModel* _nodeModel = _nodeModels->at(i);
            vector<unsigned char> _binNodeModel = binaryDictionaryNodeModel(_nodeModel, currentNodeId);
			writeToBuffer(_binNodeModel);
			nodeModelCount++;
		}

		vector<DictionaryNode*> childrenNodes = currentNode->getChildren();
		for (int i = 0; i < (int) childrenNodes.size(); ++i)
		{
			DictionaryNode* childNode = childrenNodes.at(i);
			nodeQueue.push(childNode);
		}
		currentNodeId++;
	}

	wcout << "Add NodeModels done! nodeModelCount = " << nodeModelCount << endl;
}

/* 
 * add a link between 2 lemmas by lemmaIds
 */
void DictionaryTrieBinaryFileCreator::addLinkByLemmaIdsToBuffer(int fromLemmaId, int toLemmaId) 
{
	// if fromLemmaId or toLemmaId is out of range than return
	// @todo : maxLemmaId, min = 0 or 1 ?
	if (fromLemmaId <= 0 || toLemmaId <= 0) 
	{
		return;
	}
    vector<unsigned char> binaryLemmaId = getDictionary()->getDictionaryTools()->intToCharVector3(fromLemmaId);
	writeToBuffer(binaryLemmaId);
	binaryLemmaId = getDictionary()->getDictionaryTools()->intToCharVector3(toLemmaId);
	writeToBuffer(binaryLemmaId);
	countLink++;
}	
/* 
 * add a link between 2 lemmas
 * eg : 1#2
 */
void DictionaryTrieBinaryFileCreator::addLinkToBuffer(wstring _line) 
{
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
		addLinkByLemmaIdsToBuffer(_fromLemmaId, _toLemmaId);
	}
}

/**
 * Read links from file
 */
void DictionaryTrieBinaryFileCreator::readLinksFromFileToBuffer(string _filePath) 
{
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
    // read number of links
    getline(fin, line);
    numberOfLinks = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    vector<unsigned char> binaryNumberOfLinks = getDictionary()->getDictionaryTools()->intToCharVector3(numberOfLinks);
    writeToBuffer(binaryNumberOfLinks);
    wcout << "Read links to buffer... numberOfLinks = " << numberOfLinks << endl;
    countLink = 0;
    // read links
	while(getline(fin, line)) 
	{
		// must have length >= 3, minimum 1#2
		if (line.length() >= 3) 
		{
			addLinkToBuffer(line);
		}
	}
	fin.close();
	wcout << "Read links to buffer done! countLink = " << countLink << endl;
}

void DictionaryTrieBinaryFileCreator::addAllDictionaryTrieModelToBuffer(void)
{
	wcout << "addAllDictionaryTrieModelToBuffer ... getNumberOfModels() = " << trieModelMap.size() << endl;
    vector<unsigned char> binaryNumberOfTrieModel = getDictionary()->getDictionaryTools()->intToCharVector2(trieModelMap.size());
	writeToBuffer(binaryNumberOfTrieModel);
	int trieModelCount = 0;
	// write trieModelMap by order
	for (int ind = 1; ind <= (int) trieModelMap.size(); ++ind)
	{
		if (trieModelMap.count(ind) == 0)
		{
			wcout << "ERROR 3 : DictionaryTrieBinaryFileCreator::addAllDictionaryTrieModelToBuffer : ind = " << ind << endl; 
		}
		DictionaryTrieModel* _trieModel = trieModelMap.at(ind);

        vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
		//wstring _str = L"";
        vector<unsigned char> binNumberOfTrieModelElement = getDictionary()->getDictionaryTools()->intToCharVector1((int) _trieModelElements->size());
		writeToBuffer(binNumberOfTrieModelElement);
        for (int i = 0; i < (int) _trieModelElements->size(); ++i)
		{
            DictionaryTrieModelElement* _trieModelElement = _trieModelElements->at(i);
            vector<unsigned char> binSuffixLen = getDictionary()->getDictionaryTools()->intToCharVector1((int) _trieModelElement->getSuffix()->size());
			writeToBuffer(binSuffixLen);
            vector<unsigned char> binSuffix = getDictionary()->getDictionaryTools()->wstringToCharVector(*(_trieModelElement->getSuffix()));
			writeToBuffer(binSuffix);
            vector<unsigned char> binBeginWithPo = _trieModelElement->isBeginWithPo() ? getDictionary()->getDictionaryTools()->intToCharVector1(1) : getDictionary()->getDictionaryTools()->intToCharVector1(0);
			writeToBuffer(binBeginWithPo);
            vector<unsigned char> binFeatureListId = getDictionary()->getDictionaryTools()->intToCharVector2(_trieModelElement->getFeatureListId());
			writeToBuffer(binFeatureListId);
		}
		trieModelCount++;
	}
	wcout << "addAllTrieModelToBuffer done! trieModelCount = " << trieModelCount << endl;
}

// add featureListMap to buffer
void DictionaryTrieBinaryFileCreator::addFeatureListMapToBuffer(void)
{
	wcout << "addFeatureListMapToBuffer ... featureListMap.size() = " << featureListMap.size() << endl;
    vector<unsigned char> binNumberOfFeatureList = getDictionary()->getDictionaryTools()->intToCharVector2((int) featureListMap.size());
	writeToBuffer(binNumberOfFeatureList);
	int count = 0;
	// save featureListMap in order 
	for (int iFeatureList = 1; iFeatureList <= (int) featureListMap.size(); ++iFeatureList)
	{
		if (featureListMap.count(iFeatureList) == 0)
		{
			wcout << "ERROR 1 : addFeatureListMapToBuffer : " << iFeatureList << endl;
			continue;
		}
		vector<int> featureList = featureListMap.at(iFeatureList);
        vector<unsigned char> binNumberOfFeature = getDictionary()->getDictionaryTools()->intToCharVector1((int) featureList.size());
		writeToBuffer(binNumberOfFeature);
		for (int i = 0; i < (int) featureList.size(); ++i)
		{
            vector<unsigned char> binFeatureId = getDictionary()->getDictionaryTools()->intToCharVector1((int) featureList.at(i));
			writeToBuffer(binFeatureId);
		}
		count++;
	}
	wcout << "addFeatureListMapToBuffer DONE ! ... count = " << count << endl;
}

// add featureMap to buffer
void DictionaryTrieBinaryFileCreator::addFeatureMapToBuffer(void)
{
	wcout << "addFeatureMapToBuffer ... featureMap.size() = " << featureMap.size() << endl;
    vector<unsigned char> binNumberOfFeature = getDictionary()->getDictionaryTools()->intToCharVector1((int) featureMap.size());
	writeToBuffer(binNumberOfFeature);
	int count = 0;
	// save featureMap in order 
	for (int iFeature = 1; iFeature <= (int) featureMap.size(); ++iFeature)
	{
		if (featureMap.count(iFeature) == 0)
		{
			wcout << "ERROR 2 : addFeatureMapToBuffer : " << iFeature << endl;
			continue;
		}
		wstring feature = featureMap.at(iFeature);
        vector<unsigned char> binFeatureLength = getDictionary()->getDictionaryTools()->intToCharVector1((int) feature.size());
		writeToBuffer(binFeatureLength);
        vector<unsigned char> binFeature = getDictionary()->getDictionaryTools()->wstringToCharVector(feature);
		writeToBuffer(binFeature);
		count++;
	}
	wcout << "addFeatureMapToBuffer DONE ! ... count = " << count << endl;
}


void DictionaryTrieBinaryFileCreator::buildBuffer(void)
{
	wcout << "Build buffer ..." << endl;
    buffer = new unsigned char[MAX_BUFFER_SIZE];
	bufferSize = 0;
	addAllDictionaryNodeToBuffer();
	addAllDictionaryNodeModelToBuffer();
    readLinksFromFileToBuffer(this->getDictionary()->getPathToData() + LINK_FILE_PATH);
	addAllDictionaryTrieModelToBuffer();
	addFeatureListMapToBuffer();
	addFeatureMapToBuffer();
	wcout << "Build done" << endl;
}

void DictionaryTrieBinaryFileCreator::buildBufferFromTextFile(string _filePath)
{
    wcout << "Build buffer from text file..." << endl;
    buffer = new unsigned char[MAX_BUFFER_SIZE];
    bufferSize = 0;
    wifstream fi(_filePath.c_str());
    // set endoding to UTF-8
#ifdef MSVC
    fi.imbue(locale(fi.getloc(), new codecvt_utf8<wchar_t>));
#else
    //fin.imbue(std::locale("ru_RU.UTF-8"));
    fi.imbue(std::locale("en_US.UTF-8"));
#endif
    wstring line;
    // Node
    getline(fi, line);
    int _number_of_nodes = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());

    vector<unsigned char> _binary_number_of_nodes  = getDictionary()->getDictionaryTools()->intToCharVector3(_number_of_nodes);
    writeToBuffer(_binary_number_of_nodes);
    for (int i = 0; i < _number_of_nodes; ++i)
    {
        getline(fi, line);
        /*
        if (i == 35627)
        {
            wcout << "NodeId = " << i << " Char = " << line;
        }
        */
        vector<unsigned char> _binary_node_char;
        _binary_node_char.push_back(getDictionary()->getDictionaryTools()->wcharToChar(line.at(0)));
        writeToBuffer(_binary_node_char);
        getline(fi, line);
        /*
        if (i == 35627)
        {
            wcout << " CharId = " << (int) getDictionary()->getDictionaryTools()->wcharToChar(line.at(0)) << " ParentId = " << endl;
        }
        */
        int _node_parent_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_node_parent_id = getDictionary()->getDictionaryTools()->intToCharVector3(_node_parent_id);
        writeToBuffer(_binary_node_parent_id);
    }
    wcout << "#build : Node ok: " << _number_of_nodes << endl;

    // NodeModel
    getline(fi, line);
    int _number_of_node_models = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_node_models  = getDictionary()->getDictionaryTools()->intToCharVector3(_number_of_node_models);
    writeToBuffer(_binary_number_of_node_models);
    for (int i = 0; i < _number_of_node_models; ++i)
    {
        getline(fi, line);
        int _model_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_model_id = getDictionary()->getDictionaryTools()->intToCharVector2(_model_id);
        writeToBuffer(_binary_model_id);

        getline(fi, line);
        int _lemma_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_lemma_id = getDictionary()->getDictionaryTools()->intToCharVector3(_lemma_id);
        writeToBuffer(_binary_lemma_id);

        getline(fi, line);
        int _node_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_node_id = getDictionary()->getDictionaryTools()->intToCharVector3(_node_id);
        writeToBuffer(_binary_node_id);
/*
        if (_model_id == 872)
        {
            wcout << "ModelId = " << _model_id << " LemmaId = " << _lemma_id << " NodeId = " << _node_id << endl;
        }
*/
    }
    wcout << "#build : NodeModel ok: " << _number_of_node_models << endl;

    // Links
    getline(fi, line);
    int _number_of_links = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_links = getDictionary()->getDictionaryTools()->intToCharVector3(_number_of_links);
    writeToBuffer(_binary_number_of_links);
    for (int i = 0; i < _number_of_links; ++i)
    {
        getline(fi, line);
        int _from_lemma_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_from_lemma_id = getDictionary()->getDictionaryTools()->intToCharVector3(_from_lemma_id);
        writeToBuffer(_binary_from_lemma_id);

        getline(fi, line);
        int _to_lemma_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_to_lemma_id = getDictionary()->getDictionaryTools()->intToCharVector3(_to_lemma_id);
        writeToBuffer(_binary_to_lemma_id);
    }
    wcout << "#build : Links ok: " << _number_of_links << endl;

    // TrieModel
    getline(fi, line);
    int _number_of_trie_models = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    wcout << "#build: _number_of_trie_models = " << _number_of_trie_models << endl;
    vector<unsigned char> _binary_number_of_trie_models = getDictionary()->getDictionaryTools()->intToCharVector2(_number_of_trie_models);
    writeToBuffer(_binary_number_of_trie_models);
    for (int i = 1; i <= _number_of_trie_models; ++i)
    {
        getline(fi, line);
        int _number_of_trie_model_elements = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        vector<unsigned char> _binary_number_of_trie_model_elements = getDictionary()->getDictionaryTools()->intToCharVector1(_number_of_trie_model_elements);
        writeToBuffer(_binary_number_of_trie_model_elements);
//        wcout << "#WriteTM#" << i << ": elements=" << _number_of_trie_model_elements << endl;
        for (int j = 0; j < _number_of_trie_model_elements; ++j)
        {
            // suffix
            getline(fi, line);
//            wcout << "_suffix = " << line;
            int _suffix_length = (int) line.length();
            vector<unsigned char> _binary_suffix_length = getDictionary()->getDictionaryTools()->intToCharVector1(_suffix_length);
            writeToBuffer(_binary_suffix_length);

            vector<unsigned char> _binary_suffix = getDictionary()->getDictionaryTools()->wstringToCharVector(line);
            writeToBuffer(_binary_suffix);

            // begin with PO
            getline(fi, line);
            int _begin_with_po = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
            vector<unsigned char> _binary_begin_with_po = getDictionary()->getDictionaryTools()->intToCharVector1(_begin_with_po);
            writeToBuffer(_binary_begin_with_po);

            // featureListId
            getline(fi, line);
            int _feature_list_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
            vector<unsigned char> _binary_feature_list_id = getDictionary()->getDictionaryTools()->intToCharVector2(_feature_list_id);
            writeToBuffer(_binary_feature_list_id);

//            wcout << " PO = " << _begin_with_po << " featureListId = " << _feature_list_id << endl;
        }
    }
    wcout << "#build : TrieModels ok: " << _number_of_trie_models << endl;

    // Feature lists
    getline(fi, line);
    int _number_of_feature_lists = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    wcout << "_number_of_feature_lists = " << _number_of_feature_lists << endl;
    vector<unsigned char> _binary_number_of_feature_lists = getDictionary()->getDictionaryTools()->intToCharVector2(_number_of_feature_lists);
    writeToBuffer(_binary_number_of_feature_lists);
    for (int i = 0; i < _number_of_feature_lists; ++i)
    {
        getline(fi, line);
        int _number_of_features = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
//        wcout << "   _number_of_features = " << _number_of_features << ": ";
        vector<unsigned char> _binary_number_of_features = getDictionary()->getDictionaryTools()->intToCharVector1(_number_of_features);
        writeToBuffer(_binary_number_of_features);
        for (int j = 0; j < _number_of_features; ++j)
        {
            getline(fi, line);
            int _feature_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
//            wcout << _feature_id << "; ";
            vector<unsigned char> _binary_feature_id = getDictionary()->getDictionaryTools()->intToCharVector1(_feature_id);
            writeToBuffer(_binary_feature_id);
        }
//        wcout << endl;
    }
    wcout << "#build : Feature list ok: " << _number_of_feature_lists << endl;

    // Features map
    getline(fi, line);
    int _number_of_features = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_features = getDictionary()->getDictionaryTools()->intToCharVector1(_number_of_features);
    writeToBuffer(_binary_number_of_features);
    for (int i = 1; i <= _number_of_features; ++i)
    {
        //wcout << "@feature #" << i << endl;
        // short feature
        getline(fi, line);
        int _short_feature_length = (int) line.length();
        //wcout << _short_feature_length << endl;
        //wcout << line << endl;
        vector<unsigned char> _short_binary_feature_length = getDictionary()->getDictionaryTools()->intToCharVector1(_short_feature_length);
        writeToBuffer(_short_binary_feature_length);
        vector<unsigned char> _short_binary_feature = getDictionary()->getDictionaryTools()->wstringToCharVector(line);
        writeToBuffer(_short_binary_feature);

        // long feature
        getline(fi, line);
        int _long_feature_length = (int) line.length();
        //wcout << _long_feature_length << endl;
        //wcout << line << endl;
        vector<unsigned char> _long_binary_feature_length = getDictionary()->getDictionaryTools()->intToCharVector1(_long_feature_length);
        writeToBuffer(_long_binary_feature_length);
        vector<unsigned char> _long_binary_feature = getDictionary()->getDictionaryTools()->wstringToCharVector(line);
        writeToBuffer(_long_binary_feature);
    }
    wcout << "#build : Feature Map ok: " << _number_of_features << endl;

    fi.close();
}

/**
 * Constructor of DictionaryTrieBinaryFileReader
 */
DictionaryTrieBinaryFileReader::DictionaryTrieBinaryFileReader(Dictionary* _dic, string dictionaryFile) : DictionaryTrieBinaryFileCreator(_dic)
{
    try
    {
        loadFromBinaryFile(dic->getPathToData() + dictionaryFile);
    }
    catch (int e)
    {
        if (e == FILE_NOT_FOUND_ERROR_CODE)
        {
            wcout << "Error : Dictionary binary data file not found : " << endl;
#ifdef MSVC
            _getch();
#endif
            exit(0);
        }
    }
}

/**
 * Destructor of DictionaryTrieBinaryFileReader
 */
DictionaryTrieBinaryFileReader::~DictionaryTrieBinaryFileReader(void)
{
    for (int i = 0; i < (int) DictionaryNodeList.size(); ++i)
    {
        DictionaryNode* dictionaryNode = DictionaryNodeList.at(i);
        if (dictionaryNode)
        {
            delete dictionaryNode;
        }
    }
    DictionaryNodeList.clear();
}

/**
 * Load dictionary from binary file
 */
void DictionaryTrieBinaryFileReader::loadFromBinaryFile(string _filePath)
{
    bool debug = false;
//    wcout << "loadFromBinaryFile" << endl;
    ifstream f(_filePath.c_str(), ios::in|ios::binary|ios::ate);
    unsigned char* buffer;
    if (f.is_open())
    {
        // get size of file
        int size = (int) f.tellg();
        // jump to begin of file
        f.seekg(0, ios::beg);

        //============= Read NodeVer3s ============================================================================================
        // allocate buffer
        buffer = new unsigned char[size];
        // read file
        f.read((char*) buffer, size);
        // close file
        f.close();
        // set offset begin of buffer
        int offset = 0;
        // convert 3 bytes to number of NodeVer3
        numberOfNodes = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
        if (debug)
        {
            wcout << "numberOfNodes = " << numberOfNodes << endl;
        }
        offset += 3;

        // read list of NodeVer3
        DictionaryNodeList = vector<DictionaryNode*>();
        for (int _nodeId = 0; _nodeId < numberOfNodes; ++_nodeId)
        {
            // convert first byte to wchar_t
            wchar_t _character = getDictionary()->getDictionaryTools()->charToWchar(buffer[offset]);
            // convert 3 remaining bytes to _parentId
            int _parentId = buffer[offset + 1] * 65536 + buffer[offset + 2] * 256 + buffer[offset + 3];
            offset += 4;

//            if (_nodeId == 35627)
//            {
//                wcout << "NodeId = " << _nodeId << " Char = " << _character << " ParentId = " << _parentId << endl;
//            }

            // create new NodeVer3
            DictionaryNode* _node = new DictionaryNode();
            _node->setCharacter(_character);
            if (_parentId < 0 || _parentId >= _nodeId)
            {
                //wcout << "### Error ### : loadFromBinaryFile -> id = " << _nodeId << " parentId = " << _parentId << endl;
                _node->setParentNode(NULL); // root
            }
            else
            {
                _node->setParentNode(DictionaryNodeList.at(_parentId));
                DictionaryNodeList.at(_parentId)->addChild(_node);
            }
            DictionaryNodeList.push_back(_node);
        }
        root = DictionaryNodeList.at(0);
        root->setParentNode(NULL);
        if (debug)
        {
            wcout << endl << "OK 1 : numberOfNodes = " << numberOfNodes << endl;
        }
        //================ Read NodeModels =========================================================================================

        // read number of NodeModel
        // convert 3 bytes to number of NodeModel
        int numberOfNodeModel = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
        if (debug)
        {
            wcout << "numberOfNodeModel = " << numberOfNodeModel << endl;
        }
        offset += 3;
        // read list of NodeModel
        nodeModelMap = map<int, DictionaryNodeModel*>();
        for (int i = 0; i < numberOfNodeModel; ++i)
        {
            // convert first 2 bytes to modelId
            int _modelId = buffer[offset] * 256 + buffer[offset + 1];
            // convert the next 3 bytes to _lemmaId
            int _lemmaId = buffer[offset + 2] * 65536 + buffer[offset + 3] * 256 + buffer[offset + 4];
            // convert 3 remaining bytes to _nodeId
            int _nodeVer3Id = buffer[offset + 5] * 65536 + buffer[offset + 6] * 256 + buffer[offset + 7];
            offset += 8;
            // create new NodeModel
            if (nodeModelMap.count(_lemmaId))
            {
                //wcout << "##### TrieVer5::loadFromBinaryFile -> Duplicate : lemmaId = " << _lemmaId << " modelId = " << _modelId << endl;
            }
            else
            {
                // create a NodeModel
                DictionaryNodeModel* _nodeModel = new DictionaryNodeModel(_modelId, _lemmaId);
                // add NodeModel to NodeVer3
                DictionaryNodeList.at(_nodeVer3Id)->addNodeModel(_nodeModel);
                // map lemmaId to NodeModel
                nodeModelMap.insert(pair<int, DictionaryNodeModel*>(_lemmaId, _nodeModel));

//                if (_modelId == 872)
//                {
//                    wcout << "NodeId == " << _nodeVer3Id << " ModelId = " << _modelId << endl;
//                }
            }
        }

        if (debug)
        {
            wcout << "OK 2 : numberOfNodeModel = " << numberOfNodeModel << endl;
        }
        //================ Read links =========================================================================================

        // read number of links
        // convert 3 bytes to number of links
        int numberOfLinks = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
        offset += 3;
        if (debug)
        {
            wcout << "numberOfLinks = " << numberOfLinks << endl;
        }
        // read links
        for (int i = 0; i < numberOfLinks; ++i)
        {
            // convert the first 3 bytes to _fromLemmaId
            int _fromLemmaId = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
            // convert the 3 remaining bytes to _toLemmaId
            int _toLemmaId = buffer[offset + 3] * 65536 + buffer[offset + 4] * 256 + buffer[offset + 5];
            offset += 6;
            addLinkByLemmaIds(_fromLemmaId, _toLemmaId);
        }

        if (debug)
        {
            wcout << "OK 3" << endl;
        }

        //================ Read TrieModels =========================================================================================

        // read number of TrieModel
        // convert 2 bytes to number of TrieModel
        int numberOfModels = buffer[offset] * 256 + buffer[offset + 1];
        offset += 2;
        if (debug)
        {
            wcout << "Number of TrieModels = " << numberOfModels << endl;
        }
        // read TrieModels
        for (int i = 1; i <= numberOfModels; ++i)
        {
            // create a TrieModel from string sufixesAttrs
            DictionaryTrieModel* _trieModel = new DictionaryTrieModel();
            // convert 1 bytes to numberOfTrieModelElement
            int numberOfTrieModelElement = buffer[offset];
            offset += 1;
//            wcout << "TM#" << i << ": elements=" << numberOfTrieModelElement << endl;
            for (int j = 0; j < numberOfTrieModelElement; ++j)
            {
                DictionaryTrieModelElement* _modelElement = new DictionaryTrieModelElement();
                // convert 1 byte to suffix's length
                int suffixLength = buffer[offset];
                offset += 1;
                wstring _suffix = L"";
                // read suffix
                for (int k = 0; k < suffixLength; ++k)
                {
                    _suffix.push_back(getDictionary()->getDictionaryTools()->charToWchar(buffer[offset + k]));
                }
                offset += suffixLength;
//                wcout << "_suffix = " << _suffix << " ";
                // set suffix for DictionaryTrieModelElement
                _modelElement->setSuffix(_suffix);
                // convert 1 byte to beginWithPo
                int _beginWithPo = buffer[offset];
                offset += 1;
//                wcout << "Po = " << _beginWithPo << " ";
                // set beginWithPo for DictionaryTrieModelElement
                _modelElement->setBeginWithPo(_beginWithPo == 1);
                // convert 2 byte to featureListId
                int _featureListId = buffer[offset] * 256 + buffer[offset + 1];
                offset += 2;
//                wcout << "_featureListId = " << _featureListId << endl;
                // set featureListId for DictionaryTrieModelElement
                _modelElement->setFeatureListId(_featureListId);
                // add DictionaryTrieModelElement to DictionaryTrieModel
                _trieModel->addDictionaryTrieModelElement(_modelElement);
            }
            // map modelIndex to trieModel
            mapModelIndexToTrieModel(i, _trieModel);
        }

        if (debug)
        {
            wcout << "OK 4" << endl;
        }
        //================ Read featureListMap  =========================================================================================

        // read number of FeatureList
        // convert 2 bytes to number of FeatureList
        int numberOfFeatureList = buffer[offset] * 256 + buffer[offset + 1];
        if (debug)
        {
            wcout << "numberOfFeatureList = " << numberOfFeatureList << endl;
        }
        offset += 2;
        for (int i = 1; i <= numberOfFeatureList; ++i)
        {
            // read number of features in list
            int numberOfFeature = buffer[offset];
//            wcout << "   numberOfFeature = " << numberOfFeature << ": ";
            offset += 1;
            vector<int> featureIdList = vector<int>();
            // read features
            for (int j = 0; j < numberOfFeature; ++j)
            {
                // convert 1 byte to featureId
                int featureId = buffer[offset];
//                wcout << featureId << "; ";
                offset += 1;
                // add featureId to featureIdList
                featureIdList.push_back(featureId);
            }
//            wcout << endl;
            // insert featureIdList to featureListMap
            featureListMap.insert(pair<int, vector<int> >(i, featureIdList));
        }

        if (debug)
        {
            wcout << "OK 5" << endl;
        }

        //================ Read featureMap  =========================================================================================

        // read number of features
        // convert 1 bytes to number of FeatureList

        featureIdMap.clear();
        idFeatureMap.clear();

        int _numberOfFeature = buffer[offset];
        if (debug)
        {
            wcout << "_numberOfFeature = " << _numberOfFeature << endl;
        }
        offset += 1;
        for (int i = 1; i <= _numberOfFeature; ++i)
        {

            // short feature
            // convert 1 byte to feature's length
            int _short_feature_length = buffer[offset];
            offset += 1;
            // read feature
            wstring _short_feature = L"";
            for (int j = 0; j < _short_feature_length; ++j)
            {
                //wcout << buffer[offset + j] << "; ";
                _short_feature.push_back(getDictionary()->getDictionaryTools()->charToWchar(buffer[offset + j]));
            }
            //wcout << endl;
            offset += _short_feature_length;
            // insert _feature to featureMap
            featureIdMap.insert(pair<wstring, int>(_short_feature, i));
            idFeatureMap.insert(pair<int, wstring>(i, _short_feature));
            if (debug)
            {
                wcout << "Short feature (BIN) #" << i << ": (" << _short_feature_length << ") " << _short_feature << endl;
            }

//            // long feature
//            // convert 1 byte to feature's length
//            int _long_feature_length = buffer[offset];
//            offset += 1;
//            // read feature
//            wstring _long_feature = L"";
//            for (int j = 0; j < _long_feature_length; ++j)
//            {
//                //wcout << buffer[offset + j] << "; ";
//                _long_feature.push_back(getDictionary()->getDictionaryTools()->charToWchar(buffer[offset + j]));
//            }
//            //wcout << endl;
//            offset += _long_feature_length;
//            // insert _feature to featureMap
//            featureMap.insert(pair<int, wstring>(i, _long_feature));
//            if (debug)
//            {
//                wcout << "Long feature (BIN) #" << i << ": (" << _long_feature_length << ") " << _long_feature << endl;
//            }
            //m.kudinov: fixing strange bug. looked like no short version was provided
            featureMap.insert(pair<int, wstring>(i, _short_feature));
        }
        if (debug)
        {
            wcout << "OK 6" << endl;
        }
        //================ Loading done =========================================================================================
        delete[] buffer;
        buffer = NULL;
        if (debug)
        {
            wcout << "... loadFromBinaryFile done!" << endl;
        }
    }
    else
    {
        throw FILE_NOT_FOUND_ERROR_CODE;
        wcout << "### Error ### : loadFromBinaryFile -> Unable to open file" << endl;
    }
}
