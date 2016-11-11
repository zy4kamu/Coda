#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "DictionaryInterface.h"
#include "Disambiguator.h"
#include "DisambiguatorCreator.h"
#include "ITokenizer.h"
#include "lemmatizer.h"
#include "stat-calc.h"
#include "stat-calc-tools.h"
#include "lemmatizer-adapter.h"

using namespace std;
using namespace statcalc;
using namespace Tokenization;
using namespace Disambiguation;

//void OCRcollection()
//{
//    shared_ptr<Tools::DocumentPlainCreator> cr(new Tools::DocumentPlainCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::EN);
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english"));
//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
////    stc.categoryWstringToEnum = statcalc::wikicategoryfullWstringToEnum;

//    stc.loadFileList(string(SERVER_PATH)+"OCR_text_collection/filelist_ocr.txt");
//    wcout << L"Filelist loaded"<<endl;
//    stc.calculateDocCollection(cr);
//    wcout << L"Collection converted to BOW" << endl;
//    stc.saveDocCollection(string(SERVER_PATH)+"OCR_text_collection/Stat/ocr/docCollection1.bin");
//    stc.calculateTDMatrix();
//    wcout<<L"Calculation of TDMatrix is done"<<endl;
//    stc.saveTDMatrix(string(SERVER_PATH)+"OCR_text_collection/Stat/ocr/tdmatrix1.bin",true);
//    WordDoubleStat idf = stc.calculateidf();
//    string filename = string(SERVER_PATH)+"OCR_text_collection/Stat/ocr/idf1.txt";
//    wofstream oS (filename);
//    Tools::SetLocale(oS);
//    statcalc::print_map(oS,idf);
//    oS.close();

//    WordIntStat df = stc.calculatedf();
//    filename = string(SERVER_PATH)+"OCR_text_collection/Stat/ocr/df1.txt";
//    wofstream oS1 (filename);
//    Tools::SetLocale(oS1);
//    print_map(oS1,df);
//    oS1.close();

//    WordIntStat cf = stc.calculatecf();
//    filename = string(SERVER_PATH)+"OCR_text_collection/Stat/ocr/cf1.txt";
//    wofstream oS2 (filename);
//    Tools::SetLocale(oS2);
//    print_map(oS2,cf);
//    oS2.close();
//}

//void WikiCollectionMajor()
//{
//    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::EN);
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english"));
//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
//    stc.loadFileList(string(SERVER_PATH)+"WikipediaEng/Stat/Major/filelist.txt");
//    wcout << L"Filelist loaded"<<endl;
//    stc.calculateDocCollection(cr);
////    wcout << L"Collection converted to BOW" << endl;
//    stc.saveDocCollection(string(SERVER_PATH)+"WikipediaEng/Stat/Major/docCollection.bin");
////    stc.loadDocCollection(string(SERVER_PATH)+"WikipediaEng/Stat/Major/docCollection.bin");
//    stc.calculateTDMatrix();
//    wcout<<L"Calculation of TDMatrix is done"<<endl;
//    stc.saveTDMatrix(string(SERVER_PATH)+"WikipediaEng/Stat/Major/tdmatrix.bin",true);
//    WordDoubleStat idf = stc.calculateidf();
//    string filename = string(SERVER_PATH)+"WikipediaEng/Stat/Major/idf.txt";
//    wofstream oS (filename);
//    Tools::SetLocale(oS);
//    statcalc::print_map(oS,idf);
//    oS.close();

//    WordIntStat df = stc.calculatedf();
//    filename = string(SERVER_PATH)+"WikipediaEng/Stat/Major/df.txt";
//    wofstream oS1 (filename);
//    Tools::SetLocale(oS1);
//    print_map(oS1,df);
//    oS1.close();

//    WordIntStat cf = stc.calculatecf();
//    filename = string(SERVER_PATH)+"WikipediaEng/Stat/Major/cf.txt";
//    wofstream oS2 (filename);
//    Tools::SetLocale(oS2);
//    print_map(oS2,cf);
//    oS2.close();
//}

//void WikiCollectionLong()
//{
//    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::EN);
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english"));
//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
////    stc.categoryWstringToEnum = statcalc::wikicategoryfullWstringToEnum;

