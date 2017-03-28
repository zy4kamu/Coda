#ifndef ALL_H
#define ALL_H

#include "sentence-splitter.h"
#include "ITokenizer.h"
#include "Token.h"
#include "Disambiguator.h"
#include "SyntaxParser.hpp"

namespace ccoda
{

class PublicNodesSyntaxTree : public SyntaxParser::SyntaxTree
{
    public:
    PublicNodesSyntaxTree(const SyntaxTree& tree);
    virtual SyntaxParser::SyntaxNode* getNodeByIndex(size_t idx);
};

extern "C"
{

void free_mem(void* buf_ptr);

struct cIntVector;
size_t cIntVector_getLength(const cIntVector* iv);
int* cIntVector_getPtr(const cIntVector* iv);

struct cWstring;
size_t cWstring_getLength(const cWstring* v);
wchar_t* cWstring_getPtr(cWstring* v);

struct cSentenceSplitter;
cSentenceSplitter* cSentenceSplitter_create(Tools::Language language);
void cSentenceSplitter_destroy(cSentenceSplitter* ss);
size_t cSentenceSplitter_split(cSentenceSplitter* splitter, const wchar_t* line_to_split, size_t line_length, size_t** result_borders);

struct cTokenizer
{
    shared_ptr<Tokenization::ITokenizer> This;
};

struct cTokensStorage
{
    vector<Tokenization::Token> tokens;
};

void cTokensStorage_destroy(cTokensStorage* ts);

cTokenizer* cTokenizer_create(Tools::Language language);
void cTokenizer_destroy(cTokenizer* tokenizer);
cTokensStorage* cTokenizer_tokenize(const wchar_t* line_to_split, size_t line_length);

struct cDisambiguator
{
    shared_ptr<Disambiguation::IDisambiguator> This;
};

struct cDisambiguatedDataStorage
{
    vector<Disambiguation::DisambiguatedData> disambiguated;
};

cDisambiguator* cDisambiguator_create(Tools::Language language);
void cDisambiguator_destroy(cDisambiguator* d);
cDisambiguatedDataStorage* cDisambiguator_disambiguate(cDisambiguator* d, cTokensStorage* parsedTokens);

void cDisambiguatedDataStorage_destroy(cDisambiguatedDataStorage* ds);

struct cSyntaxParser
{
    shared_ptr<SyntaxParser::ISyntaxParser> This;
};

struct cSyntaxTree;

cSyntaxParser* cSyntaxParser_create(Tools::Language language);
void cSyntaxParser_destroy(cSyntaxParser* sp);
cSyntaxTree* cSyntaxParser_parse(cSyntaxParser* syntax_parser, cDisambiguatedDataStorage* dds);

struct cSyntaxNode;

void cSyntaxTree_destroy(cSyntaxTree* t);
cSyntaxNode* cSyntaxTree_getNodeByIndex(cSyntaxTree* tree, size_t idx);
int cSyntaxTree_getRootIndex(const cSyntaxTree* tree);
int cSyntaxTree_getParentIndex(const cSyntaxTree* tree, int nodeIndex);

struct cNodeData
{
    cWstring* content;
    size_t punctuation_size;
    bool isNextSpace;
    cWstring* lemma; /**< initial form of the token*/
    cWstring* label; /**< morphology label of the token*/
    double weight; /**< weight assigned to the label by the classifier*/
    int lemmaId; /**< index of lemma in database*/
};

cIntVector* cSyntaxNode_getChildrenIndexes(const cSyntaxNode* node);
cNodeData cSyntaxNode_get_cNodeData(cSyntaxNode* node);
cWstring* cSyntaxNode_getPunctuationByIndex(cSyntaxNode* node, size_t idx);

}
}

#endif // ALL_H
