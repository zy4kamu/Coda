#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

extern "C"
{

namespace Tokenization
{

vector<Token> currentTokens;

void CreateTokenizer(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
}

size_t Tokenize(const wchar_t* sentencePtr, const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
    currentTokens = tokenizer->Tokenize(sentencePtr);
    return currentTokens.size();
}

const wchar_t* RequestContent(size_t index)
{
    return currentTokens[index].content.c_str();
}

size_t RequestPunctuationSize(size_t index)
{
    return currentTokens[index].punctuation.size();
}

const wchar_t* RequestPunctuation(size_t tokenIndex, size_t punctIndex)
{
    return currentTokens[tokenIndex].punctuation[punctIndex].c_str();
}


}

}
