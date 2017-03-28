#include "all.h"

using namespace ccoda;

cSentenceSplitter* cSentenceSplitter_create(Tools::Language language)
{
	return (cSentenceSplitter*) new _sentence_splitter::SentenceSplitter(language);
}

void cSentenceSplitter_destroy(cSentenceSplitter* ss)
{
	delete (_sentence_splitter::SentenceSplitter*) ss;
}

/// @returns number of items in results buffer
size_t cSentenceSplitter_split(cSentenceSplitter* splitter, const wchar_t* line_to_split, size_t line_length, size_t** result_borders)
{
	wstring s(line_to_split, line_length);

	auto vect = ((_sentence_splitter::SentenceSplitter*) splitter)->split(s);

	*result_borders = (size_t*) malloc(sizeof(size_t) * vect.size());
	std::copy(vect.begin(), vect.end(), *result_borders);

	return vect.size();
}

void free_mem(void* buf_ptr)
{
	free(buf_ptr);
}

cTokenizer* cTokenizer_create(Tools::Language language)
{
	auto r = new cTokenizer;

	r->This = Tokenization::ITokenizer::GetTokenizer(language);

	return r;
}

void cTokenizer_destroy(cTokenizer* tokenizer)
{
	delete tokenizer;
}

cTokensStorage* cTokenizer_tokenize(cTokenizer* tokenizer, const wchar_t* line, size_t line_length)
{
	wstring s(line, line_length);

	auto r = new cTokensStorage;

	r->tokens = tokenizer->This->Tokenize(s);

	return r;
}

void cTokensStorage_destroy(cTokensStorage* ts)
{
	delete ts;
}

cDisambiguator* cDisambiguator_create(Tools::Language language)
{
	auto r = new cDisambiguator;

	r->This = Disambiguation::IDisambiguator::GetDisambiguator(language);

	return r;
}

void cDisambiguator_destroy(cDisambiguator* d)
{
	delete d;
}

cDisambiguatedDataStorage* cDisambiguator_disambiguate(cDisambiguator* d, cTokensStorage* parsedTokens)
{
	auto r = new cDisambiguatedDataStorage;

	r->disambiguated = d->This->Disambiguate(parsedTokens->tokens);

	return r;
}

void cDisambiguatedDataStorage_destroy(cDisambiguatedDataStorage* ds)
{
	delete ds;
}

cSyntaxParser* cSyntaxParser_create(Tools::Language language)
{
	auto r = new cSyntaxParser;

	r->This = SyntaxParser::ISyntaxParser::GetSyntaxParser(language);

	return r;
}

void cSyntaxParser_destroy(cSyntaxParser* sp)
{
	delete sp;
}

cSyntaxTree* cSyntaxParser_parse(cSyntaxParser* syntax_parser, cDisambiguatedDataStorage* dds)
{
	return (cSyntaxTree*) new PublicNodesSyntaxTree(
		syntax_parser->This->Parse(dds->disambiguated)
	);
}

void cSyntaxTree_destroy(cSyntaxTree* t)
{
	delete (PublicNodesSyntaxTree*) t;
}

PublicNodesSyntaxTree::PublicNodesSyntaxTree(const SyntaxTree& tree) : SyntaxParser::SyntaxTree(tree)
{
}

SyntaxParser::SyntaxNode* PublicNodesSyntaxTree::getNodeByIndex(size_t idx)
{
	return &nodes[idx];
}

cSyntaxNode* cSyntaxTree_getNodeByIndex(cSyntaxTree* tree, size_t idx)
{
	return (cSyntaxNode*) ((PublicNodesSyntaxTree*) tree)->getNodeByIndex(idx);
}

int cSyntaxTree_getRootIndex(const cSyntaxTree* tree)
{
	return ((PublicNodesSyntaxTree*) tree)->GetRootIndex();
}

int cSyntaxTree_getParentIndex(const cSyntaxTree* tree, int nodeIndex)
{
	return ((PublicNodesSyntaxTree*) tree)->GetParentIndex(nodeIndex);
}

cIntVector* cSyntaxNode_getChildrenIndexes(const cSyntaxNode* node)
{
	return (cIntVector*) &((SyntaxParser::SyntaxNode*) node)->neighbours;
}

cNodeData cSyntaxNode_get_cNodeData(cSyntaxNode* sNode)
{
	auto node = (SyntaxParser::SyntaxNode*) sNode;

	cNodeData r;

	r.content = (cWstring*) &node->content;
	r.punctuation_size = node->punctuation.size();
	r.isNextSpace = node->isNextSpace;
	r.lemma = (cWstring*) &node->lemma;
	r.label = (cWstring*) &node->label;
	r.weight = node->weight;
	r.lemmaId = node->lemmaId;

	return r;
}

cWstring* cSyntaxNode_getPunctuationByIndex(cSyntaxNode* sNode, size_t idx)
{
	auto node = (SyntaxParser::SyntaxNode*) sNode;

	return (cWstring*) &node->punctuation[idx];
}

size_t cIntVector_getLength(const cIntVector* iv)
{
	return ((vector<int>*) iv)->size();
}

int* cIntVector_getPtr(const cIntVector* iv)
{
	return ((vector<int>*) iv)->data();
}

size_t cWstring_getLength(const cWstring* v)
{
	return ((wstring*) v)->size();
}

const wchar_t* cWstring_getPtr(cWstring* v)
{
	return ((wstring*) v)->data();
}
