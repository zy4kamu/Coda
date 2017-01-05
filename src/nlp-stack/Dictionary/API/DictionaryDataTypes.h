/**
 * @file DictionarydataTypes.h
 * @brief Definitions of constants and structure
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


#ifndef _DICTIONARY_DATA_TYPES_H_
#define _DICTIONARY_DATA_TYPES_H_

//#ifndef MSVC
//#define MSVC

//#pragma warning( disable: 4996 4129)

#include <time.h>

#include <iostream>
#include <fstream>

#ifdef MSVC
	#include <Windows.h> // List all file in folder
	#include <io.h>
	#include <conio.h>
	#include <codecvt>
#else
	#include <locale.h>
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <map>

// File not found error const
#define FILE_NOT_FOUND_ERROR_CODE 20

// Current folder is : E:\Projects\2012_12_24_Russian_Dialog_System\2013_01_21_Project_SVN\RDS\Dictionary

// File not found error const
#define FILE_NOT_FOUND_ERROR_CODE 20
//Path to dictionary binary file
//#define BIN_DIC_FILE_PATH "../../src/Dictionary/Data/DicVer6.bin"
// Path to feature list file
//#define FEATURE_PATH "../../src/Dictionary/Data/FeatureList.txt"
//// Path to N-Gram binary file
//#define BIN_N_GRAM_FILE_PATH "../../src/Dictionary/Data/NGramVer3.bin"
//// Path to suffix model binary file
//#define BINARY_SUFFIX_PATH "../../src/Dictionary/Data/SuffixModel.bin"
//// Path to stress position file
//#define STRESS_POS_FILE_PATH "../../src/Dictionary/Data/2013_06_17_StressPos_Ver2.txt"

// Russian and Latin alphabet, digits and '-'
#define WORD_CHARACTERS L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ-0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
// Russian alphabet
#define RUSSIAN_WORD_CHARACTERS L"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
// English alphabet
#define ENGLISH_WORD_CHARACTERS L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
// word delimiter in sentence
#define WORD_DELIMITER L" ,'\""
// sentence delimiter in text
#define SENTENCE_DELIMITER L".;!?\n"
// max depth of suffix model trie
#define MAX_SUFFIX_MODEL_TRIE_DEPTH 5
// N-Gram
#define N_GRAM 3  // 3-gram

// Path to dictionary binary file
//#define BIN_DIC_FILE_PATH "DicVer6.bin"
#define BIN_DIC_FILE_PATH_RUSSIAN "RussianMorphologyVer2.bin"
#define BIN_DIC_FILE_PATH_ENGLISH "EnglishMorphologyVer2.bin"
#define BIN_DIC_FILE_PATH_ENGLISH1 "EnglishMorphology.bin"
#define MORPHOLOGY_RULES_PATH "EnglishMorphologyRules.txt"
// Path to feature list file
#define FEATURE_PATH "FeatureList.txt"
// Path to N-Gram binary file
#define BIN_N_GRAM_FILE_PATH "NGramVer3.bin"
// Path to suffix model binary file
#define BINARY_SUFFIX_PATH "SuffixModel.bin"
// Path to binary stress position file
#define ACCENT_BINARY_FILE_PATH "Accent.bin"

// Path to stress position file
#define STRESS_POS_FILE_PATH "2013_07_01_StressPos_Ver3.txt"

using namespace std;

/**
 * Morphology
 */
struct Morphology
{
    // lemmaId
    int lemma_id;
    // lemma
    shared_ptr<wstring> lemma;
    // word - for Morphology by lemma_id
    shared_ptr<wstring> word;
    // list of features : VERB, NOUN,...
    vector<shared_ptr<wstring> > features;
    vector<shared_ptr<wstring> > descriptions;
    // suffix - prediction
    size_t suffix_length;
};

/**
 * Gramatical information of word 
 */
struct GrammInfo 
{
	// lemmaId
	int id;
    // word
    wstring word;
    // basic form : \u0447\u0438\u0442\u0430\u044e -> \u0447\u0438\u0442\u0430\u0442\u044c//here was cyrrilic symbols: читаю,читать
    wstring initial_form;
    // list of features : VERB, NOUN,...
	vector<wstring> features;
	vector<wstring> fid;
};

