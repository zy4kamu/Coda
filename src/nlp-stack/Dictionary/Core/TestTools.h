/**
 * TestTools.h
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


//#pragma warning( disable: 4018 )
#ifndef _TESTTOOLS_H_
#define _TESTTOOLS_H_

#include "DictionaryTrieBinaryFile.h"
#include "DictionaryTools.h"
//#include "TestNGram.h"
#include "Dictionary.h"
#include "DictionaryTrieIterator.h"
#include "MorphologicalDictionary.h"

#define RUSCORPORA_PATH "E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\Corpora\\ruscorpora_1M\\2013_06_11_text\\"
#define RUSCORPORA_LOG "RusCorporaLog.txt"

using namespace std;

Dictionary* dic;
DictionaryInterface* dicInterface;
DictionaryTools* tools;
shared_ptr<MorphologicalDictionary> morphological_dictionary;

set<wstring> _stat_words, _sentencesWithNonDicWords;
map<wstring, wstring> _stat_nonDictionaryWords;

clock_t start, finish;

struct foo
{
    int id;
};

void printGrammInfo(GrammInfo ginfo)
{
    wcout << ginfo.id << " - " << ginfo.initial_form << " - ";
    for (int j = 0; j < (int) ginfo.features.size(); ++j)
    {
        wcout << ginfo.features.at(j);
        if (j < (int) ginfo.features.size() - 1)
        {
            wcout << " - ";
        }
    }
    wcout << "------------------------------------" << endl << endl;
}

void printGrammInfos(shared_ptr<vector<GrammInfo> > ginfos)
{
    for (vector<GrammInfo>::iterator iter = ginfos->begin();
         iter != ginfos->end(); ++iter)
    {
        printGrammInfo(*iter);
    }
}
void printMorphologicalInfo(MorphologicalInfo minfo)
{
    //wcout << minfo.lemmaId << " - " << minfo.initial_form << " - " << minfo.featureListId << endl;
    wcout << minfo.lemmaId << " - " << minfo.initial_form << " - " << minfo.basicFeatureListId << " - " << minfo.featureListId << " - " << minfo.additionalInfo << " - " << minfo.frequency << endl;
    //wcout << "Feature frequency = " << dic->getSuffixModelTrie()->getFeatureFrequency(minfo.basicFeatureListId, minfo.featureListId) << endl;
    shared_ptr<GrammInfo> ginfo = std::make_shared<GrammInfo>();
    dic->getDictionaryTrie()->convertMorphologicalInfoToGrammInfo(&minfo, ginfo);
    for (vector<wstring>::iterator iter = ginfo->features.begin(); iter != ginfo->features.end(); ++iter)
    {
        wcout << *iter;
        if (iter != ginfo->features.end() - 1)
        {
            wcout << " - ";
        }
    }
    wcout << endl;
}

/**
 * 2014-03-05
 * print MorphologicalInfo in detail
 */
void printMorphologicalInfoInDetail(MorphologicalInfo * minfo)
{
    map<int, vector<int> > featureListMap = dicInterface->getDictionaryTrie()->getFeatureListMap();
//    wcout << "featureListMap.size() = " << featureListMap.size() << endl;
//    for (map<int, vector<int> >::iterator iter = featureListMap.begin(); iter != featureListMap.end(); ++iter)
//    {
//        wcout << "i = " << iter->first << ": ";
//        vector<int> vi = iter->second;
//        for (vector<int>::iterator jter = vi.begin(); jter != vi.end(); ++jter)
//        {
//            wcout << *jter << "; ";
//        }
//        wcout << endl;
//    }
    map<int, wstring> * featureMap = dicInterface->getDictionaryTrie()->getFeatureMap();
    map<int, wstring> * idFeatureMap = dicInterface->getDictionaryTrie()->getIdFeatureMap();

    wcout << "###### " << minfo->lemmaId << " - " << minfo->initial_form << " - " << minfo->basicFeatureListId << " - " << minfo->featureListId << " - " << minfo->additionalInfo << " - " << minfo->frequency << endl;
    int featureListId = minfo->basicFeatureListId;
    if (featureListId != -1 && featureListMap.count(featureListId) > 0)
    {
        vector<int> _featuresList = featureListMap.at(featureListId);
        for (int i = 0; i < (int) _featuresList.size(); ++i)
        {
            int _featureId = _featuresList.at(i);
            if (featureMap->count(_featureId) > 0)
            {
                wcout << _featureId << L" - " << featureMap->at(_featureId);
            }
            if (idFeatureMap->count(_featureId) > 0)
            {
                wcout << L" - " << idFeatureMap->at(_featureId) << endl;
            }
            else
            {
                wcout << endl;
            }
        }
    }

    featureListId = minfo->featureListId;
    if (featureListId != -1 && featureListMap.count(featureListId) > 0)
    {
        vector<int> _featuresList = featureListMap.at(featureListId);
        for (int i = 0; i < (int) _featuresList.size(); ++i)
        {
            int _featureId = _featuresList.at(i);
            if (featureMap->count(_featureId) > 0)
            {
                wcout << _featureId << L" - " << featureMap->at(_featureId);
            }
            if (idFeatureMap->count(_featureId) > 0)
            {
                wcout << L" - " << idFeatureMap->at(_featureId) << endl;
            }
            else
            {
                wcout << endl;
            }
        }
    }
}

void printMorphologicalInfoList(shared_ptr<vector<MorphologicalInfo> > _infoList)
{
    for (vector<MorphologicalInfo>::iterator iter = _infoList->begin(); iter != _infoList->end(); ++iter)
    {
        printMorphologicalInfoInDetail(&(*iter));
        wcout << "-----------------------------------------------" << endl;;
    }
}

void testSomeWord(const wstring & _word)
{
    wcout << "============ " << _word << " ==================" << endl;
    shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
    minfos->clear();
    dicInterface->getMorphologicalInfo(_word, minfos, 5);
    wcout << "minfos->size() = " << minfos->size() << endl;
    printMorphologicalInfoList(minfos);
    wcout << endl;
}

