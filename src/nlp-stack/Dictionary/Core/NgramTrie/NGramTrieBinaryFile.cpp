/**
 * NGramTrieBinaryFileCreator.cpp
 */

#include "NGramTrieBinaryFile.h"
#include "Dictionary.h"

#define N_GRAM_MAX_BUFFER_SIZE 15000000

NGramTrieBinaryFileCreator::NGramTrieBinaryFileCreator(Dictionary* _dic) : NGramTrieBuild(_dic)
{
}

NGramTrieBinaryFileCreator::~NGramTrieBinaryFileCreator(void)
{
	dic = NULL;
}

void NGramTrieBinaryFileCreator::writeToBuffer(vector<unsigned char> charVector)
{
	for (int i = 0; i < (int) charVector.size(); ++i)
	{
		buffer[bufferSize] = charVector.at(i);
		bufferSize++;
	}
}

void NGramTrieBinaryFileCreator::buildBuffer(void)
{
	wcout << "N-Gram build buffer ...";
    buffer = new unsigned char[N_GRAM_MAX_BUFFER_SIZE];
	bufferSize = 0;
	wcout << "numberOfNodes = " << numberOfNodes << endl;
    vector<unsigned char> binNumberOfNode = getDictionary()->getDictionaryTools()->intToCharVector4(numberOfNodes);
	writeToBuffer(binNumberOfNode);
	
	queue<NGramNode*> nodeQueue = queue<NGramNode*>();
	nodeQueue.push(root);
	int currentNodeId = -1;
	int _count = 1;
	
	// write node
    vector<unsigned char> binBasicFeatureListId = getDictionary()->getDictionaryTools()->intToCharVector2(root->getBasicFeatureListId());
	writeToBuffer(binBasicFeatureListId);
    vector<unsigned char> binFeatureListId = getDictionary()->getDictionaryTools()->intToCharVector2(root->getFeatureListId());
	writeToBuffer(binFeatureListId);
    vector<unsigned char> binNGramCount = getDictionary()->getDictionaryTools()->intToCharVector4(root->getNGramCount());
	writeToBuffer(binNGramCount);
    vector<unsigned char> binParentId = getDictionary()->getDictionaryTools()->intToCharVector4(currentNodeId);
	writeToBuffer(binParentId);

	while (!nodeQueue.empty())
	{
		NGramNode* currentNode = nodeQueue.front();
		nodeQueue.pop();
		currentNodeId++;

		vector<NGramNode*> childrenNodes = currentNode->getChildren();
		for (int i = 0; i < (int) childrenNodes.size(); ++i)
		{
			NGramNode* childNode = childrenNodes.at(i);
			nodeQueue.push(childNode);
			_count++;

			// write node
            vector<unsigned char> binBasicFeatureListId = getDictionary()->getDictionaryTools()->intToCharVector2(childNode->getBasicFeatureListId());
			writeToBuffer(binBasicFeatureListId);
            vector<unsigned char> binFeatureListId = getDictionary()->getDictionaryTools()->intToCharVector2(childNode->getFeatureListId());
			writeToBuffer(binFeatureListId);
            vector<unsigned char> binNGramCount = getDictionary()->getDictionaryTools()->intToCharVector4(childNode->getNGramCount());
			writeToBuffer(binNGramCount);
            vector<unsigned char> binParentId = getDictionary()->getDictionaryTools()->intToCharVector4(currentNodeId);
			writeToBuffer(binParentId);

		}
	}
	wcout << " done! Count = " << _count << endl;
}

/**
 * Save N-Gram to binary file
 */
void NGramTrieBinaryFileCreator::saveToBinaryFile(string _filePath)
{
	wcout << "N-Gram saveToBinaryFile ..." << endl;
	buildBuffer();
	ofstream f(_filePath.c_str(), ios::out|ios::binary);
    f.write((char*)&buffer[0], bufferSize);
	f.close();
	wcout << "N-Gram saveToBinaryFile done!" << endl;
}

NGramTrieBinaryFileReader::NGramTrieBinaryFileReader(Dictionary* _dic) : NGramTrie(_dic)
{
        try
        {
                loadFromBinaryFile(dic->getPathToData() + BIN_N_GRAM_FILE_PATH);
        }
        catch (int e)
        {
                if (e == FILE_NOT_FOUND_ERROR_CODE)
                {
                        wcout << "Error : File not found : " << BIN_N_GRAM_FILE_PATH << endl;
#ifdef MSVC
                        _getch();
#endif
                        exit(0);
                }
        }
}

NGramTrieBinaryFileReader::~NGramTrieBinaryFileReader(void)
{
        for (int i = 0; i < (int) NGramNodeList.size(); ++i)
        {
                NGramNode* nGamNode = NGramNodeList.at(i);
                if (nGamNode)
                {
                        delete nGamNode;
                }
        }
        NGramNodeList.clear();
        dic = NULL;
}

/**
 * Load N-Gram from binary file
 */
void NGramTrieBinaryFileReader::loadFromBinaryFile(string _filePath)
{
        ifstream f(_filePath.c_str(), ios::in|ios::binary|ios::ate);
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
                //============= Read NGramNode(s) ============================================================================================
                // convert 4 bytes to number of NGramNode
                numberOfNodes = (unsigned char) buffer[offset] * 16777216 + (unsigned char) buffer[offset + 1] * 65536
                        + (unsigned char) buffer[offset + 2] * 256 + (unsigned char) buffer[offset + 3];
                offset += 4;

                // read list of NGramNode
                NGramNodeList = vector<NGramNode*>();
                for (int _nodeId = 0; _nodeId < numberOfNodes; ++_nodeId)
                {
                        // convert 2 bytes to _basicFeatureListId
                        int _basicFeatureListId = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
                        offset += 2;

                        // convert 2 bytes to _featureListId
                        int _featureListId = (unsigned char) buffer[offset] * 256 + (unsigned char) buffer[offset + 1];
                        offset += 2;

                        // convert 4 bytes to _nGramCount
                        int _nGramCount = (unsigned char) buffer[offset] * 16777216 + (unsigned char) buffer[offset + 1] * 65536
                                + (unsigned char) buffer[offset + 2] * 256 + (unsigned char) buffer[offset + 3];
                        offset += 4;

                        // convert 4 remaining bytes to _parentId
                        int _parentId = (unsigned char) buffer[offset] * 16777216 + (unsigned char) buffer[offset + 1] * 65536
                                + (unsigned char) buffer[offset + 2] * 256 + (unsigned char) buffer[offset + 3];
                        offset += 4;

                        // create new NGramNode
                        NGramNode* _node = new NGramNode();
                        // setFeatureListId
                        _node->setFeatureListId(_basicFeatureListId, _featureListId);
                        // setFeatureListId
                        _node->setNGramCount(_nGramCount);
                        // set parentId
                        if (_parentId >= 0 && _parentId < _nodeId)
                        {
                                NGramNodeList.at(_parentId)->addChild(_node);
                        }
                        // add _node to NGramNodeList
                        NGramNodeList.push_back(_node);
                }
                root = NGramNodeList.at(0);
                delete[] buffer;
                buffer = NULL;
                //wcout << "... NGram : loadFromBinaryFile done!" << endl;
        }
        else
        {
                throw FILE_NOT_FOUND_ERROR_CODE;
                //wcout << "### Error ### : NGram : loadFromBinaryFile -> Unable to open file";
        }
}
