#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

extern "C"
{

namespace Tokenization
{

void echo()
{
    std::cout << "hello" << std::endl;
}

void CreateTokenizer()
{
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(Tools::Language::EN);
    for (const Token& token : tokenizer->Tokenize(L"Mothter is ")) {
        std::wcout << token.ToWstring() << std::endl;
    }
}

}

}