//    stc.loadFileList(string(SERVER_PATH)+"WikipediaEng/Stat/Long/filelist.txt");
//    wcout << L"Filelist loaded"<<endl;
//    stc.calculateDocCollection(cr);
////    wcout << L"Collection converted to BOW" << endl;
//    stc.saveDocCollection(string(SERVER_PATH)+"WikipediaEng/Stat/Long/docCollection.bin");
////    stc.loadDocCollection(string(SERVER_PATH)+"WikipediaEng/Stat/Long/docCollection.bin");
//    stc.calculateTDMatrix();
//    wcout<<L"Calculation of TDMatrix is done"<<endl;
//    stc.saveTDMatrix(string(SERVER_PATH)+"WikipediaEng/Stat/Long/tdmatrix.bin",true);
//    WordDoubleStat idf = stc.calculateidf();
//    string filename = string(SERVER_PATH)+"WikipediaEng/Stat/Long/idf.txt";
//    wofstream oS (filename);
//    Tools::SetLocale(oS);
//    statcalc::print_map(oS,idf);
//    oS.close();

//    WordIntStat df = stc.calculatedf();
//    filename = string(SERVER_PATH)+"WikipediaEng/Stat/Long/df.txt";
//    wofstream oS1 (filename);
//    Tools::SetLocale(oS1);
//    print_map(oS1,df);
//    oS1.close();

//    WordIntStat cf = stc.calculatecf();
//    filename = string(SERVER_PATH)+"WikipediaEng/Stat/Long/cf.txt";
//    wofstream oS2 (filename);
//    Tools::SetLocale(oS2);
//    print_map(oS2,cf);
//    oS2.close();
//}

//void Lenta032015()
//{
//    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::RU);

//    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
//    shared_ptr<Lemmatizer> lemm(new Lemmatizer(disambiguator));

//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
////    stc.categoryWstringToEnum = statcalc::wikicategoryfullWstringToEnum;
//    stc.loadFileList(string(SERVER_PATH)+"Lenta/filelist0303.txt");
//    wcout << L"Filelist loaded"<<endl;
//    stc.calculateDocCollection(cr);
//    wcout << L"Collection converted to BOW" << endl;
//    stc.saveDocCollection(string(SERVER_PATH)+"Lenta/Stat/032015/docCollection.bin");
//    stc.calculateTDMatrix();
//    wcout<<L"Calculation of TDMatrix is done"<<endl;
//    stc.saveTDMatrix(string(SERVER_PATH)+"Lenta/Stat/032015/tdmatrix.bin",true);
//    WordDoubleStat idf = stc.calculateidf();
//    string filename = string(SERVER_PATH)+"Lenta/Stat/032015/idf.txt";
//    wofstream oS (filename);
//    Tools::SetLocale(oS);
//    statcalc::print_map(oS,idf);
//    oS.close();

//    WordIntStat df = stc.calculatedf();
//    filename = string(SERVER_PATH)+"Lenta/Stat/032015/df.txt";
//    wofstream oS1 (filename);
//    Tools::SetLocale(oS1);
//    print_map(oS1,df);
//    oS1.close();

//    WordIntStat cf = stc.calculatecf();
//    filename = string(SERVER_PATH)+"Lenta/Stat/032015/cf.txt";
//    wofstream oS2 (filename);
//    Tools::SetLocale(oS2);
//    print_map(oS2,cf);
//    oS2.close();
//}

//void testPatent()
//{
//    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::RU);
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english"));
//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
////    stc.categoryWstringToEnum = statcalc::wikicategoryfullWstringToEnum;

//    stc.loadFileList(string(SERVER_PATH)+"Patents/Stat/filelist.txt");
//    wcout << L"Filelist loaded"<<endl;
//    stc.calculateDocCollection(cr);
//    wcout << L"Collection converted to BOW" << endl;
//    stc.saveDocCollection(string(SERVER_PATH)+"Patents/Stat/docCollection.bin");
//    wcout << L"DocCollection is saved"<<endl;
////    stc.loadDocCollection(string(SERVER_PATH)+"Patents/Stat/docCollection.bin");
//    stc.calculateTDMatrix();
//    wcout<<L"Calculation of TDMatrix is done"<<endl;
//    stc.saveTDMatrix(string(SERVER_PATH)+"Patents/Stat/tdmatrix.bin",true);
//    WordDoubleStat idf = stc.calculateidf();
//    string filename = string(SERVER_PATH)+"Patents/Stat/idf.txt";
//    wofstream oS (filename);
//    Tools::SetLocale(oS);
//    statcalc::print_map(oS,idf);
//    oS.close();

