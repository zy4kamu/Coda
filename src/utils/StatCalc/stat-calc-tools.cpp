/** 
 * @file    stat-calc-tools.cpp
 * @brief	source file for tools for StatCalc project
 */

#include "stat-calc-tools.h"

using namespace statcalc;
#include <iomanip>
namespace statcalc
{
	MapWstring2Category MAP;
}

TDMatrix
statcalc::trimNotFrequentWords(const TDMatrix& i_tdmatrix, Vocabulary& i_vocab, int i_treshholdDocFreq, int i_treshholdCollFreq)
{
    TDMatrix tdmatrix;
//    tdmatrix.reserve(i_tdmatrix.size());
    vector<Eigen::Triplet<ScalarType> > t;
    WordDoubleStat df, cf;
    df.setZero(i_tdmatrix.rows(),1);
    cf.setZero(i_tdmatrix.rows(),1);
    for (int k=0; k<i_tdmatrix.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(i_tdmatrix,k); it; ++it)
        {
            df(it.row(),0) += 1;
            cf(it.row(),0) += it.value();
        }
    }

    int count = 0;
    vector<int> isValid = vector<int>(i_vocab.size(),-1);
    for (int i = 0; i < df.rows() ; i++ )
    {
        if (df(i,0) >= i_treshholdDocFreq && cf(i,0) >= i_treshholdCollFreq)
        {
            isValid[i] = count++;
        }
        else
        {
            isValid[i] = -1;
        }
    }
    unordered_map<wstring,int> new_voc;
    for (auto it = i_vocab.begin();it != i_vocab.end();it++)
    {
        if (isValid[it->second] > -1)
            new_voc[it->first] = isValid[it->second];
    }

    for (int k=0; k<i_tdmatrix.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(i_tdmatrix,k); it; ++it)
        {
            if (isValid[it.row()]>-1)
                t.push_back(Eigen::Triplet<ScalarType>(isValid[it.row()],it.col(),it.value()));
        }
    }
    tdmatrix.derived().resize(new_voc.size(),i_tdmatrix.cols());
    tdmatrix.setFromTriplets(t.begin(),t.end());
    i_vocab = new_voc;
    return tdmatrix;
}

vector<wstring> statcalc::vocabularyToVector(const Vocabulary& i_voc)
{
    vector<wstring> res(i_voc.size(),wstring(L""));
    for (auto it = i_voc.begin(); it != i_voc.end(); it++)
    {
        res[it->second] = it->first;
    }
    return res;
}

void statcalc::printWordStat(const string& filename, WordDoubleStat wordStat, vector<wstring> words, bool sorted)
{
    wofstream oS (filename);
    Tools::SetLocale(oS);
    vector<double> res;
    eigentools::eigenVec2StlVec(wordStat,res);
    if (sorted)
    {
        Tools::Sort(res,words,false);
    }
    for (int i = 0; i<(int)res.size();i++)
        oS<<words[i]<<L";"<<res[i]<<std::endl;
}


void statcalc::removeWords(TDMatrix& tdmatrix, Vocabulary& vocab, const vector<wstring>& wordsList)
{
    vector<int> idx;
    idx.reserve(wordsList.size());
    vector<Eigen::Triplet<ScalarType> > t;
    t.reserve(tdmatrix.nonZeros());
    for (size_t i = 0; i < wordsList.size(); i++)
    {
        auto itr = vocab.find(wordsList[i]);
        if (itr != vocab.end())
        {
            idx.push_back(itr->second);
            vocab.erase(wordsList[i]);
        }
    }
    std::sort(idx.begin(),idx.end());
    auto it = std::unique (idx.begin(), idx.end());
    idx.resize( std::distance(idx.begin(),it) );
    int count = 0;
    vector<int> diff(tdmatrix.rows(),-1);
    for (size_t i = 0; i < diff.size();i++)
    {
        if (idx[count]==(int)i)
            count++;
        else
            diff[i] = count;
    }

    for (auto& x : vocab) x.second -= diff[x.second];

    for (int k=0; k<tdmatrix.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(tdmatrix,k); it; ++it)
        {
            if ( diff[it.row()] > -1 )
                t.push_back(Eigen::Triplet<ScalarType>(it.row()-diff[it.row()], it.col(), it.value()));
        }
    }
    tdmatrix.resize(vocab.size(),tdmatrix.cols());
    tdmatrix.setFromTriplets(t.begin(),t.end());
}

