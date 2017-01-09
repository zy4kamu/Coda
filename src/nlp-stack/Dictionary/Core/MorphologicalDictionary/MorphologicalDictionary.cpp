/**
 * MorphologicalDictionary.cpp
 */

#include "MorphologicalDictionary.h"

int MorphologicalDictionary::morphology_count = 0;
int MorphologicalDictionary::prediction_count = 0;
int MorphologicalDictionary::rule_count = 0;
int MorphologicalDictionary::e_count = 0;

MorphologicalDictionary::MorphologicalDictionary(const string & binaryFileName,
                                                 bool load_data,
                                                 shared_ptr<DictionaryTools> _tools)
{
    if (_tools == NULL)
    {
        tools = make_shared<DictionaryTools>();
    }
    else
    {
        tools = _tools;
    }
    root = NULL;
    suffix_root = NULL;

    if (load_data)
    {
        string path = BIN_DIC_DATA_PATH;

        string binary_file_path = path + binaryFileName;
        readBinaryFile(binary_file_path);

        string rules_path = path + MORPHOLOGY_RULES_PATH;
        readRulesFromTextFile(rules_path);

        init();
    }

    useRules = false;
    usePrediction = false;
    min_suffix_length = 2;
    useE = true;
}

MorphologicalDictionary::~MorphologicalDictionary()
{

}

void MorphologicalDictionary::initSuffixMNodeItem()
{
    bool debug = false;
    if (debug)
    {
        wcout << ">>> initSuffixMNodeItem: " << endl;
    }
    int suffix_count = 0;
    int total_suffix_length = 0;
    int item_count = 0;

    std::queue<shared_ptr<MNode> > node_queue;
    node_queue.push(root);
    while (!node_queue.empty())
    {
        shared_ptr<MNode> current_node = node_queue.front();
        node_queue.pop();

        for (map<int, shared_ptr<MNodeModel> >::iterator
             iter = current_node->lemmaId_MNodeModel.begin();
             iter != current_node->lemmaId_MNodeModel.end(); ++iter)
        {
            shared_ptr<MNodeModel> mNodeModel = iter->second;
            int model_id = mNodeModel->model_id;
            shared_ptr<MModel> mmodel = mmodels.at(model_id);

            for (vector<shared_ptr<MModelElement> >::iterator
                 m_iter = mmodel->elements.begin();
                 m_iter != mmodel->elements.end(); ++m_iter)
            {
                shared_ptr<MModelElement> model_element = *m_iter;

                shared_ptr<MNodeItem> node_item = std::make_shared<MNodeItem>();
                node_item->lemma_id = iter->first;
                node_item->po = model_element->po;
                node_item->feature_list_id = model_element->feature_list_id;
                map<wstring, shared_ptr<vector<shared_ptr<MNodeItem> > > >::iterator
                    mn_iter = current_node->suffix_MNodeItem.find(model_element->suffix);
                shared_ptr<vector<shared_ptr<MNodeItem> > > items;
                if (mn_iter == current_node->suffix_MNodeItem.end())
                {
                    items = make_shared<vector<shared_ptr<MNodeItem> > >();
                    current_node->suffix_MNodeItem.insert(
                     pair<wstring, shared_ptr<vector<shared_ptr<MNodeItem> > > >
                     (model_element->suffix, items));

                    suffix_count++;
                    total_suffix_length += model_element->suffix.length();
                }
                else
                {
                    items = mn_iter->second;
                }
                items->push_back(node_item);
                item_count++;
            }
        }

        // push children nodes to queue
        for (map<wchar_t, shared_ptr<MNode> >::iterator iter = current_node->children.begin();
             iter != current_node->children.end(); ++iter)
        {
            node_queue.push(iter->second);
        }
    }
    if (debug)
    {
        wcout << "suffix_count = " << suffix_count << endl;
        wcout << "total_suffix_length = " << total_suffix_length << endl;
        wcout << "item_count = " << item_count << endl;
        wcout << "========= OK =========" << endl;
    }
}

void MorphologicalDictionary::initLemmaIdLemma()
{
    bool debug = false;
    if (debug)
    {
        wcout << ">>> initLemmaIdLemma: " << endl;
    }
    int total_lemma_length = 0;

    lemmas = vector<shared_ptr<wstring> >(maxLemmaId + 1, NULL);

    std::queue<shared_ptr<MNode> > node_queue;
    node_queue.push(root);
    while (!node_queue.empty())
    {
        shared_ptr<MNode> current_node = node_queue.front();
        node_queue.pop();

        for (map<int, shared_ptr<MNodeModel> >::iterator
             iter = current_node->lemmaId_MNodeModel.begin();
             iter != current_node->lemmaId_MNodeModel.end(); ++iter)
        {
            shared_ptr<MNodeModel> mNodeModel = iter->second;
            shared_ptr<MNode> node = mNodeModel->node;
            int model_id = mNodeModel->model_id;
            shared_ptr<MModel> mmodel = mmodels.at(model_id);
            shared_ptr<MModelElement> first_model_element = *(mmodel->elements.begin());
            shared_ptr<wstring> lemma = std::make_shared<wstring>();

            vector<wchar_t> reversed_prefix;
            while (node != root)
            {
                reversed_prefix.push_back(node->character);
                node = node->parent;
            }
            if (first_model_element->po)
            {
                lemma->assign(L"по");
            }
            for (vector<wchar_t>::reverse_iterator rit = reversed_prefix.rbegin();
                 rit != reversed_prefix.rend(); ++rit)
            {
                lemma->push_back(*rit);
            }
            lemma->append(first_model_element->suffix);
            lemmas[iter->first] = lemma;
            total_lemma_length += lemma->length();
        }
        // push children nodes to queue
        for (map<wchar_t, shared_ptr<MNode> >::iterator iter = current_node->children.begin();
             iter != current_node->children.end(); ++iter)
        {
            node_queue.push(iter->second);
        }
    }
    if (debug)
    {
        wcout << "total_lemma_length = " << total_lemma_length << endl;
        wcout << "========= OK =========" << endl;
    }
}

