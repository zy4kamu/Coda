/**
 * MorphologicalDictionary.h
 *
 * Class MorphologicalDictionary
 * Light version of DictionaryTrie
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


#ifndef _MORPHOLOGICAL_DICTIONARY_H_
#define _MORPHOLOGICAL_DICTIONARY_H_

#include "DictionaryDataTypes.h"
#include "DictionaryTools.h"

using namespace std;

class MorphologicalDictionary
{
public:
    static int morphology_count;
    static int getMorphologyCount() { return morphology_count;}

    static int prediction_count;
    static int getPredictionCount() { return prediction_count;}

    static int rule_count;
    static int getRuleCount() { return rule_count;}

    static int e_count;
    static int getECount() { return e_count;}

    MorphologicalDictionary(const string & binaryFileName = BIN_DIC_FILE_PATH_RUSSIAN,
                            bool load_data = true,
                            shared_ptr<DictionaryTools> _tools = NULL);
    ~MorphologicalDictionary();

    shared_ptr<DictionaryTools> getTools() { return tools;}

    /**
     * @brief add a vector<char> to buffer
     */
    void writeToBuffer(vector<unsigned char> charVector);
    /**
     * @brief build buffer from text file
     */
    void buildBufferFromTextFile(string _filePath);
    /**
     * @brief write buffer to binary file
     */
    void writeToBinaryFile(string _filePath);

    void readBinaryFile(const string & filePath);

    void initSuffixMNodeItem();
    void initLemmaIdLemma();
    void init();

    void getMorphologyPo(const wstring & lower_word, bool po,
                         shared_ptr<vector<shared_ptr<Morphology> > > result);
    void getMorphology(const wstring & word,
                       shared_ptr<vector<shared_ptr<Morphology> > > result);

    void getMorphologyE(const wstring & word,
                       shared_ptr<vector<shared_ptr<Morphology> > > result);

    void getMorphologyOfLemmaId(int lemma_id,
                       shared_ptr<vector<shared_ptr<Morphology> > > result);

    void getFeatureListOfLemmaPo(shared_ptr<wstring> lower_lemma, bool po,
                               shared_ptr<map<int, shared_ptr<set<int> > > > lemmaId_wordFLIDs);
    void getFeatureListOfLemma(shared_ptr<wstring> lower_lemma,
                               shared_ptr<map<int, shared_ptr<set<int> > > > lemmaId_wordFLIDs);

    // Morphy's Rules
    void enableRules(bool _useRules) { useRules = _useRules;}
    bool ruleIsEnable() { return useRules;}
    void readRulesFromTextFile(const string& filePath);
    void getMorphologicalInfoListByRules(const wstring & word,
                                         shared_ptr<vector<shared_ptr<Morphology> > > result);

    // predictions by suffix (MModelElement)
    void setPrediction(bool _usePrediction) {usePrediction = _usePrediction;}
    bool predictionIsEnable() { return usePrediction;}
    void getMorphologicalPrediction(const wstring & word,
                                    shared_ptr<vector<shared_ptr<Morphology> > > result);
    void setMinSuffixLength(size_t _min_suffix_length)
    {
        min_suffix_length = _min_suffix_length;
    }

    void enableE(bool _useE) { useE = _useE;}
    bool eIsEnabled() { return useE;}

    int getMaxLemmaId() { return maxLemmaId;}

    // check morphological prediction: европарламент -> европарламенло
    void temp_1();

protected:
    // TRIE
    shared_ptr<MNode> root;
    vector<shared_ptr<MNodeModel> > lemmaId_MNodeModel;
    vector<shared_ptr<MModel> > mmodels;

    // SUFFIX
    shared_ptr<SuffixNode> suffix_root;
    vector<shared_ptr<SuffixModel> > suffix_models;

    // FEATURES
    vector<vector<int> > id_feature_list;
    vector<shared_ptr<wstring> > id_short_feature;
    vector<shared_ptr<wstring> > id_long_feature;

    // tools
    shared_ptr<DictionaryTools> tools;

    // Morphy's rules
    bool useRules;
    MRuleSet ruleSet;

    // predictions by suffix (MModelElement)
    bool usePrediction;
    // min suffix length
    size_t min_suffix_length;

    // lemma_id - lemma
    int maxLemmaId;
    vector<shared_ptr<wstring> > lemmas;

    // черный >> чёрный
    bool useE;

    // binary file builder

    // buffer
    unsigned char *buffer;
    // size of buffer
    int bufferSize;
};

#endif /* _MORPHOLOGICAL_DICTIONARY_H_ */