void testDictionary(void)
{
    wcout << "###### testDictionary ######" << endl << endl;
    dicInterface = DictionaryInterface::CreateDictionary();
    //wcout << "Number of nodes = " << dic->getSuffixModelTrie()->getNumberOfNode() << endl;
    /*
    dic->setIgnoreE(true);
    dic->setHyphenSplit(true);
    */
    dicInterface->setMainWordFormFromLinkedLemmas(true);

//    testSomeWord(L"\u041c\u0430\u0448\u0430");//here was cyrrilic symbols: Маша
//    testSomeWord(L"\u043c\u0438\u0440\u0430");//here was cyrrilic symbols: мира
//    testSomeWord(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
//    testSomeWord(L"\u043a\u043d\u0438\u0433\u0438");//here was cyrrilic symbols: книги
//    testSomeWord(L"\u044f\u0431\u043b\u043e\u043a\u043e");//here was cyrrilic symbols: яблоко
//    testSomeWord(L"\u043c\u0430\u0448\u0438\u043d\u044b");//here was cyrrilic symbols: машины
//    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a");//here was cyrrilic symbols: учебник
//    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u043e\u0432");//here was cyrrilic symbols: учебников
//    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u0430\u0445");//here was cyrrilic symbols: учебниках
//    testSomeWord(L"\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: быстрее
//    testSomeWord(L"\u043f\u0440\u0435\u043a\u0440\u0430\u0441\u043d\u043e");//here was cyrrilic symbols: прекрасно
//    testSomeWord(L"\u043a\u0440\u0430\u0441\u0438\u0432\u0430\u044f");//here was cyrrilic symbols: красивая
//    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0439\u0441\u044f");//here was cyrrilic symbols: учащийся
//    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: учащихся
//    testSomeWord(L"\u0443\u0447\u0438\u043b\u0441\u044f");//here was cyrrilic symbols: учился
//    testSomeWord(L"\u0433\u043e\u0442\u043e\u0432");//here was cyrrilic symbols: готов
//    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: побыстрее
//    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0439");//here was cyrrilic symbols: побыстрей
//    testSomeWord(L"\u043f\u043e\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: потому
//    testSomeWord(L"\u043f\u043e\u044d\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: поэтому
//    testSomeWord(L"\u043f\u043e\u044d\u0442");//here was cyrrilic symbols: поэт
//    testSomeWord(L"\u043f\u043e\u044d\u0442\u0430");//here was cyrrilic symbols: поэта
//    testSomeWord(L"\u044f");//here was cyrrilic symbols: я
//    testSomeWord(L"\u043c\u0435\u043d\u044f");//here was cyrrilic symbols: меня
//    testSomeWord(L"\u0440\u043e\u0441\u0441\u0438\u0438");//here was cyrrilic symbols: россии
//    testSomeWord(L"\u0435\u0432\u0440\u043e\u043f\u044b");//here was cyrrilic symbols: европы
//    testSomeWord(L"\u0431\u0443\u0434\u0435\u0442");//here was cyrrilic symbols: будет
//    testSomeWord(L"\u0445\u043e\u0436\u0443");//here was cyrrilic symbols: хожу
//    testSomeWord(L"\u0445\u043e\u0434\u0438\u0442");//here was cyrrilic symbols: ходит
//    testSomeWord(L"\u0437\u0435\u043b\u0451\u043d\u044b\u0439");//here was cyrrilic symbols: зелёный
//    testSomeWord(L"\u0434\u0435\u043b\u0430\u044e\u0449\u0438\u0439");//here was cyrrilic symbols: делающий
//    testSomeWord(L"\u0441\u0434\u0435\u043b\u0430\u043d\u043d\u044b\u0439");//here was cyrrilic symbols: сделанный
//    testSomeWord(L"\u0437\u0435\u043b\u0435\u043d");//here was cyrrilic symbols: зелен
//    testSomeWord(L"\u0436\u0435\u043b\u0430\u044f");//here was cyrrilic symbols: желая
//    testSomeWord(L"\u0441\u0442\u0435\u043b\u0435\u0442");//here was cyrrilic symbols: стелет
//    testSomeWord(L"\u0438\u0437-\u0437\u0430");//here was cyrrilic symbols: из,за
//    testSomeWord(L"\u043d\u0435\u0441\u043c\u043e\u0442\u0440\u044f-\u043d\u0430");//here was cyrrilic symbols: несмотря,на
//    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0430");//here was cyrrilic symbols: Москва
//    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0443");//here was cyrrilic symbols: Москву
//    testSomeWord(L"\u0444\u0430\u043a\u0443\u043b\u044c\u0442\u0435\u0442");//here was cyrrilic symbols: факультет
//    testSomeWord(L"\u0441\u043e");//here was cyrrilic symbols: со
//    testSomeWord(L"\u0437\u0443\u043b\u044f\u0442\u044c");//here was cyrrilic symbols: зулять
//    testSomeWord(L"\u044b\u044b\u0441\u043e\u0445\u0440\u0430\u043d\u0438\u0442\u0441\u044f");//here was cyrrilic symbols: ыысохранится
//    testSomeWord(L"\u044b\u044b\u0433\u0440\u0430\u0434\u0443\u0441");//here was cyrrilic symbols: ыыградус
//    testSomeWord(L"\u043a\u0440\u043e\u043e\u043b\u0438\u043a");//here was cyrrilic symbols: кроолик
//    testSomeWord(L"\u0444\u0430\u043d\u0430\u0442\u043e\u043a");//here was cyrrilic symbols: фанаток
//    testSomeWord(L"\u043f\u043e\u0433\u0443\u0433\u043b\u044f\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: погуглящихся
//    testSomeWord(L"\u043d\u0435\u0435");//here was cyrrilic symbols: нее
//    testSomeWord(L"\u043d\u0435\u0451");//here was cyrrilic symbols: неё
//    testSomeWord(L"\u043c\u0430\u043b\u0435\u0439\u0448\u0435\u0433\u043e");//here was cyrrilic symbols: малейшего
//    testSomeWord(L"\u043b\u0443\u0447\u0448\u0438\u0445");//here was cyrrilic symbols: лучших
//    testSomeWord(L"\u043d\u0435\u0432\u043d\u0435\u0432\u043d\u044f\u0442\u043d\u043e\u0441\u0442\u044c");//here was cyrrilic symbols: невневнятность

//    testSomeWord(L"\u0439\u043e\u0445\u0430\u043d\u0441\u0441\u043e\u043d");//here was cyrrilic symbols: йоханссон
//    testSomeWord(L"\u043f\u0435\u0440\u0440\u0441\u0441\u043e\u043d");//here was cyrrilic symbols: перрссон

/*
    wcout << "Test begin" << endl;
    for (int i = 0; i < 10000; ++i)
    {
        testSomeWord(L"\u0438\u043d\u0442\u0435\u0440\u043d\u0430\u0446\u0438\u043e\u043d\u0430\u043b\u0438\u0437\u0438\u0440\u0443\u044e\u0442\u0441\u044f");//here was cyrrilic symbols: интернационализируются
    }
    wcout << "Test finished" << endl;
*/
//    testSomeWord(L"Деппа");
//    testSomeWord(L"Путин");
//    testSomeWord(L"мыла");
//    testSomeWord(L"вокруг");
    testSomeWord(L"капитан-командора");
//    testSomeWord(L"все-таки");
    testSomeWord(L"кроолик");
    delete dicInterface;
}

void createMorphologyBinaryFileFromTextFile(const string & inputFilePath, const string & outputFilePath)
{
    wcout << "createMorphologyBinaryFileFromTextFile" << endl;
    morphological_dictionary = shared_ptr<MorphologicalDictionary>(
                new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN, false));
    wcout << "OK 1" << endl;
    //trie->buildBuffer();
    morphological_dictionary->buildBufferFromTextFile(inputFilePath.c_str());
    wcout << "OK 2" << endl;
    morphological_dictionary->writeToBinaryFile(outputFilePath.c_str());
    wcout << "OK 3" << endl;
}

void testNGramVer2(void)
{
/*
    wcout << "======== SAVE N-GRAM TO BINARY FILE =============" << endl;
    dic->setIgnoreE(true);

    //dic->getNGramTrieBinaryFileCreator()->updateFromFolder("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\NGramData\\", "*.txt");
    //dic->getNGramTrieBinaryFileCreator()->saveToBinaryFile("NGramVer1.bin");

    dic->getNGramTrieBinaryFileCreator()->updateFromFolder("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_03_18_OpenCorporaCorpus\\text\\", "*.txt");
    dic->getNGramTrieBinaryFileCreator()->saveToBinaryFile("NGramVer3.bin");
*/
}

void testNGram(void)
{
    dic->setIgnoreE(true);
    dic->getNGramTrie()->testSentence(L"\u043f\u0440\u043e\u0442\u043e\u0442\u0438\u043f\u044b \u0441\u0432\u043e\u0438\u0445 \u0443\u0441\u0442\u0440\u043e\u0439\u0441\u0442\u0432");//here was cyrrilic symbols: прототипы,своих,устройств
    dic->getNGramTrie()->testSentence(L"\u0441\u0435\u0433\u043e\u0434\u043d\u044f \u044f \u043f\u0440\u0438\u0448\u0435\u043b \u043d\u0430 \u0440\u0430\u0431\u043e\u0442\u0443 \u0440\u0430\u043d\u043e");//here was cyrrilic symbols: сегодня,я,пришел,на,работу,рано
    dic->getNGramTrie()->testSentence(L"\u0441\u0435\u0433\u043e\u0434\u043d\u044f \u044f \u043f\u0440\u0438\u0448\u0451\u043b \u043d\u0430 \u0440\u0430\u0431\u043e\u0442\u0443 \u0440\u0430\u043d\u043e");//here was cyrrilic symbols: сегодня,я,пришёл,на,работу,рано
    dic->getNGramTrie()->testSentence(L"\u043e\u043d \u0447\u0438\u0442\u0430\u0435\u0442 \u043a\u043d\u0438\u0433\u0443");//here was cyrrilic symbols: он,читает,книгу
}

void testListFilesInFolder(void)
{
#ifdef MSVC
    string path = "E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\NGramData\\";
    vector<string> list = vector<string>();
    string path_file = path + "*.txt";
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file.c_str(), &fd);

    if(h == INVALID_HANDLE_VALUE)
    {
        return; // no files found
    }

    while(true)
    {
        list.push_back(fd.cFileName);
        if(FindNextFile(h, &fd) == FALSE)
            break;
    }
    for (int i = 0; i < (int) list.size(); ++i)
    {
        list.at(i) = path + list.at(i);
        //wcout << list.at(i) << endl;
    }
    wcout << list.size() << endl;
#endif
}

void testVector(void)
{
    vector<int> a(10,0);
    for (int i = 0; i < (int) a.size(); ++i)
    {
        wcout << a.at(i) << " ";
        a.at(i) += i;
    }
    wcout << endl;
    for (int i = 0; i < (int) a.size(); ++i)
    {
        wcout << a.at(i) << " ";
    }
    wcout << endl;
    vector<int> b;
    b = a;
    for (int i = 0; i < (int) b.size(); ++i)
    {
        wcout << b.at(i) << " ";
    }
    wcout << endl;
}