void MorphologicalDictionary::init()
{
    initSuffixMNodeItem();
    initLemmaIdLemma();
}

void MorphologicalDictionary::getMorphologyPo(const wstring & lower_word, bool po, shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    morphology_count++;

    bool debug = false;
    size_t l = lower_word.length();
    shared_ptr<MNode> current_node = root;
    wstring current_prefix = po ? L"по" : L"";
    size_t begin_index = po ? 2 : 0;
    for (size_t i = begin_index; i <= l; ++i)
    {
        if (debug)
        {
            wcout << "Current_prefix = " << current_prefix << endl;
            wcout << "Number of models = " << current_node->lemmaId_MNodeModel.size() << endl;
        }

        wstring suffix = lower_word.substr(i, l - i);

        map<wstring, shared_ptr<vector<shared_ptr<MNodeItem> > > >::iterator s_iter
                = current_node->suffix_MNodeItem.find(suffix);
        if (s_iter != current_node->suffix_MNodeItem.end())
        {
            if (debug)
            {
                wcout << "Found suffix: " << suffix << endl;
            }
            shared_ptr<vector<shared_ptr<MNodeItem> > > items = s_iter->second;
            if (debug)
            {
                wcout << "items->size() = " << items->size() << endl;
            }
            for (vector<shared_ptr<MNodeItem> >::iterator mn_iter = items->begin();
                 mn_iter != items->end(); ++mn_iter)
            {
                shared_ptr<MNodeItem> item = *mn_iter;
                if (item->feature_list_id <= 0 || item->po != po)
                {
                    continue;
                }
                shared_ptr<Morphology> morphology = std::make_shared<Morphology>();
                morphology->lemma_id = item->lemma_id;
                morphology->lemma = lemmas.at(item->lemma_id);
                morphology->word = make_shared<wstring>(lower_word);
                morphology->suffix_length = l - i;

                //morphology->features
                if (debug)
                {
                    wcout << "item->feature_list_id = " << item->feature_list_id << endl;
                }
                for (vector<int>::iterator
                     f_iter = id_feature_list.at(item->feature_list_id).begin();
                     f_iter != id_feature_list.at(item->feature_list_id).end();
                     ++f_iter)
                {
                    if (debug)
                    {
                        wcout << "*f_iter = " << *f_iter << endl;
                    }
                    morphology->features.push_back(id_short_feature.at(*f_iter));
                    morphology->descriptions.push_back(id_long_feature.at(*f_iter));
                    if (debug)
                    {
                        wcout << "f_iter ok " << endl;
                    }
                }
                result->push_back(morphology);
            }
        }
        // go to child node
        if (i == l)
        {
            break;
        }
        map<wchar_t, shared_ptr<MNode> >::iterator child_iterator = current_node->children.find(lower_word.at(i));
        if (child_iterator == current_node->children.end())
        {
            break;
        }
        current_node = child_iterator->second;
        current_prefix.push_back(lower_word.at(i));
    }
    if (debug)
    {
        wcout << "getMorphologyPo >> OK" << endl;
    }
}

void addFidToSet(int lemmaId, int fid, shared_ptr<map<int, shared_ptr<set<int> > > > s)
{
    map<int, shared_ptr<set<int> > >::iterator iter = s->find(lemmaId);
    if (iter == s->end())
    {
        shared_ptr<set<int> > ss = make_shared<set<int> >();
        ss->insert(fid);
        s->insert(pair<int, shared_ptr<set<int> > >(lemmaId, ss));
    }
    else
    {
        shared_ptr<set<int> > ss = iter->second;
        ss->insert(fid);
    }
}

void MorphologicalDictionary::getFeatureListOfLemmaPo(shared_ptr<wstring> lower_lemma,
              bool po, shared_ptr<map<int, shared_ptr<set<int> > > > lemmaId_wordFLIDs)
{
    bool debug = false;
    if (debug)
    {
        wcout << "### getFeatureListOfLemmaPo, po = " << po << endl;
        wcout << ">>> lower_lemma = " << *lower_lemma << endl;
    }
    size_t l = lower_lemma->length();
    shared_ptr<MNode> current_node = root;
    wstring current_prefix = po ? L"по" : L"";
    size_t begin_index = po ? 2 : 0;
    for (size_t i = begin_index; i <= l; ++i)
    {
        wstring suffix = lower_lemma->substr(i, l - i + 1);

        map<wstring, shared_ptr<vector<shared_ptr<MNodeItem> > > >::iterator s_iter
                = current_node->suffix_MNodeItem.find(suffix);
        if (s_iter != current_node->suffix_MNodeItem.end())
        {
            shared_ptr<vector<shared_ptr<MNodeItem> > > items = s_iter->second;
            for (vector<shared_ptr<MNodeItem> >::iterator mn_iter = items->begin();
                 mn_iter != items->end(); ++mn_iter)
            {
                shared_ptr<MNodeItem> item = *mn_iter;
                if (item->po == po)
                {
                    shared_ptr<MNodeModel> mNodeModel = lemmaId_MNodeModel.at(item->lemma_id);
                    int model_id = mNodeModel->model_id;
                    shared_ptr<MModel> mmodel = mmodels.at(model_id);
                    for (vector<shared_ptr<MModelElement> >::iterator m_iter = mmodel->elements.begin();
                         m_iter != mmodel->elements.end(); ++m_iter)
                    {
                        shared_ptr<MModelElement> model_element = *m_iter;
                        addFidToSet(item->lemma_id, model_element->feature_list_id, lemmaId_wordFLIDs);
                    }
                }
            }
        }
        // go to child node
        if (i == l)
        {
            break;
        }
        map<wchar_t, shared_ptr<MNode> >::iterator child_iterator = current_node->children.find(lower_lemma->at(i));
        if (child_iterator == current_node->children.end())
        {
            break;
        }
        current_node = child_iterator->second;
        current_prefix.push_back(lower_lemma->at(i));
    }
}

