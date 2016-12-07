/** 
 * @file    stat-calc.cpp
 * @brief	source file for StatCalc class for calculating other statics of words and documets
 */
#include "stat-calc-tools.h"
#include "stat-calc.h"
#include "Strings/Splitter.h"
#include "document-processor.h"
#include "ISentenceSplitter.h"
#include <cmath>

using namespace statcalc;
using lemmatizer::ILemmatizer;

StatCalc::StatCalc(shared_ptr<ITokenizer> i_tokenizer, shared_ptr<ILemmatizer> i_lemmatizer, shared_ptr<_sentence_splitter::ISentenceSplitter> i_splitter)
{
    m_docProcessor = shared_ptr<DocumentProcessor>(
                new DocumentProcessor(i_tokenizer,i_lemmatizer,i_splitter));
    m_predefinedVocabulary = false;
}

TDMatrix&
StatCalc::tdmatrix(void)
{
    return this->m_tdmatrix;
}

DocID&
StatCalc::docID(void)
{
    return this->m_docID;
}

Vocabulary&
StatCalc::vocabulary()
{
    return this->m_vocabulary;
}

void
StatCalc::set_vocabulary(const Vocabulary& i_voc)
{
    this->m_vocabulary = i_voc;
    m_predefinedVocabulary = true;
}

bool &StatCalc::predefinedVocabulary(void)
{
    return this->m_predefinedVocabulary;
}

void
StatCalc::clean()
{
    this->m_tdmatrix.setZero();
    this->m_docID.clear();
    this->vocabulary().clear();
    this->m_vecTriplets.clear();
}

WordDoubleStat&
StatCalc::df(void)
{
    return m_df;
}

WordDoubleStat&
StatCalc::cf(void)
{
    return m_cf;
}

IDCategory&
StatCalc::idCategory()
{
    return this->m_idCategory;
}

bool
StatCalc::saveTDMatrix(const string& i_filename, bool withDocNames)
{
    ofstream oStream(i_filename,ios::binary|ios::out);
    if ( !m_tdmatrix.cols() )
    {
        cout<<"Nothing to store!"<<endl;
        return false;
    }

    if (!oStream.is_open())
    {
        cout<<"Can\'t open file for writing\n.Filename:"<<i_filename<<endl;
        return false;
    }
    saveTDMatrix_(oStream);
    saveIDCategory_(oStream);
    saveVocabulary_(oStream);
    if (withDocNames)
    {
        saveDocID_(oStream);
    }
    oStream.close();
    return true;
}

bool
StatCalc::loadTDMatrix(const string& i_filename, bool withDocNames)
{
    clean();
    ifstream iStream(i_filename,ios::in|ios::binary);
    if (!iStream.is_open())
    {
        cout<<"Can\'t open file for reading\n.Filename:"<<i_filename<<endl;
        return false;
    }
    loadTDMatrix_(iStream);
    loadIDCategory_(iStream);
    loadVocabulary_(iStream);
    if (withDocNames)
    {
        loadDocID_(iStream);
    }
    iStream.close();
    return true;
}

void
StatCalc::saveTDMatrix_(ofstream& i_oStream)
{
    eigentools::saveSparseMatrixBinary(m_tdmatrix,i_oStream);
}

void
StatCalc::saveDocID_(ofstream& i_oStream)
{
    int size = (int)m_docID.size();
    i_oStream.write((char*)& size,4);
    for (auto it = m_docID.begin(); it != m_docID.end(); it++)
    {
        Tools::WriteString_Binary(i_oStream,it->first);
        i_oStream.write((char*)& it->second,4);
    }
}

void
StatCalc::saveIDCategory_(ofstream& i_oStream)
{
    int size = (int)m_idCategory.size();
    i_oStream.write((char*)& size,4);
    for (auto it = m_idCategory.begin(); it != m_idCategory.end(); it++)
    {
        i_oStream.write((char*)& it->first,4);
        it->second.saveBinary(i_oStream);
//        i_oStream.write((char*)& it->second,4);
    }
}

void
StatCalc::loadTDMatrix_(ifstream& i_iStream)
{
    eigentools::loadSparseMatrixBinary(m_tdmatrix,i_iStream);
}

