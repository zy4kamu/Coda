#include "gtest/gtest.h"
#include "DictionaryCreator.h"
#include "DictionaryFacade.h"

using DictionaryOps::DictionaryFacade;
using DictionaryOps::DictionaryCreator;

TEST_F (TestDictionaryAndTokenizer, TestTogether)
{
    DictionaryFacade dictRus = DictionaryCreator::getDictionaryCreator().getDictionary(Tools::RU);

}