void MorphologicalDictionary::getFeatureListOfLemma(shared_ptr<wstring> lower_lemma,
                    shared_ptr<map<int, shared_ptr<set<int> > > > lemmaId_wordFLIDs)
{
    this->getFeatureListOfLemmaPo(lower_lemma, false, lemmaId_wordFLIDs);
    if (lower_lemma->length() >= 2
            && lower_lemma->at(0) == L'п' && lower_lemma->at(1) == L'о')
    {
        this->getFeatureListOfLemmaPo(lower_lemma, true, lemmaId_wordFLIDs);
    }
}

void MorphologicalDictionary::readBinaryFile(const string & filePath)
{
    bool debug = false;
    maxLemmaId = 0;
    if (debug)
    {
        wcout << "loadFromBinaryFile" << endl;
    }
    ifstream f(filePath.c_str(), ios::in|ios::binary|ios::ate);
    if (f.is_open())
    {
        // get size of file
        int size = (int) f.tellg();
        // jump to begin of file
        f.seekg(0, ios::beg);

        //============= 1. Nodes ===============
        // allocate buffer
        buffer = new unsigned char[size];
        // read file
        f.read((char*) buffer, size);
        // close file
        f.close();
        // set offset begin of buffer
        int offset = 0;
        // convert 3 bytes to number of NodeVer3
        int numberOfNodes = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
        offset += 3;
        if (debug)
        {
            wcout << "numberOfNodes = " << numberOfNodes << endl;
        }

        // read list of Nodes
        vector<shared_ptr<MNode> > _nodes;
        for (int _nodeId = 0; _nodeId < numberOfNodes; ++_nodeId)
        {
            // convert first byte to wchar_t
            unsigned char _char_code = buffer[offset];
            wchar_t _character = tools->charToWchar(_char_code);
            // convert 3 remaining bytes to _parentId
            int _parentId = buffer[offset + 1] * 65536 + buffer[offset + 2] * 256 + buffer[offset + 3];
            offset += 4;
            // create new NodeVer3
            shared_ptr<MNode> _node = std::make_shared<MNode>();
            _node->character = _character;
            if (_parentId >= 0 && _parentId < (int) _nodes.size())
            {
                _node->parent = _nodes.at(_parentId);
                _nodes.at(_parentId)->children.insert(pair<wchar_t, shared_ptr<MNode> >(_character, _node));
            }
            else
            {
                _node->parent = NULL;
            }
            _nodes.push_back(_node);
        }
        root = _nodes.at(0);
        if (debug)
        {
            wcout << "OK 1 : numberOfNodes = " << numberOfNodes << endl;
//            wcout << root->children.size() << endl;
        }
        //================ 2. NodeModels ===================

        // read number of NodeModel
        // convert 3 bytes to number of NodeModel
        int numberOfNodeModel = buffer[offset] * 65536 + buffer[offset + 1] * 256 + buffer[offset + 2];
        offset += 3;
        if (debug)
        {
            wcout << "numberOfNodeModel = " << numberOfNodeModel << endl;
        }
        lemmaId_MNodeModel = vector<shared_ptr<MNodeModel> >(400000, NULL); // @TODO
        // read list of NodeModel
        for (int i = 0; i < numberOfNodeModel; ++i)
        {
            // convert first 2 bytes to modelId
            int _modelId = buffer[offset] * 256 + buffer[offset + 1];
            // convert the next 3 bytes to _lemmaId
            int _lemmaId = buffer[offset + 2] * 65536 + buffer[offset + 3] * 256 + buffer[offset + 4];
            // convert 3 remaining bytes to _nodeId
            int _nodeId = buffer[offset + 5] * 65536 + buffer[offset + 6] * 256 + buffer[offset + 7];
            offset += 8;

            if (maxLemmaId < _lemmaId)
            {
                maxLemmaId = _lemmaId;
            }

            shared_ptr<MNodeModel> node_model = std::make_shared<MNodeModel>();
            node_model->model_id = _modelId;
            lemmaId_MNodeModel.at(_lemmaId) = node_model;
            node_model->node = _nodes.at(_nodeId);
            _nodes.at(_nodeId)->lemmaId_MNodeModel.insert(pair<int, shared_ptr<MNodeModel> >(_lemmaId, node_model));
        }
        if (debug)
        {
            wcout << "OK 2 : numberOfNodeModel = " << numberOfNodeModel << endl;
        }

        //================ 3. TrieModels ====================

        // read number of TrieModel
        // convert 2 bytes to number of TrieModel
        int numberOfModels = buffer[offset] * 256 + buffer[offset + 1];
        offset += 2;
        if (debug)
        {
            wcout << "Number of TrieModels = " << numberOfModels << endl;
        }
        mmodels.clear();
        mmodels.push_back(NULL); // mmodels[0] = NULL
        // read TrieModels
        for (int i = 1; i <= numberOfModels; ++i)
        {
            // create a TrieModel from string sufixesAttrs
            shared_ptr<MModel> _mModel = std::make_shared<MModel>();
            // convert 1 bytes to numberOfTrieModelElement
            int numberOfTrieModelElement = buffer[offset];
            offset += 1;
            for (int j = 0; j < numberOfTrieModelElement; ++j)
            {
                shared_ptr<MModelElement> _modelElement = std::make_shared<MModelElement>();
                // convert 1 byte to suffix's length
                int suffixLength = buffer[offset];
                offset += 1;
                // read suffix
                for (int k = 0; k < suffixLength; ++k)
                {
                    _modelElement->suffix.push_back(tools->charToWchar(buffer[offset + k]));
                }
                offset += suffixLength;
                // convert 1 byte to beginWithPo
                _modelElement->po = (buffer[offset] == 1);
                offset += 1;
                // convert 2 byte to featureListId
                int _featureListId = buffer[offset] * 256 + buffer[offset + 1];
                offset += 2;
                // set featureListId for DictionaryTrieModelElement
                _modelElement->feature_list_id = _featureListId;
                // add DictionaryTrieModelElement to DictionaryTrieModel
                _mModel->elements.push_back(_modelElement);
            }
            // map modelIndex to trieModel
            mmodels.push_back(_mModel);
        }

        if (debug)
        {
            wcout << "OK 4 : Trie models ok" << endl;
        }

        //================ 4. featureLists  =========================================================================================

        // read number of FeatureList
        // convert 2 bytes to number of FeatureList
        int numberOfFeatureList = buffer[offset] * 256 + buffer[offset + 1];
        offset += 2;
        if (debug)
        {
            wcout << "numberOfFeatureList = " << numberOfFeatureList << endl;
        }

        id_feature_list.clear();
        id_feature_list.push_back(vector<int>()); // id_feature_list[0] = NULL

        for (int i = 1; i <= numberOfFeatureList; ++i)
        {
            // read number of features in list
            int numberOfFeature = buffer[offset];
            offset += 1;
            vector<int> featureIdList = vector<int>();
            // read features
            for (int j = 0; j < numberOfFeature; ++j)
            {
                // convert 1 byte to featureId
                int featureId = buffer[offset];
                offset += 1;
                // add featureId to featureIdList
                featureIdList.push_back(featureId);
            }
            // insert featureIdList to featureListMap
            id_feature_list.push_back(featureIdList);
        }

        if (debug)
        {
            wcout << "OK 5 : Feature lists ok" << endl;
        }

        //================ 5. features  =====================

        // read number of features
        // convert 1 bytes to number of FeatureList

        id_short_feature.clear();
        id_short_feature.push_back(NULL);
        id_long_feature.clear();
        id_long_feature.push_back(NULL);

        int _numberOfFeatures = buffer[offset];
        if (debug)
        {
            wcout << "_numberOfFeature = " << _numberOfFeatures << endl;
        }
        offset += 1;
        for (int i = 1; i <= _numberOfFeatures; ++i)
        {
            // short feature
            // convert 1 byte to feature's length
            int _short_feature_length = buffer[offset];
            offset += 1;
            // read feature
            shared_ptr<wstring> _short_feature = std::make_shared<wstring>();
            _short_feature->clear();
            for (int j = 0; j < _short_feature_length; ++j)
            {
                _short_feature->push_back(tools->charToWchar(buffer[offset + j]));
            }
            offset += _short_feature_length;
            id_short_feature.push_back(_short_feature);

            // long feature
            // convert 1 byte to feature's length
            int _long_feature_length = buffer[offset];
            offset += 1;
            // read feature
            shared_ptr<wstring> _long_feature = std::make_shared<wstring>();
            _long_feature->clear();
            for (int j = 0; j < _long_feature_length; ++j)
            {
                _long_feature->push_back(tools->charToWchar(buffer[offset + j]));
            }
            offset += _long_feature_length;
            // insert _feature to featureMap
            id_long_feature.push_back(_long_feature);
            short_to_long_[*_short_feature] = *_long_feature;
        }
        if (debug)
        {
            wcout << "OK 6 : Features ok" << endl;
        }

        // SUFFIX

        // number of suffix nodes
        int numberOfSuffixNodes = buffer[offset] * 65536
                + buffer[offset + 1] * 256 + buffer[offset + 2];
        offset += 3;
        if (debug)
        {
            wcout << "numberOfSuffixNodes = " << numberOfSuffixNodes << endl;
        }

        // read list of SuffixNodes
        vector<shared_ptr<SuffixNode> > _suffix_nodes;
        for (int _suffix_node_id = 0;
             _suffix_node_id < numberOfSuffixNodes; ++_suffix_node_id)
        {
            // convert first byte to wchar_t
            unsigned char _char_code = buffer[offset];
            wchar_t _character = tools->charToWchar(_char_code);
            // convert 3 remaining bytes to _parentId
            int _suffix_parent_node_id = buffer[offset + 1] * 65536
                    + buffer[offset + 2] * 256 + buffer[offset + 3];
            offset += 4;
            // create new NodeVer3
            shared_ptr<SuffixNode> _suffix_node = std::make_shared<SuffixNode>();
            _suffix_node->character = _character;
            _suffix_node->children.clear();
            if (_suffix_parent_node_id >= 0 &&
                    _suffix_parent_node_id < (int) _suffix_nodes.size())
            {
                _suffix_nodes.at(_suffix_parent_node_id)->children.insert(
                 pair<wchar_t, shared_ptr<SuffixNode> >(_character, _suffix_node));
            }
            _suffix_node->suffix_trie_model_ids.clear();
            int number_of_models = buffer[offset] * 256 + buffer[offset + 1];
            offset += 2;
            for (int j = 0; j < number_of_models; ++j)
            {
                int _suffix_model_id = buffer[offset] * 65536
                        + buffer[offset + 1] * 256 + buffer[offset + 2];
                offset += 3;
                _suffix_node->suffix_trie_model_ids.push_back(_suffix_model_id);
            }
            _suffix_nodes.push_back(_suffix_node);
        }
        if (numberOfSuffixNodes > 0)
        {
            suffix_root = _suffix_nodes.at(0);
        }
        if (debug)
        {
            wcout << "OK 7 : numberOfSuffixNodes = " << numberOfSuffixNodes << endl;
        }

        // suffix models
        suffix_models.clear();
        suffix_models.push_back(NULL);

        int number_of_suffix_models = buffer[offset] * 65536
                + buffer[offset + 1] * 256 + buffer[offset + 2];
        offset += 3;

        if (debug)
        {
            wcout << "number_of_suffix_models = " << number_of_suffix_models << endl;
        }

        for (int _suffix_model_id = 0; _suffix_model_id <= number_of_suffix_models;
             ++ _suffix_model_id)
        {
            shared_ptr<SuffixModel> _suffix_model = make_shared<SuffixModel>();
            // convert 2 byte to featureListId
            int _featureListId = buffer[offset] * 256 + buffer[offset + 1];
            offset += 2;
            // set featureListId for DictionaryTrieModelElement
            _suffix_model->feature_list_id = _featureListId;
            _suffix_model->lemma_suffix.clear();
            // convert 1 byte to suffix's length
            int lemmaSuffixLength = buffer[offset];
            offset += 1;
            // read suffix
            for (int k = 0; k < lemmaSuffixLength; ++k)
            {
                _suffix_model->lemma_suffix.push_back(tools->charToWchar(buffer[offset + k]));
            }
            offset += lemmaSuffixLength;
            suffix_models.push_back(_suffix_model);
        }
        if (debug)
        {
            wcout << "OK 8 : SuffixModels ok = " << number_of_suffix_models << endl;
        }
        delete[] buffer;
        buffer = NULL;
    }
    else
    {
        throw FILE_NOT_FOUND_ERROR_CODE;
        wcout << "### Error ### : loadFromBinaryFile -> Unable to open file" << endl;
    }
}