//    WordIntStat df = stc.calculatedf();
//    filename = string(SERVER_PATH)+"Patents/Stat/df.txt";
//    wofstream oS1 (filename);
//    Tools::SetLocale(oS1);
//    print_map(oS1,df);
//    oS1.close();

//    WordIntStat cf = stc.calculatecf();
//    filename = string(SERVER_PATH)+"Patents/Stat/cf.txt";
//    wofstream oS2 (filename);
//    Tools::SetLocale(oS2);
//    print_map(oS2,cf);
//    oS2.close();
//}

//void test()
//{
//    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

//    shared_ptr<ITokenizer> ti = ITokenizer::GetTokenizer(Tools::Language::RU);
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english"));
//    shared_ptr<_sentence_splitter::SentenceSplitter> splitter(
//                new _sentence_splitter::SentenceSplitter(SSPLITTER_CONFIG_ENGLISH));
//    StatCalc stc(ti,lemm,splitter);
////    stc.categoryWstringToEnum = statcalc::wikicategoryfullWstringToEnum;
//    stc.loadTDMatrix(string(SERVER_PATH)+"WikipediaEng/Stat/Long/tdmatrix.bin",true);
//    IDCategory idcat = stc.idCategory();
//    SparseMat a,b;
//    vector<wstring> vw;
//    vector<int> vd;
//    statcalc::tdmatrixToEigenSparseMatrixWithClasses(
//                stc.tdmatrix(),
//                idcat,
//                int(statcalc::WCF_COUNT)+1,
//                a,
//                b,
//                vw,
//                vd
//                );
////    statcalc::tdmatrixToEigenSparseMatrix(stc.tdmatrix(),b,vw,vd);
//    eigentools::saveSparseMatrix(b,string(SERVER_PATH)+"WikipediaEng/Stat/Long/catDoc.mat");
//}

void exec()
{
    shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);
    string path = "/home/aromanenko/workspace/ethnical/filelist.txt";
    shared_ptr<ITokenizer> ti;
//    shared_ptr<ILemmatizer> lemm;
    shared_ptr<_sentence_splitter::ISentenceSplitter> splitter = _sentence_splitter::ISentenceSplitter::GetSentenceSplitter(Tools::Language::RU);
    ti = ITokenizer::GetTokenizer(Tools::Language::RU);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
    shared_ptr<ILemmatizer> lemm(new Lemmatizer(disambiguator));
//    shared_ptr<ILemmatizer> lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english",true));
    StatCalc stc(ti,lemm,splitter);
//    stc.loadFileList("/home/aromanenko/workspace/Data/filelist_test.txt");
    wcout << L"Filelist loaded"<<endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    stc.loadTDMatrix(string(SERVER_PATH)+"Data/Stat/tdmatrix.bin",true);
    Tools::DocumentExtended stopWordDoc(string(SERVER_PATH)+"Data/Stat/stopWords.txt");
    statcalc::removeWords(stc.tdmatrix(),stc.vocabulary(),stopWordDoc.content());
    auto voc = stc.vocabulary();
    auto mat = statcalc::trimNotFrequentWords(stc.tdmatrix(),voc,10,10);

    gettimeofday(&end, NULL);

    statcalc::saveCollectionUCI(mat,voc,string(SERVER_PATH)+"Data/Stat/","20150801_trimmed10");

//    auto dp = stc.docProcessor();

//    auto doc = cr->loadDocument("/home/aromanenko/workspace/Data/text.txt");
//    dp->processDocument(doc,stc);
//        auto& voc = stc.vocabulary();
//    auto& docBOW = dp->docBOW();
//    for (auto& x : voc)
//        wcout<<endl<<x.first<<L" "<<docBOW[x.second];
}

