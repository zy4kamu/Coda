/**
 * DictionaryTrie.cpp
 */

#include "DictionaryTrie.h"
#include "Dictionary.h"

wstring FeatureAlphaNumeric = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789";

int DictionaryTrie::morphology_count = 0;

/**
 * Constructor of DictionaryTrie
 */
DictionaryTrie::DictionaryTrie(Dictionary* _dic)
{
    dic = _dic;

    root = NULL;
    numberOfNodes = 0;

    trieModelMap.clear();

    nodeModelMap.clear();
    numberOfNodeModels = 0;

    featureListMap.clear();
    featureMap.clear();
    featureIdMap.clear();

    numberOfLinks = 0;

    //createFeatureIdMap(dic->getPathToData() + FEATURE_PATH);
/*
    try
    {
        if (language == L"ru")
        {
            createFeatureIdMap(dic->getPathToData() + FEATURE_PATH);
        }
    }
    catch (int e)
    {
        if (e == FILE_NOT_FOUND_ERROR_CODE)
        {
            wcout << "Error : File not found : " << FEATURE_PATH << endl;
#ifdef MSVC
            _getch();
#endif
            exit(0);
        }
    }
*/
    setMainWordFormFromLinkedLemmas(true);
}

/**
 * Destructor of DictionaryTrie
 */
DictionaryTrie::~DictionaryTrie(void)
{
    if (root)
    {
        //delete root;
        root = NULL;
    }
    map<int, DictionaryTrieModel*>::iterator iter_1;
    for (iter_1 = trieModelMap.begin(); iter_1 != trieModelMap.end(); ++iter_1)
    {
        DictionaryTrieModel* _dictionaryTrieModel = iter_1->second;
        if (_dictionaryTrieModel)
        {
            delete _dictionaryTrieModel;
        }
    }
    trieModelMap.clear();

    map<int, DictionaryNodeModel*>::iterator iter_2;
    for (iter_2 = nodeModelMap.begin(); iter_2 != nodeModelMap.end(); ++iter_2)
    {
        DictionaryNodeModel* _dictionaryNodeModel = iter_2->second;
        if (_dictionaryNodeModel)
        {
            delete _dictionaryNodeModel;
        }
    }
    nodeModelMap.clear();

    featureListMap.clear();
    featureMap.clear();

    featureIdMap.clear();
    idFeatureMap.clear();

    dic = NULL;
}

void DictionaryTrie::createFeatureIdMap(const string & _filePath)
{
    wcout << "createFeatureIdMap :  " << endl;
    featureIdMap.clear();
    idFeatureMap.clear();
    /*
    wstring wFilePath;
    wFilePath.assign(_filePath.begin(), _filePath.end());
    */
    // open file
    wifstream fin(_filePath.c_str());
    // set encoding to UTF-8
#ifdef MSVC
    fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
    //fin.imbue(std::locale("ru_RU.UTF-8"));
    fin.imbue(std::locale("en_US.UTF-8"));
#endif
    if (fin.is_open())
    {
        wstring line;
        int _id = 0;
        while(getline(fin, line)) {
            _id++;
            wstring _feature = dic->getDictionaryTools()->refineStringByAlphabet(line, FeatureAlphaNumeric);
            featureIdMap[_feature] = _id;
            idFeatureMap[_id] = _feature;
            //wcout << line << " >> " << _id << ": " << _feature << endl;
        }
        // close file
        fin.close();
    }
    else
    {
        throw FILE_NOT_FOUND_ERROR_CODE;
    }
}

/*
 * Get node model by lemmaId
 */
DictionaryNodeModel* DictionaryTrie::getNodeModelByLemmaId(int _lemmaId)
{
    if (nodeModelMap.count(_lemmaId) > 0) {
        return nodeModelMap.at(_lemmaId);
    }
    return NULL;
}

/**
 * map _modelIndex to _trieModel
 */
void DictionaryTrie::mapModelIndexToTrieModel(int _modelIndex, DictionaryTrieModel* _trieModel)
{
    if (_modelIndex >= 0 && _trieModel->isValid()) {
        trieModelMap.insert(pair<int, DictionaryTrieModel*>(_modelIndex, _trieModel));
    }
}

/*
 * add a LPM + modelIndex
 * eg : #11195&1
 *      #500&2
 *      \u0451\u0436\u0438\u043a#38&3//here was cyrrilic symbols: ёжик
 */