void MorphologicalDictionary::readRulesFromTextFile(const string& filePath)
{
    bool debug = false;
    if (debug)
    {
        wcout << "readRulesFromTextFile ... " << endl;
    }
    wifstream fi(filePath.c_str());
    if (!fi.is_open())
    {
        wcout << "Rules file not found!!!" << endl;
        return;
    }
#ifdef MSVC
    fi.imbue(locale(fi.getloc(), new codecvt_utf8<wchar_t>));
#else
    fi.imbue(std::locale("en_US.UTF-8"));
#endif
    wstring line;
    wstring _word_suffix;
    wstring _lemma_suffix;
    int _word_feature_list_id;
    int _lemma_feature_list_id;
    int count = -1;
    ruleSet.morphologyRules.clear();
    getline(fi, line);
    line = line.substr(0, line.length() - 1);
    ruleSet.min_lemma_length = (size_t) atoi(tools->wstring2string(line).c_str());
    while (getline(fi, line))
    {
        line = line.substr(0, line.length() - 1);
        count = (count + 1) % 4;
        if (count == 0)
        {
            _word_suffix = line;
        }
        else if (count == 1)
        {
            _lemma_suffix = line;
        }
        else if (count == 2)
        {
            _lemma_feature_list_id = atoi(tools->wstring2string(line).c_str());
        }
        else
        {
            _word_feature_list_id = atoi(tools->wstring2string(line).c_str());
            MRule rule;
            rule.word_suffix = _word_suffix;
            rule.lemma_suffix = _lemma_suffix;
            rule.word_feature_list_id = _word_feature_list_id;
            rule.lemma_feature_list_id = _lemma_feature_list_id;
            ruleSet.morphologyRules.push_back(rule);
        }
    }
    fi.close();
    if (debug)
    {
        wcout << "readRulesFromTextFile ok, number of rules = "
              << ruleSet.morphologyRules.size() << endl;
    }
}

