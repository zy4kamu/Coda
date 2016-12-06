

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

}

}