void
StatCalc::loadDocID_(ifstream &i_iStream)
{
    int size;
    i_iStream.read((char*)& size, 4);
    for (int i = 0; i < size; i++)
    {
        string word;
        Tools::ReadString(i_iStream,word);
        unsigned int id;
        i_iStream.read((char*)& id,4);
        m_docID[word] = id;
    }
}

void
StatCalc::saveVocabulary_(ofstream& i_oStream)
{
    Tools::save_map_binary(i_oStream,m_vocabulary);
}

void
StatCalc::loadVocabulary_(ifstream &i_iStream)
{
    Tools::load_map_binary(i_iStream,m_vocabulary);
}

void
StatCalc::loadIDCategory_(ifstream &i_iStream)
{
    int size;
    i_iStream.read((char*)& size, 4);
    for (int i = 0; i < size; i++)
    {
        unsigned int id;
        i_iStream.read((char*)& id,4);
        Category domain;
        domain.loadBinary(i_iStream);
//        i_iStream.read((char*)& domain, 4);
        m_idCategory[id] = domain;
    }
}

void
StatCalc::loadFileList(const string &i_filelist, const string& prefix)
{
    Tools::DocumentPlain filelist(i_filelist);
    unsigned int id = 0;
    string str;
    for ( size_t i = 0; i < filelist.content().size();i++ )
    {
		if (Tools::ConvertWstringToString(filelist.content()[i], str))
		{
            str = prefix + str;
            if (m_docID.find(str) == m_docID.end())
                m_docID[str] = id++;
		}
	}
}

void
StatCalc::calculateTDMatrix(shared_ptr<Tools::DocumentCreator> i_docCreator)
{
    if (m_docID.empty())
    {
        wcout << L"Error in loaded data. Use loadFilelist() function please"<<endl;
        return;
    }
    unordered_map<int,int> docBOW;
    unsigned int count = 0;
    for (DocID::iterator itr = m_docID.begin();
            itr != m_docID.end();
            itr++)
    {
        shared_ptr<Tools::Document> doc = i_docCreator->loadDocument(itr->first);
//        m_vecTriplets.reserve(100000000);
        m_idCategory.reserve(m_docID.size());
        Category cat;
        cat.label() = cat.fromWstring(doc->category());
        m_idCategory[itr->second] = cat;

        wcout << count << L". Process doc with ID = " << itr->second;
        wcout << L". Domain "+doc->category()<<endl;
        count += 1;

        bool success = m_docProcessor->processDocument(doc,*this);
        if (success)
        {
            unordered_map<int,int>& docBOW = m_docProcessor->docBOW();
            for (auto it = docBOW.begin();it != docBOW.end();it++)
            {
                m_vecTriplets.emplace_back(it->first,itr->second,it->second);
            }
        }
        else
        {
            wcout<< L"Error in processing plain file. id = " << itr->second << endl;
        }
    }
    m_tdmatrix.derived().resize(m_vocabulary.size(),m_docID.size());
    m_tdmatrix.setFromTriplets(m_vecTriplets.begin(),m_vecTriplets.end());
    m_vecTriplets.clear();
}

void
StatCalc::calculateTDMatrix(vector<shared_ptr<Tools::Document> >& docVec)
{
	if (docVec.empty())
	{
		wcout << L"Set of input documents is empty" << endl;
		return;
	}
	unordered_map<int, int> docBOW;
	unsigned int count = 0;
	for (auto& x : docVec)
	{
		wcout << L"Processing document #" << count << std::endl;

		bool success = m_docProcessor->processDocument(x, *this);
		if (success)
		{
			unordered_map<int, int>& docBOW = m_docProcessor->docBOW();
			for (auto it = docBOW.begin(); it != docBOW.end(); it++)
			{
				m_vecTriplets.emplace_back(it->first, count, it->second);
			}
			m_docID[std::to_string(count)] = count;
			count ++;
		}
		else
		{
			wcout << L"Error in processing document #" << count << endl;
		}
	}
	m_tdmatrix.derived().resize(m_vocabulary.size(), m_docID.size());
	m_tdmatrix.setFromTriplets(m_vecTriplets.begin(), m_vecTriplets.end());
	m_vecTriplets.clear();
}