void statcalc::saveCollectionUCI(const TDMatrix &i_tdm, const Vocabulary &i_voc, const string& folder ,const string &collectionName)
{
    string space(" ");
    auto vocab = statcalc::vocabularyToVector(i_voc);
    wofstream oStreamDict(folder+"vocab."+collectionName+".txt");
    for (auto& x : vocab)
        oStreamDict << x << std::endl;
    oStreamDict.close();

    ofstream oStreamDocWord(folder+"docword."+collectionName+".txt");
    oStreamDocWord << i_tdm.cols() << std::endl;
    oStreamDocWord << i_tdm.rows() << std::endl;
    oStreamDocWord << std::setprecision(10) << i_tdm.sum() << std::endl;
    for (int k=0; k < i_tdm.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(i_tdm,k); it; ++it)
        {
            oStreamDocWord << it.col()+1<<space<<it.row()+1<<space<<it.value()<<std::endl;
        }
    }
    oStreamDocWord.close();
}


//ScalarType statcalc::similarityDocWithTitle(WordIntStat &docBOW, WordIntStat &titleBOW, WordIntStat& voc, shared_ptr<eigentools::PlsaOnlineRegularizedEM> tm)
//{
//    DenseMat repr(voc.size(),2);
//    repr.setZero(voc.size(),2);
//    for (auto i = docBOW.begin(); i != docBOW.end(); i++)
//    {
//        if (voc.count(i->first))
//            repr(voc[i->first],0) = i->second;
//    }
//    for (auto i = titleBOW.begin(); i != titleBOW.end(); i++)
//    {
//        if (voc.count(i->first))
//            repr(voc[i->first],1) = i->second;
//    }
////    DenseMat phi = tm.phi();
//    DenseMat res = tm->decomposeCollection(repr,tm->phi());
//    DenseMat theta = res.block(0,0,res.rows()-2,res.cols());
//    repr = theta.col(0);
//    res = theta.col(1);
//    return topicmodelingcosineMeasure(repr,res);
//}

//ScalarType statcalc::similarityDocWithTitle(const string &filename, WordIntStat &voc, shared_ptr<topicmodelingPlsaOnlineRegularizedEM> tm, shared_ptr<statcalc::DocumentProcessor> docProc)
//{
////    std::shared_ptr<Tools::Document> doc(filename);
////    WordIntStat docBOW, titleBOW;
////    docProc->processDocument(doc,docBOW);
////    docProc->processDocument(vector<wstring>(1,docProc->title()),titleBOW);
////    if (docBOW.empty() || titleBOW.empty())
////    {
////        std::cerr<<"EMPTY doc or title\n";
////    }
////    return similarityDocWithTitle(docBOW,titleBOW,voc,tm);
//}

//ScalarType statcalc::avgSimilarityDocWithTitle(const string &filelist, WordIntStat &voc, shared_ptr<topicmodelingPlsaOnlineRegularizedEM> tm, shared_ptr<StatCalc> statcalc)
//{
//    statcalc->loadFileList(filelist);
//    unordered_map<string,unsigned int> docs = statcalc->docID();
//    ScalarType sum = 0;
//    for (auto itr = docs.begin();itr!=docs.end();itr++)
//    {
//        sum += similarityDocWithTitle(itr->first,voc,tm,statcalc->docProcessor());
//    }
//    sum /= (ScalarType)docs.size();
//    return sum;
//}
