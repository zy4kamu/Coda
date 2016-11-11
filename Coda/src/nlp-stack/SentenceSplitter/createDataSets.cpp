#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <boost/regex.hpp>
#include "feature-extractor.h"
#include "Tools.h"


using namespace std;

int main()
{
   setlocale(LC_ALL,"");
    std::wifstream in("/home/mkudinov/sentences");

    if(!in)
    {
        std::cout << "sadfgasdfasrg";
    }

    SetLocale(in);

    boost::wregex re_non_letter_symbols(L"[[:punct:]]");

    _sentence_splitter::FeatureExtractor fExtr(3, 3);

    std::wstring line_next = L"", line = L"";

    std::getline(in, line);

    std::vector<std::vector<std::wstring>> allSentsFeatures;

    while(std::getline(in, line_next))
    {
        int end1 = 0;
        std::vector<std::vector<std::wstring>> sentFeats;

        boost::wsregex_iterator m1(line.begin(), line.end(), re_non_letter_symbols);
        boost::wsregex_iterator m2;

        std::for_each(m1, m2,
            [&fExtr, &line, &sentFeats](const boost::match_results<std::wstring::const_iterator>&  entry) mutable
            {
                 sentFeats.push_back(fExtr.getFeatures(entry.position(size_t(0)), line));

            });

        for(int i = 0; i < sentFeats.size() - 1; i++)
        {
            sentFeats[i].push_back(L"0");
        }
        sentFeats[sentFeats.size() - 1].push_back(L"1");

        end1 = sentFeats.size() - 1;

        allSentsFeatures.insert( allSentsFeatures.end(), sentFeats.begin(), sentFeats.end());

        sentFeats.clear();

        std::wstring two_sentences = line + line_next;

        boost::wsregex_iterator m3(two_sentences.begin(), two_sentences.end(), re_non_letter_symbols);
        boost::wsregex_iterator m4;


        std::for_each(m3, m4,
            [&fExtr, &two_sentences, &sentFeats](const boost::match_results<std::wstring::const_iterator>&  entry) mutable
            {
                 sentFeats.push_back(fExtr.getFeatures(entry.position(size_t(0)), two_sentences));

            });

        for(int i = 0; i < sentFeats.size() - 1; i++)
        {
            sentFeats[i].push_back(L"0");
        }
        sentFeats[sentFeats.size() - 1].push_back(L"1");
        sentFeats[end1].pop_back();
        sentFeats[end1].push_back(L"1");


        allSentsFeatures.push_back(sentFeats[end1]);
        line = std::move(line_next);

    }

    in.close();

    std::wofstream out("/home/mkudinov/features");

    SetLocale(out);

    for(int i = 0; i < allSentsFeatures.size(); i++)
    {
        for(int j = 0; j < allSentsFeatures[i].size(); j++)
        {
            out << allSentsFeatures[i][j] << L'\t';
        }
        out << std::endl;
    }

    out.close();

    return 0;
}