WordDoubleStat&
StatCalc::calculatedf(void)
{
    m_df.setZero(this->tdmatrix().rows(),1);
    if (!m_tdmatrix.cols())
    {
        wcout << L"Data is not loaded"<< endl;
        return m_df;
    }
    //calcullation of document frequency
    for (int k=0; k<m_tdmatrix.outerSize(); ++k)
    {
        for (SparseMatrix<double>::InnerIterator it(m_tdmatrix,k); it; ++it)
        {
            m_df(it.row()) += 1;
        }
    }

    return m_df;
}

WordDoubleStat&
StatCalc::calculateidf(void)
{
    m_idf.setZero(m_tdmatrix.rows(),1);
    if (!m_tdmatrix.cols())
    {
        wcout << L"Data is not loaded"<< endl;
        return m_idf;
    }
    if (!m_df.sum())
    {
        calculatedf();
    }
    double N = (double)m_idCategory.size();
    m_idf = (m_df.array().pow(-1) * N).log();
    return m_idf;
}

WordDoubleStat&
StatCalc::calculatecf(Category i_domain)
{
//    if (m_docCollection.empty() && m_tdmatrix.empty())
//    {
//        wcout << L"Data is not loaded"<< endl;
//        return m_cf;
//    }
//    if (m_tdmatrix.empty() && !m_docCollection.empty())
//    {
//        this->calculateTDMatrix();
//    }
//    WordIntStat cf;
//    for (auto itr = m_tdmatrix.begin(); itr != m_tdmatrix.end(); itr++)
//    {
//        vector<pair<unsigned int, unsigned int> > v_pair = itr->second;
//        unsigned int word_cf = 0;
//        for (size_t i = 0; i < v_pair.size(); i++)
//        {
//            if (m_idCategory[v_pair[i].first]==i_domain)
//            {
//                word_cf += v_pair[i].second;
//            }
//        }
//        if (word_cf)
//        {
//            cf[itr->first] = word_cf;
//        }
//    }
    return m_cf;
}

WordDoubleStat&
StatCalc::calculatecf(void)
{
    m_cf.setZero(m_tdmatrix.rows(),1);
    if (!m_tdmatrix.cols())
    {
        wcout << L"Data is not loaded"<< endl;
        return m_cf;
    }
    //    m_df.reserve(m_tdmatrix.size());
    m_cf = m_tdmatrix * eigentools::DenseMat::Ones(m_tdmatrix.cols(),1);

    return m_cf;
}

bool StatCalc::loadOldTDMatrix(const string& i_filename, bool withDocNames)
{
    clean();
    ifstream iStream(i_filename,ios::in|ios::binary);
    if (!iStream.is_open())
    {
        cout<<"Can\'t open file for reading\n.Filename:"<<i_filename<<endl;
        return false;
    }
    loadOldTDMatrix_(iStream);
    loadIDCategory_(iStream);
    if (withDocNames)
    {
        loadDocID_(iStream);
    }
    iStream.close();
    return true;
}

void StatCalc::loadOldTDMatrix_(ifstream &i_iStream)
{
#ifdef MSVC
    locale loc = std::locale();
#else
    locale loc = std::locale("ru_RU.UTF-8");
#endif
    int size,vsize,value,col,docNum = 0;
    i_iStream.read((char*)& size, 4);
    m_vocabulary.reserve(size);
    for (int i = 0; i < size; i++)
    {
        wstring word;
        Tools::ReadWString(i_iStream,word,loc);
        m_vocabulary[word] = i;
        i_iStream.read((char*)& vsize,4);
        for (int j = 0; j < vsize; j++)
        {
            i_iStream.read((char*)& col,4);
            i_iStream.read((char*)& value,4);
            m_vecTriplets.push_back(Eigen::Triplet<int>(i,col,value));
            docNum = (docNum>col)? docNum : col;
        }
    }
    m_tdmatrix.resize(size,docNum+1);
    m_tdmatrix.setFromTriplets(m_vecTriplets.begin(),m_vecTriplets.end());
}

