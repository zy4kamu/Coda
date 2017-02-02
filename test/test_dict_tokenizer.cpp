#include <memory>
#include "gtest/gtest.h"
#include "DictionaryCreator.h"
#include "DictionaryFacade.h"
#include "Disambiguator.h"

#include "ITokenizer.h"

using std::shared_ptr;
using Tokenization::ITokenizer;
using Disambiguation::IDisambiguator;
using DictionaryOps::DictionaryFacade;
using DictionaryOps::DictionaryCreator;

TEST (TestDictionaryAndTokenizer, TestTogether)
{
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(Tools::RU);
    shared_ptr<IDisambiguator> disambiguator = IDisambiguator::GetDisambiguator(Tools::RU);
    shared_ptr<DictionaryFacade> dict = DictionaryCreator::getDictionaryCreator().getDictionary(Tools::RU);
    tokenizer->Tokenize(L"МИД пригрозил ограничить выезд американских дипломатов по России");
}

