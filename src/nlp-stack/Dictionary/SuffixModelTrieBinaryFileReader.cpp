/**
 * SuffixModelTrieBinaryFileReader.cpp
 */

#include "SuffixModelTrieBinaryFileReader.h"
#include "Dictionary.h"

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

