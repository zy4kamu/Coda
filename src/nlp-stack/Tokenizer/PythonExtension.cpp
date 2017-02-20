#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

extern "C"
{

namespace Tokenization
{

void CreateTokenizer(const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
}

/* FUNCTIONS RELATED TO TOKENIZATION */

vector<Token> currentTokens;

size_t Tokenize(const wchar_t* sentencePtr, const char* languagePtr)
{
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
    currentTokens = tokenizer->Tokenize(sentencePtr);
    return currentTokens.size();
}

const wchar_t* RequestContent(size_t index)
{
    const wchar_t *s = currentTokens[index].content.c_str();
    return s;
}

size_t RequestPunctuationSize(size_t index)
{
    return currentTokens[index].punctuation.size();
}

const wchar_t* RequestPunctuation(size_t tokenIndex, size_t punctIndex)
{
    return currentTokens[tokenIndex].punctuation[punctIndex].c_str();
}

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

vector<Token> parsedTokens;

void ResetParsedTokens()
{
    parsedTokens.clear();
}

void PushParsedContent(const wchar_t* content)
{
    parsedTokens.emplace_back();
    parsedTokens.back().content = content;
}

void PushParsedPunctuation(const wchar_t* punctuation)
{
    parsedTokens.back().punctuation.push_back(punctuation);
}

}

}
