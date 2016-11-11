/**
 * SuffixModelTrieBinaryFileCreator.cpp
 */

#include "SuffixModelTrieBinaryFileCreator.h"
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

