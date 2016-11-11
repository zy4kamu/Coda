/**
 * NGramTrieBinaryFileCreator.cpp
 */

#include "NGramTrieBinaryFileCreator.h"
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

