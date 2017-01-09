/**
 * SuffixModelTrieBinaryFileCreator.cpp
 */

#include "SuffixModelTrieBinaryFile.h"
#include "Dictionary.h"

#define N_GRAM_MAX_BUFFER_SIZE 15000000
//#define BINARY_SUFFIX_PATH "SuffixModel.bin"

SuffixModelTrieBinaryFileCreator::SuffixModelTrieBinaryFileCreator(Dictionary* _dic) : SuffixModelTrieBuild(_dic)
{
	max_frequency_size = 974;
	max_frequency = 42031;
	max_feature_frequency = 265360;
	saveToBinaryFile(BINARY_SUFFIX_PATH);
}

SuffixModelTrieBinaryFileCreator::~SuffixModelTrieBinaryFileCreator(void)
{
}

void SuffixModelTrieBinaryFileCreator::writeToBuffer(vector<unsigned char> charVector)
{
	for (int i = 0; i < (int) charVector.size(); ++i)
	{
		buffer[bufferSize] = charVector.at(i);
		bufferSize++;
	}
}

vector<unsigned char> SuffixModelTrieBinaryFileCreator::binarySuffixModelNode(SuffixModelNode * _node, int _parentId)
{
    vector<unsigned char> _result;
	_result.push_back(getDictionary()->getDictionaryTools()->wcharToChar(_node->getCharacter()));
    vector<unsigned char> binParentId = getDictionary()->getDictionaryTools()->intToCharVector3(_parentId);
	_result.insert(_result.end(), binParentId.begin(), binParentId.end());
	
	if (max_frequency_size < (int) _node->getFeatureFrequencyMap().size())
	{
		max_frequency_size = (int) _node->getFeatureFrequencyMap().size();
		wcout << "update max_frequency_size = " << max_frequency_size << endl;
	}

	map<int, int> _frequencyMap = _node->getFeatureFrequencyMap();
    vector<unsigned char> binSizeOfFrequencyMap = getDictionary()->getDictionaryTools()->intToCharVector2((int) _frequencyMap.size());
	_result.insert(_result.end(), binSizeOfFrequencyMap.begin(), binSizeOfFrequencyMap.end());
	if (!_frequencyMap.size())
	{
		return _result;
	}
	map<int, int>::iterator iter;
	for (iter = _frequencyMap.begin(); iter != _frequencyMap.end(); ++iter)
	{
		int _featureId = iter->first;
        vector<unsigned char> binFeatureId = getDictionary()->getDictionaryTools()->intToCharVector3(_featureId);
		_result.insert(_result.end(), binFeatureId.begin(), binFeatureId.end());
		int _frequency = iter->second;
		
		if (max_frequency < _frequency)
		{
			max_frequency = _frequency;
			wcout << "update max_frequency = " << max_frequency << endl;
		}
		
        vector<unsigned char> binFrequency = getDictionary()->getDictionaryTools()->intToCharVector2(_frequency);
		_result.insert(_result.end(), binFrequency.begin(), binFrequency.end());
	}
	return _result;
}

void SuffixModelTrieBinaryFileCreator::buildBuffer(void)
{
	wcout << "SuffixModelTrieBinaryFileCreator build buffer ...";
    buffer = new unsigned char[N_GRAM_MAX_BUFFER_SIZE];
	bufferSize = 0;
	wcout << "numberOfNodes = " << numberOfNodes << endl;
    vector<unsigned char> binNumberOfNode = getDictionary()->getDictionaryTools()->intToCharVector3(numberOfNodes);
	writeToBuffer(binNumberOfNode);
	
	queue<SuffixModelNode*> nodeQueue = queue<SuffixModelNode*>();
	nodeQueue.push(root);
	int currentNodeId = -1;
	int _count = 1;
	
	// write root
    vector<unsigned char> binRoot = binarySuffixModelNode(root, currentNodeId);
	writeToBuffer(binRoot);

	while (!nodeQueue.empty())
	{
		SuffixModelNode* currentNode = nodeQueue.front();
		nodeQueue.pop();
		currentNodeId++;

		vector<SuffixModelNode*> childrenNodes = currentNode->getChildrenNode();
		for (int i = 0; i < (int) childrenNodes.size(); ++i)
		{
			SuffixModelNode* childNode = childrenNodes.at(i);
			nodeQueue.push(childNode);
			_count++;
			// write node
            vector<unsigned char> binCurrentNode = binarySuffixModelNode(childNode, currentNodeId);
			writeToBuffer(binCurrentNode);
		}
	}
	wcout << " Node count = " << _count << endl;
	_count = 0;
	// featureIdFrequency
	wcout << "featureIdFrequency.size = " << (int) featureIdFrequency.size() << endl;
    vector<unsigned char> binFeatureIdFrequencySize = getDictionary()->getDictionaryTools()->intToCharVector2((int) featureIdFrequency.size());
	writeToBuffer(binFeatureIdFrequencySize);
	map<int, int>::iterator iter;
	for (iter = featureIdFrequency.begin(); iter != featureIdFrequency.end(); ++iter)
	{
		int _featureId = iter->first;
        vector<unsigned char> binFeatureId = getDictionary()->getDictionaryTools()->intToCharVector3(_featureId);
		writeToBuffer(binFeatureId);
		int _frequency = iter->second;
        vector<unsigned char> binFrequency = getDictionary()->getDictionaryTools()->intToCharVector3(_frequency);
		writeToBuffer(binFrequency);
		_count++;
		if (max_feature_frequency < _frequency)
		{
			max_feature_frequency = _frequency;
			//wcout << "update max_frequency = " << max_frequency << endl;
		}
	}
	wcout << "Done! Count = " << _count << endl;
	wcout << "max_frequency_size = " << max_frequency_size << endl;
	wcout << "max_frequency = " << max_frequency << endl;
	wcout << "max_feature_frequency = " << max_feature_frequency << endl;
}