void MorphologicalDictionary::getMorphologicalInfoListByRules(const wstring & lower_word, shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    rule_count++;

    bool debug = false;

    set<pair<wstring, int> > resultSet; // avoid duplications
    resultSet.clear();

    size_t lw = lower_word.length();
    for (vector<MRule>::iterator iter = ruleSet.morphologyRules.begin();
         iter != ruleSet.morphologyRules.end(); ++iter)
    {
        MRule rule = *iter;
        size_t lws = rule.word_suffix.length();
        size_t lls = rule.lemma_suffix.length();

        // check length
        if (lw < lws || lw - lws + lls < ruleSet.min_lemma_length)
        {
            continue;
        }

        // check suffix
        int i1 = lw - 1;
        int i2 = lws - 1;
        bool ok = true;
        while (i2 >= 0)
        {
            if (lower_word.at(i1) != rule.word_suffix.at(i2))
            {
                ok = false;
                break;
            }
            i1--;
            i2--;
        }
        if (!ok)
        {
            continue;
        }

        // lemma
        shared_ptr<wstring> lemma = std::make_shared<wstring>();
        lemma->clear();
        for (size_t i = 0; i < lw - lws; ++i)
        {
            lemma->push_back(lower_word.at(i));
        }
        for (size_t i = 0; i < lls; ++i)
        {
            lemma->push_back(rule.lemma_suffix.at(i));
        }

        if (debug)
        {
            wcout << "Found, lemma = " << endl;
        }

        // check <lemma, word_feature_list_id>
        pair<wstring, int> pp(*lemma, rule.word_feature_list_id);
        if (resultSet.find(pp) != resultSet.end())
        {
            if (debug)
            {
                wcout << "### Duplication: " << *lemma << " - " << rule.word_feature_list_id << endl;
            }
            continue;
        }
        resultSet.insert(pp);

        // get feature_list_id of all word forms of lemma
        shared_ptr<map<int, shared_ptr<set<int> > > > lemmaId_wordFLIDs = make_shared<map<int, shared_ptr<set<int> > > >();
        this->getFeatureListOfLemma(lemma, lemmaId_wordFLIDs);

        // check feature_list_id
        for (map<int, shared_ptr<set<int> > >::iterator l_iter = lemmaId_wordFLIDs->begin();
             l_iter != lemmaId_wordFLIDs->end(); ++l_iter)
        {
            shared_ptr<set<int> > ss = l_iter->second;
            if (ss->find(rule.lemma_feature_list_id) != ss->end() &&
                ss->find(rule.word_feature_list_id) == ss->end())
            {
                if (debug)
                {
                    wcout << ">>> FOUND" << endl;
                    wcout << "rule.word_suffix = " << rule.word_suffix << endl;
                    wcout << "rule.word_feature_list_id = " << rule.word_feature_list_id << endl;
                    wcout << "rule.lemma_suffix = " << rule.lemma_suffix << endl;
                    wcout << "rule.lemma_feature_list_id = " << rule.lemma_feature_list_id << endl;
                    wcout << endl;
                }
                shared_ptr<Morphology> morphology = std::make_shared<Morphology>();
                morphology->lemma_id = l_iter->first;
                morphology->lemma = lemma;
                morphology->word = make_shared<wstring>(lower_word);
                morphology->suffix_length = lws;

                // features
                for (vector<int>::iterator
                     f_iter = id_feature_list.at(rule.word_feature_list_id).begin();
                     f_iter != id_feature_list.at(rule.word_feature_list_id).end();
                     ++f_iter)
                {
                    morphology->features.push_back(id_short_feature.at(*f_iter));
                    morphology->descriptions.push_back(id_long_feature.at(*f_iter));
                }
                result->push_back(morphology);
            }
        }
    }
}

