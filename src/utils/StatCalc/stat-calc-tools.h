/** 
 * @file    stat-calc-tools.h
 * @brief	header file for tools for statcalc project
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


#ifndef _STAT_CALC_TOOLS_
#define _STAT_CALC_TOOLS_

#include "stat-calc.h"
//#include "statcalc-data-types.h"
#include "eigen-data-types.h"
#include "eigen-tools.h"
//#include "document-processor.h"
#include <iostream>
#include "Tools.h"
#include <algorithm>

using namespace std;
using namespace eigentools;

namespace statcalc
{


/**
 * printing unordered_map<wstring, class T>
 */
template<class T>
void
print_map(wostream& o_out, unordered_map<wstring, T>& i_map)
{
    vector<wstring> words = Tools::Keys(i_map);
    vector<T> values = Tools::Values(i_map);
    Tools::Sort(values,words,false);
    for (size_t i = 0; i < words.size();i++)
    {
        o_out << words[i]<< L";" << values[i]<< endl;
    }
}

/**
 * @brief LOG2 calculates binary logarithm
 * @param n logarithm's argument
 * @return binary logarithm of argument
 */
double LOG2( double n ) ;

TDMatrix trimNotFrequentWords(const TDMatrix& i_tdmatrix, Vocabulary& i_vocab, int i_treshholdDocFreq, int i_treshholdCollFreq);

template<typename Derived>
void tdmatrixToEigenSparseMatrix(const TDMatrix& i_tdmatrix, SparseMatrix<Derived> &o_spmat,WordDoubleStat& i_idf)
{
    typedef typename SparseMatrix<Derived>::Scalar Scalar;
    vector<Eigen::Triplet<Scalar> > t;
    for (int k=0; k<i_tdmatrix.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(i_tdmatrix,k); it; ++it)
        {
            t.push_back(Eigen::Triplet<Scalar>(it.row(),it.col(),(Scalar)(1.0+log((double)it.value())*i_idf(it.row()))));
        }
    }
    o_spmat.derived().resize(i_tdmatrix.rows(),i_tdmatrix.cols());
    o_spmat.setFromTriplets(t.begin(),t.end());
}

template<typename Derived>
void docBOW2EigenDenseMatrix(statcalc::Vocabulary& voc, unordered_map<int,int> &docBOW, Vocabulary& docBow_voc, DenseBase<Derived>& o_mat)
{
    o_mat.derived().resize(voc.size(),1);
    o_mat.setZero();
    for (auto it = docBow_voc.begin(); it != docBow_voc.end(); it++)
    {
        int count = docBOW.count(it->second);
        if (count > 0)
        {
            auto itr = voc.find(it->first);
            if (itr!=voc.end())
            {
                o_mat.coeffRef(itr->second,0) = count;
            }
        }
    }
}

template<typename Derived>
void idCatToEigenSparseMatrix(
        IDCategory& idCat,
        int CategoryNumbers,
        SparseMatrix<Derived> &o_spmatCatDoc
        )
{
    typedef typename SparseMatrix<Derived>::Scalar Scalar;
    vector<Eigen::Triplet<Scalar> > t_catDoc;
    o_spmatCatDoc.derived().resize(CategoryNumbers,idCat.size());
    if (idCat.begin()->second.isEnum())
        for (auto it = idCat.begin();it!=idCat.end();it++)
            t_catDoc.push_back(Eigen::Triplet<Scalar>(it->second.labelToVec()[0], it->first,(Scalar)1));
    else
    {
        int id = 0;
        unordered_map<int,int> labelMap;
        for (auto it = idCat.begin();it!=idCat.end();it++)
        {
            vector<int> vec = it->second.labelToVec();
            for (size_t j = 0; j < 0; j++)
            {
                labelMap[vec[j]] = id++;
                t_catDoc.push_back(Eigen::Triplet<Scalar>(labelMap[vec[j]], it->first,1));
            }
        }
        Tools::save_map_binary("catId_map.bin",labelMap);
        wofstream oStream("catId_map.bin");
        for (auto it = labelMap.begin(); it != labelMap.end(); it++)
        {
            oStream<<it->second<<L";"<<it->first<<std::endl;
        }
        oStream.close();
    }
    o_spmatCatDoc.setFromTriplets(t_catDoc.begin(),t_catDoc.end());
}

void removeWords(TDMatrix& tdmatrix, Vocabulary &vocab, const vector<wstring>& wordsList);

void saveCollectionUCI(const TDMatrix& i_tdm, const Vocabulary& i_voc, const string &folder, const string& collectionName);

vector<wstring> vocabularyToVector(const Vocabulary& i_voc);

void printWordStat(const string& filename, WordDoubleStat wordStat, vector<wstring> words, bool sorted = true);

//ScalarType similarityDocWithTitle(const string& filename, WordIntStat& voc, shared_ptr<topicmodelingPlsaOnlineRegularizedEM> tm, shared_ptr<statcalc::DocumentProcessor> docProc);

//ScalarType similarityDocWithTitle(WordIntStat& docBOW, WordIntStat& titleBOW, WordIntStat& voc, shared_ptr<topicmodelingPlsaOnlineRegularizedEM> tm);

//ScalarType avgSimilarityDocWithTitle(const string& filelist, WordIntStat& voc, shared_ptr<topicmodelingPlsaOnlineRegularizedEM> tm, shared_ptr<statcalc::StatCalc> statcalc);

}
#endif //_STAT_CALC_TOOLS_
