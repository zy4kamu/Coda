#include "gtest/gtest.h"
#include "DisambiguatorInterface.h"
#include "DisambiguatorLearnerTester.h"
#include "Tools.h"

using Disambiguation::DisambiguatorInterface;
using Disambiguation::DisambiguatedData;
using Tokenization::TokenizerInterface;

TEST (Disambiguator, MainTest)
{
    Tools::PrepareConsole();
    shared_ptr<DictionaryInterface> dictionary = Disambiguation::GetDictionary();
    shared_ptr<DisambiguatorInterface> disambiguator = DisambiguatorInterface::CreateDisambiguator(dictionary);
    shared_ptr<TokenizerInterface> tokenizer = TokenizerInterface::CreateTokenizer();

    wstring sentence = L"Я бью женщин и детей потому, что я красавчик.";
    vector<Token> tokens = tokenizer->Tokenize(sentence);
    vector<DisambiguatedData> disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@МН@ЖЕН@ВИН@ОД", disambiguated[2].label.c_str());
    EXPECT_STREQ(L"S@МН@МУЖ@ВИН@ОД", disambiguated[4].label.c_str());

    sentence = L"я ем сосиски и вкусные пряники.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@МН@ЖЕН@ВИН@НЕОД", disambiguated[2].label.c_str());
    EXPECT_STREQ(L"A@МН@ВИН@НЕОД", disambiguated[4].label.c_str());
    EXPECT_STREQ(L"S@МН@МУЖ@ВИН@НЕОД", disambiguated[5].label.c_str());

    sentence = L"покажи фильмы";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

//    EXPECT_STREQ(L"V@СОВ@ПОВ@ЕД@2-Л@ПЕРЕХ", disambiguated[0].label.c_str());
    EXPECT_STREQ(L"S@МН@МУЖ@ВИН@НЕОД", disambiguated[1].label.c_str());

    sentence = L"Я забиваю гвозди с помощью метро.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@МН@МУЖ@ВИН@НЕОД", disambiguated[2].label.c_str());
    EXPECT_STREQ(L"S@ЕД@СРЕД@РОД@НЕОД", disambiguated[4].label.c_str());

    sentence = L"я ем метро.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ВИН@НЕОД", disambiguated[2].label.c_str());

    sentence = L"с метро мир становится лучше.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ТВОР@НЕОД", disambiguated[1].label.c_str());
    EXPECT_STREQ(L"S@ЕД@МУЖ@ИМ@ОД", disambiguated[2].label.c_str());

    sentence = L"я иду к метро.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ДАТ@НЕОД", disambiguated[3].label.c_str());

    sentence = L"он идет к метро.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ДАТ@НЕОД", disambiguated[3].label.c_str());

    sentence = L"мы бьем стариков и старух.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@МН@МУЖ@ВИН@ОД", disambiguated[2].label.c_str());
    EXPECT_STREQ(L"CONJ", disambiguated[3].label.c_str());
    EXPECT_STREQ(L"S@МН@ЖЕН@ВИН@ОД", disambiguated[4].label.c_str());

    sentence = L"мы пошли к метро.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ДАТ@НЕОД", disambiguated[3].label.c_str());

    sentence = L"станция московского метро";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@РОД@НЕОД", disambiguated[2].label.c_str());

    sentence = L"в подмосковье началось обсуждение";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ПР@НЕОД", disambiguated[1].label.c_str());
    EXPECT_STREQ(L"S@ЕД@СРЕД@ИМ@НЕОД", disambiguated[3].label.c_str());

    sentence = L"шквалистый ветер с грозой ожидается в подмосковье";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"S@ЕД@СРЕД@ПР@НЕОД", disambiguated[6].label.c_str());

    sentence = L"Локомотивную бригаду московского метро уволили за халатность.";
    tokens = tokenizer->Tokenize(sentence);
    disambiguated = disambiguator->Disambiguate(tokens, true);

    EXPECT_STREQ(L"A@ЕД@ЖЕН@ВИН", disambiguated[0].label.c_str());
    EXPECT_STREQ(L"S@ЕД@ЖЕН@ВИН@НЕОД", disambiguated[1].label.c_str());
    EXPECT_STREQ(L"S@ЕД@МУЖ@РОД@ОД", disambiguated[2].label.c_str());
    EXPECT_STREQ(L"S@ЕД@СРЕД@РОД@НЕОД", disambiguated[3].label.c_str());
    EXPECT_STREQ(L"S@ЕД@ЖЕН@ВИН@НЕОД", disambiguated[6].label.c_str());
}