void DictionaryTrie::addLPMModel(const wstring & _lpm, int _modelId, int _lemmaId)
{
    DictionaryNode* current = root;
    // if _lpm is empty
    if ( _lpm.length() == 0 ) {
        // create a NodeModel
        DictionaryNodeModel* _nodeModel = new DictionaryNodeModel(_modelId, _lemmaId);
        numberOfNodeModels++;
        // add NodeModel to current node
        current->addNodeModel(_nodeModel);
        // map lemmaId to NodeModel
        nodeModelMap.insert(pair<int, DictionaryNodeModel*>(_lemmaId, _nodeModel));
        return;
    }
    // if _lpm is not empty
    for ( int i = 0; i < (int) _lpm.length(); i++ ) {
        DictionaryNode* child = current->findChild(_lpm[i]);
        // if child node found
        if ( child != NULL ) {
            current = child;
        }
        // if child node not found -> create new node
        else {
            DictionaryNode* tmp = new DictionaryNode();
            numberOfNodes++;
            tmp->setCharacter(_lpm[i]);
            current->addChild(tmp);
            current = tmp;
        }
        // set model index and lemma index for ending-node
        if ( i + 1 == static_cast<int>(_lpm.length())) {
            // create a NodeModel
            DictionaryNodeModel* _nodeModel = new DictionaryNodeModel(_modelId, _lemmaId);
            numberOfNodeModels++;
            // add NodeModel to current node
            current->addNodeModel(_nodeModel);
            // map lemmaId to NodeModel
            nodeModelMap.insert(pair<int, DictionaryNodeModel*>(_lemmaId, _nodeModel));
        }
    }
}

/*
 * add a link between 2 lemmas by lemmaIds
 */
void DictionaryTrie::addLinkByLemmaIds(int fromLemmaId, int toLemmaId)
{
    // if fromLemmaId or toLemmaId is out of range than return
    // @todo : maxLemmaId, min = 0 or 1 ?
    if (fromLemmaId <= 0 || toLemmaId <= 0) {
        return;
    }
    // Get from and to NodeModels by lemmaId
    DictionaryNodeModel *fromNodeModel = getNodeModelByLemmaId(fromLemmaId);
    DictionaryNodeModel *toNodeModel = getNodeModelByLemmaId(toLemmaId);
    // add link if 2 nodes found
    if (fromNodeModel && toNodeModel) {
        toNodeModel->setLink(fromNodeModel);
    }
}

/**
 * Get TrieModel by modelId
 */
DictionaryTrieModel* DictionaryTrie::getTrieModelByModelId(int _modelId)
{
    if (trieModelMap.count(_modelId) > 0) {
        return trieModelMap.at(_modelId);
    }
    return NULL;
}