/**
 * Morphological information of word 
 */
struct MorphologicalInfo 
{
	// lemmaId
	int lemmaId;
    // word
    wstring word;
    // basic form : \u0447\u0438\u0442\u0430\u044e -> \u0447\u0438\u0442\u0430\u0442\u044c//here was cyrrilic symbols: читаю,читать
    wstring initial_form;
    // feature set Id
	int featureListId;
	// feature set Id of basic form, = 0 if word is basic form
	int basicFeatureListId;
	// length of suffix in trie
	int suffix_length;
	// frequency : how many times suffix appears in dictionary and corpus
	int frequency;
	// additional information : word is in dictionary, is misspell, ... 
	wstring additionalInfo;
	// position in lemma, for adding stress
	int pos;
};

/**
 * Morphological Analyze of word 
 */
struct MorphologicalAnalyze
{
	// word
	wstring word;
	// results of morphological analyze
	vector<MorphologicalInfo> minfos;
	// best variant of results
	int bestVariant;
	// current variant (in demo)
	int currentVariant;
};

/**
 * N-Gram Morphological Analyze of sentences 
 */
struct NGramMorphologicalAnalyze
{
	// max N-Gram value
	int maxNGram;
	// morphological analyze of each word in sentence
	vector<MorphologicalAnalyze> mAnalyze;
};

/**
 * struct WordForm for finding word form by basic form and features 
 */
struct WordForm
{
	// word form
	wstring word;
	// morphological information
	MorphologicalInfo info;
};

/**
 * struct WordFormWithStress for adding stress to word
 */
struct WordFormWithStress
{
	// word
	wstring word;
	// position of stress
	int stressPos;
	// lemmaId
	int lemmaId;
	// basic form
	wstring initial_form;
	// feature list id
	int featureListId;
	// feature list id of basic form
	int basicFeatureListId;
};

/**
 * Struct WordInfo
 * Stress information
 * Morphological features by OpenCorpora
 */
struct WordInfo
{
	// word
	wstring word;
	// lemmaId
	int lemmaId;
	// word
	wstring wordWithStress;
	// features by OpenCorpora
	vector<wstring> openCorporaFeatures;
};

// MorphologicalDictionary

struct MRule
{
    wstring word_suffix;
    wstring lemma_suffix;
    int word_feature_list_id;
    int lemma_feature_list_id;
};

struct MRuleSet
{
    vector<MRule> morphologyRules;
    size_t min_lemma_length; // min length of lemma (Is -> I is not correct)
};

struct MModelElement
{
    wstring suffix;
    int feature_list_id;
    // begins with "по"
    bool po;
};

struct MModel
{
    vector<shared_ptr<MModelElement> > elements;
};

struct MNode;

struct MNodeModel
{
    shared_ptr<MNode> node;
    int model_id;
//    shared_ptr<MNodeModel> link;
};

struct MNodeItem
{
    int lemma_id;
    int feature_list_id;
//    int feature_list_id_of_lemma;
    bool po;
};

struct MNode
{
    wchar_t character;
    // map<char_code, child_node>
    map<wchar_t, shared_ptr<MNode> > children;
    shared_ptr<MNode> parent;
    // map<lemma_id, MNodeModel>
    map<int, shared_ptr<MNodeModel> > lemmaId_MNodeModel;
    //map<suffix, MNodeItem>
    map<wstring, shared_ptr<vector<shared_ptr<MNodeItem> > > > suffix_MNodeItem;
};

// SUFFIX

struct SuffixNode
{
    wchar_t character;
    // map<char, suffix_child_node>
    map<wchar_t, shared_ptr<SuffixNode> > children;
    // vector<suffix_trie_model_id>
    vector<int> suffix_trie_model_ids;
};

struct SuffixModel
{
    int feature_list_id;
    wstring lemma_suffix;
};

//#endif /* MSVC */
#endif /* _DICTIONARY_DATA_TYPES_H_ */
