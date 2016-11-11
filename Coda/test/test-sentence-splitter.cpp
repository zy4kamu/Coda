#include "gtest/gtest.h"
#include <tuple>
#include <vector>
#include <string>
#include <cctype>
#include "ISentenceSplitter.h"

TEST (SentenceSplitter, MainTest)
{
    setlocale(LC_ALL,"");
  // setlocale(LC_NUMERIC, "C");
    shared_ptr<_sentence_splitter::ISentenceSplitter> spltr = _sentence_splitter::ISentenceSplitter::GetSentenceSplitter(Tools::Language::RU);
    std::wstring input = L"Сэмюэл Л. Джексон и Роберт Дауни мл. снова взялись за своё. Они стали грабить банки.";
    std::vector<size_t> borders = spltr->split(input);
    ASSERT_EQ(borders.size(), 2);
    input = L"Всё обошлось без серьёзных потерь и FILM:[ Принц полукровка ] продолжил лидировать, заработав 43,4 млн. долларов в 13000 кинозалах в 64 территориях за уик-энд, вместе с которыми его общая касса выросла до 493 млн. долларов международных сборов и 747,7 млн. общемировых.";
    borders = spltr.split(input);
    ASSERT_EQ(borders.size(), 1);
  //  EXPECT_EQ(borders[0], 112);
  //  EXPECT_EQ(borders[1], 193);

}