void testSet(void)
{
    wcout << "Test set : " << endl;
/*
    vector<int> v1;
    v1.push_back(1);
    v1.push_back(4);
    v1.push_back(3);
    v1.push_back(5);
    v1.push_back(2);
    std::sort(v1.begin(), v1.end());
    vector<int> v2;
    v2.push_back(5);
    v2.push_back(4);
    v2.push_back(3);
    v2.push_back(2);
    v2.push_back(1);
    std::sort(v2.begin(), v2.end());
    set<vector<int> > s;
    s.insert(v1);
    s.insert(v2);
    wcout << s.size() << endl;
*/
    set<int> s1 = set<int>();
    s1.insert(1);
    s1.insert(2);
    set<int> s2 = set<int>();
    s2.insert(3);
    s2.insert(1);
    set<set<int> > s = set<set<int> >();
    s.insert(s1);
    s.insert(s2);
    wcout << s.size() << endl;

    map<int, vector<int> > flm = dic->getDictionaryTrie()->getFeatureListMap();

    map<set<int>, int> flm_r = map<set<int>, int>();

    for (map<int, vector<int> >::iterator i_flm = flm.begin(); i_flm != flm.end(); ++i_flm)
    {
        int featureListId = i_flm->first;
        vector<int> featureIdVector = i_flm->second;

        wcout << featureListId;
        for (int i = 0; i < (int) featureIdVector.size(); ++i)
        {
            wcout << " - " << dic->getDictionaryTrie()->idToShortId(featureIdVector.at(i));
        }
        wcout << endl;

        set<int> featureIdSet = set<int>();
        for (int i = 0; i < (int) featureIdVector.size(); ++i)
        {
            featureIdSet.insert(featureIdVector.at(i));
        }
        if (!flm_r.count(featureIdSet))
        {
            flm_r[featureIdSet] = featureListId;
        }
        else
        {
            wcout << ">>> " << flm_r[featureIdSet] << " # " << featureListId << endl;
        }
    }
//	int currentMapSize = flm_r.size();
    wcout << "Map size = " << flm_r.size() << endl;
}

void testOneSentenceAnalyze(Dictionary* dic, wstring _sentence)
{
    wcout << "========================================================" << endl;
    wcout << _sentence << endl;
    vector<MorphologicalAnalyze> analyze = dic->getMorphologicalAnalyze(_sentence);
    for (int i = 0; i < (int) analyze.size(); ++i)
    {
        MorphologicalAnalyze tmp = analyze.at(i);
        wcout << "--------------------------------------------------------" << endl;
        for (int j = 0; j < (int) tmp.minfos.size(); ++j)
        {
            wcout << tmp.minfos.at(j).featureListId << " ";
        }
        wcout << endl << "Best Variant = " << tmp.minfos.at(tmp.bestVariant).featureListId << endl;
    }
}

void testSentenceAnalyze(void)
{
    dic->setIgnoreE(true);
    testOneSentenceAnalyze(dic, L"\u043f\u0440\u043e\u0442\u043e\u0442\u0438\u043f\u044b \u0441\u0432\u043e\u0438\u0445 \u0443\u0441\u0442\u0440\u043e\u0439\u0441\u0442\u0432");//here was cyrrilic symbols: прототипы,своих,устройств
    testOneSentenceAnalyze(dic, L"\u0433\u043e\u0434\u0430 \u0434\u0432\u0430 \u043d\u0430\u0437\u0430\u0434");//here was cyrrilic symbols: года,два,назад
}

void testTestNGram(void)
{
/*
    TestNGram* testNGram = new TestNGram();
    testNGram->test();
    delete testNGram;
*/
}

void testSomeNeologism(wstring _word)
{
    /*
    //vector<MorphologicalInfo> _infoList = dic->getDictionaryTrie()->getMorphologicalPrediction(_word);
    vector<MorphologicalInfo> _infoList = dic->getSuffixModelTrie()->getMorphologicalPredictionBySuffix(_word);

    wcout << endl << "============ " << _word << " ==================" << endl;
    printMorphologicalInfoList(_infoList);
    */
}

void testNeologism(void)
{
    wcout << "--------- Test Neologism  ----------------" << endl;
    dic->setIgnoreE(true);
    wcout << "SuffixModelTrieBinaryFileReader : Number of nodes = " << dic->getSuffixModelTrie()->getNumberOfNode() << endl;
    finish = clock();
    testSomeNeologism(L"\u0437\u0443\u043b\u044f\u0442\u044c");//here was cyrrilic symbols: зулять
    testSomeNeologism(L"\u044b\u044b\u0441\u043e\u0445\u0440\u0430\u043d\u0438\u0442\u0441\u044f");//here was cyrrilic symbols: ыысохранится
    testSomeNeologism(L"\u044b\u044b\u0433\u0440\u0430\u0434\u0443\u0441");//here was cyrrilic symbols: ыыградус
    testSomeNeologism(L"\u043a\u0440\u043e\u043e\u043b\u0438\u043a");//here was cyrrilic symbols: кроолик
    testSomeNeologism(L"\u0444\u0430\u043d\u0430\u0442\u043e\u043a");//here was cyrrilic symbols: фанаток
    testSomeNeologism(L"\u043f\u043e\u0433\u0443\u0433\u043b\u044f\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: погуглящихся
}

void testMap_2013_03_26(void)
{
    map<int, int> a;
    a[1] = 10;
    a[2] = 20;
    a[3] = 30;
    a.at(2) += 5;
    for (int i = 1; i <= 3; ++i)
    {
        wcout << a.at(i) << endl;
    }
}

void testSplit(void)
{
    vector<wstring> tokens;
    wstring _str = L"(((\u0447\u0443\u0442\u044c \u043d\u0435 \u043f\u0440\u0438\u0431\u0438\u043b\u0430 aaa-aaa \u0440\u0435\u0436\u0438\u0441\u0441\u0435\u0440\u0430 - \u0434\u0443\u043c\u0430\u044e, \u0437\u0430 \u0434\u0435\u043b\u043e)";//here was cyrrilic symbols: чуть,не,прибила,режиссера,думаю,за,дело
    tokens = tools->dictionarySplit(_str, WORD_CHARACTERS, false);
    for (int i = 0; i < (int) tokens.size(); ++i)
    {
        wcout << tokens.at(i) << endl;
    }
}

void statisticLine(wstring _line_1)
{
    wstring _line = dic->getDictionaryTools()->toLowerCase(_line_1);
    vector<wstring> _words = dic->getDictionaryTools()->dictionarySplit(_line, WORD_CHARACTERS, false);
    int _numberOfNonDicWords = 0;
    for (int i = 0; i < (int) _words.size(); ++i)
    {
        wstring _word = _words.at(i);
        _stat_words.insert(_word);
        shared_ptr<vector<MorphologicalInfo> > minfos = std::make_shared<vector<MorphologicalInfo> >();
        dic->getDictionaryTrie()->getMorphologicalInfoList(_word, minfos);
        // try e
        if (!minfos->size())
        {
            vector<wstring> modifications = dic->getDictionaryTools()->getAllEeVariant(_word);
            for (int i = 0; i < (int) modifications.size(); ++i)
            {
                wstring str = modifications.at(i);
                shared_ptr<vector<MorphologicalInfo> > tmp = std::make_shared<vector<MorphologicalInfo> >();
                dic->getDictionaryTrie()->getMorphologicalInfoList(str, tmp);
                minfos->insert(minfos->end(), tmp->begin(), tmp->end());
            }
        }
        // try -
        if (!minfos->size() && _word.find(L"-") != string::npos)
        {
            vector<wstring> modifications = dic->getDictionaryTools()->dictionarySplit(_word, L"-");
            for (int i = 0; i < (int) modifications.size(); ++i)
            {
                wstring str = modifications.at(i);
                shared_ptr<vector<MorphologicalInfo> > tmp = std::make_shared<vector<MorphologicalInfo> >();
                dic->getDictionaryTrie()->getMorphologicalInfoList(str, tmp);
                minfos->insert(minfos->end(), tmp->begin(), tmp->end());
            }
        }
        // non-dictionary word
        if ((!minfos->size()) && (!_stat_nonDictionaryWords.count(_word)))
        {
            _stat_nonDictionaryWords.insert(pair<wstring, wstring>(_word, _line));
            _numberOfNonDicWords++;
        }
    }
    if (_numberOfNonDicWords)
    {
        _sentencesWithNonDicWords.insert(_line_1);
    }
}

void statisticFile(string _filePath)
{
    wifstream fin(_filePath.c_str());
#ifdef MSVC
    fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t>));
#else
    fin.imbue(std::locale("en_US.UTF-8"));
#endif
    wstring _line;
    while(getline(fin, _line))
    {
        if (_line.size())
        {
            statisticLine(_line);
        }
    }
}

void statisticFolder(string _path, string _fileType)
{
#ifdef MSVC
    string path_file = _path + "*.txt";
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile(path_file.c_str(), &fd);

    if (h == INVALID_HANDLE_VALUE)
    {
        return; // no files found
    }
    int _fileCount = 0;
    while(true)
    {
        string _fileName = fd.cFileName;
        string _filePath = _path + _fileName;
        _fileCount++;
        wcout << _fileCount << endl;
        statisticFile(_filePath);
        if(FindNextFile(h, &fd) == FALSE)
            break;
    }
#endif
}

void statisticWriteToFile(string _filePath) {
    // open file to write
    std::wofstream fout(_filePath.c_str());
    // set encoding to UTF-8
#ifdef MSVC
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>));
#else
    fout.imbue(std::locale("en_US.UTF-8"));