void MorphologicalDictionary::getMorphologicalPrediction(const wstring & lower_word,
                                shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    prediction_count++;

    bool debug = false;
    int l = lower_word.length();
    if (debug)
    {
        wcout << "Prediction: Word = " << lower_word << " length = " << l << endl;
    }
    shared_ptr<SuffixNode> current_node = suffix_root;
    for (int i = l - 1; i >= 0; --i)
    {
        wchar_t character = lower_word.at(i);
        map<wchar_t, shared_ptr<SuffixNode> >::iterator scn_iter
                = current_node->children.find(character);
        if (scn_iter != current_node->children.end())
        {
            current_node = scn_iter->second;
        }
        else
        {
            for (vector<int>::iterator
                 m_iter = current_node->suffix_trie_model_ids.begin();
                 m_iter != current_node->suffix_trie_model_ids.end(); ++m_iter)
            {
                int suffix_model_id = *m_iter;
                shared_ptr<SuffixModel> suffix_model = suffix_models.at(suffix_model_id);
                if (suffix_model->feature_list_id <= 0)
                {
                    continue;
                }

                shared_ptr<Morphology> morphology = std::make_shared<Morphology>();
                morphology->lemma_id = 0;
                morphology->suffix_length = l - 1 - i;
                // lemma
                shared_ptr<wstring> lemma = make_shared<wstring>(lower_word, 0, i + 1);
                lemma->append(suffix_model->lemma_suffix);
                morphology->lemma = lemma;
                morphology->word = make_shared<wstring>(lower_word);

                // feature from current model elements
                for (vector<int>::iterator
                     f_iter = id_feature_list.at(suffix_model->feature_list_id).begin();
                     f_iter != id_feature_list.at(suffix_model->feature_list_id).end();
                     ++f_iter)
                {
                    morphology->features.push_back(id_short_feature.at(*f_iter));
                    morphology->descriptions.push_back(id_long_feature.at(*f_iter));
                }
                result->push_back(morphology);
            }
            break;
        }
    }
}

void MorphologicalDictionary::getMorphologyE(const wstring & lower_word,
                   shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    bool debug = false;
    size_t l = lower_word.length();
    for (size_t i = 0; i < l; ++i)
    {
        if (lower_word.at(i) == L'е')
        {
            e_count++;
            wstring e_word(lower_word);
            e_word[i] = L'ё';

            // get morphology
            if (debug)
            {
                wcout << "E:getMorphologyPo: " << e_word << endl;
            }
            this->getMorphologyPo(e_word, false, result);
            if (debug)
            {
                wcout << "E:getMorphologyPo: ok" << e_word << endl;
            }

            // "по"
            if (l >= 2 && e_word.at(0) == L'п' && e_word.at(1) == L'о')
            {
                if (debug)
                {
                    wcout << "E:getMorphologyPo:Po: " << e_word << endl;
                }
                this->getMorphologyPo(e_word, true, result);
            }
        }
    }
    if (debug)
    {
        wcout << "getMorphologyE >> ok" << endl;
    }
}

void MorphologicalDictionary::getMorphology(const wstring & word,
                              shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    bool debug = false;
    // convert word to lower-cased
    size_t l = word.length();
    wstring lower_word;
//    lower_word.clear();
//    wstring lower_word = word;
    int e_count = 0;
    int ee_count = 0;
    for (size_t i = 0; i < l; ++ i)
    {
        wchar_t lower_ch = tools->charToLowerCase(word.at(i));
        if (lower_ch == L'е')
        {
            e_count++;
        }
        else if (lower_ch == L'ё')
        {
            ee_count++;
        }
        lower_word.push_back(lower_ch);
//        lower_word[i] = towlower(lower_word.c_str()[i]);
    }
    if (debug)
    {
        wcout << "lower_word = " << lower_word << endl;
    }

    // get morphology
    this->getMorphologyPo(lower_word, false, result);

    // "по"
    if (l >= 2 && lower_word.at(0) == L'п' && lower_word.at(1) == L'о')
    {
        this->getMorphologyPo(lower_word, true, result);
    }

    if (useE && e_count > 0 && ee_count == 0)
    {
        if (debug)
        {
            wcout << "E: " << endl;
        }
        this->getMorphologyE(lower_word, result);
    }

    // morphology by rules (English)
    if (useRules)
    {
        if (debug)
        {
            wcout << "Rules (English): " << endl;
        }
        this->getMorphologicalInfoListByRules(lower_word, result);
    }

    // morphology prediction (Russian)
    if (usePrediction && result->empty())
    {
        if (debug)
        {
            wcout << "Prediction (Russian): " << endl;
        }
        this->getMorphologicalPrediction(lower_word, result);
    }
}

void MorphologicalDictionary::getMorphologyOfLemmaId(int lemma_id,
                   shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    bool debug = false;
    if (debug)
    {
        wcout << "getMorphologyOfLemmaId : " << lemma_id << endl;
    }
    if (lemma_id <= 0 || lemma_id >= (int) lemmaId_MNodeModel.size())
    {
        return;
    }
    shared_ptr<MNodeModel> node_model = lemmaId_MNodeModel.at(lemma_id);
    if (!node_model)
    {
        return;
    }
    int model_id = node_model->model_id;
    shared_ptr<MModel> mmodel = mmodels.at(model_id);

    shared_ptr<MNode> node = node_model->node;

    vector<wchar_t> reversed_prefix;
    while (node != root)
    {
        reversed_prefix.push_back(node->character);
        node = node->parent;
    }

    wstring prefix;
    for (vector<wchar_t>::reverse_iterator rit = reversed_prefix.rbegin();
         rit != reversed_prefix.rend(); ++rit)
    {
        prefix.push_back(*rit);
    }

    for (vector<shared_ptr<MModelElement> >::iterator m_iter = mmodel->elements.begin();
         m_iter != mmodel->elements.end(); ++m_iter)
    {
        shared_ptr<MModelElement> element = *m_iter;
        if (element->feature_list_id <= 0)
        {
            continue;
        }
        shared_ptr<Morphology> morphology = std::make_shared<Morphology>();
        morphology->lemma_id = lemma_id;
        morphology->lemma = lemmas.at(lemma_id);

        // word
        shared_ptr<wstring> word = make_shared<wstring>();
        if (element->po)
        {
            word->assign(L"по");
        }
        word->append(prefix);
        word->append(element->suffix);
        morphology->word = word;
        if (debug)
        {
            wcout << "prefix = " << prefix
                  << " suffix = " << element->suffix
                  << " po = " << element->po << endl;
        }

        //morphology->features
        for (vector<int>::iterator
             f_iter = id_feature_list.at(element->feature_list_id).begin();
             f_iter != id_feature_list.at(element->feature_list_id).end();
             ++f_iter)
        {
            morphology->features.push_back(id_short_feature.at(*f_iter));
            morphology->descriptions.push_back(id_long_feature.at(*f_iter));
        }

        result->push_back(morphology);
    }
}