void universal(int argc, char* argv[])
{
    if (argc < 2 )
    {
    //    dataExtraction();
    //    kinopoiskCF();
    //    init();
//        OCRcollection();
        exec();
    //    WikiCollectionLong();
//        Lenta032015();
    //    Independent();
//        test();
    }
    else if (argc == 4)
    {
        string lang = string(argv[1]);
        string filelist = string(argv[2]);
        string path = string(argv[3]);
        cout<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<std::endl;
        shared_ptr<Tools::DocumentExtendedCreator> cr(new Tools::DocumentExtendedCreator);

        shared_ptr<ITokenizer> ti;
        shared_ptr<ILemmatizer> lemm;
        shared_ptr<_sentence_splitter::ISentenceSplitter> splitter;
        if (lang == "en")
        {
            ti = ITokenizer::GetTokenizer(Tools::Language::EN);
//            lemm = shared_ptr<ILemmatizer>(new LemmatizerStemmerAdapter<Token>("english",false));
            shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::EN_FAST);
            lemm = shared_ptr<Lemmatizer>(new Lemmatizer(disambiguator));
            splitter = _sentence_splitter::ISentenceSplitter::GetSentenceSplitter(Tools::Language::EN);
        }
        else if (lang == "ru" )
        {
            ti = ITokenizer::GetTokenizer(Tools::Language::RU);
            shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(Tools::Language::RU);
            lemm = shared_ptr<Lemmatizer>(new Lemmatizer(disambiguator));
            splitter = _sentence_splitter::ISentenceSplitter::GetSentenceSplitter(Tools::Language::EN);
        }
        StatCalc stc(ti,lemm,splitter);
        stc.loadFileList(filelist);
        wcout << L"Filelist loaded"<<endl;
        struct timeval start, end;
        gettimeofday(&start, NULL);

        stc.calculateTDMatrix(cr);

        gettimeofday(&end, NULL);
        double delta = ((end.tv_sec  - start.tv_sec) * 1000000u +
                        end.tv_usec - start.tv_usec) / 1.e6;
        wcout<<delta<<endl;
        vector<wstring> words = vocabularyToVector(stc.vocabulary());
        wcout<<L"Calculation of TDMatrix is done"<<endl;
        stc.saveTDMatrix(path+string("tdmatrix.bin"),true);

        WordDoubleStat idf = stc.calculateidf();
        string filename = path + string("idf.txt");
        printWordStat(filename,idf,words,true);

        WordDoubleStat df = stc.calculatedf();
        filename = path+string("df.txt");
        printWordStat(filename,df,words,true);

        WordDoubleStat cf = stc.calculatecf();
        filename = path+string("cf.txt");
        printWordStat(filename,cf,words,true);
    }
    else
    {
        cout<<"input language(en,ru)\npath to filelist\npath to folder for saving\n"<<endl;
    }
}

int main(int argc, char* argv[])
{
    Tools::PrepareConsole();
    universal(argc,argv);
//    testPatent();
    return 0;
}

//void kinopoiskCF()
//{
//    shared_ptr<ITokenizer> ti = ITokenizer::CreateTokenizer(Tools::Language::RU);
//    shared_ptr<Disambiguation::IDisambiguatorCreator> creator =
//        Disambiguation::IDisambiguatorCreator::GetDisambiguatorCreator(Tools::Language::RU);
//    shared_ptr<IDisambiguator> disambiguator = creator->GetDisambiguator();
//    shared_ptr<Lemmatizer> lemm(new Lemmatizer(disambiguator));

//    StatCalc stc(ti,lemm);
//    stc.loadTDMatrix("/home/aromanenko/projects/Ru_News/Stat/Bin/tdmatrix.bin");
//    WordIntStat cf = stc.calculatecf(CONTENTS_PVOD);
//    string file = "/home/aromanenko/projects/Ru_News/Stat/Bin/cf_PVOD.bin";
//    save_map_binary(file,cf);
//    file = "/home/aromanenko/projects/Ru_News/Stat/cf_PVOD.txt";
//    wofstream oStream(file);
//    Tools::SetLocale(oStream);
//    print_map(oStream,cf);
//    oStream.close();
//}
