#include "SyntaxParser.hpp"

extern "C"
{

namespace SyntaxParser
{

/* FUNCTIONS RELATED TO SYNTAX PARSER */

void CreateSyntaxParser(const char* languagePtr);
void SyntaxParse(const char* languagePtr);
int GetRootIndex();
const wchar_t* GetSentence();
int GetParentIndex(size_t tokenIndex);
void Draw(const char* outputFile, bool openPDF);

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

extern SyntaxTree parsedTree;

void CreateTreeFromParsedDisambiguated();
void SetParent(int nodeIndex, int parentIndex);

}

}
