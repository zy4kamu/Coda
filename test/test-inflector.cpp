#include "gtest/gtest.h"

#include <string>
#include "DisambiguatorInterface.h"
#include "DictionaryInterface.h"
#include "tagsetConstants.h"
#include "Tools.h"
#include "Inflector.h"

using Disambiguation::DisambiguatorInterface;

class TestInflector : public :: testing::Test
{
protected:
    static void SetUpTestCase()
    {
#ifdef MSVC
        system("chcp 1251");
        std::locale::global(std::locale("Russian"));
#else
        setlocale(LC_ALL,"");
#endif
        dict = shared_ptr<DictionaryInterface>(
            DictionaryInterface::CreateDictionary());
        dict->setMainWordFormFromLinkedLemmas(false);
        disambiguator = DisambiguatorInterface::CreateDisambiguator(dict);
    }
    static void TearDownTestCase()
    {
//        delete disambiguator;
//        delete dict;
    }

    void SetUp()
    {
         inflector = new inflector::Inflector(dict, disambiguator, std::string(INFLECTOR_CONFIG_PATH));
    }

    void TearDown()
    {
         delete inflector;
    }
    static shared_ptr<DictionaryInterface> dict;
    static shared_ptr<DisambiguatorInterface> disambiguator;
    inflector::Inflector* inflector;
};

shared_ptr<DictionaryInterface>  TestInflector::dict;
shared_ptr<DisambiguatorInterface> TestInflector::disambiguator;

/*TEST_F (TestInflector, Noun_phrase_singular_to_genitive_singular)
{
    std::wstring result = inflector->inflect(L"синий мяч", inflector::GENITIVE, inflector::NUMBER_UNSPEC);
    EXPECT_STREQ(L"синего мяча", result.c_str());
}

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


