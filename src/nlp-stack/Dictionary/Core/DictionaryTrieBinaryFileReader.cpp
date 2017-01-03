/**
 * DictionaryTrieBinaryFileReader.cpp
 */

#include "DictionaryTrieBinaryFileReader.h"
#include "Dictionary.h"

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