/**
 * Save N-Gram to binary file
 */
void SuffixModelTrieBinaryFileCreator::saveToBinaryFile(string _filePath)
{
	wcout << "SuffixModelTrieBinaryFileCreator saveToBinaryFile ..." << endl;
	buildBuffer();
	ofstream f(_filePath.c_str(), ios::out|ios::binary);
    f.write((char*)&buffer[0], bufferSize);
	f.close();
	wcout << "SuffixModelTrieBinaryFileCreator saveToBinaryFile done!" << endl;
}

SuffixModelTrieBinaryFileReader::SuffixModelTrieBinaryFileReader(Dictionary* _dic) : SuffixModelTrie(_dic)
{
    try
    {
        loadFromBinaryFile(dic->getPathToData() + BINARY_SUFFIX_PATH);
    }
    catch (int e)
    {
        if (e == FILE_NOT_FOUND_ERROR_CODE)
        {
            wcout << "Error : File not found : " << BINARY_SUFFIX_PATH << endl;
#ifdef MSVC
            _getch();
#endif
            exit(0);
        }
    }
}

SuffixModelTrieBinaryFileReader::~SuffixModelTrieBinaryFileReader(void)
{
    for (int i = 0; i < (int) SuffixModelNodeList.size(); ++i)
    {
        SuffixModelNode* node = SuffixModelNodeList.at(i);
        if (node)
        {
            delete node;
        }
    }
    SuffixModelNodeList.clear();
}

/**
 * Load dictionary from binary file
 */
void SuffixModelTrieBinaryFileReader::loadFromBinaryFile(string _filePath)
{
    ifstream f(_filePath.c_str(), ios::in|ios::binary|ios::ate);
    char* buffer;
    if (f.is_open())
    {
        // get size of file
        int size = (int) f.tellg();
        // jump to begin of file
        f.seekg(0, ios::beg);

        //============= Read SuffixModelNode(s) ============================================================================================
        // allocate buffer
        buffer = new char[size];
        // read file
        f.read(buffer, size);
        // close file
        f.close();
        // set offset begin of buffer
        int offset = 0;
        // convert 3 bytes to number of NodeVer3
        numberOfNodes = (unsigned char) buffer[offset] * 65536 + (unsigned char) buffer[offset + 1] * 256 + (unsigned char) buffer[offset + 2];
        offset += 3;
        // read list of SuffixModelNode
        SuffixModelNodeList = vector<SuffixModelNode*>();
        for (int _nodeId = 0; _nodeId < numberOfNodes; ++_nodeId)
        {
            // convert first byte to wchar_t
            wchar_t _character = getDictionary()->getDictionaryTools()->charToWchar((unsigned char) buffer[offset]);
            // convert 3 remaining bytes to _parentId
            int _parentId = (unsigned char) buffer[offset + 1] * 65536 + (unsigned char) buffer[offset + 2] * 256 + (unsigned char) buffer[offset + 3];
            offset += 4;

            // create new SuffixModelNode
            SuffixModelNode* _node = new SuffixModelNode(_character);
            // addChildNode
            if (_parentId >= 0 && _parentId < _nodeId)
            {
                SuffixModelNodeList.at(_parentId)->addChildNode(_node);
            }
            // size of frequency map
            int _sizeOfFrequencyMap = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
            offset += 2;
            map<int, int> _featureFrequencyMap = map<int, int>();
            for (int i = 0; i < _sizeOfFrequencyMap; ++i)
            {
                int _featureId = (unsigned char) buffer[offset] * 65536 + (unsigned char) buffer[offset + 1] * 256 + (unsigned char) buffer[offset + 2];
                offset += 3;
                int _frequency = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
                offset += 2;
                _featureFrequencyMap.insert(pair<int, int>(_featureId, _frequency));
            }
            _node->setFeatureFrequencyMap(_featureFrequencyMap);
            SuffixModelNodeList.push_back(_node);
        }
        root = SuffixModelNodeList.at(0);
        //============= Read featureIdFrequency ============================================================================================
        // convert 3 bytes to number of SuffixModelNode
        int featureIdFrequencySize = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
        offset += 2;
        for (int i = 0; i < featureIdFrequencySize; ++i)
        {
            int _featureId = (unsigned char) buffer[offset] * 65536 + (unsigned char) buffer[offset + 1] * 256 + (unsigned char) buffer[offset + 2];
            offset += 3;
            int _frequency = (unsigned char) buffer[offset] * 65536 + (unsigned char) buffer[offset + 1] * 256 + (unsigned char) buffer[offset + 2];
            offset += 3;
            featureIdFrequency.insert(pair<int, int>(_featureId, _frequency));
        }
        delete[] buffer;
        buffer = NULL;
    }
    else
    {
        throw FILE_NOT_FOUND_ERROR_CODE;
        //wcout << "### Error ### : loadFromBinaryFile -> Unable to open file";
    }
}
