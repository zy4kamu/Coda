/** 
 * @file    stat-calc.h
 * @brief	header file for StatCalc class for calculating other statics of words and documets
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


#ifndef _STAT_CALC_
#define _STAT_CALC_

#include "statcalc-data-types.h"
//#include "stat-calc-tools.h"
#include <algorithm>
#include <functional>
#include "document-processor.h"
using lemmatizer::ILemmatizer;
using Tokenization::ITokenizer;
using _sentence_splitter::ISentenceSplitter;

namespace statcalc
{
/**
 * @class	StatCalc
 * @brief	Class for obsorbing word statistics from texts, saving, storing, etc...
 */
class StatCalc
{
public:

    /**
     * @brief	default constructor
     */
    StatCalc(shared_ptr<ITokenizer> i_tokenizer, shared_ptr<ILemmatizer> i_lemmatizer, shared_ptr<ISentenceSplitter> i_splitter);

    /**
     * @brief Getter for m_tdmatrix
     * @return m_tdmatrix
     */
    TDMatrix &tdmatrix(void);

    /**
     * @brief Getter for m_docID
     * @return m_docID
     */
    DocID &docID(void);

    /**
     * @brief getter for m_idCategory
     * @return m_idCategory
     */
    IDCategory &idCategory();

    /**
     * @brief Getter for m_idf
     * @return m_idf
     */
    WordDoubleStat idf();

    /**
     * @brief Getter for m_df
     * @return m_df (document freaquncy)
     */
    WordDoubleStat &df();

    /**
     * @brief Getter for m_cf
     * @return m_cf (collection freaquncy)
     */
    WordDoubleStat &cf();

    /**
     * @brief MI is getter for m_MI
     * @return m_MI
     */
    vector<WordDoubleStat> MI();

    /**
     * @brief Function for cleaning all data
     */
    void clean();

    /**
     * @brief loadFileList is function for filling structures
     * m_docID and m_idCategory
     * @param[in] i_filelist - name of file with pathes to documents and corresponding domains
     */
    void loadFileList(const string& i_filelist, const string &prefix = string(""));

    /**
     * @brief Function for storing m_tdmatrix in binary format
     * @param[in] i_filename name of file
     * @return true in case of success, false - otherwise
     */
    bool saveTDMatrix(const string& i_filename,bool withDocNames = false);

    /**
     * @brief Function for loading data from binary file
     * @param[in] i_filename name of file
     * @return true in case of success, false - otherwise
     */
    bool loadTDMatrix(const string& i_filename,bool withDocNames = false);

    bool loadOldTDMatrix(const string& i_filename, bool withDocNames = false);

    /**
     * @brief calculateTDMatrix is function for filling m_tdmatrix
     */
    void calculateTDMatrix(shared_ptr<Tools::DocumentCreator> i_docCreator);

	void calculateTDMatrix(vector<shared_ptr<Tools::Document> >& docVec);
    /**
     * @brief Calculate document freaquency (m_df). Based on m_tdmatrix data
     * @return m_df
     */
    WordDoubleStat &calculatedf(void);

    /**
     * @brief Calculate cocument freaquency for defined domain. Based on m_tdmatrix data. CF is not stored to m_cf!!!!
     * @param[in] i_domain is domain for cf calculation
     * @return m_df
     */
    WordDoubleStat &calculatecf(Category i_domain);

    /**
     * @brief calculatecf computes collection frequency of token
     * @return m_cf
     */
    WordDoubleStat &calculatecf(void);

    /**
     * @brief Calculate invert document frequency (m_idf). Based on m_df data
     */
    WordDoubleStat &calculateidf(void);

    /**
     * @brief Calculate mutual information (m_MI)
     */
    vector<WordDoubleStat> &calculateMI(void);

    Vocabulary& vocabulary();

    void set_vocabulary(const Vocabulary& i_voc);

    bool& predefinedVocabulary(void);

    /**
     * @brief calculateCentroids is function for obtaining centroids of classes
     * @return
     */
    vector<WordDoubleStat> &calculateCentroids(void);

    shared_ptr<DocumentProcessor> docProcessor()
    {
        return m_docProcessor;
    }

    friend class statcalc::DocumentProcessor;

private:
    void saveTDMatrix_(ofstream& i_oStream);
    void saveDocID_(ofstream& i_oStream);
    void saveIDCategory_(ofstream& i_oStream);
    void saveVocabulary_(ofstream& i_oStream);

    void loadTDMatrix_(ifstream& i_iStream);
    void loadDocID_(ifstream& i_iStream);
    void loadIDCategory_(ifstream& i_iStream);
    void loadVocabulary_(ifstream& i_iStream);
    void loadOldTDMatrix_(ifstream& i_iStream);

    shared_ptr<DocumentProcessor> m_docProcessor;

    DocID m_docID;
    DocCollectionBOW m_vecTriplets;
    Vocabulary m_vocabulary;
    IDCategory m_idCategory;
    TDMatrix m_tdmatrix;
    WordDoubleStat m_df;
    WordDoubleStat m_cf;
    WordDoubleStat m_idf;
    vector<WordDoubleStat> m_MI;
    vector<WordDoubleStat> m_centroids;
    bool m_predefinedVocabulary;
};

}
#endif //_STAT_CALC_
