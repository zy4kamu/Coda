#include <string>
#include <vector>
#include <boost/regex.hpp>
#include "sentence-splitter.h"
#include "ssplitter-feature-extractor.h"
#include "logistic-regression.h"
#include <iostream>

namespace _sentence_splitter
{

static std::string getLangConfig(const Tools::Language i_langID)
{
    if(i_langID == Tools::RU) {
        return SSPLITTER_CONFIG_RUSSIAN;
    } else {
        return SSPLITTER_CONFIG_ENGLISH;
    }
}

SentenceSplitter::SentenceSplitter(const Tools::Language i_langID)
    : language(i_langID)
    , m_model( getLangConfig(i_langID) )
{

}

SentenceSplitter::SentenceSplitter(const std::string& i_model)
    : language(Tools::Language::EN)
    , m_model(i_model)/*, featureLogger("splitter.log")*/
{

}

std::vector<size_t> SentenceSplitter::split(const std::wstring& i_line_to_split)
{
    std::vector<size_t> result;

    boost::wregex re_non_letter_symbols(L"[[:punct:]]");
    boost::wsregex_iterator m1(i_line_to_split.begin(), i_line_to_split.end(), re_non_letter_symbols);
    boost::wsregex_iterator m2;
    std::for_each(m1, m2,
        [this, &i_line_to_split, &result](const boost::match_results<std::wstring::const_iterator>&  entry) mutable
        {
            if(this->testEntryOnSentenceSplit_(entry.position(size_t(0)), i_line_to_split))
            {
                result.push_back(entry.position(size_t(0)));
            }
        });

    return result;
}

bool SentenceSplitter::testEntryOnSentenceSplit_(size_t i_position, const std::wstring& i_line_to_split)
{
   FeatureExtractor aaa{3,3};
   std::vector<std::wstring> contextWindow = aaa.getFeatures(i_position, i_line_to_split);

   int cl = m_model.getClass(contextWindow);

  /* if(featureLogger) {
       for(size_t i = 0; i < contextWindow.size(); i++)
       {
           featureLogger << (contextWindow[i]) << "\t";
       }
       featureLogger << cl << std::endl;
   }*/
   return cl == 1;
}

} //namespace _sentence_splitter
