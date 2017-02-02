#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

extern "C"
{

namespace Tokenization
{

void CreateTokenizer(const char* languagePtr)
{
    fprintf(stderr, "Yep 0\n");
    fflush(stderr);
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
}

/* FUNCTIONS RELATED TO TOKENIZATION */

vector<Token> currentTokens;

size_t Tokenize(const wchar_t* sentencePtr, const char* languagePtr)
{
    fprintf(stderr, "Yep\n");
    fflush(stderr);
    Tools::Language language = Tools::StringToLanguage(languagePtr);
    shared_ptr<ITokenizer> tokenizer = ITokenizer::GetTokenizer(language);
    currentTokens = tokenizer->Tokenize(sentencePtr);
    fprintf(stderr, "Response size: %d\n", currentTokens.size());
    fflush(stderr);
    return currentTokens.size();
}

const wchar_t* RequestContent(size_t index)
{
    fprintf(stderr, "RequestContent: index=%d", (int)index);
    fflush(stderr);
    const wchar_t *s = currentTokens[index].content.c_str();
    fprintf(stderr, s ? " not null\n" : "null\n");
    fflush(stderr);
    FILE *fout = fopen("/tmp/boo.txt", "ab");
    int n = 0;
    for (; s[n]; n++) {
    }
    fwrite(s, sizeof(wchar_t), n, fout);
    fclose(fout);
    fprintf(stderr, "Written to /tmp/boo.txt\n");
    fflush(stderr);
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
