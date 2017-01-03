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
    vector<Morphology> morphology_list = m_dictRus->getMorphologicalInformation(L"Россия");
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


/*
TEST_F (TestInflector, Noun_phrase_singular_to_genitive_plural)
{
    std::wstring result = inflector->inflect(L"синий мяч", inflector::GENITIVE, inflector::PLURAL);

    EXPECT_STREQ(L"синих мячей", result.c_str());
}

TEST_F (TestInflector, Longer_group)
{
    std::wstring result = inflector->inflect(L"маленький стол из стали", inflector::GENITIVE);

    EXPECT_STREQ(L"маленького стола из стали", result.c_str());
}

TEST_F (TestInflector, Longer_group_super)
{
    std::wstring result = inflector->inflect(L"малейший стол из стали", inflector::GENITIVE, inflector::SINGULAR);

    EXPECT_STREQ(L"малейшего стола из стали", result.c_str());
}

TEST_F (TestInflector, Pronoun_singular)
{
    std::wstring result = inflector->inflect(L"я", inflector::GENITIVE, inflector::SINGULAR);

    EXPECT_STREQ(L"меня", result.c_str());
}

TEST_F (TestInflector, Pronoun_singular_to_plural)
{
    std::wstring result = inflector->inflect(L"я", inflector::GENITIVE, inflector::PLURAL);

    EXPECT_STREQ(L"нас", result.c_str());
}

TEST_F (TestInflector, Pronoun_plural)
{
    std::wstring result = inflector->inflect(L"все возрасты", inflector::GENITIVE);

    EXPECT_STREQ(L"всех возрастов", result.c_str());
}

TEST_F (TestInflector, stuff)
{
    std::wstring result = inflector->inflect(L"лёд", inflector::GENITIVE);

    EXPECT_STREQ(L"льда", result.c_str());
}*/