void DictionaryTrie::getMorphologicalInfoList(const wstring & _word, bool _beginWithPo, shared_ptr<vector<MorphologicalInfo> > result)
{
    morphology_count++;
//    wcout << "getMorphologicalInfoList : word = " << _word << endl;
    // initialize empty list of GrammInfo
    result->clear();
    // begin from root
    DictionaryNode* current = root;
    // _beginWithPo = true    : begin from 1-st character (_word[2])
    // _beginWithPo = false   : begin from 3-st character (_word[0])
    int i_word = _beginWithPo ? 1 : -1;
    int w_len = (int) _word.length();
    wstring _prefix(L"");
    while (i_word < w_len)
    {
        //wcout << "_prefix = " << _prefix << endl;
        // get list of NodeModel of current node
        vector<DictionaryNodeModel*> * _nodeModels = current->getNodeModels();
        // for each NodeModel
        for (vector<DictionaryNodeModel*>::iterator i_model = _nodeModels->begin(); i_model != _nodeModels->end(); ++i_model) {
            DictionaryNodeModel * _nodeModel = *i_model;
            // get modelId of NodeModel
            int _trieModelId = _nodeModel->getModelId();
            //wcout << _trieModelId << " ";
//            if (_trieModelId == 872)
//            {
//                wcout << "_trieModelId = " << _trieModelId << endl;
//            }
            // get TrieModel by _trieModelId
            DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
            // continue if TrieModel not found
            if (!_trieModel) {
//                if (i_word > 3)
//                    wcout << "WTF???" << endl;
                continue;
            }

            // get list of TrieModelElement of TrieModel
            vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();

            // continue if _trieModelElements is empty
//            if (_trieModelId == 872)
//                wcout << "_trieModelElements.size() = " << _trieModelElements.size() << endl;
            if (_trieModelElements->empty()) {
                continue;
            }
            // basic form
            wstring basicForm(L"");
            // get first TrieModelElement
            DictionaryTrieModelElement* _firstTrieModelElement = _trieModelElements->at(0);

            // get link of NodeModel
            DictionaryNodeModel* _linkNode = _nodeModel->getLink();
            // if NodeModel has a link
            if (_linkNode && mainWordFormFromLinkedLemmas) {
                // get string from root to link node
                wstring _link_prefix = _linkNode->getStringFromRoot();
                // get link TrieModel
                DictionaryTrieModel *_linkTrieModel = getTrieModelByModelId(_linkNode->getModelId());
                // get first TrieModelElement
                DictionaryTrieModelElement* _firstLinkTrieModelElement = _linkTrieModel->getFirstDictionaryTrieModelElement();
                // get basic form
                basicForm = _link_prefix;
                basicForm.append(*(_firstLinkTrieModelElement->getSuffix()));
            } else { // node does not have a link
                // get basic form
                basicForm = _prefix;
                basicForm.append(*(_firstTrieModelElement->getSuffix()));
            }
            int matchCount = 0;
            // for each TrieModelElement, from the 2nd TrieModelElement
            int _posCount = 0;
            for (vector<DictionaryTrieModelElement*>::iterator i_model_element = _trieModelElements->begin() + 1; i_model_element != _trieModelElements->end(); ++i_model_element)
            {
//                if (i_word > 2)
//                    wcout << "IN FOR LOOP" << endl;
                DictionaryTrieModelElement* _trieModelElement = *i_model_element;
                // continue if PO-attr of TrieModelElement not match with _beginWithPo
                if (_trieModelElement->isBeginWithPo() != _beginWithPo) {
                    continue;
                }
                // get suffix of TrieModelElement
                //wstring _suffix = _trieModelElement->getSuffix();
//                if (_trieModelId == 872)
//                    wcout << "prefix = " << _prefix << " >> suffix = " << _suffix << endl;
                // concat prefix and suffix
                wstring _prefix_suffix = _prefix;
                //_prefix_suffix.append(_suffix);
                _prefix_suffix.append(*(_trieModelElement->getSuffix()));
                // if match with _word
                if (_word.compare(_prefix_suffix) == 0) {
                    // create new GrammInfo
                    MorphologicalInfo _morphologicalInfo;
                    // word
                    _morphologicalInfo.word = _word;
                    // set id for GrammInfo
                    _morphologicalInfo.lemmaId = _nodeModel->getLemmaId();
                    // set initial form for _morphologicalInfo
                    _morphologicalInfo.initial_form = basicForm;
                    // set featureListId for _morphologicalInfo
                    _morphologicalInfo.featureListId = _trieModelElement->getFeatureListId();
                    // set basicFeatureListId for _morphologicalInfo
                    _morphologicalInfo.basicFeatureListId = _firstTrieModelElement->getFeatureListId();
                    // frequency
                    _morphologicalInfo.frequency = -1;

                    // @SUPER @TODO
                    _posCount++;
                    _morphologicalInfo.pos = _posCount;

                    // add _grammInfo to result
                    result->push_back(_morphologicalInfo);
                    // count number of matches
                    matchCount++;
                }
            }
            // if no match was found, consider the 1st TrieModelElement
            if (matchCount == 0) {
//                if (i_word > 2)
//                    wcout << "CONSIDER 1ST ELEMENT" << endl;
                DictionaryTrieModelElement* _trieModelElement = *(_trieModelElements->begin());
                // continue if PO-attr of TrieModelElement not match with _beginWithPo
                if ((_trieModelElement->isBeginWithPo() != _beginWithPo) || (_trieModelElement->getFeatureListId() == 0)) {
                    continue;
                }
                // get suffix of TrieModelElement
                //wstring _suffix = _trieModelElement->getSuffix();
//                if (i_word > 2)
//                    wcout << "prefix = " << _prefix << " >> suffix == " << _suffix << endl;
                // concat prefix and suffix
                wstring _prefix_suffix = _prefix;
                //_prefix_suffix.append(_suffix);
                _prefix_suffix.append(*(_trieModelElement->getSuffix()));
                // if match with _word
                if (_word.compare(_prefix_suffix) == 0) {
//                    wcout << _prefix_suffix << ">> FOUND!!!" << endl;
                    // create new MorphologicalInfo
                    MorphologicalInfo _morphologicalInfo;
                    // word
                    _morphologicalInfo.word = _word;
                    // set id for MorphologicalInfo
                    _morphologicalInfo.lemmaId = _nodeModel->getLemmaId();
                    // set initial form for GrammInfo
                    _morphologicalInfo.initial_form = basicForm;
                    // set basicFeatureListId for _morphologicalInfo
                    _morphologicalInfo.basicFeatureListId = _trieModelElement->getFeatureListId();
                    // features
                    _morphologicalInfo.featureListId = 0;
                    // frequency
                    _morphologicalInfo.frequency = -1;

                    _morphologicalInfo.pos = 0;

                    // add _grammInfo to result
                    result->push_back(_morphologicalInfo);
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
        // @TODO
        _prefix = _word.substr(0, i_word + 1);
        // find child node by next character
        DictionaryNode* tmp = current->findChild(_word[i_word]);
        // break if child node not found
        if ( tmp == NULL ) {
            break;
        }
        // set current node to child node
        current = tmp;
    }
//    wcout << ">> result = " << _morphologicalInfoList.size() << endl;
    //return _morphologicalInfoList;
}

/**
 * Get list of MorphologicalInfo by a given word
 */
void DictionaryTrie::getMorphologicalInfoList(const wstring & _word, shared_ptr<vector<MorphologicalInfo> > result)
{
    getMorphologicalInfoList(_word, false, result);
    if (getDictionary()->getDictionaryTools()->stringBeginsWithPo(_word)) {
        shared_ptr<vector<MorphologicalInfo> > result_po = std::make_shared<vector<MorphologicalInfo> >();
        //vector<MorphologicalInfo> _grammInfoList_Po = getMorphologicalInfoList(_word, true);
        getMorphologicalInfoList(_word, true, result_po);
        //_morphologicalInfoList.insert(_morphologicalInfoList.end(), _grammInfoList_Po.begin(), _grammInfoList_Po.end());
        result->insert(result->end(), result_po->begin(), result_po->end());
    }
    //return _morphologicalInfoList;
}

/**
 * convert featureListId to features
 */
vector<vector<wstring> > DictionaryTrie::convertFeatureListIdToFeaturePairs(int featureListId)
{
    vector<vector<wstring> > result;
    if (featureListMap.count(featureListId) > 0)
    {
        vector<int> _featuresList = featureListMap.at(featureListId);
        for (int i = 0; i < (int) _featuresList.size(); ++i)
        {
            int _featureId = _featuresList.at(i);
            wstring short_feature = L"";
            wstring long_feature = L"";
            if (idFeatureMap.count(_featureId) > 0)
            {
                short_feature = idFeatureMap.at(_featureId);
            }
            if (featureMap.count(_featureId) > 0)
            {
                long_feature = featureMap.at(_featureId);
            }
            vector<wstring> res;
            res.push_back(short_feature);
            res.push_back(long_feature);
            result.push_back(res);
        }
    }
    return result;
}

/**
 * convert MorphologicalInfo to GrammInfo
 */
void DictionaryTrie::convertMorphologicalInfoToGrammInfo(MorphologicalInfo * minfo, shared_ptr<GrammInfo> ginfo)
{
    ginfo->word = minfo->word;
    ginfo->id = minfo->lemmaId;
    ginfo->initial_form = minfo->initial_form;
    vector<wstring> _features;
    vector<int> _FID;
    if (minfo->basicFeatureListId != -1 && featureListMap.count(minfo->basicFeatureListId) > 0)
    {
        vector<int> * _featuresList = &featureListMap.at(minfo->basicFeatureListId);
        for (vector<int>::iterator iter = _featuresList->begin(); iter != _featuresList->end(); ++iter)
        {
            if (featureMap.count(*iter) > 0)
            {
                _features.push_back(featureMap.at(*iter));
            }
            _FID.push_back(*iter);
        }
    }
    if (featureListMap.count(minfo->featureListId) > 0)
    {
        vector<int> * _featuresList = &featureListMap.at(minfo->featureListId);
        for (vector<int>::iterator iter = _featuresList->begin(); iter != _featuresList->end(); ++iter)
        {
            if (featureMap.count(*iter) > 0)
            {
                _features.push_back(featureMap.at(*iter));
            }
            _FID.push_back(*iter);
        }
    }
    ginfo->features = _features;
    ginfo->fid = _FID;
}

/**
 * Get list of MorphologicalInfo of a neologism by its suffix
 * return only variants with max length of suffix
 * based on dictionary
 */
void DictionaryTrie::getMorphologicalPrediction(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result)
{
    set<int> featureListIdSet = set<int>();
    int max_suffix_length = 0;
    map<int, DictionaryTrieModel*>::iterator iter;
    for (iter = trieModelMap.begin(); iter != trieModelMap.end(); ++iter)
    {
        int matchCount = 0;
        DictionaryTrieModel* _trieModel = iter->second;
        // get list of TrieModelElement of TrieModel
        vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();
        // continue if _trieModelElements is empty
        if (_trieModelElements->empty()) {
            continue;
        }
        for (vector<DictionaryTrieModelElement*>::iterator it = _trieModelElements->begin();
             it != _trieModelElements->end(); ++it)
        {
            DictionaryTrieModelElement* _trieModelElement = *it;
            // suffix of model element
            wstring _model_suffix = *(_trieModelElement->getSuffix());
            // continue if suffix is empty or longer than word
            if (_model_suffix.length() == 0 || _model_suffix.length() > word.length() || featureListIdSet.count(_trieModelElement->getFeatureListId()) != 0)
            {
                continue;
            }
            // suffix of word
            wstring _word_suffix = word.substr(word.length() - _model_suffix.length());
            // if word ends with suffix of model element
            if (_word_suffix.compare(_model_suffix) == 0)
            {
                // create new MorphologicalInfo
                MorphologicalInfo _morphologicalInfo;
                // set id for GrammInfo
                _morphologicalInfo.word = word;
                _morphologicalInfo.lemmaId = 0;
                // set initial form for MorphologicalInfo
                _morphologicalInfo.initial_form = word.substr(0, word.length() - _model_suffix.length()) + *(_trieModelElements->at(0)->getSuffix());
                //_morphologicalInfo.initial_form = _model_suffix;
                // features
                _morphologicalInfo.featureListId = _trieModelElement->getFeatureListId();
                // insert featureListId to featureListIdSet
                featureListIdSet.insert(_trieModelElement->getFeatureListId());
                // set basicFeatureListId for _morphologicalInfo
                _morphologicalInfo.basicFeatureListId = it == _trieModelElements->begin() ? 0 : _trieModelElements->at(0)->getFeatureListId();
                // set suffix_length for _morphologicalInfo
                _morphologicalInfo.suffix_length = (int) _model_suffix.length();
                // update max_suffix_length
                if (max_suffix_length < (int) _model_suffix.length())
                {
                    max_suffix_length = (int) _model_suffix.length();
                }
                // add _grammInfo to result
                result->push_back(_morphologicalInfo);
                matchCount++;
            }
        }
    }
    // remove variants with suffix's length less than max_suffix_length
    /*
    int i = 0;
    while (i < (int) result.size())
    {
        if (result.at(i).suffix_length < max_suffix_length)
        {
            result.erase(result.begin() + i);
        }
        else
        {
            i++;
        }
    }
    */
}

/**
 * Get list of MorphologicalInfo by a given lemmaId
 */
void DictionaryTrie::getMorphologicalInfoListOfLemmaId(int lemmaId, shared_ptr<vector<MorphologicalInfo> > result)
{
    if (!nodeModelMap.count(lemmaId))
    {
        return;
    }
    DictionaryNodeModel* _nodeModel = nodeModelMap.at(lemmaId);
    if(mainWordFormFromLinkedLemmas) {
        DictionaryNodeModel* prev_node = _nodeModel;
        while(prev_node != 0) {
            _nodeModel = prev_node;
            prev_node = prev_node->getLink();
        }
    }
    wstring _prefix = _nodeModel->getNode()->getStringFromRoot();
    // get modelId of NodeModel
    int _trieModelId = _nodeModel->getModelId();
    // get TrieModel by _trieModelId
    DictionaryTrieModel *_trieModel = getTrieModelByModelId(_trieModelId);
    // continue if TrieModel not found
    if (!_trieModel) {
        return;
    }

    // get list of TrieModelElement of TrieModel
    vector<DictionaryTrieModelElement*> * _trieModelElements = _trieModel->getDictionaryTrieModelElements();

    // continue if _trieModelElements is empty
    if (_trieModelElements->empty()) {
        return;
    }
    // basic form
    wstring basicForm = L"";
    // get first TrieModelElement
    DictionaryTrieModelElement* _firstTrieModelElement = _trieModelElements->at(0);
    // for some parts of speech, initial form is duplcated. We will check and maybe remove it.
    size_t initialFormIndex = result->size();
    bool initialFormIsDuplicated = false;

    basicForm = _prefix;
    basicForm.append(*(_firstTrieModelElement->getSuffix()));

    // TrieModelElements
    for (vector<DictionaryTrieModelElement*>::iterator i_model_element = _trieModelElements->begin();
         i_model_element != _trieModelElements->end(); ++i_model_element)
    {
        DictionaryTrieModelElement* _trieModelElement = *i_model_element;
        // get suffix of TrieModelElement
        //wstring _suffix = _trieModelElement->getSuffix();
        // concat prefix and suffix
        wstring word = _prefix + *(_trieModelElement->getSuffix());
        if (_trieModelElement->isBeginWithPo())
        {
            word = L"по" + word;
        }
        // create new MorphologicalInfo
        MorphologicalInfo _morphologicalInfo;
        // word
        _morphologicalInfo.word = word;
        // set id for MorphologicalInfo
        _morphologicalInfo.lemmaId = _nodeModel->getLemmaId();
        // set initial form for _morphologicalInfo
        _morphologicalInfo.initial_form = basicForm;
        // set featureListId for _morphologicalInfo
        _morphologicalInfo.featureListId = _trieModelElement->getFeatureListId();
        // set basicFeatureListId for _morphologicalInfo
        _morphologicalInfo.basicFeatureListId = _firstTrieModelElement->getFeatureListId();
        if (_morphologicalInfo.featureListId == _morphologicalInfo.basicFeatureListId)
        {
            _morphologicalInfo.featureListId = 0;
        } else {
            if(word.compare(basicForm) == 0) {
                initialFormIsDuplicated = true;
            }
        }
        // frequency
        _morphologicalInfo.frequency = -1;

        // add _morphologicalInfo to result
        result->push_back(_morphologicalInfo);
    }
    if(initialFormIsDuplicated) {
        result->erase(result->begin() + initialFormIndex);
    }
    bool mainWordFormFromLinkedLemmasActual = mainWordFormFromLinkedLemmas;
    mainWordFormFromLinkedLemmas = false;
    vector<DictionaryNodeModel*> children = _nodeModel->getLinkFrom();
    for(size_t i = 0; i < children.size(); i++) {
        getMorphologicalInfoListOfLemmaId(children[i]->getLemmaId(), result);
    }
    mainWordFormFromLinkedLemmas = mainWordFormFromLinkedLemmasActual;
}

void DictionaryTrie::readRulesFromTextFile(const string& filePath)
{
    //wcout << "readRulesFromTextFile ... " << endl;
    wifstream fi(filePath.c_str());
    if (!fi.is_open())
    {
        wcout << "Rule file not found!!!" << endl;
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
    ruleSet.min_lemma_length = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
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
            _lemma_feature_list_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
        }
        else
        {
            _word_feature_list_id = atoi(this->getDictionary()->getDictionaryTools()->wstring2string(line).c_str());
            MorphologyRule rule;
            rule.word_suffix = _word_suffix;
            rule.lemma_suffix = _lemma_suffix;
            rule.word_feature_list_id = _word_feature_list_id;
            rule.lemma_feature_list_id = _lemma_feature_list_id;
            ruleSet.morphologyRules.push_back(rule);
        }
    }
    fi.close();
    //wcout << "readRulesFromTextFile ok" << endl;
}

void DictionaryTrie::getMorphologicalInfoListByRules(const wstring & word, shared_ptr<vector<MorphologicalInfo> > result)
{
    bool debug = false;
    result->clear(); // result
    set<pair<wstring, int> > resultSet; // set<lemma, featureListId> to avoid duplications
    resultSet.clear();
    for (vector<MorphologyRule>::iterator iter = ruleSet.morphologyRules.begin();
         iter != ruleSet.morphologyRules.end(); ++iter)
    {
        MorphologyRule rule = *iter;
        if ((int) (word.length() + rule.lemma_suffix.length() - rule.word_suffix.length())
                >= ruleSet.min_lemma_length
             && this->getDictionary()->getDictionaryTools()->isSuffix(rule.word_suffix, word))
        {
            wstring _lemma = word.substr(0, word.length() - rule.word_suffix.length());
            _lemma += rule.lemma_suffix;
            if (debug)
            {
                wcout << "_lemma = " << _lemma << endl;
            }

            // find lemmaIds
            set<int> lemma_ids;
            lemma_ids.clear();
            shared_ptr<vector<MorphologicalInfo> > lemma_morphological_infos = std::make_shared<vector<MorphologicalInfo> >();
            this->getMorphologicalInfoList(_lemma, lemma_morphological_infos);

            // if lemma not found in dictionary >> ignore
            if (!lemma_morphological_infos->size())
            {
                continue;
            }
            if (debug)
            {
                wcout << "lemmaIds: " << endl;
            }

            // map<lemmaFID, lemmaId>
            map<int, int> lemmaFID_lemmaId;
            lemmaFID_lemmaId.clear();

            for (vector<MorphologicalInfo>::iterator m_iter = lemma_morphological_infos->begin();
                 m_iter != lemma_morphological_infos->end(); ++m_iter)
            {
                MorphologicalInfo minfo = *m_iter;
                lemma_ids.insert(minfo.lemmaId);
                lemmaFID_lemmaId[minfo.featureListId] = minfo.lemmaId;
                if (debug)
                {
                    wcout << "lemmaId = " << minfo.lemmaId << " - Fid = " << minfo.featureListId << endl;
                }
            }

            // find featureListIds of lemmaIds
            if (debug)
            {
                wcout << "lemma - word - featureListId: " << endl;
            }
            set<int> wordFIDSet; // set<feature_list_id_of_word>
            wordFIDSet.clear();
            for (set<int>::iterator l_iter = lemma_ids.begin(); l_iter != lemma_ids.end(); ++l_iter)
            {
                int lemma_id = *l_iter;
                if (debug)
                {
                    wcout << "Morphology of lemma : " << lemma_id << endl;
                }
                std::shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
                this->getMorphologicalInfoListOfLemmaId(lemma_id, minfos);

                // word
                for (vector<MorphologicalInfo>::iterator m_iter = minfos->begin();
                     m_iter != minfos->end(); ++m_iter)
                {
                    MorphologicalInfo minfo = *m_iter;
                    wordFIDSet.insert(minfo.featureListId);
                    if (debug)
                    {
                        wcout << "[Word] lemma =  " << minfo.initial_form << " - word = " << minfo.word << " - wordFID = " << minfo.featureListId << endl;
                    }
                }
            }

            pair<wstring, int> p(_lemma, rule.word_feature_list_id);
            if (resultSet.count(p) > 0 || wordFIDSet.count(rule.word_feature_list_id) > 0
                    || lemmaFID_lemmaId.count(rule.lemma_feature_list_id) == 0)
            {
                if (debug)
                {
                    wcout << "# " << _lemma << " [WFLI] " << rule.word_feature_list_id
                          << " [LFLI] " << rule.lemma_feature_list_id << " >> Continue" << endl;
                }
                continue;
            }
            MorphologicalInfo _morphologicalInfo;
            // word
            _morphologicalInfo.word = word;
            // set id for MorphologicalInfo
            _morphologicalInfo.lemmaId = lemmaFID_lemmaId.at(rule.lemma_feature_list_id);
            // set initial form for _morphologicalInfo
            _morphologicalInfo.initial_form = _lemma;
            // set featureListId for _morphologicalInfo
            _morphologicalInfo.featureListId = rule.word_feature_list_id;
            // set basicFeatureListId for _morphologicalInfo
            _morphologicalInfo.basicFeatureListId = 0;
            // frequency
            _morphologicalInfo.frequency = 0;

            // add _morphologicalInfo to result
            result->push_back(_morphologicalInfo);
            resultSet.insert(p);
        }
    }

    //return result;
}

void DictionaryTrie::enableRules(bool _useRules)
{
    useRules = _useRules;
    if (useRules)
    {
        readRulesFromTextFile(this->getDictionary()->getPathToData() + MORPHOLOGY_RULES_PATH);
    }
}