void MorphologicalDictionary::writeToBuffer(vector<unsigned char> charVector)
{
    for (int i = 0; i < (int) charVector.size(); ++i)
    {
        buffer[bufferSize] = charVector.at(i);
        bufferSize++;
    }
}

void MorphologicalDictionary::buildBufferFromTextFile(string _filePath)
{
    bool debug = true;
    if (debug)
    {
        wcout << "Build buffer from text file..." << endl;
    }
    buffer = new unsigned char[100000000];
    bufferSize = 0;
    wifstream fi(_filePath.c_str());
    // set encoding to UTF-8
#ifdef MSVC
    fi.imbue(locale(fi.getloc(), new codecvt_utf8<wchar_t>));
#else
    //fin.imbue(std::locale("ru_RU.UTF-8"));
    fi.imbue(std::locale("en_US.UTF-8"));
#endif
    wstring line;
    // Node
    getline(fi, line);
    int _number_of_nodes = atoi(tools->wstring2string(line).c_str());

    vector<unsigned char> _binary_number_of_nodes
            = tools->intToCharVector3(_number_of_nodes);
    writeToBuffer(_binary_number_of_nodes);
    for (int i = 0; i < _number_of_nodes; ++i)
    {
        getline(fi, line);
        vector<unsigned char> _binary_node_char;
        _binary_node_char.push_back(tools->wcharToChar(line.at(0)));
        writeToBuffer(_binary_node_char);
        getline(fi, line);
        int _node_parent_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_node_parent_id
                = tools->intToCharVector3(_node_parent_id);
        writeToBuffer(_binary_node_parent_id);
    }
    if (debug)
    {
        wcout << "#build : Node ok: " << _number_of_nodes << endl;
    }

    // NodeModel
    getline(fi, line);
    int _number_of_node_models = atoi(tools->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_node_models
            = tools->intToCharVector3(_number_of_node_models);
    writeToBuffer(_binary_number_of_node_models);
    for (int i = 0; i < _number_of_node_models; ++i)
    {
        getline(fi, line);
        int _model_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_model_id = tools->intToCharVector2(_model_id);
        writeToBuffer(_binary_model_id);

        getline(fi, line);
        int _lemma_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_lemma_id = tools->intToCharVector3(_lemma_id);
        writeToBuffer(_binary_lemma_id);

        getline(fi, line);
        int _node_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_node_id = tools->intToCharVector3(_node_id);
        writeToBuffer(_binary_node_id);
    }
    if (debug)
    {
        wcout << "#build : NodeModel ok: " << _number_of_node_models << endl;
    }

    // TrieModel
    getline(fi, line);
    int _number_of_trie_models = atoi(tools->wstring2string(line).c_str());
    wcout << "#build: _number_of_trie_models = " << _number_of_trie_models << endl;
    vector<unsigned char> _binary_number_of_trie_models = tools->intToCharVector2(_number_of_trie_models);
    writeToBuffer(_binary_number_of_trie_models);
    for (int i = 1; i <= _number_of_trie_models; ++i)
    {
        getline(fi, line);
        int _number_of_trie_model_elements = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_number_of_trie_model_elements
                = tools->intToCharVector1(_number_of_trie_model_elements);
        writeToBuffer(_binary_number_of_trie_model_elements);
        for (int j = 0; j < _number_of_trie_model_elements; ++j)
        {
            // suffix
            getline(fi, line);
            int _suffix_length = (int) line.length();
            vector<unsigned char> _binary_suffix_length
                    = tools->intToCharVector1(_suffix_length);
            writeToBuffer(_binary_suffix_length);

            vector<unsigned char> _binary_suffix
                    = tools->wstringToCharVector(line);
            writeToBuffer(_binary_suffix);

            // begin with PO
            getline(fi, line);
            int _begin_with_po = atoi(tools->wstring2string(line).c_str());
            vector<unsigned char> _binary_begin_with_po
                    = tools->intToCharVector1(_begin_with_po);
            writeToBuffer(_binary_begin_with_po);

            // featureListId
            getline(fi, line);
            int _feature_list_id = atoi(tools->wstring2string(line).c_str());
            vector<unsigned char> _binary_feature_list_id
                    = tools->intToCharVector2(_feature_list_id);
            writeToBuffer(_binary_feature_list_id);
        }
    }
    if (debug)
    {
        wcout << "#build : TrieModels ok: " << _number_of_trie_models << endl;
    }

    // Feature lists
    getline(fi, line);
    int _number_of_feature_lists
            = atoi(tools->wstring2string(line).c_str());
    if (debug)
    {
        wcout << "_number_of_feature_lists = " << _number_of_feature_lists << endl;
    }
    vector<unsigned char> _binary_number_of_feature_lists
            = tools->intToCharVector2(_number_of_feature_lists);
    writeToBuffer(_binary_number_of_feature_lists);
    for (int i = 0; i < _number_of_feature_lists; ++i)
    {
        getline(fi, line);
        int _number_of_features = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_number_of_features
                = tools->intToCharVector1(_number_of_features);
        writeToBuffer(_binary_number_of_features);
        for (int j = 0; j < _number_of_features; ++j)
        {
            getline(fi, line);
            int _feature_id = atoi(tools->wstring2string(line).c_str());
            vector<unsigned char> _binary_feature_id
                    = tools->intToCharVector1(_feature_id);
            writeToBuffer(_binary_feature_id);
        }
    }
    if (debug)
    {
        wcout << "#build : Feature list ok: " << _number_of_feature_lists << endl;
    }

    // Features map
    getline(fi, line);
    int _number_of_features = atoi(tools->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_features
            = tools->intToCharVector1(_number_of_features);
    writeToBuffer(_binary_number_of_features);
    for (int i = 1; i <= _number_of_features; ++i)
    {
        // short feature
        getline(fi, line);
        int _short_feature_length = (int) line.length();
        //wcout << line << endl;
        vector<unsigned char> _short_binary_feature_length
                = tools->intToCharVector1(_short_feature_length);
        writeToBuffer(_short_binary_feature_length);
        vector<unsigned char> _short_binary_feature
                = tools->wstringToCharVector(line);
        writeToBuffer(_short_binary_feature);

        // long feature
        getline(fi, line);
        int _long_feature_length = (int) line.length();
        vector<unsigned char> _long_binary_feature_length
                = tools->intToCharVector1(_long_feature_length);
        writeToBuffer(_long_binary_feature_length);
        vector<unsigned char> _long_binary_feature
                = tools->wstringToCharVector(line);
        writeToBuffer(_long_binary_feature);
    }
    if (debug)
    {
        wcout << "#build : Feature Map ok: " << _number_of_features << endl;
    }

    // SUFFIX: sumber of suffix = 641965

    // Suffix nodes
    getline(fi, line);
    // number of nodes <= 188520
    int _number_of_suffix_nodes = atoi(tools->wstring2string(line).c_str());

    vector<unsigned char> _binary_number_of_suffix_nodes
            = tools->intToCharVector3(_number_of_suffix_nodes);
    writeToBuffer(_binary_number_of_suffix_nodes);
    for (int i = 0; i < _number_of_suffix_nodes; ++i)
    {
        // node char
        getline(fi, line);
        vector<unsigned char> _binary_node_char;
        _binary_node_char.push_back(tools->wcharToChar(line.at(0)));
        writeToBuffer(_binary_node_char);
        getline(fi, line);
        // parent id
        int _suffix_node_parent_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_suffix_node_parent_id
                = tools->intToCharVector3(_suffix_node_parent_id);
        writeToBuffer(_binary_suffix_node_parent_id);

        // number of node models <= 556
        getline(fi, line);
        int _number_of_node_models = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_number_of_node_models
                = tools->intToCharVector2(_number_of_node_models);
        writeToBuffer(_binary_number_of_node_models);
        // node models
        for (int j = 0; j < _number_of_node_models; ++j)
        {
            // suffix model id <= 632527
            getline(fi, line);
            int _suffix_model_id = atoi(tools->wstring2string(line).c_str());
            vector<unsigned char> _binary_suffix_model_id
                    = tools->intToCharVector3(_suffix_model_id);
            writeToBuffer(_binary_suffix_model_id);
        }
    }

    if (debug)
    {
        wcout << "#build : Suffix nodes ok: " << _number_of_suffix_nodes << endl;
    }

    // number of suffix models = 703729
    getline(fi, line);
    int _number_of_suffix_models = atoi(tools->wstring2string(line).c_str());
    vector<unsigned char> _binary_number_of_suffix_models
            = tools->intToCharVector3(_number_of_suffix_models);
    writeToBuffer(_binary_number_of_suffix_models);
    // suffix models
    for (int _suffix_model_id = 0;
         _suffix_model_id < _number_of_suffix_models;
         ++_suffix_model_id)
    {
        //
        getline(fi, line);
        int _feature_list_id = atoi(tools->wstring2string(line).c_str());
        vector<unsigned char> _binary_feature_list_id
                = tools->intToCharVector2(_feature_list_id);
        writeToBuffer(_binary_feature_list_id);
        // lemma suffix
        getline(fi, line);
        int _lemma_suffix_length = (int) line.length();
        vector<unsigned char> _binary_lemma_suffix_length
                = tools->intToCharVector1(_lemma_suffix_length);
        writeToBuffer(_binary_lemma_suffix_length);

        vector<unsigned char> _binary_lemma_suffix
                = tools->wstringToCharVector(line);
        writeToBuffer(_binary_lemma_suffix);
    }

    if (debug)
    {
        wcout << "#build : Suffix models ok: " << _number_of_suffix_models << endl;
    }

    fi.close();
}

void MorphologicalDictionary::writeToBinaryFile(string _filePath)
{
    wcout << "Write Trie to binary file ..." << endl;
    ofstream f(_filePath.c_str(), ios::out|ios::binary);
    f.write((char *)&buffer[0], bufferSize);
    f.close();
    wcout << "Write Trie to binary file done!" << endl;
}

void MorphologicalDictionary::temp_1()
{
    /*
    // find model indices
    vector<int> model_indices;
    for (int model_index = 1; model_index < mmodels.size(); ++model_index)
    {
        shared_ptr<MModel> mmodel = mmodels.at(model_index);
        shared_ptr<MModelElement> first_element = mmodel->elements.at(0);
        if (first_element->suffix != L"ло")
        {
            continue;
        }
        for (vector<shared_ptr<MModelElement> >::iterator e_iter
             = mmodel->elements.begin() + 1; e_iter != mmodel->elements.end(); ++e_iter)
        {
            shared_ptr<MModelElement> element = *e_iter;
            if (element->suffix == L"т")
            {
                model_indices.push_back(model_index);
                wcout << "Model: " << model_index << endl;
                break;
            }
        }
    }
    */
    // find lemma IDs
    for (int lemma_id = 1; lemma_id < (int) lemmaId_MNodeModel.size(); ++lemma_id)
    {
        if (lemmaId_MNodeModel.at(lemma_id) &&
            lemmaId_MNodeModel.at(lemma_id)->model_id == 754)
        {
            wcout << "Lemma ID = " << lemma_id << endl;
        }
    }
}