#endif
    fout << "Total words                : " << (int) _stat_words.size() << endl;
    fout << "Total non-dictionary words : " << (int) _stat_nonDictionaryWords.size() << endl;
    fout << "=====================================================" << endl;
    map<wstring, wstring>::iterator iter;
    for (iter = _stat_nonDictionaryWords.begin(); iter != _stat_nonDictionaryWords.end(); ++iter)
    {
        fout << iter->first << " <<<" << iter->second << ">>>" << endl;
    }
    // close file when finish
    fout.close();
}

void writeSentencesWithNonDicWordsToFile(string _filePath)
{
    // open file to write
    std::wofstream fout(_filePath.c_str());
    // set encoding to UTF-8
#ifdef MSVC
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>));
#else
    fout.imbue(std::locale("en_US.UTF-8"));
#endif
    fout << "Total sentences with non-dictionary words : " << (int) _sentencesWithNonDicWords.size() << endl;
    fout << "=====================================================" << endl;
    set<wstring>::iterator iter;
    for (iter = _sentencesWithNonDicWords.begin(); iter != _sentencesWithNonDicWords.end(); ++iter)
    {
        fout << *iter << endl;
    }
    // close file when finish
    fout.close();
}

void staticsticDictionaryWord(void)
{
    wcout << "======== staticsticDictionaryWord ======" << endl;
    _stat_words = set<wstring>();
    _sentencesWithNonDicWords = set<wstring>();
    _stat_nonDictionaryWords = map<wstring, wstring>();
    statisticFolder("C:\\Python33\\_Ha\\2013_04_10_TextRefine\\","*.txt");
    statisticWriteToFile("2013_04_10_StatisticNonDictionaryWords.txt");
    writeSentencesWithNonDicWordsToFile("2013_04_10_SentencesWithNonDicWords.txt");
}

void testDeleteObject(void)
{
    vector<foo*> fooVector = vector<foo*>();
    foo* f1 = new foo();
    f1->id = 1;
    foo* f2 = new foo();
    f2->id = 2;
    fooVector.push_back(f1);
    fooVector.push_back(f2);

    for (int i = 0; i < (int) fooVector.size(); ++i)
    {
        foo* f = fooVector.at(i);
        if (f)
        {
            fooVector.at(i) = NULL;
            delete f;
        }
    }

    //delete f1;
    f1 = NULL;
    //delete f2;
    f2 = NULL;

    fooVector.clear();
}

void testMorphologicalAnalyze(void)
{
    wcout << "--------- Test Morphological Analyze  ----------------" << endl;
    dic->getTestMorphologicalAnalyze()->test();
}

void testOneWordWithStress(wstring word)
{
    wcout << endl << "------ " << word << " ------" << endl;
    vector<WordFormWithStress> WFWS = dic->getDictionaryTrie()->getWordFormWithStress(word);
    for (int i = 0; i < (int) WFWS.size(); ++i)
    {
        WordFormWithStress WF = WFWS.at(i);
        wstring word_with_stress = dic->getDictionaryTrie()->convertWordFormWithStressToWstring(WF);
        wcout << word_with_stress << endl;
    }
}

