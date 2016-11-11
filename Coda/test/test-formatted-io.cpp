#include "gtest/gtest.h"
#include <tuple>
#include <vector>
#include <string>
#include "token-stream-document-loader.hpp"

using namespace _formatted_documents_io;

using TokenType = TokenInfo<std::wstring,double,int,std::wstring>;

/*TEST (Formatted, Reading)
{
    setlocale(LC_ALL,"");
    TokenStreamDocumentLoader<std::wstring,double,int,std::wstring> reader{TOKENSTREAM_IO_TEST_FILE, L'\t', L'\n', L'\n'};

    std::wstring controlStr1 = L"слово1";
    double controlDouble1 = 0.59;
    int controlInt1 = 1;
    std::wstring controlField1 = L"поле1";

    std::wstring controlStr2 = L"слово2";
    double controlDouble2 = 0.5924;
    int controlInt2 = -2;
    std::wstring controlField2 = L"поле2";

    while(!reader.isFinished())
    {
        std::vector<TokenType> tg{reader.getTokenGroup()};
        EXPECT_EQ(std::get<0>(tg[0]), controlStr1);
        EXPECT_EQ(std::get<1>(tg[0]), controlDouble1);
        EXPECT_EQ(std::get<2>(tg[0]), controlInt1);
        EXPECT_EQ(std::get<3>(tg[0]), controlField1);

        EXPECT_EQ(std::get<0>(tg[1]), controlStr2);
        EXPECT_EQ(std::get<1>(tg[1]), controlDouble2);
        EXPECT_EQ(std::get<2>(tg[1]), controlInt2);
        EXPECT_EQ(std::get<3>(tg[1]), controlField2);
    }
}*/