vector<WordDoubleStat>&
StatCalc::calculateMI(void)
{
//    m_MI.clear();
//    if (m_docCollection.empty() && m_tdmatrix.empty())
//    {
//        wcout << L"Data is not loaded"<< endl;
//        return m_MI;
//    }
//    if (m_tdmatrix.empty() && !m_docCollection.empty())
//    {
//        this->calculateTDMatrix();
//    }
//    double N = (double)m_idCategory.size();
//    vector<double> NDocDomain((size_t)COUNT_DOMAIN_TYPES,0.0);
//    for (auto itr = m_idCategory.begin(); itr != m_idCategory.end(); itr++)
//    {
//        NDocDomain[(int) itr->second] += 1;
//    }
//    for (unsigned int domain = 0; domain < (unsigned int)COUNT_DOMAIN_TYPES; domain++)
//    {
//        double N00,N01,N10,N11,N_0,N_1,N0_,N1_, _N11, _N10, _N01, _N00;
//        WordDoubleStat mi_dom;

//        N_1 = NDocDomain[domain];
//        N_0 = N - N_1;
//        for (auto itr = m_tdmatrix.begin(); itr != m_tdmatrix.end(); itr++)
//        {
//            N1_ = (itr->second).size();
//            N0_ = N - N1_;
//            N11 = 0;
//            for (size_t i = 0; i < itr->second.size(); i++)
//            {
//                unsigned int docDom = itr->second[i].first;
//                if (m_idCategory[docDom]==domain)
//                {
//                    N11 += 1;
//                }
//            }
//            N10 = N1_ - N11;
//            N01 = N_1 - N11;
//            N00 = N_0 - N10;
//            _N11 = (N11==0.0)? 0.0000001 : N11;
//            _N01 = (N01==0.0)? 0.0000001 : N01;
//            _N10 = (N10==0.0)? 0.0000001 : N10;
//            _N00 = (N00==0.0)? 0.0000001 : N00;

//            mi_dom[itr->first] =
//                    (N11/N)*LOG2(_N11*N/(N1_*N_1)) +
//                    (N01/N)*LOG2(_N01*N/(N0_*N_1)) +
//                    (N10/N)*LOG2(_N10*N/(N1_*N_0)) +
//                    (N00/N)*LOG2(_N00*N/(N0_*N_0)) ;
//        }
//        m_MI.push_back(mi_dom);
//    }
    return m_MI;
}

double statcalc::LOG2( double n )
{  
    return log( n ) / log( 2.0 );  
}

vector<WordDoubleStat>&
StatCalc::calculateCentroids(void)
{
//    m_centroids.clear();
//    if (m_docCollection.empty() && m_tdmatrix.empty() )
//    {
//        wcout << L"Data is not loaded"<< endl;
//        return m_centroids;
//    }
//    if (m_tdmatrix.empty() && !m_docCollection.empty())
//    {
//        this->calculateTDMatrix();
//    }

//    calculateidf();

//    vector<double> NDocDomain(COUNT_DOMAIN_TYPES,0.0);
//    unordered_map<unsigned int, WordDoubleStat> wf_idf;

//    for (auto itr = m_docCollection.begin(); itr != m_docCollection.end(); itr++)
//    {
//        WordDoubleStat doc_wf_idf;
//        WordIntStat doc_tf = itr->second;

//        Category domain = m_idCategory[itr->first];
//        NDocDomain[(int)domain] += 1.0;

//        for (auto it = doc_tf.begin(); it != doc_tf.end(); it++)
//        {
//            doc_wf_idf[it->first] = (1.0 + log((double)(it->second)))*m_idf[it->first];
//        }
//        wf_idf[itr->first] = doc_wf_idf;
//    }

//    WordDoubleStat empty_map;
//    //empty_map.reserve(m_idf.size());
//    for (auto itr = m_idf.begin(); itr != m_idf.end(); itr++)
//        empty_map[itr->first] = 0.0;
//    m_centroids = vector<WordDoubleStat>(COUNT_DOMAIN_TYPES, empty_map);

//    for (auto itr = wf_idf.begin(); itr != wf_idf.end(); itr++)
//    {
//        Category domain = m_idCategory[itr->first];
//        WordDoubleStat doc_wf_idf = itr->second;
//        for (auto it = doc_wf_idf.begin(); it != doc_wf_idf.end(); it++)
//        {
//            m_centroids[(int)domain][it->first] += it->second / NDocDomain[domain];
//        }
//    }

    return m_centroids;
}