void AnalyzeAccentDictionary(void)
{
    wcout << "====== " << "AnalyzeAccentDictionary" << " ======" << endl;
    dic->getDictionaryTrie()->updateStressFromFile();
    dic->getDictionaryTrie()->printAllModelToFile("StressPosVer1.txt");
    testOneWordWithStress(L"\u0430\u0431\u0430\u0436\u0443\u0440");//here was cyrrilic symbols: абажур
    testOneWordWithStress(L"\u0430\u0431\u0430\u043c\u0438");//here was cyrrilic symbols: абами
    testOneWordWithStress(L"\u0443\u043d\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442");//here was cyrrilic symbols: университет
    testOneWordWithStress(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
}

void TestAccentDictionary(void)
{
    wcout << "====== " << "TestAccentDictionary" << " ======" << endl;
    wcout << "Load accent ... ";
    finish = clock();
    wcout << "Done. Time = " << (finish - start) / CLOCKS_PER_SEC << " s" << endl;
    testOneWordWithStress(L"\u0430\u0431\u0430\u0436\u0443\u0440");//here was cyrrilic symbols: абажур
    testOneWordWithStress(L"\u0441\u0442\u0443\u0434\u0435\u043d\u0442");//here was cyrrilic symbols: студент
    testOneWordWithStress(L"\u0434\u0435\u043b\u0430\u0442\u044c");//here was cyrrilic symbols: делать
    testOneWordWithStress(L"\u0440\u0430\u0431\u043e\u0442\u0430\u044e");//here was cyrrilic symbols: работаю
    testOneWordWithStress(L"\u0443\u0447\u0443\u0441\u044c");//here was cyrrilic symbols: учусь
    testOneWordWithStress(L"\u043a\u0430\u043b\u0435\u043d\u0434\u0430\u0440\u044c");//here was cyrrilic symbols: календарь
    testOneWordWithStress(L"\u043a\u043e\u043c\u043f\u044c\u044e\u0442\u0435\u0440");//here was cyrrilic symbols: компьютер
    testOneWordWithStress(L"\u0440\u043e\u0441\u0441\u0438\u044f");//here was cyrrilic symbols: россия
    testOneWordWithStress(L"\u043c\u0430\u043c\u0430");//here was cyrrilic symbols: мама
    testOneWordWithStress(L"\u0443\u043d\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442");//here was cyrrilic symbols: университет
    testOneWordWithStress(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testOneWordWithStress(L"\u043f\u0440\u0435\u043f\u043e\u0434\u0430\u0432\u0430\u0442\u0435\u043b\u0435\u0439");//here was cyrrilic symbols: преподавателей
    testOneWordWithStress(L"\u044f\u0437\u044b\u043a\u043e\u0432\u043e\u0439");//here was cyrrilic symbols: языковой
}

void testDictionaryInterface()
{
    DictionaryInterface* dicInterface = DictionaryInterface::CreateDictionary();
    shared_ptr<vector<GrammInfo> > ginfos = std::make_shared<vector<GrammInfo> >();
    dicInterface->getGrammInfo(L"\u0448\u043a\u043e\u043b\u044b", ginfos);//here was cyrrilic symbols: школы
    for (int i = 0; i < (int) ginfos->size(); ++i)
    {
        printGrammInfo(ginfos->at(i));
    }
    delete dicInterface;
}

// 2013-06-04 Ver_1 : update all variants, save duplications to log file
void AnalyzeAccentDictionary_Ver1(void)
{
    wcout << "====== " << "Analyze Accent Dictionary Ver_1 : " << " ======" << endl;
    wcout << "Begin update : " << endl;
    dic->getDictionaryTrie()->updateStressFromFile_Ver1();
    dic->getDictionaryTrie()->saveLogToFile_Ver1();
    dic->getDictionaryTrie()->printAllModelToFile("StressPos_Ver1.txt");

    testOneWordWithStress(L"\u0430\u0431\u0430\u0436\u0443\u0440");//here was cyrrilic symbols: абажур
    testOneWordWithStress(L"\u0441\u0442\u0443\u0434\u0435\u043d\u0442");//here was cyrrilic symbols: студент
    testOneWordWithStress(L"\u0434\u0435\u043b\u0430\u0442\u044c");//here was cyrrilic symbols: делать
    testOneWordWithStress(L"\u0440\u0430\u0431\u043e\u0442\u0430\u044e");//here was cyrrilic symbols: работаю
    testOneWordWithStress(L"\u0443\u0447\u0443\u0441\u044c");//here was cyrrilic symbols: учусь
    testOneWordWithStress(L"\u043a\u0430\u043b\u0435\u043d\u0434\u0430\u0440\u044c");//here was cyrrilic symbols: календарь
    testOneWordWithStress(L"\u043a\u043e\u043c\u043f\u044c\u044e\u0442\u0435\u0440");//here was cyrrilic symbols: компьютер
    testOneWordWithStress(L"\u0440\u043e\u0441\u0441\u0438\u044f");//here was cyrrilic symbols: россия
    testOneWordWithStress(L"\u043c\u0430\u043c\u0430");//here was cyrrilic symbols: мама
    testOneWordWithStress(L"\u0443\u043d\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442");//here was cyrrilic symbols: университет
    testOneWordWithStress(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testOneWordWithStress(L"\u043f\u0440\u0435\u043f\u043e\u0434\u0430\u0432\u0430\u0442\u0435\u043b\u0435\u0439");//here was cyrrilic symbols: преподавателей
    testOneWordWithStress(L"\u044f\u0437\u044b\u043a\u043e\u0432\u043e\u0439");//here was cyrrilic symbols: языковой
}

// 2013-06-14 : Test accent dictionary on RusCorpora
void testAccentRusCorpora(void)
{
    wcout << "====== " << "Test accent dictionary on RusCorpora : " << " ======" << endl;
    wcout << "Read stress pos ...";

    //dic->getDictionaryTrie()->readAllModelFromFile("2013_06_11_StressPos_Ver1.txt");
    dic->getDictionaryTrie()->readAllModelFromFile("2013_06_17_StressPos_Ver2.txt");

    wcout << " Done!" << endl << "Test begin : " << endl;
    dic->getDictionaryTrie()->testRusCorporaFolder(RUSCORPORA_PATH, "*.txt");

    //dic->getDictionaryTrie()->testRusCorporaWriteLogToFile(RUSCORPORA_LOG);
    dic->getDictionaryTrie()->testRusCorporaWriteLogToFile("2013_06_17_RusCorporaLog.txt");

    //dic->getDictionaryTrie()->printAllModelToFile("2013_06_17_StressPos_Ver2.txt");

    //testOneWordWithStress(L"\u0430\u043d\u0433\u043b\u0438\u0438");//here was cyrrilic symbols: англии
    wcout << " Done!" << endl << "Delete dictionary : " << endl;
}

// 2013-06-22 Ver_2 : Lookup special words from Accent.txt in RusCorpora
void AnalyzeAccentDictionary_Ver2(void)
{
    wcout << "====== " << "Analyze Accent Dictionary Ver_2 : " << " ======" << endl;
    dic->getDictionaryTrie()->readRusCorporaFeatureMap("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_06_10_Grammen_Translation\\RusCorporaFeatureId.txt");
    //dic->getDictionaryTrie()->readAccentLogFile("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\2013_06_14_TestAccentRusCorpora\\StressAndLemmaLog.txt");
    dic->getDictionaryTrie()->analyzeRusCorporaVer2_folder("E:\\Projects\\2012_12_24_Russian_Dialog_System\\___Data\\Corpora\\ruscorpora_1M\\2013_06_22_parsed\\", "*.txt");
    wcout << "Done." << endl << "Write stress pos to file ... ";
    dic->getDictionaryTrie()->printAllModelToFile("2013_07_01_StressPos_Ver3.txt");
    wcout << "Done!" << endl << "Delete dictionary ... " << endl;
}

// 2013-06-25 Dictionary & Accent Interface
void testDictionaryAccentInterfaceOneWord(wstring word)
{
    wcout << "============ " << word << " ==================" << endl;
    vector<WordInfo> _wordInfo = dicInterface->getWordInfo(word);
    for (int i = 0; i < (int) _wordInfo.size(); ++i)
    {
        WordInfo wInfo = _wordInfo.at(i);
        wcout << wInfo.word << " = " << wInfo.wordWithStress;
        for (int j = 0; j < (int) wInfo.openCorporaFeatures.size(); ++j)
        {
            wcout << " + " << wInfo.openCorporaFeatures.at(j);
        }
        wcout << endl;
        if (i < (int) _wordInfo.size() - 1)
        {
            wcout << "-----------------------------------------------" << endl;;
        }
    }
    wcout << endl;
}

// 2013-06-25 Dictionary & Accent Interface
void testDictionaryAccentInterface(void)
{
    testDictionaryAccentInterfaceOneWord(L"\u043b\u0435\u0441\u0430");//here was cyrrilic symbols: леса
    testDictionaryAccentInterfaceOneWord(L"\u043d\u0430\u0447\u0430\u043b\u043e");//here was cyrrilic symbols: начало
    testDictionaryAccentInterfaceOneWord(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testDictionaryAccentInterfaceOneWord(L"\u043a\u043d\u0438\u0433\u0438");//here was cyrrilic symbols: книги
    testDictionaryAccentInterfaceOneWord(L"\u044f\u0431\u043b\u043e\u043a\u043e");//here was cyrrilic symbols: яблоко
    testDictionaryAccentInterfaceOneWord(L"\u043c\u0430\u0448\u0438\u043d\u044b");//here was cyrrilic symbols: машины
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a");//here was cyrrilic symbols: учебник
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u043e\u0432");//here was cyrrilic symbols: учебников
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u0430\u0445");//here was cyrrilic symbols: учебниках
    testDictionaryAccentInterfaceOneWord(L"\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: быстрее
    testDictionaryAccentInterfaceOneWord(L"\u043f\u0440\u0435\u043a\u0440\u0430\u0441\u043d\u043e");//here was cyrrilic symbols: прекрасно
    testDictionaryAccentInterfaceOneWord(L"\u043a\u0440\u0430\u0441\u0438\u0432\u0430\u044f");//here was cyrrilic symbols: красивая
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0430\u0449\u0438\u0439\u0441\u044f");//here was cyrrilic symbols: учащийся
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0430\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: учащихся
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0438\u043b\u0441\u044f");//here was cyrrilic symbols: учился
    testDictionaryAccentInterfaceOneWord(L"\u0433\u043e\u0442\u043e\u0432");//here was cyrrilic symbols: готов
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: побыстрее
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0439");//here was cyrrilic symbols: побыстрей
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: потому
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u044d\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: поэтому
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u044d\u0442");//here was cyrrilic symbols: поэт
    testDictionaryAccentInterfaceOneWord(L"\u043f\u043e\u044d\u0442\u0430");//here was cyrrilic symbols: поэта
    testDictionaryAccentInterfaceOneWord(L"\u044f");//here was cyrrilic symbols: я
    testDictionaryAccentInterfaceOneWord(L"\u043c\u0435\u043d\u044f");//here was cyrrilic symbols: меня
    testDictionaryAccentInterfaceOneWord(L"\u0440\u043e\u0441\u0441\u0438\u0438");//here was cyrrilic symbols: россии
    testDictionaryAccentInterfaceOneWord(L"\u0435\u0432\u0440\u043e\u043f\u044b");//here was cyrrilic symbols: европы
    testDictionaryAccentInterfaceOneWord(L"\u0431\u0443\u0434\u0435\u0442");//here was cyrrilic symbols: будет
    testDictionaryAccentInterfaceOneWord(L"\u0445\u043e\u0436\u0443");//here was cyrrilic symbols: хожу
    testDictionaryAccentInterfaceOneWord(L"\u0445\u043e\u0434\u0438\u0442");//here was cyrrilic symbols: ходит
    testDictionaryAccentInterfaceOneWord(L"\u0437\u0435\u043b\u0451\u043d\u044b\u0439");//here was cyrrilic symbols: зелёный
    testDictionaryAccentInterfaceOneWord(L"\u0434\u0435\u043b\u0430\u044e\u0449\u0438\u0439");//here was cyrrilic symbols: делающий
    testDictionaryAccentInterfaceOneWord(L"\u0441\u0434\u0435\u043b\u0430\u043d\u043d\u044b\u0439");//here was cyrrilic symbols: сделанный
    testDictionaryAccentInterfaceOneWord(L"\u0437\u0435\u043b\u0435\u043d");//here was cyrrilic symbols: зелен
    testDictionaryAccentInterfaceOneWord(L"\u0436\u0435\u043b\u0430\u044f");//here was cyrrilic symbols: желая
    testDictionaryAccentInterfaceOneWord(L"\u041c\u043e\u0441\u043a\u0432\u0430");//here was cyrrilic symbols: Москва
    testDictionaryAccentInterfaceOneWord(L"\u041c\u043e\u0441\u043a\u0432\u0443");//here was cyrrilic symbols: Москву
    testDictionaryAccentInterfaceOneWord(L"\u0444\u0430\u043a\u0443\u043b\u044c\u0442\u0435\u0442");//here was cyrrilic symbols: факультет
    testDictionaryAccentInterfaceOneWord(L"\u0441\u043e");//here was cyrrilic symbols: со
    testDictionaryAccentInterfaceOneWord(L"\u0430\u0431\u0430\u0436\u0443\u0440");//here was cyrrilic symbols: абажур
    testDictionaryAccentInterfaceOneWord(L"\u0441\u0442\u0443\u0434\u0435\u043d\u0442");//here was cyrrilic symbols: студент
    testDictionaryAccentInterfaceOneWord(L"\u0434\u0435\u043b\u0430\u0442\u044c");//here was cyrrilic symbols: делать
    testDictionaryAccentInterfaceOneWord(L"\u0440\u0430\u0431\u043e\u0442\u0430\u044e");//here was cyrrilic symbols: работаю
    testDictionaryAccentInterfaceOneWord(L"\u0443\u0447\u0443\u0441\u044c");//here was cyrrilic symbols: учусь
    testDictionaryAccentInterfaceOneWord(L"\u043a\u0430\u043b\u0435\u043d\u0434\u0430\u0440\u044c");//here was cyrrilic symbols: календарь
    testDictionaryAccentInterfaceOneWord(L"\u043a\u043e\u043c\u043f\u044c\u044e\u0442\u0435\u0440");//here was cyrrilic symbols: компьютер
    testDictionaryAccentInterfaceOneWord(L"\u0440\u043e\u0441\u0441\u0438\u044f");//here was cyrrilic symbols: россия
    testDictionaryAccentInterfaceOneWord(L"\u043c\u0430\u043c\u0430");//here was cyrrilic symbols: мама
    testDictionaryAccentInterfaceOneWord(L"\u0443\u043d\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442");//here was cyrrilic symbols: университет
    testDictionaryAccentInterfaceOneWord(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testDictionaryAccentInterfaceOneWord(L"\u043f\u0440\u0435\u043f\u043e\u0434\u0430\u0432\u0430\u0442\u0435\u043b\u0435\u0439");//here was cyrrilic symbols: преподавателей
    testDictionaryAccentInterfaceOneWord(L"\u044f\u0437\u044b\u043a\u043e\u0432\u043e\u0439");//here was cyrrilic symbols: языковой
}

void testRebuildDictionary(void)
{
    wcout << "====== " << "Test rebuild dictionary : " << " ======" << endl;
    wcout << "Create dictionary ... " << endl;
    wcout << "Done." << endl;
    testSomeWord(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testSomeWord(L"\u043a\u043d\u0438\u0433\u0438");//here was cyrrilic symbols: книги
    testSomeWord(L"\u044f\u0431\u043b\u043e\u043a\u043e");//here was cyrrilic symbols: яблоко
    testSomeWord(L"\u043c\u0430\u0448\u0438\u043d\u044b");//here was cyrrilic symbols: машины
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a");//here was cyrrilic symbols: учебник
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u043e\u0432");//here was cyrrilic symbols: учебников
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u0430\u0445");//here was cyrrilic symbols: учебниках
    testSomeWord(L"\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: быстрее
    testSomeWord(L"\u043f\u0440\u0435\u043a\u0440\u0430\u0441\u043d\u043e");//here was cyrrilic symbols: прекрасно
    testSomeWord(L"\u043a\u0440\u0430\u0441\u0438\u0432\u0430\u044f");//here was cyrrilic symbols: красивая
    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0439\u0441\u044f");//here was cyrrilic symbols: учащийся
    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: учащихся
    testSomeWord(L"\u0443\u0447\u0438\u043b\u0441\u044f");//here was cyrrilic symbols: учился
    testSomeWord(L"\u0433\u043e\u0442\u043e\u0432");//here was cyrrilic symbols: готов
    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: побыстрее
    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0439");//here was cyrrilic symbols: побыстрей
    testSomeWord(L"\u043f\u043e\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: потому
    testSomeWord(L"\u043f\u043e\u044d\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: поэтому
    testSomeWord(L"\u043f\u043e\u044d\u0442");//here was cyrrilic symbols: поэт
    testSomeWord(L"\u043f\u043e\u044d\u0442\u0430");//here was cyrrilic symbols: поэта
    testSomeWord(L"\u044f");//here was cyrrilic symbols: я
    testSomeWord(L"\u043c\u0435\u043d\u044f");//here was cyrrilic symbols: меня
    testSomeWord(L"\u0440\u043e\u0441\u0441\u0438\u0438");//here was cyrrilic symbols: россии
    testSomeWord(L"\u0435\u0432\u0440\u043e\u043f\u044b");//here was cyrrilic symbols: европы
    testSomeWord(L"\u0431\u0443\u0434\u0435\u0442");//here was cyrrilic symbols: будет
    testSomeWord(L"\u0445\u043e\u0436\u0443");//here was cyrrilic symbols: хожу
    testSomeWord(L"\u0445\u043e\u0434\u0438\u0442");//here was cyrrilic symbols: ходит
    testSomeWord(L"\u0437\u0435\u043b\u0451\u043d\u044b\u0439");//here was cyrrilic symbols: зелёный
    testSomeWord(L"\u0434\u0435\u043b\u0430\u044e\u0449\u0438\u0439");//here was cyrrilic symbols: делающий
    testSomeWord(L"\u0441\u0434\u0435\u043b\u0430\u043d\u043d\u044b\u0439");//here was cyrrilic symbols: сделанный
    testSomeWord(L"\u0437\u0435\u043b\u0435\u043d");//here was cyrrilic symbols: зелен
    testSomeWord(L"\u0436\u0435\u043b\u0430\u044f");//here was cyrrilic symbols: желая
    testSomeWord(L"\u0441\u0442\u0435\u043b\u0435\u0442");//here was cyrrilic symbols: стелет
    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0430");//here was cyrrilic symbols: Москва
    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0443");//here was cyrrilic symbols: Москву
    testSomeWord(L"\u0444\u0430\u043a\u0443\u043b\u044c\u0442\u0435\u0442");//here was cyrrilic symbols: факультет

    dic->getDictionaryTrie()->rebuildDictionary();

    testSomeWord(L"\u0447\u0438\u0442\u0430\u044e");//here was cyrrilic symbols: читаю
    testSomeWord(L"\u043a\u043d\u0438\u0433\u0438");//here was cyrrilic symbols: книги
    testSomeWord(L"\u044f\u0431\u043b\u043e\u043a\u043e");//here was cyrrilic symbols: яблоко
    testSomeWord(L"\u043c\u0430\u0448\u0438\u043d\u044b");//here was cyrrilic symbols: машины
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a");//here was cyrrilic symbols: учебник
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u043e\u0432");//here was cyrrilic symbols: учебников
    testSomeWord(L"\u0443\u0447\u0435\u0431\u043d\u0438\u043a\u0430\u0445");//here was cyrrilic symbols: учебниках
    testSomeWord(L"\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: быстрее
    testSomeWord(L"\u043f\u0440\u0435\u043a\u0440\u0430\u0441\u043d\u043e");//here was cyrrilic symbols: прекрасно
    testSomeWord(L"\u043a\u0440\u0430\u0441\u0438\u0432\u0430\u044f");//here was cyrrilic symbols: красивая
    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0439\u0441\u044f");//here was cyrrilic symbols: учащийся
    testSomeWord(L"\u0443\u0447\u0430\u0449\u0438\u0445\u0441\u044f");//here was cyrrilic symbols: учащихся
    testSomeWord(L"\u0443\u0447\u0438\u043b\u0441\u044f");//here was cyrrilic symbols: учился
    testSomeWord(L"\u0433\u043e\u0442\u043e\u0432");//here was cyrrilic symbols: готов
    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0435");//here was cyrrilic symbols: побыстрее
    testSomeWord(L"\u043f\u043e\u0431\u044b\u0441\u0442\u0440\u0435\u0439");//here was cyrrilic symbols: побыстрей
    testSomeWord(L"\u043f\u043e\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: потому
    testSomeWord(L"\u043f\u043e\u044d\u0442\u043e\u043c\u0443");//here was cyrrilic symbols: поэтому
    testSomeWord(L"\u043f\u043e\u044d\u0442");//here was cyrrilic symbols: поэт
    testSomeWord(L"\u043f\u043e\u044d\u0442\u0430");//here was cyrrilic symbols: поэта
    testSomeWord(L"\u044f");//here was cyrrilic symbols: я
    testSomeWord(L"\u043c\u0435\u043d\u044f");//here was cyrrilic symbols: меня
    testSomeWord(L"\u0440\u043e\u0441\u0441\u0438\u0438");//here was cyrrilic symbols: россии
    testSomeWord(L"\u0435\u0432\u0440\u043e\u043f\u044b");//here was cyrrilic symbols: европы
    testSomeWord(L"\u0431\u0443\u0434\u0435\u0442");//here was cyrrilic symbols: будет
    testSomeWord(L"\u0445\u043e\u0436\u0443");//here was cyrrilic symbols: хожу
    testSomeWord(L"\u0445\u043e\u0434\u0438\u0442");//here was cyrrilic symbols: ходит
    testSomeWord(L"\u0437\u0435\u043b\u0451\u043d\u044b\u0439");//here was cyrrilic symbols: зелёный
    testSomeWord(L"\u0434\u0435\u043b\u0430\u044e\u0449\u0438\u0439");//here was cyrrilic symbols: делающий
    testSomeWord(L"\u0441\u0434\u0435\u043b\u0430\u043d\u043d\u044b\u0439");//here was cyrrilic symbols: сделанный
    testSomeWord(L"\u0437\u0435\u043b\u0435\u043d");//here was cyrrilic symbols: зелен
    testSomeWord(L"\u0436\u0435\u043b\u0430\u044f");//here was cyrrilic symbols: желая
    testSomeWord(L"\u0441\u0442\u0435\u043b\u0435\u0442");//here was cyrrilic symbols: стелет
    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0430");//here was cyrrilic symbols: Москва
    testSomeWord(L"\u041c\u043e\u0441\u043a\u0432\u0443");//here was cyrrilic symbols: Москву
    testSomeWord(L"\u0444\u0430\u043a\u0443\u043b\u044c\u0442\u0435\u0442");//here was cyrrilic symbols: факультет

    wcout << "Done!" << endl << "Delete dictionary ... " << endl;
}

void testPointer(void)
{
    vector<foo*> fooVector = vector<foo*>();
    foo* f1 = new foo();
    f1->id = 1;
    foo* f2 = new foo();
    f2->id = 2;
    fooVector.push_back(f1);
    fooVector.push_back(f2);
    foo* f3 = fooVector.at(1);
    f3->id = 3;
    wcout << fooVector.at(1)->id << endl;
}

void testSomeWordForm(wstring _word, wstring _features, bool includeLinkedLemmas = true)

{
    wcout << "====== " << _word << " (" << _features << ") ====== Include linked lemmas = " << includeLinkedLemmas << " ======" << endl;
    vector<wstring> featuresList = tools->dictionarySplit(_features, WORD_DELIMITER);
    vector<WordForm> wordForms = dicInterface->getWordFormByFeatures(_word, featuresList, includeLinkedLemmas);
    for (int i = 0; i < (int) wordForms.size(); ++i)
    {
        wcout << wordForms.at(i).word << endl;
        //printMorphologicalInfo(wordForms.at(i).info);
        // 2014-03-05
        printMorphologicalInfoInDetail(&(wordForms.at(i).info));
    }
}

void testFindingWordFormByFeatures(void)
{
    wcout << "============ testFindingWordFormByFeatures ============" << endl;
    testSomeWordForm(L"\u0447\u0438\u0442\u0430\u0442\u044c", L"sing 3per");//here was cyrrilic symbols: читать
    testSomeWordForm(L"\u043b\u044e\u0431\u0438\u043b", L"sing 1per pres");//here was cyrrilic symbols: любил
    testSomeWordForm(L"\u0448\u043a\u043e\u043b\u0430", L"sing gent");//here was cyrrilic symbols: школа
    testSomeWordForm(L"\u043c\u0435\u0434\u0432\u0435\u0434\u044c", L"sing gent");//here was cyrrilic symbols: медведь
    testSomeWordForm(L"\u0443\u043d\u0438\u0436\u0435\u043d\u043d\u044b\u0435", L"plur pssv gent");//here was cyrrilic symbols: униженные
    testSomeWordForm(L"\u0443\u043d\u0438\u0436\u0435\u043d\u043d\u044b\u0435", L"plur pssv datv");//here was cyrrilic symbols: униженные
    testSomeWordForm(L"\u043c\u0430\u043b\u0435\u043d\u044c\u043a\u0438\u0439", L"ADJF gent plur");//here was cyrrilic symbols: маленький
    testSomeWordForm(L"\u043c\u0430\u043b\u0435\u043d\u044c\u043a\u0438\u0439", L"ADJF gent plur", false);//here was cyrrilic symbols: маленький
}

void testDictionaryTrieIterator()
{
    DictionaryTrieIterator* trieIterator = new DictionaryTrieIterator(dic);
    trieIterator->setLemmaOnly(false);
    trieIterator->iterate();
    vector<wstring> _result = trieIterator->getResult();
    wcout << _result.size() << endl;
    for (int i = 0; i < 50; ++i)
    {
        wcout << _result.at(i) << endl;
    }
    delete trieIterator;
}

void testSomeLemmaId(int lemmaId)
{
    shared_ptr<vector<MorphologicalInfo> > _infoList = std::make_shared<vector<MorphologicalInfo> >();
    dicInterface->getMorphologicalInfoOfLemmaId(lemmaId, _infoList);
    wstring lemma = dicInterface->getLemmaOfLemmaId(lemmaId);
    wcout << "============ " << lemmaId << " ==================" << endl;
    wcout << "Lemma = " << lemma << endl;
    wcout << "------------ morphological info : ---------------" << endl;
    printMorphologicalInfoList(_infoList);
    wcout << endl;
}

void testFindWordByLemmaId()
{
    dicInterface->setMainWordFormFromLinkedLemmas(true);
    testSomeLemmaId(329545);
}

void testEnglishMorphology()
{
    wcout << "============ Load Dictionary ... ";
    dicInterface = DictionaryInterface::CreateDictionary(BIN_DIC_FILE_PATH_ENGLISH, 1);
    dicInterface->enableRules(true);
    finish = clock();
    wcout << " Done! Time = " << (finish - start) * 1000 / CLOCKS_PER_SEC << " ms ============" << endl;
    //wcout << "Number of nodes = " << dicInterface->getDictionaryTrie()->numberOfNodes() << endl;
/*
    testSomeWord(L"world");
    testSomeWord(L"loves");
    testSomeWord(L"loved");
    testSomeWord(L"children");
    testSomeWord(L"beautiful");
    testSomeWord(L"English");
    testSomeWord(L"55");
    testSomeWord(L"one");
    testSomeWord(L"108-year-old");
    testSomeWord(L".");
    testSomeWord(L",");
    testSomeWord(L"Elsevier");
    testSomeWord(L"go-it-alone");
    testSomeWord(L"godfather");
    testSomeWord(L"cat");
    testSomeWord(L"cats");
    testSomeWord(L"glossolalia");
*/
    testSomeWord(L"crocodile");
    testSomeWord(L"crocodiles");
    testSomeWord(L"blowing");
    testSomeWord(L"is");
    testSomeWord(L"haved");
    testSomeWord(L"had");
    testSomeWord(L"tower");
    testSomeWord(L"better");
    testSomeWord(L"bawls");
    testSomeWord(L"man");
    testSomeWord(L"unknown");
    testSomeWord(L"crew");
    testSomeWord(L"known");
    testSomeWord(L"implementers");
    testSomeWord(L"a.m.");
    delete dicInterface;
}

void fixRussianMorphology()
{
    dic = new Dictionary();
    // init old char map
    dic->getDictionaryTools()->initCharMap_old();
    DictionaryTrieBinaryFileReader* trie = new DictionaryTrieBinaryFileReader(dic);
    // init new char map
    dic->getDictionaryTools()->initCharMap();
    // write new binary file
    trie->buildBuffer();
    trie->writeToBinaryFile("//home//mhle//Coda//config//Dictionary//Data//RussianMorphology.bin");

    delete trie;
    delete dic;
}

void exportDictionaryToTextFile()
{
    wcout << "#exportDictionaryToTextFile" << endl;
    dic = new Dictionary();
    wcout << "Load dictionary ok" << endl;
    dic->getDictionaryTrie()->exportDictionaryToTextFile("//home//mhle//Coda//config//Dictionary//Data//RussianMorphology.txt");
    wcout << "export to text file ok" << endl;
    delete dic;
}

void printMorphologies(shared_ptr<vector<shared_ptr<Morphology> > > result)
{
    for (vector<shared_ptr<Morphology> >::iterator iter = result->begin();
         iter != result->end(); ++iter)
    {
        shared_ptr<Morphology> item = *iter;
        wcout << "lemma_id = " << item->lemma_id << " word = " << *(item->word)
              << " >> lemma = " << *(item->lemma) << " >> suffix length = "
              << item->suffix_length << endl;
//        wcout << "item->features.size() = " << item->features.size() << endl;
//        wcout << "item->descriptions.size() = " << item->descriptions.size() << endl;
        vector<shared_ptr<wstring> >::iterator d_iter = item->descriptions.begin();
        for (vector<shared_ptr<wstring> >::iterator f_iter = item->features.begin();
             f_iter != item->features.end(); ++f_iter)
        {
            wcout << *(*f_iter) << " - " << *(*d_iter) << endl;
            d_iter++;
        }
        wcout << "------------------------------------" << endl << endl;
    }
}

void testMorphologyOfWord(const wstring & word)
{
    shared_ptr<vector<shared_ptr<Morphology> > > result = make_shared<vector<shared_ptr<Morphology> > >();
    morphological_dictionary->getMorphology(word, result);
    wcout << "================== " << word << " (" << result->size() << ") ==================" << endl;
    printMorphologies(result);
}

void testMorphologyOfText(const wstring & text)
{
    vector<wstring> tokens = tools->dictionarySplit(text, WORD_CHARACTERS, false);
    for (vector<wstring>::iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
    {
        testMorphologyOfWord(*iter);
    }
}

// light version
void testMorphologicalDictionaryRussian()
{
    wcout << "###### testMorphologicalDictionaryRussian ######" << endl << endl;
    morphological_dictionary = shared_ptr<MorphologicalDictionary>(new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN));
    morphological_dictionary->setPrediction(true);

    testMorphologyOfWord(L"Россия");
    testMorphologyOfWord(L"поехали");
    testMorphologyOfWord(L"мыло");
    testMorphologyOfWord(L"все");
    testMorphologyOfWord(L"всё");
    testMorphologyOfWord(L"еще");
    testMorphologyOfWord(L"ещё");
    testMorphologyOfWord(L"зеленый");
    testMorphologyOfWord(L"зелёный");
    testMorphologyOfWord(L"черный");
    testMorphologyOfWord(L"чёрный");
    testMorphologyOfText(L"Биржевой курс доллара упал ниже 50 рублей впервые с середины апреля");
    testMorphologyOfText(L"зулять ыысохранится кроолик фанаток нее неё");
    testMorphologyOfWord(L"вокруг");
    testMorphologyOfWord(L"капитан-командора");
    testMorphologyOfWord(L"все-таки");
    testMorphologyOfWord(L"кроолик");
    testMorphologyOfWord(L"легче");
    testMorphologyOfWord(L"хуже");
    testMorphologyOfWord(L"ЕВРОПАРЛАМЕНТ");
    testMorphologyOfWord(L"студент");
    testMorphologyOfWord(L"джихадистам");
    testMorphologyOfWord(L"награждён");
}

void testMorphologicalDictionaryEnglish()
{
    morphological_dictionary = shared_ptr<MorphologicalDictionary>(new MorphologicalDictionary(BIN_DIC_FILE_PATH_ENGLISH));
    morphological_dictionary->enableRules(true);
    testMorphologyOfWord(L"America");
    testMorphologyOfText(L"Big cities and small historic towns top the list of most popular places to visit in America, according to the travel site TripAdvisor.");
    testMorphologyOfText(L"crocodiles blowing haved tower bawls better man unknown crew known implementers");
    testMorphologyOfWord(L"towns");
    testMorphologyOfWord(L"blowing");
    testMorphologyOfWord(L"crocodiles");
    testMorphologyOfText(L"force-fed bewept laded overwound wrapt hove bred outsold caught overbought stunk blest sought lost met unbound wept awoke");
    testMorphologyOfWord(L"Boulogne");
}

void compareSpeedOfMorphology(const string filePath, wstring language, int LOOPS)
{
    wcout << "TEST : Language = " << language << endl;
    DictionaryTrie::morphology_count = 0;
    MorphologicalDictionary::morphology_count = 0;
    MorphologicalDictionary::prediction_count = 0;
    MorphologicalDictionary::rule_count = 0;
    vector<wstring> tokens;
    vector<wstring> _tokens;

    clock_t t0, t1;

    // read file
    wifstream fi(filePath.c_str());
    if (!fi.is_open())
    {
        wcout << "#ERROR: File not found!!!" << endl;
        return;
    }
#ifdef MSVC
    fi.imbue(locale(fi.getloc(), new codecvt_utf8<wchar_t>));
#else
    fi.imbue(std::locale("en_US.UTF-8"));
#endif
    wstring line;
    while (getline(fi, line))
    {
        _tokens = tools->dictionarySplit(line, WORD_CHARACTERS, false);
        tokens.insert(tokens.end(), _tokens.begin(), _tokens.end());
    }
    fi.close();

    wcout << "Read file ok! tokens.size() = " << tokens.size() << endl;

    // init dictionaries
    if (language == L"ru")
    {
        dicInterface = DictionaryInterface::CreateDictionary();
        morphological_dictionary = shared_ptr<MorphologicalDictionary>(new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN));
        morphological_dictionary->setPrediction(true);
        morphological_dictionary->enableE(true);
    }
    else if (language == L"en")
    {
        dicInterface = DictionaryInterface::CreateDictionary("EnglishMorphology.bin");
        morphological_dictionary = shared_ptr<MorphologicalDictionary>(new MorphologicalDictionary(BIN_DIC_FILE_PATH_ENGLISH));
        morphological_dictionary->enableRules(true);
    }
    else
    {
        wcout << "#Language " << language << "???" << endl;
        return;
    }
    shared_ptr<vector<GrammInfo> > ginfos;
    shared_ptr<vector<shared_ptr<Morphology> > > minfos;

    wcout << "Dictionaries initialization ok! Test begins! LOOPS = " << LOOPS << endl;

    // test DictionaryTrie
    t0 = clock();

    for (int i_loop = 0; i_loop < LOOPS; ++i_loop)
    {
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            ginfos = std::make_shared<vector<GrammInfo> >();
            dicInterface->getGrammInfo(tokens.at(i), ginfos);
            //printGrammInfos(ginfos);
        }
    }
    t1 = clock();
    wcout << "Old version test = " << (t1 - t0) * 1000 / CLOCKS_PER_SEC << " ms ("
          << DictionaryTrie::getMorphologyCount() << " morphology calls)" << endl
          << "_______________________________________________________" << endl << endl;

    // test MorphologicalDictionary
    t0 = clock();

    for (int i_loop = 0; i_loop < LOOPS; ++i_loop)
    {
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            minfos = std::make_shared<vector<shared_ptr<Morphology> > >();
            morphological_dictionary->getMorphology(tokens.at(i), minfos);
            //printMorphologies(minfos);
        }
    }
    t1 = clock();
    wcout << "New version test = " << (t1 - t0) * 1000 / CLOCKS_PER_SEC << " ms" << endl
          << MorphologicalDictionary::getMorphologyCount() << " morphology calls" << endl
          << MorphologicalDictionary::getPredictionCount() << " prediction calls" << endl
          << MorphologicalDictionary::getRuleCount() << " rule calls" << endl
          << MorphologicalDictionary::getECount() << " e calls" << endl
          << "=======================================================" << endl << endl;
}

void testMorphologyOfLemmaId(int lemma_id)
{
    shared_ptr<vector<shared_ptr<Morphology> > > result
            = make_shared<vector<shared_ptr<Morphology> > >();
    morphological_dictionary->getMorphologyOfLemmaId(lemma_id, result);
    printMorphologies(result);
}

void testLemmaId()
{
    wcout << "###### testLemmaId ######" << endl << endl;
    morphological_dictionary = shared_ptr<MorphologicalDictionary>(
                new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN));
//    morphological_dictionary = shared_ptr<MorphologicalDictionary>(
//                new MorphologicalDictionary(BIN_DIC_FILE_PATH_ENGLISH));
//    morphological_dictionary->enableLink(true);
//    testMorphologyOfLemmaId(49812);
//    testMorphologyOfLemmaId(49813);
//    testMorphologyOfLemmaId(53260);
//    testMorphologyOfLemmaId(53261);
//    testMorphologyOfLemmaId(146053);
    testMorphologyOfLemmaId(86642);
    testMorphologyOfLemmaId(237654);
    testMorphologyOfLemmaId(309162);
    testMorphologyOfLemmaId(336469);
    testMorphologyOfLemmaId(342597);
    testMorphologyOfLemmaId(348527);
    testMorphologyOfLemmaId(364566);
    testMorphologyOfLemmaId(367346);
    testMorphologyOfLemmaId(338210);
}

void checkMorphologicalPrediction()
{
    morphological_dictionary = shared_ptr<MorphologicalDictionary>(
                new MorphologicalDictionary(BIN_DIC_FILE_PATH_RUSSIAN));
    morphological_dictionary->temp_1();
}

void test(void)
{
    tools = new DictionaryTools();
//    testNGramVer2();
//    testNGram();
//    testListFilesInFolder();
//    testVector();
//    testSentenceAnalyze();
//    testTestNGram();
//    testNeologism();
//    testSplit();
//    staticsticDictionaryWord();
//    testDeleteObject();
//    testMorphologicalAnalyze();
//    AnalyzeAccentDictionary();
//    testSet();
//    TestAccentDictionary();
//    testDictionaryInterface();
//    AnalyzeAccentDictionary_Ver1();
//    testAccentRusCorpora();
//    AnalyzeAccentDictionary_Ver2();
//    testDictionaryAccentInterface();
//    testRebuildDictionary();
//    testPointer();
//    testFindingWordFormByFeatures();
//    testDictionaryTrieIterator();
//    testFindWordByLemmaId();
//    fixRussianMorphology();
//    exportDictionaryToTextFile();

//    testDictionary();
//    testEnglishMorphology();


//    createMorphologyBinaryFileFromTextFile("//home//mhle//Samsung//EnglishMorphology_2015_03_04//PennTreeBank//lemma_corpus_6.txt", "//home//mhle//Samsung//EnglishMorphology_2015_03_04//PennTreeBank//EnglishMorphologyVer2.bin");

//    createMorphologyBinaryFileFromTextFile(
//     "//home//mhle//Samsung//EnglishMorphology_2015_03_04//PennTreeBank//lemma_corpus_6.txt",
//     "//home//mhle//Coda//config//Dictionary//Data//EnglishMorphologyVer2.bin");

//    createMorphologyBinaryFileFromTextFile(
//     "//home//mhle//Samsung//RussianMorphology_2015_06_09//dict.opcorpora_6.txt",
//     "//home//mhle//Coda//config//Dictionary//Data//RussianMorphologyVer2.bin");

//    testMorphologicalDictionaryRussian();
    testMorphologicalDictionaryEnglish();
//    compareSpeedOfMorphology("//home//mhle//Samsung//___Others//RussianText.txt", L"ru", 10);
//    compareSpeedOfMorphology("//home//mhle//Samsung//___Others//EnglishText.txt", L"en", 10);
//    testLemmaId();
//    checkMorphologicalPrediction();

    delete tools;
}

#endif /* _TESTTOOLS_H_ */
