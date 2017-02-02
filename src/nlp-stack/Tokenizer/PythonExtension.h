#include "ITokenizer.h"
#include "TokenizerEn.h"
#include "Tools.h"

extern "C"
{

namespace Tokenization
{


void createTokenizer(const char* languagePtr);

/* FUNCTIONS RELATED TO TOKENIZATION */

extern vector<Token> currentTokens;

size_t tokenize(const wchar_t* sentencePtr, const char* languagePtr);
const wchar_t* requestContent(size_t index);
size_t requestPunctuationSize(size_t index);
const wchar_t* requestPunctuation(size_t tokenIndex, size_t punctIndex);

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

extern vector<Token> parsedTokens;

void resetParsedTokens();
void pushParsedContent(const wchar_t* content);
void pushParsedPunctuation(const wchar_t* punctuation);

}

}
