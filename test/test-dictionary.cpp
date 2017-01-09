#include "gtest/gtest.h"
#include "DictionaryCreator.h"
#include "DictionaryFacade.h"

using DictionaryOps::DictionaryFacade;
using DictionaryOps::DictionaryCreator;

class TestDictionary : public::testing::Test
{
protected:
    void SetUp()
    {
        m_dictRus = DictionaryCreator::getDictionaryCreator().getDictionary(Tools::RU);
        m_dictEng = DictionaryCreator::getDictionaryCreator().getDictionary(Tools::EN);
    }

    void TearDown()
    {
        DictionaryCreator::getDictionaryCreator().removeDictionary(Tools::RU);
        DictionaryCreator::getDictionaryCreator().removeDictionary(Tools::EN);

    }
    shared_ptr<DictionaryFacade> m_dictRus;
    shared_ptr<DictionaryFacade> m_dictEng;
};

TEST_F (TestDictionary, TestMorphologicalInfoRus)
{
    vector<Morphology> morphology_list = m_dictRus->getMorphologicalInformation(L"России");
    EXPECT_EQ(1, morphology_list.size());
    Morphology morphology = morphology_list[0];
    EXPECT_STREQ(L"NOUN", morphology.features[0]->c_str());
    EXPECT_STREQ(L"россия", morphology.lemma->c_str());
    EXPECT_STREQ(L"nomn", morphology.features[5]->c_str());
    EXPECT_STREQ(L"sing", morphology.features[3]->c_str());
}

TEST_F (TestDictionary, TestWordSynthesisRus)
{
    vector<wstring> features = {L"NOUN", L"inan", L"femn", L"sing", L"Sgtm", L"gent", L"Geox"};
    vector<wstring> variants =  m_dictRus->synthesizeWordform(L"россия", features);
    ASSERT_EQ(1, variants.size());
    EXPECT_STREQ(L"россии", variants[0].c_str());
}

